#include "server/websocket-session.hpp"

void
WebsocketSession::on_accept(beast::error_code ec)
{
  if (ec) {
    return fail(ec, "accept");
  }

  do_read();
}

void
WebsocketSession::do_read()
{
  ws_.async_read(
    buffer_,
    beast::bind_front_handler(
      &WebsocketSession::on_read,
      shared_from_this()));
}

void
WebsocketSession::on_read(
  beast::error_code ec,
  std::size_t bytes_transferred)
{
  boost::ignore_unused(bytes_transferred);

  if (ec == websocket::error::closed) {
    return;
  }

  if (ec) {
    fail(ec, "read");
  }

  ws_.text(ws_.got_text());
  ws_.async_write(
    buffer_.data(),
    beast::bind_front_handler(
      &WebsocketSession::on_write,
      shared_from_this()));
}

void
WebsocketSession::on_write(
  beast::error_code ec,
  std::size_t bytes_transferred)
{
  boost::ignore_unused(bytes_transferred);

  if (ec) {
    return fail(ec, "write");
  }

  buffer_.consume(buffer_.size());

  do_read();
}
