#include "auth/authenticator.hpp"
#include "auth/exceptions.hpp"
#include "auth/util.hpp"

const char kSessionCookieName[] = "sessionid";

Account Authenticator::Authenticate(const http::request<http::string_body>& req) {
  return registry_->GetAccount(GetSessionCookie(req));
}
