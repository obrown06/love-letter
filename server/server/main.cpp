#include <boost/beast/core.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/bind_executor.hpp>
#include "server/server/listener.hpp"
#include <cstdlib>
#include <memory>
#include <iostream>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: love-letter <address> <port>\n";
    return EXIT_FAILURE;
  }
  const auto address = net::ip::make_address(argv[1]);
  const auto port = static_cast<unsigned short>(std::atoi(argv[2]));

  net::io_context ioc;

  std::make_shared<Listener>(ioc, tcp::endpoint(address, port))->run();

  net::signal_set signals(ioc, SIGINT, SIGTERM);
  signals.async_wait(
    [&](const beast::error_code&, int) {
      ioc.stop();
    }
  );

  ioc.run();

  return EXIT_SUCCESS;
}
