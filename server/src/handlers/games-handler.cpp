#include "handlers/games-handler.hpp"
#include "handlers/util.hpp"
#include "json-api/games.hpp"
#include "json-api/exceptions.hpp"
#include "models/exceptions.hpp"

const char kRouteName[] = "/api/games";

std::string GamesHandler::GetRoute() const {
  return kRouteName;
}

http::response<http::string_body>
GamesHandler::HandleRequest(const http::request<http::string_body>& req) {
  authenticator_->Authenticate(req);
  if (req.method() == http::verb::post) {
    return HandlePOST(req);
  }

  return MakeJsonHttpResponse(http::status::not_implemented, req, std::string());
}

http::response<http::string_body>
GamesHandler::HandlePOST(const http::request<http::string_body>& req) {
  return HandleCreateGameRequest(req);
}

http::response<http::string_body>
GamesHandler::HandleCreateGameRequest(const http::request<http::string_body>& req) {
  try {
    Game game = JSONToGame(req.body());
    registry_->InsertGame(game);
    return MakeJsonHttpResponse(http::status::ok, req, GetCreatedGameResponse(game.GetId())); 
  }
  catch (InvalidJsonException& e) {
    return MakeJsonHttpResponse(http::status::bad_request, req, std::string("Invalid request format!"));
  }
  catch (GameAlreadyStartedException& e) {
    return MakeJsonHttpResponse(http::status::bad_request, req, std::string("Game is already started!"));
  }
  catch (DuplicatePlayerException& e) {
    return MakeJsonHttpResponse(http::status::bad_request, req, std::string("Player is already registered!"));
  }
  catch (DuplicateGameIdException& e) {
    return MakeJsonHttpResponse(http::status::bad_request, req, std::string("Game is already created!"));
  }
}
