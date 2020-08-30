#include "auth/authenticator.hpp"
#include "storage/storage.hpp"
#include "handlers/dispatcher.hpp"
#include "models/games-registry.hpp"

void InitAndRegisterHandlers(Storage* storage,
                             Authenticator* authenticator,
                             AccountsRegistry* accounts_registry,
                             GamesRegistry* games_registry,
                             HandlerDispatcher* dispatcher);
