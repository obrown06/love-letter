#include "handlers/util.hpp"
#include "handlers/accounts-handler.hpp"
#include "json-api/auth.hpp"

const char kDelineator = '/';

size_t GetLastIdxOfDelineator(const std::string& url, char delineator) {
  size_t last_idx = 0;
  for (size_t idx = 0; idx < url.size(); idx++) {
    if (url[idx] == delineator) {
      last_idx = idx;
    }
  }
  return last_idx;
}

std::string GetRoute(const std::string& url) {
  size_t last_idx = GetLastIdxOfDelineator(url, kDelineator);
  return url.substr(0, last_idx);
}

std::string GetTarget(const std::string& url) {
  size_t last_idx = GetLastIdxOfDelineator(url, kDelineator);
  return url.substr(last_idx + 1, url.size() - (last_idx + 1));
}

http::response<http::string_body>
MakeJsonHttpResponse(const http::status& status,
                     const http::request<http::string_body>& req,
                     const std::string& body)
{
  http::response<http::string_body> res{status, req.version()};
  res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
  res.set(http::field::content_type, "text/json");
  res.keep_alive(req.keep_alive());
  res.body() = body;
  res.prepare_payload();
  return res;
}

http::response<http::string_body>
MakeRedirectResponse(const http::request<http::string_body>& req,
                     const std::string& to)
{
  http::response<http::string_body> res{http::status::see_other, req.version()};
  res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
  res.set(http::field::location, to);
  res.keep_alive(req.keep_alive());
  res.prepare_payload();
  return res;
}

http::response<http::string_body>
MakeNotLoggedInResponse(const http::request<http::string_body>& req) {
  http::response<http::string_body> res{http::status::forbidden, req.version()};
  res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
  res.set(http::field::content_type, "text/json");
  res.keep_alive(req.keep_alive());
  res.body() = GetNotLoggedInJson();
  res.prepare_payload();
  return res;
}

http::response<http::string_body>
MakeJsonHttpResponseWithLoginCookie(const http::request<http::string_body>& req,
                                    const std::string& key) {
  auto res = MakeJsonHttpResponse(http::status::ok, req, std::string("Success!"));
  res.set(http::field::set_cookie, "sessionid=" + key + "; Path=/");
  return res;
}
