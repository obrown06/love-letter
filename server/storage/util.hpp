#ifndef SERVER_STORAGE_UTIL_H
#define SERVER_STORAGE_UTIL_H

#include "server/storage/exceptions.hpp"

#include <functional>
#include <sstream>
#include <string>
#include <sqlite3.h>

template <typename Callback, typename CallbackArg>
void ExecuteSql(const std::string& sql,
                const Callback& callback,
                sqlite3* database,
                CallbackArg* callback_arg)
{
  char* sqlite_errmsg;
  int rc = sqlite3_exec(database, sql.c_str(), callback, callback_arg, &sqlite_errmsg);
  if (rc != SQLITE_OK) {
    std::stringstream err_msg;
    err_msg << "Sqlite failure: " << sqlite_errmsg;
    sqlite3_free(sqlite_errmsg);
    throw SqliteException(err_msg.str());
  }
}

#endif
