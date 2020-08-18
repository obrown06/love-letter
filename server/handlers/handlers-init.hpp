#include "server/auth/authenticator.hpp"
#include "server/storage/storage.hpp"
#include "server/handlers/dispatcher.hpp"

void InitAndRegisterHandlers(Storage* storage,
                             Authenticator* authenticator,
                             AccountsRegistry* accounts_registry,
                             HandlerDispatcher* dispatcher);
