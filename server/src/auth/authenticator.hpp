#ifndef SERVER_AUTH_AUTHENTICATOR_H
#define SERVER_AUTH_AUTHENTICATOR_H

#include "auth/accounts-registry.hpp"

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

namespace beast = boost::beast;                 // from <boost/beast.hpp>
namespace http = beast::http;                   // from <boost/beast/http.hpp>

class Authenticator {
public:
  Authenticator(AccountsRegistry* accounts_registry) : registry_(accounts_registry) {};

  Account& Authenticate(const http::request<http::string_body>& req);
private:
  AccountsRegistry* registry_;
};

#endif
