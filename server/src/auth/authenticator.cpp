#include "auth/authenticator.hpp"
#include "auth/exceptions.hpp"
#include "auth/util.hpp"

#include <iostream>

const char kSessionCookieName[] = "sessionid";

Account& Authenticator::Authenticate(const http::request<http::string_body>& req) {
  std::string sanitized_list = SanitizeHTTPParam(std::string(req[http::field::cookie]));
  for (const auto& param : http::param_list{sanitized_list}) {
    if (param.first.compare(kSessionCookieName) == 0) {
      return registry_->GetAccount(std::string(param.second));
    }
  }
  throw NotLoggedInException();
}
