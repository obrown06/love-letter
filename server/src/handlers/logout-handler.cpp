#include "handlers/logout-handler.hpp"

#include "auth/accounts-registry.hpp"
#include "auth/authenticator.hpp"
#include "auth/exceptions.hpp"
#include "auth/util.hpp"
#include "storage/exceptions.hpp"
#include "json-api/accounts.hpp"
#include "json-api/exceptions.hpp"

#include <sstream>

const char kRouteName[] = "/api/logout";

std::string LogoutHandler::GetRoute() const {
  return kRouteName;
}

http::response<http::string_body>
LogoutHandler::HandleRequest(const http::request<http::string_body>& req) {
  authenticator_->Authenticate(req);
  if (req.method() == http::verb::post) {
    return HandlePOST(req);
  }

  return MakeJsonHttpResponse(http::status::not_implemented, req, std::string());
}

http::response<http::string_body>
LogoutHandler::HandlePOST(const http::request<http::string_body>& req) {
  try {
    accounts_registry_->RemoveAccount(GetSessionCookie(req));
    return MakeJsonHttpResponse(http::status::ok, req, std::string());
  }
  catch (NotLoggedInException& e) {
    return MakeNotLoggedInResponse(req);
  }
  catch (NotFoundException& e) {
    return MakeJsonHttpResponse(http::status::bad_request, req, std::string("No account with matching username!"));
  }
}
