#ifndef SERVER_STORAGE_STORAGE_H
#define SERVER_STORAGE_STORAGE_H

#include "models/account.hpp"

#include <memory>
#include <pqxx/pqxx>
#include <string>
#include <sqlite3.h>
#include <utility>
#include <vector>

class Storage {
 public:
  explicit Storage(const std::string& name);
  Storage(const Storage&) = delete;
  Storage& operator=(const Storage&) = delete;

  ~Storage() {};

  // Inserts a new database row with `account.GetUsername()` as its primary key.
  void InsertAccount(const Account& account);

  // Inserts or updates the database row with `account.GetUsername()` as its
  // primary key.
  void InsertOrUpdateAccount(const Account& account);

  // Loads the account with matching `username`. If no account is found, throws
  // NotFoundException.
  Account LoadAccount(const std::string& username);

  // Loads all accounts in the DB.
  std::unique_ptr<std::vector<Account>> LoadAllAccounts();

 private:
  std::unique_ptr<pqxx::connection> database_;
};

#endif
