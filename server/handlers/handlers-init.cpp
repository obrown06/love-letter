#include "server/handlers/handlers-init.hpp"
#include "server/handlers/accounts-handler.hpp"

#include <memory>

void InitAndRegisterHandlers(Storage* storage, Authenticator* authenticator, HandlerDispatcher* dispatcher) {
 std::shared_ptr<AccountsHandler> accounts_handler = std::make_shared<AccountsHandler>(storage, authenticator);
 dispatcher->RegisterHandler(accounts_handler);
}
