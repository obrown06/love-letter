#ifndef SERVER_HANDLERS_DISPATCHER_H
#define SERVER_HANDLERS_DISPATCHER_H

#include "handlers/base-handler.hpp"
#include "handlers/util.hpp"
#include "auth/exceptions.hpp"

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>

namespace beast = boost::beast;                 // from <boost/beast.hpp>
namespace http = beast::http;                   // from <boost/beast/http.hpp>

namespace {
  const char kLoginRouteName[] = "/api/login/";
};

class HandlerDispatcher {
 public:
   HandlerDispatcher() = default;

   void RegisterHandler(std::shared_ptr<BaseHandler> handler);

   template<class Body, class Allocator, class Send>
   void handle_request(
     http::request<Body, http::basic_fields<Allocator>>&& req,
     Send&& send)
   {
     std::string route = GetRoute(std::string(req.target()));
     if (routes_to_handlers_.find(route) == routes_to_handlers_.end()) {
       std::string not_found_body = "No page found at the route '" + route + "'.";
       return send(MakeJsonHttpResponse(http::status::not_found, req, not_found_body));
     }
     std::shared_ptr<BaseHandler> handler = routes_to_handlers_.at(route);
     try {
       return send(handler->HandleRequest(req));
     }
     catch (NotLoggedInException& e) {
       return send(MakeRedirectResponse(req, kLoginRouteName));
     }
   }
 private:
   std::map<std::string, std::shared_ptr<BaseHandler>> routes_to_handlers_;
};

#endif
