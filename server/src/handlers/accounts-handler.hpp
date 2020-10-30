#ifndef SERVER_HANDLERS_ACCOUNTS_HANDLER_H
#define SERVER_HANDLERS_ACCOUNTS_HANDLER_H

#include "auth/authenticator.hpp"
#include "storage/storage.hpp"
#include "handlers/base-handler.hpp"
#include "handlers/util.hpp"

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

#include <string>

namespace beast = boost::beast;                 // from <boost/beast.hpp>
namespace http = beast::http;                   // from <boost/beast/http.hpp>

class AccountsHandler : public BaseHandler {
  public:
    AccountsHandler(Storage* storage,
                    Authenticator* authenticator,
                    AccountsRegistry* registry) : storage_(storage),
                                                  authenticator_(authenticator),
                                                  accounts_registry_(registry) {}

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
