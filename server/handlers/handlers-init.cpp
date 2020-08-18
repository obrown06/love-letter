#include "server/handlers/handlers-init.hpp"
#include "server/handlers/accounts-handler.hpp"
#include "server/handlers/login-handler.hpp"

#include <memory>

void InitAndRegisterHandlers(Storage* storage,
                             Authenticator* authenticator,
                             AccountsRegistry* accounts_registry,
                             HandlerDispatcher* dispatcher) {
 auto accounts_handler = std::make_shared<AccountsHandler>(storage, authenticator);
 auto login_handler = std::make_shared<LoginHandler>(storage, authenticator, accounts_registry);
 dispatcher->RegisterHandler(accounts_handler);
 dispatcher->RegisterHandler(login_handler);
}
