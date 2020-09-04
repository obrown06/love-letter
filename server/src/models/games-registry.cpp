#include "models/games-registry.hpp"
#include "models/exceptions.hpp"

#include <utility>

void GamesRegistry::InsertGame(const Game& game) {
  if (registry_.find(game.GetId()) != registry_.end()) {
    throw DuplicateGameIdException(game.GetId());
  }
  Game copy = game;
  std::unordered_set<WebsocketSession*> sessions;
  std::pair<Game, std::unordered_set<WebsocketSession*>> pair(game, sessions);
  registry_.insert({game.GetId(), pair});
}

void GamesRegistry::InsertSession(const std::string& game_id, WebsocketSession* session) {
  if (registry_.find(game_id) == registry_.end()) {
    throw NoGameRegisteredException(game_id);
  }
  registry_.at(game_id).second.insert(session);
}

void GamesRegistry::RemoveSession(const std::string& game_id, WebsocketSession* session) {
  if (registry_.find(game_id) == registry_.end()) {
    throw NoGameRegisteredException(game_id);
  }
  registry_.at(game_id).second.erase(session);
}

void GamesRegistry::UpdateGameAndBroadcast(const std::string& game_id, const std::string& msg) {
  if (registry_.find(game_id) == registry_.end()) {
    throw NoGameRegisteredException(game_id);
  }
  for (auto* session : registry_.at(game_id).second) {
    session->send(msg);
  }
}
