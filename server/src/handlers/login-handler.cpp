#include "handlers/login-handler.hpp"

#include "auth/accounts-registry.hpp"
#include "auth/authenticator.hpp"
#include "models/account.hpp"
#include "storage/exceptions.hpp"
#include "json-api/accounts.hpp"
#include "json-api/exceptions.hpp"

#include <sstream>

const char kRouteName[] = "/api/login";
const char kHomeRouteName[] = "/api/";

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
      return MakeJsonHttpResponse(http::status::bad_request, req, std::string("Incorrect password!"));
    }
    std::string session_key = accounts_registry_->InsertAccount(account);
    auto res = MakeJsonHttpResponse(http::status::ok, req, std::string("Success!"));
    res.set(http::field::set_cookie, "sessionid=" + session_key + "; Path=/");
    return res;
  }
  catch (InvalidJsonException& e) {
    return MakeJsonHttpResponse(http::status::bad_request, req, std::string("Invalid request format!"));
  }
  catch (NotFoundException& e) {
    return MakeJsonHttpResponse(http::status::bad_request, req, std::string("No account with matching username!"));
  }
}
