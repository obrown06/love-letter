#include "handlers/logout-handler.hpp"

#include "auth/accounts-registry.hpp"
#include "auth/authenticator.hpp"
#include "auth/exceptions.hpp"
#include "auth/util.hpp"
#include "storage/exceptions.hpp"
#include "json-api/accounts.hpp"
#include "json-api/exceptions.hpp"

#include <sstream>
#include <iostream>

const char kRouteName[] = "/api/logout";

std::string LogoutHandler::GetRoute() const {
  return kRouteName;
}

http::response<http::string_body>
LogoutHandler::HandleRequest(const http::request<http::string_body>& req) {
  std::cout << "in logoutHandler handle request" << std::endl;
  authenticator_->Authenticate(req);
  if (req.method() == http::verb::post) {
    return HandlePOST(req);
  }

  return MakeJsonHttpResponse(http::status::not_implemented, req, std::string());
}

http::response<http::string_body>
LogoutHandler::HandlePOST(const http::request<http::string_body>& req) {
  std::cout << "logging out!" << std::endl;
  try {
    accounts_registry_->RemoveAccount(GetSessionCookie(req));
    return MakeJsonHttpResponse(http::status::ok, req, std::string());
  }
  catch (NotLoggedInException& e) {
    return MakeJsonHttpResponse(http::status::ok, req, std::string());
  }
  catch (NotFoundException& e) {
    return MakeJsonHttpResponse(http::status::ok, req, std::string());
  }
}
