#ifndef SERVER_MODELS_GAMES_REGISTRY_HPP
#define SERVER_MODELS_GAMES_REGISTRY_HPP

#include "models/game.hpp"
#include "server/websocket-session.hpp"

#include <map>
#include <string>

class WebsocketSession;

class GamesRegistry {
public:
  GamesRegistry() {};

  void UpdateGameAndBroadcast(const std::string& game_id, const std::string& msg);
  void InsertGame(const Game& game);
  void InsertSession(const std::string& id, WebsocketSession* session);

private:
  std::map<std::string, std::pair<Game, std::vector<WebsocketSession*>>> registry_;

};

#endif
