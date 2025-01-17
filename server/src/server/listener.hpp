#ifndef SERVER_LISTENER_HPP
#define SERVER_LISTENER_HPP

#include "auth/authenticator.hpp"
#include "handlers/dispatcher.hpp"
#include "models/games-registry.hpp"

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/bind_executor.hpp>
#include <memory>
#include <iostream>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

class Listener : public std::enable_shared_from_this<Listener>
{
public:
  Listener(net::io_context& ioc,
           tcp::endpoint endpoint,
           HandlerDispatcher* dispatcher,
           GamesRegistry* registry,
           Authenticator* authenticator);
  void run();

private:
  void do_accept();
  void on_accept(beast::error_code ec, tcp::socket socket);
  net::io_context& ioc_;
  tcp::acceptor acceptor_;
  HandlerDispatcher* dispatcher_;
  GamesRegistry* registry_;
  Authenticator* authenticator_;
};

#endif
