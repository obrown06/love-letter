#include "server/handlers/accounts-handler.hpp"
#include "server/models/account.hpp"
#include "server/storage/exceptions.hpp"

#include <json/json.h>

#include <sstream>
#include <iostream>

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
    return std::make_pair(http::status::not_found, std::string("No account found!"));
  }
  catch (StorageException& e) {
    return std::make_pair(http::status::internal_server_error, body.str());
  }
  return std::make_pair(http::status::ok, body.str());
}

std::pair<http::status, const std::string>
AccountsHandler::HandlePOST(const std::string& body) {
  try {
    Json::Value json;
    Json::CharReaderBuilder builder;
    Json::CharReader * reader = builder.newCharReader();
    std::string errors;
    bool parsingSuccessful = reader->parse(body.c_str(), body.c_str() + body.size(), &json, &errors);
    std::cout << "SUCCESS? " << parsingSuccessful << "\n";
    Account account(json["username"].asString(), json["password"].asString(), json["email"].asString());
    storage_->InsertAccount(account);
  }
  catch (StorageException& e) {
    return std::make_pair(http::status::bad_request, std::string("Account with username already exists!"));
  }
  return std::make_pair<http::status, const std::string>(http::status::ok, std::string());
}
