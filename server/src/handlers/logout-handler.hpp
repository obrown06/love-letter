#ifndef SERVER_HANDLERS_LOGOUT_HANDLER_H
#define SERVER_HANDLERS_LOGOUT_HANDLER_H

#include "auth/authenticator.hpp"
#include "handlers/base-handler.hpp"
#include "handlers/util.hpp"

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

#include <string>

namespace beast = boost::beast;                 // from <boost/beast.hpp>
namespace http = beast::http;                   // from <boost/beast/http.hpp>

class LogoutHandler : public BaseHandler {
  public:
    LogoutHandler(Authenticator* authenticator,
                 AccountsRegistry* accounts_registry)
                 : authenticator_(authenticator),
                   accounts_registry_(accounts_registry) {}

    virtual std::string GetRoute() const;

    virtual http::response<http::string_body>
    HandleRequest(const http::request<http::string_body>& req);

  private:
    http::response<http::string_body> HandlePOST(const http::request<http::string_body>& req);
    Authenticator* authenticator_;
    AccountsRegistry* accounts_registry_;
};

#endif
