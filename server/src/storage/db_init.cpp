#include "storage/db_init.hpp"
#include "storage/util.hpp"
#include "storage/exceptions.hpp"

#include <iostream>
#include <regex>
#include <stdexcept>
#include <sstream>
#include <string>
#include <vector>
#include <boost/format.hpp>
#include <pqxx/pqxx>


const std::vector<std::pair<std::string, std::string>> kTablesToCreationSQL = {
  {
    "ACCOUNTS",
    "CREATE TABLE IF NOT EXISTS ACCOUNTS(" \
      "USERNAME TEXT  PRIMARY KEY    NOT NULL," \
      "PASSWORD TEXT                 NOT NULL," \
      "EMAIL    TEXT                 NOT NULL," \
      "WINS     INT                  DEFAULT 0," \
      "LOSSES   INT                  DEFAULT 0," \
      "POINTS   INT                  DEFAULT 0);"
  },
};

constexpr char kDatabaseUrlEnvVarName = "DATABASE_URL";
constexpr char kDatabaseUrlRegex[] = "postgres://([^:]*):([^@]*)@([^:]*):([^/]*)/(.*)";

void InitializePostgresTable(pqxx::connection& conn, const std::string& creation_sql) {
  pqxx::work W(conn);
  W.exec(creation_sql);
  W.commit();
  std::cout << "Initializing table!\n";
  return;
}

void InitializePostgresTables(pqxx::connection& conn) {
  for (const auto& table_pair : kTablesToCreationSQL) {
    InitializePostgresTable(conn, table_pair.second);
  }
}

std::vector<std::string> ParseDatabaseUrl() {
  std::cout << "url is: " << std::getenv(kDatabaseUrlEnvVarName) << std::endl;
  std::string url = std::string(std::getenv(kDatabaseUrlEnvVarName));
  std::regex re(kDatabaseUrlRegex);

  std::vector<std::string> parsed;

  std::smatch m;
  std::regex_search(url, m, re);
  std::cout << "match size = " << m.size() << std::endl;
  for (int i = 1; i < 6; i++) {
    std::cout << "pushing back: " << m.str(i) << std::endl;
    parsed.push_back(m.str(i));
  }
  return parsed;
}

std::unique_ptr<pqxx::connection> InitializePostgres(const std::string& name) {
  std::cout << "name is: " << name << std::endl;
  try {
    std::vector<std::string> parsed = ParseDatabaseUrl();

    auto conn = std::make_unique<pqxx::connection>(
      "user = " + parsed[0] + \
      " password = " + parsed[1] + \
      " host = " + parsed[2] + \
      " port = " + parsed[3] + \
      " dbname = " + parsed[4]);
    if (conn->is_open()) {
      std::cout << "Opened database successfully!" << std::endl;
    } else {
      throw std::runtime_error("Can't open database!");
    }
    InitializePostgresTables(*conn);
    std::cout << "Initialized db!" << std::endl;
    return conn;
  } catch(const std::exception& e) {
    std::cerr << e.what() << "\n";
  }
}
