#include "handlers/accounts-handler.hpp"

#include "auth/authenticator.hpp"
#include "models/account.hpp"
#include "storage/exceptions.hpp"
#include "json-api/accounts.hpp"
#include "json-api/auth.hpp"
#include "json-api/exceptions.hpp"

#include <sstream>
#include <iostream>

const char kRouteName[] = "/api/accounts";

std::string AccountsHandler::GetRoute() const {
  return kRouteName;
}

http::response<http::string_body>
AccountsHandler::HandleRequest(const http::request<http::string_body>& req) {
  if (req.method() == http::verb::get) {
    return HandleGET(req);
  } else if (req.method() == http::verb::post) {
    return HandlePOST(req);
  }

  return MakeJsonHttpResponse(http::status::not_implemented, req, std::string());
}

http::response<http::string_body>
AccountsHandler::HandleGET(const http::request<http::string_body>& req) {
  std::cout << "IN AccountsHandler::HandleGET" << std::endl;
  authenticator_->Authenticate(req);
  std::string body;
  http::status status;
  try {
    std::string target = GetTarget(std::string(req.target()));
    std::unique_ptr<std::vector<Account>> accounts_vec;
    if (target.empty()) {
      accounts_vec = storage_->LoadAllAccounts();
    } else {
      accounts_vec = std::make_unique<std::vector<Account>>();
      accounts_vec->push_back(storage_->LoadAccount(target));
    }
    body = AccountsToJSON(*accounts_vec);
    std::cout << "Returning accounts!" << std::endl;
    return MakeJsonHttpResponse(http::status::ok, req, body);
  }
  catch (NotFoundException& e) {
    return MakeJsonHttpResponse(http::status::not_found, req, std::string("No account found!"));
  }
  catch (StorageException& e) {
    return MakeJsonHttpResponse(http::status::internal_server_error, req, std::string());
  }
}

http::response<http::string_body>
AccountsHandler::HandlePOST(const http::request<http::string_body>& req) {
  try {
    Account account = JSONToAccount(req.body());
    storage_->InsertAccount(account);
    std::string session_key = accounts_registry_->InsertAccount(account);
    return MakeJsonHttpResponseWithLoginCookie(req, session_key);
  }
  catch (InvalidJsonException& e) {
    return MakeJsonHttpResponse(http::status::bad_request, req, std::string("Invalid request format!"));
  }
  catch (StorageException& e) {
    return MakeJsonHttpResponse(http::status::bad_request, req, GetPreexistingAccountJson());
  }
}
