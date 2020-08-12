#include "server/handlers/accounts-handler.hpp"
#include "server/models/account.hpp"
#include "server/storage/exceptions.hpp"
#include "server/json-api/accounts.hpp"
#include "server/json-api/exceptions.hpp"

#include <sstream>

const char kRouteName[] = "/accounts";

std::string AccountsHandler::GetRoute() const {
  return kRouteName;
}

std::pair<http::status, const std::string>
AccountsHandler::HandleRequest(const std::string& target,
                               const http::verb& method,
                               const std::string& body) {
  if (method == http::verb::get) {
    return HandleGET(target);
  } else if (method == http::verb::post) {
    return HandlePOST(body);
  }

  return std::make_pair(http::status::not_implemented, std::string());
}

std::pair<http::status, const std::string>
AccountsHandler::HandleGET(const std::string& target) {
  std::string body;
  try {
    std::unique_ptr<std::vector<Account>> accounts_vec;
    if (target.empty()) {
      accounts_vec = storage_->LoadAllAccounts();
    } else {
      accounts_vec = std::make_unique<std::vector<Account>>();
      accounts_vec->push_back(storage_->LoadAccount(target));
    }
    body = AccountsToJSON(*accounts_vec);
  }
  catch (NotFoundException& e) {
    return std::make_pair(http::status::not_found, std::string("No account found!"));
  }
  catch (StorageException& e) {
    return std::make_pair(http::status::internal_server_error, body);
  }
  return std::make_pair(http::status::ok, body);
}

std::pair<http::status, const std::string>
AccountsHandler::HandlePOST(const std::string& body) {
  try {
    Account account = JSONToAccount(body);
    storage_->InsertAccount(account);
  }
  catch (InvalidJsonException& e) {
    return std::make_pair(http::status::bad_request, std::string("Invalid request format!"));
  }
  catch (StorageException& e) {
    return std::make_pair(http::status::bad_request, std::string("Account with username already exists!"));
  }
  return std::make_pair<http::status, const std::string>(http::status::ok, std::string());
}
