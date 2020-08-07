#include "util.h"

#include <sqlite3.h>
#include <sstream>

void MaybeThrowException(int sqlite_rc, char** sqlite_errmsg) {
  if (sqlite_rc == SQLITE_OK) {
    return;
  }
  std::stringstream err_msg;
  err_msg << "Storage failure: " << *sqlite_errmsg;
  sqlite3_free(*sqlite_errmsg);
  throw std::runtime_error(err_msg.str());
}
