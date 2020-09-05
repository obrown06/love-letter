#include "handlers/handlers-init.hpp"
#include "handlers/accounts-handler.hpp"
#include "handlers/games-handler.hpp"
#include "handlers/login-handler.hpp"
#include "handlers/logout-handler.hpp"

#include <memory>

void InitAndRegisterHandlers(Storage* storage,
                             Authenticator* authenticator,
                             AccountsRegistry* accounts_registry,
                             GamesRegistry* games_registry,
                             HandlerDispatcher* dispatcher) {
 auto accounts_handler = std::make_shared<AccountsHandler>(storage, authenticator);
 auto login_handler = std::make_shared<LoginHandler>(storage, authenticator, accounts_registry);
 auto logout_handler = std::make_shared<LogoutHandler>(authenticator, accounts_registry);
 auto games_handler = std::make_shared<GamesHandler>(storage, authenticator, games_registry);
 dispatcher->RegisterHandler(games_handler);
 dispatcher->RegisterHandler(accounts_handler);
 dispatcher->RegisterHandler(login_handler);
 dispatcher->RegisterHandler(logout_handler);
}
