#ifndef SERVER_HANDLERS_BASE_HANDLER_H
#define SERVER_HANDLERS_BASE_HANDLER_H

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>

#include <string>

namespace beast = boost::beast;                 // from <boost/beast.hpp>
namespace http = beast::http;                   // from <boost/beast/http.hpp>

class BaseHandler {
public:
  virtual std::string GetRoute() const = 0;
  virtual http::response<http::string_body>
  HandleRequest(const http::request<http::string_body>& req) = 0;
};

#endif
