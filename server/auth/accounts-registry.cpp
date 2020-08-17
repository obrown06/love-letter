#include "server/auth/accounts-registry.hpp"
#include "server/auth/exceptions.hpp"

Account& AccountsRegistry::GetAccount(const std::string& session_key) {
  if (!IsAccountRegistered(session_key)) {
    throw NotLoggedInException();
  }
  return signed_in_accounts_.at(session_key);
}

bool AccountsRegistry::IsAccountRegistered(const std::string& session_key) {
  return signed_in_accounts_.find(session_key) != signed_in_accounts_.end();
}

std::string AccountsRegistry::InsertAccount(const Account& account) {
  signed_in_accounts_.insert(std::pair<std::string, Account>("testtest", account));
  return "testtest";
}

void AccountsRegistry::RemoveAccount(const std::string& session_key) {
  signed_in_accounts_.erase(session_key);
}
