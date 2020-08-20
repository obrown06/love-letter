#ifndef SERVER_AUTH_ACCOUNTS_REGISTRY_H
#define SERVER_AUTH_ACCOUNTS_REGISTRY_H

#include "models/account.hpp"
#include <boost/functional/hash.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

#include <unordered_map>
#include <string>

class AccountsRegistry {
  public:
    AccountsRegistry() {};

    Account& GetAccount(const std::string& session_key);
    std::string InsertAccount(const Account& account);
    void RemoveAccount(const std::string& session_key);
  private:
    bool IsAccountRegistered(const boost::uuids::uuid& session_key);
    std::unordered_map<boost::uuids::uuid, Account, boost::hash<boost::uuids::uuid>> signed_in_accounts_;
};

#endif
