#include "handlers/dispatcher.hpp"
#include "handlers/handlers-init.hpp"
#include "server/listener.hpp"
#include "server/websocket-session.hpp"
#include "storage/storage.hpp"
#include "auth/accounts-registry.hpp"
#include "auth/authenticator.hpp"
#include "models/games-registry.hpp"

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
  auto games_registry = std::make_unique<GamesRegistry>(storage.get());
  auto authenticator = std::make_unique<Authenticator>(accounts_registry.get());

  // For testing purposes
  Account account("nick", "meh", "nick@gmail.com", 0, 0, 0);
  storage->InsertOrUpdateAccount(account);

  Account account4("reallyreallylongname", "meh", "nick@gmail.com", 0, 0, 0);
  storage->InsertOrUpdateAccount(account4);

  Account account2("brick", "meh", "nick@gmail.com", 0, 0, 0);
  storage->InsertOrUpdateAccount(account2);

  Account account3("chau", "meh", "nick@gmail.com", 0, 0, 0);
  storage->InsertOrUpdateAccount(account3);

  auto dispatcher = std::make_unique<HandlerDispatcher>();
  InitAndRegisterHandlers(storage.get(),
                          authenticator.get(),
                          accounts_registry.get(),
                          games_registry.get(),
                          dispatcher.get());

  std::make_shared<Listener>(ioc,
                             tcp::endpoint(address, port),
                             dispatcher.get(),
                             games_registry.get(),
                             authenticator.get())->run();

  net::signal_set signals(ioc, SIGINT, SIGTERM);
  signals.async_wait(
    [&](const beast::error_code&, int) {
      ioc.stop();
    }
  );

  ioc.run();

  return EXIT_SUCCESS;
}
