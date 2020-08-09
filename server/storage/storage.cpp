#include "server/storage/storage.hpp"
#include "server/storage/db_init.hpp"
#include "server/storage/util.hpp"
#include "server/storage/exceptions.hpp"

#include <iostream>
#include <boost/format.hpp>

const char kDatabaseFileName[] = "LoveLetter.db";
const char kInsertOrReplaceAccountSQL[] = "INSERT OR REPLACE INTO ACCOUNTS (USERNAME, PASSWORD, EMAIL) " \
                                      "VALUES ('%1%', '%2%', '%3%');";
const char kLoadAccountByUsernameSQL[] = "SELECT * FROM ACCOUNTS WHERE USERNAME='%1%';";

namespace {

static int unused_callback(void *NotUsed, int argc, char **argv, char **colname) {
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
  char* err_msg;
  std::string sql = (boost::format(kInsertOrReplaceAccountSQL)
                      % account.GetUsername()
                      % account.GetPassword()
                      % account.GetEmail()).str();
  ThrowSqliteExceptionIfError(sqlite3_exec(database_,
                                  sql.c_str(),
                                  unused_callback,
                                  0,
                                  &err_msg),
                      &err_msg);
}

Account Storage::LoadAccount(const std::string& username) {
  std::vector<Account> accounts;
  auto load_all_accounts = [](void* accounts, int count, char** data, char **columns) -> int {
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
  };
  char* err_msg;
  std::string sql = (boost::format(kLoadAccountByUsernameSQL) % username).str();
  ThrowSqliteExceptionIfError(sqlite3_exec(database_,
                                  sql.c_str(),
                                  load_all_accounts,
                                  &accounts,
                                  &err_msg),
                      &err_msg);

  if (accounts.empty()) {
    throw NotFoundException("No account found with username!");
  } else if (accounts.size() > 1) {
    throw std::runtime_error("More than one account found with username!");
  }

  return accounts[0];
}

int main() {
  try {
    Account account("nick", "nick_pass", "nick@gmail.com");
    Storage storage(kDatabaseFileName);
    storage.InsertOrUpdateAccount(account);
    Account loaded_account = storage.LoadAccount("nick");
    std::cout << "Found account with username: " << loaded_account.GetUsername() << " and password: " << loaded_account.GetPassword() << " and email: " << loaded_account.GetEmail() << "\n";
  } catch (std::exception& e) {
    std::cerr << e.what() << "\n";
  }
  return 0;
}
