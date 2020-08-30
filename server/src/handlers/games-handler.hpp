#ifndef SERVER_HANDLER_GAMES_HANDLER_H
#define SERVER_HANDLER_GAMES_HANDLER_H

#include "handlers/base-handler.hpp"
#include "auth/authenticator.hpp"
#include "models/games-registry.hpp"
#include "storage/storage.hpp"

#include <string>

class GamesHandler : public BaseHandler {
public:
  GamesHandler(Storage* storage, Authenticator* authenticator, GamesRegistry* registry) :
    storage_(storage),
    authenticator_(authenticator),
    registry_(registry) {}

  virtual std::string GetRoute() const;

  virtual http::response<http::string_body>
  HandleRequest(const http::request<http::string_body>& req);

private:
  http::response<http::string_body>
  HandlePOST(const http::request<http::string_body>& req);

  http::response<http::string_body>
  HandleCreateGameRequest(const http::request<http::string_body>& req);

  Storage* storage_;
  Authenticator* authenticator_;
  GamesRegistry* registry_;
};

#endif
