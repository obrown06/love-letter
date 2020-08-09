#ifndef SERVER_STORAGE_UTIL_H
#define SERVER_STORAGE_UTIL_H

#include <string>

void ThrowSqliteExceptionIfError(int sqlite_rc, char** sqlite_errmsg);

#endif
