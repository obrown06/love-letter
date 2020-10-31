#include "storage/db_init.hpp"
#include "storage/util.hpp"
#include "storage/exceptions.hpp"

#include <iostream>
#include <stdexcept>
#include <sstream>
#include <string>
#include <vector>
#include <boost/format.hpp>


const std::vector<std::pair<std::string, std::string>> kTablesToCreationSQL = {
  {
    "ACCOUNTS",
    "CREATE TABLE ACCOUNTS(" \
      "USERNAME TEXT  PRIMARY KEY    NOT NULL," \
      "PASSWORD TEXT                 NOT NULL," \
      "EMAIL    TEXT                 NOT NULL," \
      "WINS     INT                  DEFAULT 0," \
      "LOSSES   INT                  DEFAULT 0," \
      "POINTS   INT                  DEFAULT 0);"
  },
};

const std::string kCheckIfTableExistsSQL = "SELECT name FROM sqlite_master WHERE type='table' AND name='%1%';";

bool TableExists(sqlite3* db, const std::string& table_name) {
  bool table_exists = false;
  auto check_if_table_exists = [](void* table_exists, int count, char** data, char **columns) -> int {
    bool* result = (bool*) table_exists;
    *result = count > 0;
    return 0;
  };
  std::string sql = (boost::format(kCheckIfTableExistsSQL) % table_name).str();
  ExecuteSql(sql, check_if_table_exists, db, &table_exists);
  return table_exists;
}

void InitializeTable(sqlite3* db, const std::string& creation_sql) {
  auto unused_callback = [](void* unused, int count, char** data, char **columns) -> int {
    return 0;
  };
  int unused;
  ExecuteSql(creation_sql, unused_callback, db, &unused);
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

sqlite3* InitializeDB(const std::string& file_path) {
  sqlite3* db;
  int rc;
  rc = sqlite3_open(file_path.c_str(), &db);
  if (rc != SQLITE_OK) {
    std::cerr << "Failed to initialize Database: " << sqlite3_errmsg(db) << "\n";
  } else {
    std::cout << "Opened database successfully!\n";
  }

  try {
    InitializeTables(db);
  } catch(SqliteException& e) {
    std::cerr << e.what() << "\n";
  }

  return db;
}
