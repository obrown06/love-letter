#include "listener.hpp"
#include "session.hpp"
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

#include "util.hpp"

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

Listener::Listener(net::io_context& ioc, tcp::endpoint endpoint)
  : ioc_(ioc),
    acceptor_(net::make_strand(ioc))
{
  beast::error_code ec;

  // Open acceptor.
  acceptor_.open(endpoint.protocol(), ec);
  if (ec)
  {
    fail(ec, "open");
    return;
  }

  acceptor_.set_option(net::socket_base::reuse_address(true), ec);
  if (ec) {
    fail(ec, "set_option");
    return;
  }

  acceptor_.bind(endpoint, ec);
  if (ec) {
    fail(ec, "bind");
    return;
  }

  acceptor_.listen(net::socket_base::max_listen_connections, ec);
  if (ec) {
    fail(ec, "listen");
    return;
  }
}

void Listener::run() {
  net::dispatch(
    acceptor_.get_executor(),
    beast::bind_front_handler(
      &Listener::do_accept,
      shared_from_this()));
}

void Listener::do_accept() {
  acceptor_.async_accept(
    net::make_strand(ioc_),
    beast::bind_front_handler(
      &Listener::on_accept,
      shared_from_this()));
}

void Listener::on_accept(beast::error_code ec, tcp::socket socket) {
  if (ec) {
    fail(ec, "accept");
  } else {
    std::make_shared<HTTPSession>(std::move(socket))->run();
  }

  // Accept another connection
  do_accept();
}
