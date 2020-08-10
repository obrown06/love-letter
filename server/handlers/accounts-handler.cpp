#include "server/handlers/accounts-handler.hpp"
#include "server/models/account.hpp"
#include "server/storage/exceptions.hpp"

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
  }

  throw std::runtime_error("bad");
}

std::pair<http::status, const std::string>
AccountsHandler::HandleGET(const std::string& target) {
  std::stringstream body;
  try {
    if (target.empty()) {
      auto accounts_vec = storage_->LoadAllAccounts();
      for (const auto& account : *accounts_vec) {
        body << "{\n\t\t" << "username: " << account.GetUsername() << "\n}\n";
      }
    } else {
      Account account = storage_->LoadAccount(target);
      body << "{\n\t\t" << "username: " << account.GetUsername() << "\n}\n";
    }
  }
  catch (NotFoundException& e) {
    return std::make_pair(http::status::not_found, body.str());
  }
  catch (StorageException& e) {
    return std::make_pair(http::status::internal_server_error, body.str());
  }
  return std::make_pair<http::status, const std::string>(http::status::ok, body.str());
}
