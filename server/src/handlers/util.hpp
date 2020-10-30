#ifndef SERVER_HANDLERS_UTIL_H
#define SERVER_HANDLERS_UTIL_H

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

#include <string>

namespace beast = boost::beast;                 // from <boost/beast.hpp>
namespace http = beast::http;                   // from <boost/beast/http.hpp>

std::string GetRoute(const std::string& url);

std::string GetTarget(const std::string& url);

http::response<http::string_body>
MakeJsonHttpResponse(const http::status& status,
                     const http::request<http::string_body>& req,
                     const std::string& body);

http::response<http::string_body>
MakeRedirectResponse(const http::request<http::string_body>& req,
                     const std::string& to);

http::response<http::string_body>
MakeNotLoggedInResponse(const http::request<http::string_body>& req);

http::response<http::string_body>
MakeJsonHttpResponseWithLoginCookie(const http::request<http::string_body>& req,
                                    const std::string& key);

#endif
