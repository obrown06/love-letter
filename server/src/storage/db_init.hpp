#ifndef SERVER_STORAGE_DB_INIT_H
#define SERVER_STORAGE_DB_INIT_H

#include <string>
#include <memory>
#include <pqxx/pqxx>

std::unique_ptr<pqxx::connection> InitializePostgres(const std::string& name);

#endif
