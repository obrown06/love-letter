#ifndef SERVER_HANDLERS_DISPATCHER_H
#define SERVER_HANDLERS_DISPATCHER_H

#include "server/handlers/base-handler.hpp"
#include "server/handlers/util.hpp"

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>

namespace beast = boost::beast;                 // from <boost/beast.hpp>
namespace http = beast::http;                   // from <boost/beast/http.hpp>

class HandlerDispatcher {
 public:
   HandlerDispatcher() = default;

   template<class Body, class Allocator, class Send>
   void handle_request(
     http::request<Body, http::basic_fields<Allocator>>&& req,
     Send&& send)
   {
     const auto not_found =
       [&req](beast::string_view target)
       {
           http::response<http::string_body> res{http::status::not_found, req.version()};
           res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
           res.set(http::field::content_type, "text/html");
           res.keep_alive(req.keep_alive());
           res.body() = "No page found at the route '" + std::string(target) + "'.";
           res.prepare_payload();
           return res;
       };
     std::string route = GetRoute(std::string(req.target()));
     if (routes_to_handlers_.find(route) == routes_to_handlers_.end()) {
       return send(not_found(route));
     }
     BaseHandler* handler = routes_to_handlers_.at(route);
     auto status_and_body = handler->HandleRequest(GetTarget(std::string(req.target())),
                                                   req.method(),
                                                   req.body());
     const auto found =
       [&status_and_body, &req]()
       {
         http::response<http::string_body> res{status_and_body.first, req.version()};
         res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
         res.set(http::field::content_type, "text/html");
         res.keep_alive(req.keep_alive());
         res.body() = status_and_body.second;
         res.prepare_payload();
         return res;
       };
     return send(found());
   }
 private:
   std::map<std::string, BaseHandler*> routes_to_handlers_;
};

#endif
