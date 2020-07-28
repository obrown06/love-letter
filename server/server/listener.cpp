#include "./listener.hpp"
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

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

void fail(beast::error_code ec, const std::string& action) {
  std::cerr << action << " failed with the following message: " << ec.message() << "\n";
}

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
  std::cout << "Accepted!\n\n";
}
