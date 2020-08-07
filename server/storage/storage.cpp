#include "storage.h"
#include "db_init.h"
#include "util.h"

#include <iostream>
#include <boost/format.hpp>

const char kDatabaseFileName[] = "LoveLetter.db";
const char kInsertOrReplaceAccountSQL[] = "INSERT OR REPLACE INTO ACCOUNTS (USERNAME, PASSWORD, EMAIL) " \
                                      "VALUES ('%1%', '%2%', '%3%');";
const char kLoadAccountByUsernameSQL[] = "SELECT * FROM ACCOUNTS WHERE 'USERNAME' == '%1%'";
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
  MaybeThrowException(sqlite3_exec(database_,
                                  (boost::format(kInsertOrReplaceAccountSQL) % account.GetUsername() % account.GetPassword() % account.GetEmail()).str().c_str(),
                                  unused_callback,
                                  0,
                                  &err_msg),
                      &err_msg);
}

/*Account Storage::LoadAccount(const std::string& username) {
  char* err_msg;
  MaybeThrowException(sqlite3_exec(database_,
                                  (boost::format(kLoadAccountByUsernameSQL) % username).str().c_str(),
                                  unused_callback,
                                  0,
                                  &err_msg),
                      &err_msg);
}*/

int main() {
  try {
    Account account("nick", "nick_pass", "nick@gmail.com");
    Storage storage(kDatabaseFileName);
    storage.InsertOrUpdateAccount(account);
  } catch (std::exception& e) {
    std::cerr << e.what();
  }
  return 0;
}
