#ifndef SERVER_UTIL_H
#define SERVER_UTIL_H

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/bind_executor.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/strand.hpp>
#include <boost/make_unique.hpp>
#include <boost/optional.hpp>
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

namespace beast = boost::beast;                 // from <boost/beast.hpp>
namespace http = beast::http;                   // from <boost/beast/http.hpp>
namespace websocket = beast::websocket;         // from <boost/beast/websocket.hpp>
namespace net = boost::asio;                    // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;               // from <boost/asio/ip/tcp.hpp>

void fail(beast::error_code ec, const std::string& action);

// This function produces an HTTP response for the given request.
// The type of the response object depends on the contents of the request,
// so the interface requires the caller to pass a generic lambda for
// receiving the response.
template<class Body, class Allocator, class Send>
void
handle_request(
  http::request<Body, http::basic_fields<Allocator>>&& req,
  Send&& send) {

  // Returns a bad request response
  const auto bad_request =
  [&req](beast::string_view why)
  {
    http::response<http::string_body> res{http::status::bad_request, req.version()};
    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(http::field::content_type, "text/html");
    res.keep_alive(req.keep_alive());
    res.body() = std::string(why);
    res.prepare_payload();
    return res;
  };

  const auto not_found =
    [&req](beast::string_view target)
    {
        http::response<http::string_body> res{http::status::not_found, req.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());
        res.body() = "The resource '" + std::string(target) + "' was not found.";
        res.prepare_payload();
        return res;
    };

  const auto server_error =
  [&req](beast::string_view what)
  {
    http::response<http::string_body> res{http::status::internal_server_error, req.version()};
    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(http::field::content_type, "text/html");
    res.keep_alive(req.keep_alive());
    res.body() = "An error occured: '" + std::string(what) + "'";
    res.prepare_payload();
    return res;
  };

  if (req.method() != http::verb::get && req.method() != http::verb::head) {
    return send(bad_request("Unknown HTTP-method"));
  }

  if (req.method() == http::verb::head) {
    http::response<http::empty_body> res{http::status::ok, req.version()};
    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(http::field::content_type, "text/html");
    res.keep_alive(req.keep_alive());
    return send(std::move(res));
  }

  beast::error_code ec;
  http::file_body::value_type body;
  body.open("example.html", beast::file_mode::scan, ec);

  // Handle the case where the file doesn't exist
  if(ec == beast::errc::no_such_file_or_directory)
      return send(not_found(req.target()));

  // Handle an unknown error
  if(ec)
      return send(server_error(ec.message()));

  // Cache the size since we need it after the move
  auto const size = body.size();

  // Respond to HEAD request
  if(req.method() == http::verb::head)
  {
      http::response<http::empty_body> res{http::status::ok, req.version()};
      res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
      res.set(http::field::content_type, "text/html");
      res.content_length(size);
      res.keep_alive(req.keep_alive());
      return send(std::move(res));
  }

  // Respond to GET request
  http::response<http::file_body> res{
      std::piecewise_construct,
      std::make_tuple(std::move(body)),
        std::make_tuple(http::status::ok, req.version())};
  res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
  res.set(http::field::content_type, "text/html");
  res.content_length(size);
  res.keep_alive(req.keep_alive());
  return send(std::move(res));
}

#endif
