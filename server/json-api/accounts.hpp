#ifndef SERVER_JSON_API_ACCOUNTS_H
#define SERVER_JSON_API_ACCOUNTS_H

#include "server/models/account.hpp"

#include <string>
#include <vector>

std::string AccountsToJSON(const std::vector<Account>& accounts);

Account JSONToAccount(const std::string& json);

#endif
