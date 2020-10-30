#include "handlers/login-handler.hpp"

#include "auth/accounts-registry.hpp"
#include "auth/authenticator.hpp"
#include "json-api/accounts.hpp"
#include "json-api/auth.hpp"
#include "json-api/exceptions.hpp"
#include "models/account.hpp"
#include "storage/exceptions.hpp"

#include <sstream>

const char kRouteName[] = "/api/login";

std::string LoginHandler::GetRoute() const {
  return kRouteName;
}

http::response<http::string_body>
LoginHandler::HandleRequest(const http::request<http::string_body>& req) {
  if (req.method() == http::verb::post) {
    return HandlePOST(req);
  }

  return MakeJsonHttpResponse(http::status::not_implemented, req, std::string());
}

http::response<http::string_body>
LoginHandler::HandlePOST(const http::request<http::string_body>& req) {
  try {
    std::pair<std::string, std::string> username_password_pair = JSONToUsernamePasswordPair(req.body());
    Account account = storage_->LoadAccount(username_password_pair.first);
    if (account.GetPassword().compare(username_password_pair.second) != 0) {
      return MakeJsonHttpResponse(http::status::bad_request, req, GetIncorrectPasswordJson());
    }
    std::string session_key = accounts_registry_->InsertAccount(account);
    return MakeJsonHttpResponseWithLoginCookie(req, session_key);
  }
  catch (InvalidJsonException& e) {
    return MakeJsonHttpResponse(http::status::bad_request, req, std::string("Invalid request format!"));
  }
  catch (NotFoundException& e) {
    return MakeJsonHttpResponse(http::status::bad_request, req, GetNoMatchingUsernameJson());
  }
}
