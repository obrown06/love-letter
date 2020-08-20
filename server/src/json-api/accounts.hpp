#ifndef SERVER_JSON_API_ACCOUNTS_H
#define SERVER_JSON_API_ACCOUNTS_H

#include "models/account.hpp"

#include <string>
#include <vector>

std::string AccountsToJSON(const std::vector<Account>& accounts);

Account JSONToAccount(const std::string& json);

std::pair<std::string, std::string> JSONToUsernamePasswordPair(const std::string& json);

#endif
