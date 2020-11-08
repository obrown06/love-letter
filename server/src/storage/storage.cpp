#include "storage/storage.hpp"
#include "storage/db_init.hpp"
#include "storage/util.hpp"
#include "storage/exceptions.hpp"

#include <boost/format.hpp>
#include <cstring>
#include <iostream>
#include <stdlib.h>

const char kInsertOrReplaceAccountSQL[] = "INSERT INTO ACCOUNTS (USERNAME, PASSWORD, EMAIL, WINS, LOSSES, POINTS) " \
                                      "VALUES ('%1%', '%2%', '%3%', %4%, %5%, %6%)" \
                                      "ON CONFLICT(USERNAME) DO UPDATE " \
                                      "SET PASSWORD = excluded.PASSWORD, " \
                                      "    EMAIL    = excluded.EMAIL, " \
                                      "    WINS     = excluded.WINS, " \
                                      "    LOSSES   = excluded.LOSSES, " \
                                      "    POINTS   = excluded.POINTS;";
const char kInsertAccountSQL[] = "INSERT INTO ACCOUNTS (USERNAME, PASSWORD, EMAIL, WINS, LOSSES, POINTS) " \
                                      "VALUES ('%1%', '%2%', '%3%', %4%, %5%, %6%);";
const char kLoadAccountByUsernameSQL[] = "SELECT * FROM ACCOUNTS WHERE USERNAME='%1%';";
const char kLoadAllAccounts[] = "SELECT * FROM ACCOUNTS;";

Storage::Storage(const std::string& name) : database_(std::move(InitializePostgres(name))) {}

void Storage::InsertOrUpdateAccount(const Account& account) {
  std::cout << "inserting account " << account.GetUsername() << std::endl;
  pqxx::work txn(*database_);
  std::string sql = (boost::format(kInsertOrReplaceAccountSQL)
  % account.GetUsername()
  % account.GetPassword()
  % account.GetEmail()
  % account.GetWins()
  % account.GetLosses()
  % account.GetPoints()).str();
  txn.exec(sql);
  txn.commit();
}

void Storage::InsertAccount(const Account& account) {
  pqxx::work txn(*database_);
  std::string sql = (boost::format(kInsertAccountSQL)
                      % account.GetUsername()
                      % account.GetPassword()
                      % account.GetEmail()
                      % account.GetWins()
                      % account.GetLosses()
                      % account.GetPoints()).str();
  txn.exec(sql);
  txn.commit();
}

Account Storage::LoadAccount(const std::string& username) {
  std::cout << "loading account for username: " << username << std::endl;
  std::vector<Account> accounts;
  pqxx::work txn(*database_);
  std::string sql = (boost::format(kLoadAccountByUsernameSQL) % username).str();
  pqxx::result R( txn.exec( sql ));
  for (auto row : R) {
    std::cout << "got row" << std::endl;
    accounts.push_back(Account(row["USERNAME"].c_str(),
                               row["PASSWORD"].c_str(),
                               row["EMAIL"].c_str(),
                               row["WINS"].as<int>(),
                               row["LOSSES"].as<int>(),
                               row["POINTS"].as<int>()));
  }
  if (accounts.empty()) {
    throw NotFoundException("No account found with username!");
  } else if (accounts.size() > 1) {
    throw std::runtime_error("More than one account found with username!");
  }
  return accounts[0];
}


std::unique_ptr<std::vector<Account>> Storage::LoadAllAccounts() {
  auto accounts = std::make_unique<std::vector<Account>>();
  pqxx::work txn(*database_);
  pqxx::result R( txn.exec( kLoadAllAccounts ));
  for (auto row : R) {
    accounts->push_back(Account(row["USERNAME"].c_str(),
                               row["PASSWORD"].c_str(),
                               row["EMAIL"].c_str(),
                               row["WINS"].as<int>(),
                               row["LOSSES"].as<int>(),
                               row["POINTS"].as<int>()));
  }
  return accounts;
}

/*int main() {
  try {
    Account account("nick", "nick_pass", "nick@gmail.com");
    Storage storage(kDatabaseFileName);
    storage.InsertOrUpdateAccount(account);
    Account loaded_account = storage.LoadAccount("nick");
    std::cout << "Found account with username: " << loaded_account.GetUsername() << " and password: " << loaded_account.GetPassword() << " and email: " << loaded_account.GetEmail() << "\n";
    std::unique_ptr<std::vector<Account>> all_accounts = storage.LoadAllAccounts();
    std::cout << "Number of accounts: " << all_accounts->size() << "\n";
  } catch (std::exception& e) {
    std::cerr << e.what() << "\n";
  }
  return 0;
}*/
