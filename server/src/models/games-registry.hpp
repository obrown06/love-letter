#ifndef SERVER_MODELS_GAMES_REGISTRY_HPP
#define SERVER_MODELS_GAMES_REGISTRY_HPP

#include "models/game.hpp"
#include "server/websocket-session.hpp"

#include <map>
#include <string>
#include <unordered_set>

class WebsocketSession;

class GamesRegistry {
public:
  GamesRegistry() {};

  void UpdateGameAndBroadcast(const std::string& game_id, const std::string& msg);
  void InsertGame(const Game& game);
  void InsertSession(const std::string& game_id, WebsocketSession* session);
  void RemoveSession(const std::string& game_id, WebsocketSession* session);

private:
  std::map<std::string, std::pair<Game, std::unordered_set<WebsocketSession*>>> registry_;

};

#endif
