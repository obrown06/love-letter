#ifndef SERVER_HANDLERS_LOGIN_HANDLER_H
#define SERVER_HANDLERS_LOGIN_HANDLER_H

#include "server/auth/authenticator.hpp"
#include "server/storage/storage.hpp"
#include "server/handlers/base-handler.hpp"
#include "server/handlers/util.hpp"

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

#include <string>

namespace beast = boost::beast;                 // from <boost/beast.hpp>
namespace http = beast::http;                   // from <boost/beast/http.hpp>

class LoginHandler : public BaseHandler {
  public:
    LoginHandler(Storage* storage,
                 Authenticator* authenticator,
                 AccountsRegistry* accounts_registry)
                 : storage_(storage),
                   authenticator_(authenticator),
                   accounts_registry_(accounts_registry) {}

    virtual std::string GetRoute() const;

    virtual http::response<http::string_body>
    HandleRequest(const http::request<http::string_body>& req);

  private:
    http::response<http::string_body> HandleGET(const http::request<http::string_body>& req);
    http::response<http::string_body> HandlePOST(const http::request<http::string_body>& req);
    Storage* storage_;
    Authenticator* authenticator_;
    AccountsRegistry* accounts_registry_;
};

#endif
