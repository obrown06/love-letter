#ifndef SERVER_AUTH_ACCOUNTS_REGISTRY_H
#define SERVER_AUTH_ACCOUNTS_REGISTRY_H

#include "server/models/account.hpp"

#include <map>
#include <string>

class AccountsRegistry {
  public:
    AccountsRegistry() {};

    Account& GetAccount(const std::string& session_key);
    bool IsAccountRegistered(const std::string& session_key);
    std::string InsertAccount(const Account& account);
    void RemoveAccount(const std::string& session_key);
  private:
    std::map<std::string, Account> signed_in_accounts_;
};

#endif
