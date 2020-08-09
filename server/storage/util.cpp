#include "util.h"
#include "exceptions.h"

#include <sqlite3.h>
#include <sstream>

void ThrowSqliteExceptionIfError(int sqlite_rc, char** sqlite_errmsg) {
  if (sqlite_rc == SQLITE_OK) {
    return;
  }
  std::stringstream err_msg;
  err_msg << "Sqlite failure: " << *sqlite_errmsg;
  sqlite3_free(*sqlite_errmsg);
  throw SqliteException(err_msg.str());
}
