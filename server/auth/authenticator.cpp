#include "server/auth/authenticator.hpp"
#include "server/auth/exceptions.hpp"

const char kSessionCookieName[] = "session-id";

Account& Authenticator::Authenticate(const http::request<http::string_body>& req) {
  for (auto& param : http::param_list(req[http::field::cookie])) {
    if (param.first.compare(kSessionCookieName) == 0) {
      return registry_.GetAccount(std::string(param.second));
    }
  }
  throw NotLoggedInException();
}
