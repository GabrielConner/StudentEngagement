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



char LoginUser(char email[255], char password[32]) {
  static char const* sql = "SELECT * FROM user WHERE email = ? AND password = ?";

  sqlite3_stmt* stmt = nullptr;
  int result = sqlite3_prepare(db, sql, -1, &stmt, nullptr);
  if (result != SQLITE_OK) {
    PrintError("Failed to generate LoginUser statement");
    return -1;
  }
  sqlite3_bind_text(stmt, 1, email, -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);

  result = sqlite3_step(stmt);

  if (result != SQLITE_ROW) {
    sqlite3_finalize(stmt);
    return -1;
  }

  int perm = sqlite3_column_int(stmt, 3);
  return perm;
}


}; // namespace database
}; // namespace ste



namespace {

}; // namespace