#include "models/games-registry.hpp"
#include "models/exceptions.hpp"

#include "json-api/games.hpp"

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

void GamesRegistry::UpdateGameAndBroadcast(const GameUpdate& game_update) {
  if (registry_.find(game_update.game_id) == registry_.end()) {
    throw NoGameRegisteredException(game_update.game_id);
  }
  Game& game = registry_.at(game_update.game_id).first;
  game.ProcessUpdate(game_update);
  std::string json = GameToJSON(game);
  std::cout << "about to broadcast\n";
  for (auto* session : registry_.at(game.GetId()).second) {
    session->send(json);
  }
}
