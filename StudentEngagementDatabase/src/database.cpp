#include "database.h"
#include "utility.h"
#include "models.h"

#include "sqlite3.h"

#include <filesystem>
#include <iostream>
#include <fstream>

using namespace ::ste;
using namespace ::ste::models;


namespace {

sqlite3* db = nullptr;

}; // namespace



namespace ste {
namespace database {

bool Start() {
  char* errMsg = nullptr;

  // Checks if need to generate tables
  bool exists = std::filesystem::exists("db.sqlite3");

  // Open or create
  int result = sqlite3_open("db.sqlite3", &db);
  if (result != SQLITE_OK || db == nullptr) {
    PrintError(sqlite3_errmsg(db));
  }


  // If the initialization scripts exist
  if (!std::filesystem::exists("sql/init/")) {
    PrintError("Failed to find initialization scripts");
    return true;
  }


  // Get total size of all the files
  std::uintmax_t totalSizeReq = 1;
  for (auto const& path : std::filesystem::directory_iterator("sql/init/")) {
    if (!path.is_regular_file()) {
      continue;
    }
    totalSizeReq += path.file_size();
  }


  char* sqlGlob = (char*)calloc(totalSizeReq, 1);
  char* curPos = sqlGlob;

  // Read them into sqlGlob
  for (auto const& path : std::filesystem::directory_iterator("sql/init/")) {
    if (!path.is_regular_file()) {
      continue;
    }

    std::ifstream str(path.path(), std::ios::binary);
    std::uintmax_t size = path.file_size();
    str.seekg(0);
    str.read(curPos, size);
    curPos += size;
    str.close();
  }


  // Execute all init statements
  result = sqlite3_exec(db, sqlGlob, nullptr, nullptr, &errMsg);
  if (result != SQLITE_OK) {
    if (errMsg != nullptr) {
      PrintError(errMsg);
      sqlite3_free(errMsg);
    } else {
      PrintError("Failed to call initialization scripts");
    }
  }

  free(sqlGlob);

  return exists;
}


void Fill() {
  if (!std::filesystem::exists("sql/startup.sql")) {
    return;
  }

  char* buffer = nullptr;
  std::ifstream str("sql/startup.sql", std::ios::binary);
  size_t size = std::filesystem::file_size("sql/startup.sql");
  buffer = (char*)calloc(size + 1, 1);
  str.read(buffer, size);
  str.close();

  char* errMsg = nullptr;
  int result = sqlite3_exec(db, buffer, nullptr, nullptr, &errMsg);
  if (result != SQLITE_OK) {
    if (errMsg != nullptr) {
      PrintError(errMsg);
      sqlite3_free(errMsg);
    } else {
      PrintError("Failed to call initialization scripts");
    }
  }

  free(buffer);
}



void End() {
  sqlite3_close(db);
}




void AddEvent(const Event& val) {
  static char const* sql  = "INSERT INTO event (title, location, time, exp_attendance, audio_video, budget) VALUES (?, ?, ?, ?, ?, ?);";
  sqlite3_stmt* stmt = nullptr;
  int result = sqlite3_prepare(db, sql, -1, &stmt, nullptr);
  if (result != SQLITE_OK) {
    PrintError("Failed to generate AddEvent statement");
    return;
  }
  sqlite3_bind_text(stmt, 1, val.title, -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 2, val.location, -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 3, val.time, -1, SQLITE_STATIC);
  sqlite3_bind_int(stmt, 4, val.exp_attendance);
  sqlite3_bind_int(stmt, 5, val.audio_video);
  sqlite3_bind_int(stmt, 6, val.budget);

  sqlite3_step(stmt);

  sqlite3_finalize(stmt);
}



void AddResource(const Resource& val) {
  static char const* sql = "INSERT INTO resource (event_id, name) VALUES (?, ?);";
  sqlite3_stmt* stmt = nullptr;
  int result = sqlite3_prepare(db, sql, -1, &stmt, nullptr);
  if (result != SQLITE_OK) {
    PrintError("Failed to generate AddResource statement");
    return;
  }
  sqlite3_bind_int(stmt, 1, val.event_id);
  sqlite3_bind_text(stmt, 2, val.name, -1, SQLITE_STATIC);

  sqlite3_step(stmt);

  sqlite3_finalize(stmt);
}



void GiveUserBadge(int user, int badge) {
  static char const* sql = "INSERT INTO badge_student (badge_id, student_id) VALUES (?, ?);";
  sqlite3_stmt* stmt = nullptr;
  int result = sqlite3_prepare(db, sql, -1, &stmt, nullptr);
  if (result != SQLITE_OK) {
    PrintError("Failed to generate GiveUserBadge statement");
    return;
  }
  sqlite3_bind_int(stmt, 1, badge);
  sqlite3_bind_int(stmt, 2, user);

  sqlite3_step(stmt);

  sqlite3_finalize(stmt);
}



void AddStudentToEvent(int student, int event) {
  static char const* sql = "INSERT INTO event_student (event_id, student_id) VALUES (?, ?);";
  sqlite3_stmt* stmt = nullptr;
  int result = sqlite3_prepare(db, sql, -1, &stmt, nullptr);
  if (result != SQLITE_OK) {
    PrintError("Failed to generate AddStudentToEvent statement");
    return;
  }
  sqlite3_bind_int(stmt, 1, event);
  sqlite3_bind_int(stmt, 2, student);

  sqlite3_step(stmt);

  sqlite3_finalize(stmt);
}



User LoginUser(char email[255], char password[32]) {
  static char const* sql = "SELECT * FROM user WHERE email = ? AND password = ?";

  User ret = User();
  ret.user_id = -1;

  sqlite3_stmt* stmt = nullptr;
  int result = sqlite3_prepare(db, sql, -1, &stmt, nullptr);
  if (result != SQLITE_OK) {
    PrintError("Failed to generate LoginUser statement");
    return ret;
  }
  sqlite3_bind_text(stmt, 1, email, -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);

  result = sqlite3_step(stmt);

  if (result != SQLITE_ROW) {
    sqlite3_finalize(stmt);
    return ret;
  }

  ret.user_id = sqlite3_column_int(stmt, 0);
  strcpy_s(ret.email,(char*)sqlite3_column_text(stmt, 1));
  memset(ret.password, 0, sizeof(ret.password));
  ret.permission = sqlite3_column_int(stmt, 3);
  return ret;
}



Student GetStudentAccount(int user_id) {
  static char const* sql = "SELECT * FROM student WHERE user_id = ?";

  Student ret = Student();
  ret.student_id = -1;

  sqlite3_stmt* stmt = nullptr;
  int result = sqlite3_prepare(db, sql, -1, &stmt, nullptr);
  if (result != SQLITE_OK) {
    PrintError("Failed to generate GetStudentAccount statement");
    return ret;
  }
  sqlite3_bind_int(stmt, 1, user_id);

  result = sqlite3_step(stmt);

  if (result != SQLITE_ROW) {
    sqlite3_finalize(stmt);
    return ret;
  }

  ret.student_id = sqlite3_column_int(stmt, 0);
  ret.points = sqlite3_column_int(stmt, 1);
  ret.user_id = user_id;
  return ret;
}



std::vector<Student> GetPointLeaderboard() {
  static char const* sql = "SELECT * FROM student ORDER BY points LIMIT 5";

  std::vector<Student> retList;
  retList.reserve(5);

  sqlite3_stmt* stmt = nullptr;
  int result = sqlite3_prepare(db, sql, -1, &stmt, nullptr);
  if (result != SQLITE_OK) {
    PrintError("Failed to generate GetPointLeaderboard statement");
    return retList;
  }

  while (sqlite3_step(stmt) == SQLITE_ROW) {
    Student student = Student();
    student.student_id = sqlite3_column_int(stmt, 0);
    student.points = sqlite3_column_int(stmt, 1);
    student.user_id = sqlite3_column_int(stmt, 2);
    retList.push_back(student);
  }

  sqlite3_finalize(stmt);

  return retList;
}



std::vector<Event> GetStudentEvents(int student_id) {
  static char const* sql = "SELECT (event_id, title, location, time, exp_attendance, audio_video, budget) FROM event_student WHERE student_id = ? JOIN event ON event.event_id = event_student.event_id";

  std::vector<Event> retList;

  sqlite3_stmt* stmt = nullptr;
  int result = sqlite3_prepare(db, sql, -1, &stmt, nullptr);
  if (result != SQLITE_OK) {
    PrintError("Failed to generate GetStudentEvents statement");
    return retList;
  }
  sqlite3_bind_int(stmt, 1, student_id);


  while (sqlite3_step(stmt) == SQLITE_ROW) {
    Event event = Event();
    event.event_id = sqlite3_column_int(stmt, 0);
    strcpy_s(event.title, (char*)sqlite3_column_text(stmt, 1));
    strcpy_s(event.location, (char*)sqlite3_column_text(stmt, 2));
    strcpy_s(event.time, (char*)sqlite3_column_text(stmt, 3));
    event.exp_attendance = sqlite3_column_int(stmt, 4);
    event.audio_video = sqlite3_column_int(stmt, 5);
    event.budget = sqlite3_column_int(stmt, 6);

    retList.push_back(event);
  }

  sqlite3_finalize(stmt);

  return retList;
}




std::vector<Event> GetUpcomingEvents() {
  static char const* sql = "SELECT * FROM event ORDER BY time LIMIT 5";

  std::vector<Event> retList;
  retList.reserve(5);

  sqlite3_stmt* stmt = nullptr;
  int result = sqlite3_prepare(db, sql, -1, &stmt, nullptr);
  if (result != SQLITE_OK) {
    PrintError("Failed to generate GetUpcomingEvents statement");
    return retList;
  }

  while (sqlite3_step(stmt) == SQLITE_ROW) {
    Event event = Event();
    event.event_id = sqlite3_column_int(stmt, 0);
    strcpy_s(event.title, (char*)sqlite3_column_text(stmt, 1));
    strcpy_s(event.location, (char*)sqlite3_column_text(stmt, 2));
    strcpy_s(event.time, (char*)sqlite3_column_text(stmt, 3));
    event.exp_attendance = sqlite3_column_int(stmt, 4);
    event.audio_video = sqlite3_column_int(stmt, 5);
    event.budget = sqlite3_column_int(stmt, 6);

    retList.push_back(event);
  }

  sqlite3_finalize(stmt);

  return retList;
}




void UpdateStudentPoints(int student_id, int newPoints) {
  static char const* sql = "UPDATE student SET points = ? WHERE student_id = ?";
  sqlite3_stmt* stmt = nullptr;
  int result = sqlite3_prepare(db, sql, -1, &stmt, nullptr);
  if (result != SQLITE_OK) {
    PrintError("Failed to generate UpdateStudentPoints statement");
    return;
  }
  sqlite3_bind_int(stmt, 1, newPoints);
  sqlite3_bind_int(stmt, 2, student_id);

  sqlite3_step(stmt);

  sqlite3_finalize(stmt);
}



}; // namespace database
}; // namespace ste



namespace {

}; // namespace