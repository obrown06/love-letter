#include "server/handlers/login-handler.hpp"

#include "server/auth/accounts-registry.hpp"
#include "server/auth/authenticator.hpp"
#include "server/models/account.hpp"
#include "server/storage/exceptions.hpp"
#include "server/json-api/accounts.hpp"
#include "server/json-api/exceptions.hpp"

#include <sstream>

const char kRouteName[] = "/login";
const char kHomeRouteName[] = "/";

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
    accounts_registry_->InsertAccount(account);
  }
  catch (InvalidJsonException& e) {
    return MakeJsonHttpResponse(http::status::bad_request, req, std::string("Invalid request format!"));
  }
  catch (NotFoundException& e) {
    return MakeJsonHttpResponse(http::status::bad_request, req, std::string("No account with matching username!"));
  }
  auto res = MakeRedirectResponse(req, kHomeRouteName);
  res.set(http::field::set_cookie, "sessionid=testtest");
  return res;
}
