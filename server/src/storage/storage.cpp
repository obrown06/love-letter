#include "storage/storage.hpp"
#include "storage/db_init.hpp"
#include "storage/util.hpp"
#include "storage/exceptions.hpp"

#include <iostream>
#include <boost/format.hpp>

const char kDatabaseFileName[] = "LoveLetter.db";
const char kInsertOrReplaceAccountSQL[] = "INSERT OR REPLACE INTO ACCOUNTS (USERNAME, PASSWORD, EMAIL) " \
                                      "VALUES ('%1%', '%2%', '%3%');";
const char kInsertAccountSQL[] = "INSERT INTO ACCOUNTS (USERNAME, PASSWORD, EMAIL) " \
                                      "VALUES ('%1%', '%2%', '%3%');";
const char kLoadAccountByUsernameSQL[] = "SELECT * FROM ACCOUNTS WHERE USERNAME='%1%';";
const char kLoadAllAccounts[] = "SELECT * FROM ACCOUNTS;";

namespace {

int unused_callback(void *NotUsed, int argc, char **argv, char **colname) {
  return 0;
}

int load_all_accounts_callback(void* accounts, int count, char** data, char** columns) {
  std::vector<Account>* accounts_vec = (std::vector<Account>*) accounts;
  std::string username, password, email;
  for (size_t i = 0; i < count; i++) {
    if (!strcmp(columns[i], "USERNAME")) {
      username = data[i];
    } else if (!strcmp(columns[i], "PASSWORD")) {
      password = data[i];
    } else {
      email = data[i];
    }
  }
  accounts_vec->push_back(Account(username, password, email));
  return 0;
}

}  // namespace

Storage::Storage(const std::string& file_path) {
  database_ = InitializeDB(file_path);
}

Storage::~Storage() {
  std::cout << "Closing database\n";
  sqlite3_close(database_);
}

void Storage::InsertOrUpdateAccount(const Account& account) {
  std::string sql = (boost::format(kInsertOrReplaceAccountSQL)
                      % account.GetUsername()
                      % account.GetPassword()
                      % account.GetEmail()).str();
  int unused;
  ExecuteSql(sql, unused_callback, database_, &unused);
}

void Storage::InsertAccount(const Account& account) {
  std::string sql = (boost::format(kInsertAccountSQL)
                      % account.GetUsername()
                      % account.GetPassword()
                      % account.GetEmail()).str();
  int unused;
  ExecuteSql(sql, unused_callback, database_, &unused);
}

Account Storage::LoadAccount(const std::string& username) {
  std::vector<Account> accounts;
  std::string sql = (boost::format(kLoadAccountByUsernameSQL) % username).str();
  ExecuteSql(sql, load_all_accounts_callback, database_, &accounts);
  if (accounts.empty()) {
    throw NotFoundException("No account found with username!");
  } else if (accounts.size() > 1) {
    throw std::runtime_error("More than one account found with username!");
  }

  return accounts[0];
}

std::unique_ptr<std::vector<Account>> Storage::LoadAllAccounts() {
  auto accounts = std::make_unique<std::vector<Account>>();
  ExecuteSql(std::string(kLoadAllAccounts),
             load_all_accounts_callback,
             database_,
             accounts.get());
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
