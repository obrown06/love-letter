#ifndef SERVER_STORAGE_STORAGE_H
#define SERVER_STORAGE_STORAGE_H

#include "server/models/account.hpp"

#include <memory>
#include <string>
#include <sqlite3.h>

class Storage {
 public:
  explicit Storage(const std::string& db_path);
  Storage(const Storage&) = delete;
  Storage& operator=(const Storage&) = delete;

  ~Storage();

  // Inserts or updates the database row with `username` as its primary key.
  void InsertOrUpdateAccount(const Account& account);

  Account LoadAccount(const std::string& username);

  std::unique_ptr<std::vector<Account>> LoadAllAccounts();

 private:
  sqlite3* database_;
};

#endif
