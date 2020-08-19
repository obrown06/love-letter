#include "server/auth/accounts-registry.hpp"
#include "server/auth/exceptions.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>

Account& AccountsRegistry::GetAccount(const std::string& session_key) {
  try {
    boost::uuids::uuid uuid = boost::lexical_cast<boost::uuids::uuid>(session_key);
    if (!IsAccountRegistered(uuid)) {
      throw NotLoggedInException();
    }
    return signed_in_accounts_.at(uuid);
  }
  catch (std::exception& e){
    throw NotLoggedInException();
  }
}

bool AccountsRegistry::IsAccountRegistered(const boost::uuids::uuid& session_uuid) {
  return signed_in_accounts_.find(session_uuid) != signed_in_accounts_.end();
}

std::string AccountsRegistry::InsertAccount(const Account& account) {
  boost::uuids::basic_random_generator<boost::mt19937> gen;
  boost::uuids::uuid uuid = gen();
  signed_in_accounts_.insert(std::pair<boost::uuids::uuid, Account>(uuid, account));
  return boost::uuids::to_string(uuid);
}

void AccountsRegistry::RemoveAccount(const std::string& session_key) {
  boost::uuids::uuid uuid = boost::lexical_cast<boost::uuids::uuid>(session_key);
  signed_in_accounts_.erase(uuid);
}
