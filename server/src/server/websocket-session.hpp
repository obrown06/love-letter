#ifndef SERVER_WEBSOCKET_SESSION_HPP
#define SERVER_WEBSOCKET_SESSION_HPP

#include <memory>

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/bind_executor.hpp>

#include "server/util.hpp"

class WebsocketSession : public std::enable_shared_from_this<WebsocketSession>
{
  public:
    explicit WebsocketSession(tcp::socket&& socket) : ws_(std::move(socket)) {}

    template<class Body, class Allocator>
    void
    do_accept(http::request<Body, http::basic_fields<Allocator>> req)
    {
      ws_.set_option(
        websocket::stream_base::timeout::suggested(
          beast::role_type::server));
      ws_.set_option(websocket::stream_base::decorator(
        [](websocket::response_type& res)
        {
          res.set(http::field::server,
              std::string(BOOST_BEAST_VERSION_STRING) + " love-letter");
        }
      ));

      ws_.async_accept(
        req,
        beast::bind_front_handler(
          &WebsocketSession::on_accept,
          shared_from_this()));
    }

  private:
    void on_accept(beast::error_code ec);

    void do_read();

    void on_read(beast::error_code ec, std::size_t bytes_transferred);

    void on_write(beast::error_code ec, std::size_t bytes_transferred);

    websocket::stream<beast::tcp_stream> ws_;
    beast::flat_buffer buffer_;
};

#endif
