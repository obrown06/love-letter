#include "auth/authenticator.hpp"
#include "auth/exceptions.hpp"

#include <iostream>

const char kSessionCookieName[] = "sessionid";

Account& Authenticator::Authenticate(const http::request<http::string_body>& req) {
  for (const auto& param : http::param_list{req[http::field::cookie]}) {
    if (param.first.compare(kSessionCookieName) == 0) {
      return registry_->GetAccount(std::string(param.second));
    }
  }
  throw NotLoggedInException();
}
