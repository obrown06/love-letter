#include "auth/authenticator.hpp"
#include "storage/storage.hpp"
#include "handlers/dispatcher.hpp"

void InitAndRegisterHandlers(Storage* storage,
                             Authenticator* authenticator,
                             AccountsRegistry* accounts_registry,
                             HandlerDispatcher* dispatcher);
