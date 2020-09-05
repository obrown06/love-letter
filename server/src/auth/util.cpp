#include "auth/util.hpp"
#include "auth/exceptions.hpp"

const std::string kHTTPParamStart = ";";
const char kSessionCookieName[] = "sessionid";

std::string SanitizeHTTPParam(const std::string& param) {
  if (param.size() > 0 && param[0] == kHTTPParamStart[0]) {
    return param;
  }
  return kHTTPParamStart + param;
}

std::string GetSessionCookie(const http::request<http::string_body>& req) {
  std::string sanitized_list = SanitizeHTTPParam(std::string(req[http::field::cookie]));
  for (const auto& param : http::param_list{sanitized_list}) {
    if (param.first.compare(kSessionCookieName) == 0) {
      return std::string(param.second);
    }
  }
  throw NotLoggedInException();
}
