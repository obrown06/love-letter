#include "server/handlers/dispatcher.hpp"
#include "server/handlers/handlers-init.hpp"
#include "server/server/listener.hpp"
#include "server/storage/storage.hpp"
#include "server/auth/accounts-registry.hpp"
#include "server/auth/authenticator.hpp"

#include <boost/beast/core.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/bind_executor.hpp>
#include <cstdlib>
#include <memory>
#include <iostream>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

const char kDatabaseFileName[] = "LoveLetter.db";

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: love-letter <address> <port>\n";
    return EXIT_FAILURE;
  }
  const auto address = net::ip::make_address(argv[1]);
  const auto port = static_cast<unsigned short>(std::atoi(argv[2]));

  net::io_context ioc;

  auto storage = std::make_unique<Storage>(kDatabaseFileName);
  auto accounts_registry = std::make_unique<AccountsRegistry>();
  auto authenticator = std::make_unique<Authenticator>(accounts_registry.get());

  // For testing purposes
  Account account("nick", "nick_pass", "nick@gmail.com");
  storage->InsertOrUpdateAccount(account);

  auto dispatcher = std::make_unique<HandlerDispatcher>();
  InitAndRegisterHandlers(storage.get(),
                          authenticator.get(),
                          accounts_registry.get(),
                          dispatcher.get());

  std::make_shared<Listener>(ioc, tcp::endpoint(address, port), dispatcher.get())->run();

  net::signal_set signals(ioc, SIGINT, SIGTERM);
  signals.async_wait(
    [&](const beast::error_code&, int) {
      ioc.stop();
    }
  );

  ioc.run();

  return EXIT_SUCCESS;
}
