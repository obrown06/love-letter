#ifndef SERVER_STORAGE_DB_INIT_H
#define SERVER_STORAGE_DB_INIT_H

#include <sqlite3.h>
#include <string>

sqlite3* InitializeDB(const std::string& file_path);

#endif
