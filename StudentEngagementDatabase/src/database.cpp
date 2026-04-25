#include "database.h"

#include "sqlite3.h"
#include "utility.h"

#include <filesystem>
#include <iostream>
#include <fstream>

using namespace ::ste;


namespace {

sqlite3* db = nullptr;

}; // namespace



namespace ste {
namespace database {

void Start() {
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
    return;
  }


  // Get total size of all the files
  std::uintmax_t totalSizeReq = 0;
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

    std::ifstream str(path.path());
    std::uintmax_t size = path.file_size();
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
}



void End() {
  sqlite3_close(db);
}


}; // namespace database
}; // namespace ste



namespace {

}; // namespace