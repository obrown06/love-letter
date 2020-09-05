#ifndef SERVER_AUTH_UTIL_HPP
#define SERVER_AUTH_UTIL_HPP

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

#include <string>

namespace beast = boost::beast;                 // from <boost/beast.hpp>
namespace http = beast::http;                   // from <boost/beast/http.hpp>

std::string SanitizeHTTPParam(const std::string& param);

std::string GetSessionCookie(const http::request<http::string_body>& req);

#endif
