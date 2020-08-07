#ifndef SERVER_STORAGE_UTIL_H
#define SERVER_STORAGE_UTIL_H

void MaybeThrowException(int sqlite_rc, char** sqlite_errmsg);

#endif
