#ifndef SERVER_MODELS_GAMES_REGISTRY_HPP
#define SERVER_MODELS_GAMES_REGISTRY_HPP

#include "models/game.hpp"
#include "server/websocket-session.hpp"
#include "storage/storage.hpp"

#include <map>
#include <string>
#include <unordered_set>

class WebsocketSession;

class GamesRegistry {
public:
  GamesRegistry(Storage* storage) : storage_(storage) {};

  void UpdateGameAndBroadcast(const GameUpdate& game_update);
  void InsertGame(const Game& game);
  void InsertSession(const std::string& game_id, WebsocketSession* session);
  void RemoveSession(const std::string& game_id, WebsocketSession* session);

private:
  void UpdateAccountsWithGameResults(const Game& game);
  std::map<std::string, std::pair<Game, std::unordered_set<WebsocketSession*>>> registry_;
  Storage* storage_;
};

#endif
