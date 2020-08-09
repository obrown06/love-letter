#include "server/server/session.hpp"
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/bind_executor.hpp>
#include "util.hpp"
#include <memory>
#include <iostream>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

void HTTPSession::run() {
  net::dispatch(
    stream_.get_executor(),
    beast::bind_front_handler(
      &HTTPSession::do_read,
      this->shared_from_this()
    )
  );
}

void HTTPSession::do_read() {
  // Construct a new parser for each message
  parser_.emplace();

  // Apply a reasonable limit to the allowed size
  // of the body in bytes to prevent abuse
  parser_->body_limit(10000);

  // Set the timeout.
  stream_.expires_after(std::chrono::seconds(30));

  // Read a request using the parser-oriented interface
  http::async_read(
    stream_,
    buffer_,
    *parser_,
    beast::bind_front_handler(
      &HTTPSession::on_read,
      shared_from_this()));
}

void HTTPSession::on_read(beast::error_code ec, std::size_t bytes_transferred) {
  boost::ignore_unused(bytes_transferred);

  // They closed the connection
  if (ec == http::error::end_of_stream) {
    return do_close();
  }

  if (ec) {
    return fail(ec, "read");
  }

  handle_request(parser_->release(), queue_);

  if (!queue_.is_full()) {
    do_read();
  }
}

void HTTPSession::on_write(bool close, beast::error_code ec, std::size_t bytes_transferred) {
  boost::ignore_unused(bytes_transferred);

  if (ec) {
    return fail(ec, "write");
  }

  if (close) {
    // This means we should close the connection, usually because
    // the repsonse indicated "Connection: close"
    return do_close();
  }

  if (queue_.on_write()) {
    do_read();
  }
}

void HTTPSession::do_close() {
  // Send a TCP shutdown
  beast::error_code ec;
  stream_.socket().shutdown(tcp::socket::shutdown_send, ec);
}
