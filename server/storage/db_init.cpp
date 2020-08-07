#include <sqlite3.h>
#include <string>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <sstream>
#include <boost/format.hpp>

const std::vector<std::pair<std::string, std::string>> kTablesToCreationSQL = {
  {
    "ACCOUNTS",
    "CREATE TABLE ACCOUNTS(" \
      "USERNAME TEXT  PRIMARY KEY    NOT NULL," \
      "PASSWORD TEXT                 NOT NULL," \
      "EMAIL    TEXT                 NOT NULL);"
  },
  {
    "GAMES",
    "CREATE TABLE GAMES(" \
      "ID   TEXT PRIMARY KEY    NOT NULL," \
      "SIZE INT                 NOT NULL);"
  },
  {
    "RANK",
    "CREATE TABLE RANK(" \
      "ID       TEXT PRIMARY KEY    NOT NULL," \
      "USERNAME INT                 NOT NULL," \
      "GAME_ID  INT                 NOT NULL," \
      "RANK     INT                 NOT NULL," \
      "FOREIGN KEY(USERNAME) REFERENCES ACCOUNTS(USERNAME)," \
      "FOREIGN KEY(GAME_ID)  REFERENCES GAMES(ID));"
  }
};

const char kDatabaseFileName[] = "LoveLetter.db";
const std::string kCheckIfTableExistsSQL = "SELECT name FROM sqlite_master WHERE type='table' AND name='%1%';";

bool TableExists(sqlite3* db, const std::string& table_name) {
  bool table_exists = false;
  auto check_if_table_exists = [](void* table_exists, int count, char** data, char **columns) -> int {
    bool* result = (bool*) table_exists;
    *result = count > 0;
    return 0;
  };
  int rc = sqlite3_exec(db,
               (boost::format(kCheckIfTableExistsSQL) % table_name).str().c_str(),
               check_if_table_exists,
               &table_exists,
               NULL);
  if (rc) {
    std::stringstream err_msg;
    err_msg << "Failed to check if table exists, with error: " << sqlite3_errmsg(db);
    throw std::runtime_error(err_msg.str());
  }
  return table_exists;
}

void InitializeTable(sqlite3* db, const std::string& creation_sql) {
  auto unused_callback = [](void* unused, int count, char** data, char **columns) -> int {
    return 0;
  };
  int rc = sqlite3_exec(db, creation_sql.c_str(), unused_callback, 0, NULL);
  if (rc) {
    std::stringstream err_msg;
    err_msg << "Failed to initialize table, with error: " << sqlite3_errmsg(db);
    throw std::runtime_error(err_msg.str());
  }
  std::cout << "Initializing table!\n";
  return;
}

void InitializeTables(sqlite3* db) {
  for (const auto& table_pair : kTablesToCreationSQL) {
    if (!TableExists(db, table_pair.first)) {
      InitializeTable(db, table_pair.second);
    }
  }
}

sqlite3* InitializeDB() {
  sqlite3* db;
  int rc;
  rc = sqlite3_open(kDatabaseFileName, &db);
  if (rc) {
    std::cerr << "Failed to initialize Database: " << sqlite3_errmsg(db) << "\n";
  } else {
    std::cout << "Opened database successfully!\n";
  }

  try {
    InitializeTables(db);
  } catch(std::exception& e) {
    std::cerr << e.what() << "\n";
  }

  sqlite3_close(db);

  return db;
}

int main() {
  InitializeDB();
}
