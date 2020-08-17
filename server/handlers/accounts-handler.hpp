#ifndef SERVER_HANDLERS_ACCOUNTS_HANDLER_H
#define SERVER_HANDLERS_ACCOUNTS_HANDLER_H

#include "server/auth/authenticator.hpp"
#include "server/storage/storage.hpp"
#include "server/handlers/base-handler.hpp"
#include "server/handlers/util.hpp"

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

#include <string>

namespace beast = boost::beast;                 // from <boost/beast.hpp>
namespace http = beast::http;                   // from <boost/beast/http.hpp>

class AccountsHandler : public BaseHandler {
  public:
    AccountsHandler(Storage* storage, Authenticator* authenticator) : storage_(storage) {}

    virtual std::string GetRoute() const;

    virtual std::pair<http::status, const std::string>
    HandleRequest(const http::request<http::string_body>& req);

  private:
    std::pair<http::status, const std::string> HandleGET(const http::request<http::string_body>& req);
    std::pair<http::status, const std::string> HandlePOST(const http::request<http::string_body>& req);
    Storage* storage_;
    Authenticator* authenticator_;
};

#endif
