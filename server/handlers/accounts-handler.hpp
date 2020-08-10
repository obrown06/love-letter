#ifndef SERVER_HANDLERS_ACCOUNTS_HANDLER_H
#define SERVER_HANDLERS_ACCOUNTS_HANDLER_H

#include "server/storage/storage.hpp"
#include "server/handlers/base-handler.hpp"

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

#include <string>

namespace beast = boost::beast;                 // from <boost/beast.hpp>
namespace http = beast::http;                   // from <boost/beast/http.hpp>

class AccountsHandler : public BaseHandler {
  public:
    AccountsHandler(Storage* storage) : storage_(storage) {}

    virtual std::string GetRoute() const;

    virtual std::pair<http::status, const std::string> HandleRequest(const std::string& target,
                                                                     const http::verb& method,
                                                                     const std::string& body);
  private:
    std::pair<http::status, const std::string> HandleGET(const std::string& target);
    Storage* storage_;
};

#endif
