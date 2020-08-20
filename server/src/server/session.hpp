#ifndef SERVER_SESSION_HPP
#define SERVER_SESSION_HPP

#include "handlers/dispatcher.hpp"
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

class HTTPSession : public std::enable_shared_from_this<HTTPSession>
{
public:
  HTTPSession(tcp::socket&& socket, HandlerDispatcher* dispatcher) :
    stream_(std::move(socket)), queue_(*this), dispatcher_(dispatcher) {}

  // Start the session
  void run();
private:
  class Queue {
  public:
    explicit Queue(HTTPSession& self) : self_(self) {
      static_assert(limit > 0, "queue limit must be positive");
      items_.reserve(limit);
    }

    // Indicates whether the queue is full.
    bool
    is_full() const {
      return items_.size() >= limit;
    }

    // Called when a message finishes sending
    // Returns `true` if the caller should initiate a read
    bool
    on_write() {
      BOOST_ASSERT(!items_.empty());
      auto const was_full = is_full();
      items_.erase(items_.begin());
      if (!items_.empty()) {
        (*items_.front())();
      }
      return was_full;
    }

    // Called by the HTTP handler to send a response.
    template<bool isRequest, class Body, class Fields>
    void
    operator()(http::message<isRequest, Body, Fields>&& msg) {
      struct work_impl : work {
        HTTPSession& self_;
        http::message<isRequest, Body, Fields> msg_;

        work_impl(
          HTTPSession& self,
          http::message<isRequest, Body, Fields>&& msg)
          : self_(self),
            msg_(std::move(msg)) {}

        void
        operator()() {
          http::async_write(
            self_.stream_,
            msg_,
            beast::bind_front_handler(
              &HTTPSession::on_write,
              self_.shared_from_this(),
              msg_.need_eof()));
        }
      };

      // Allocate and store the work
      items_.push_back(boost::make_unique<work_impl>(self_, std::move(msg)));

      // If there was no previous work, start this one
      if (items_.size() == 1) {
        (*items_.front())();
      }
    };
  private:
    enum {
      limit = 8,
    };

    struct work {
      virtual ~work() = default;
      virtual void operator()() = 0;
    };

    HTTPSession& self_;
    std::vector<std::unique_ptr<work>> items_;
  };

  void do_read();
  void on_read(beast::error_code ec, std::size_t bytes_transferred);
  void on_write(bool close, beast::error_code ec, std::size_t bytes_transferred);
  void do_close();

  beast::tcp_stream stream_;
  beast::flat_buffer buffer_;
  Queue queue_;
  HandlerDispatcher* dispatcher_;

  // The parser is stored in an optional container so we can construct it
  // from scratch at the beginning of each new message.
  boost::optional<http::request_parser<http::string_body>> parser_;
};

#endif
