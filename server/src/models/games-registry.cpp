#include "models/games-registry.hpp"
#include "models/exceptions.hpp"

#include <utility>
#include <iostream>

void GamesRegistry::InsertGame(const Game& game) {
  if (registry_.find(game.GetId()) != registry_.end()) {
    throw DuplicateGameIdException(game.GetId());
  }
  Game copy = game;
  std::vector<WebsocketSession*> sessions;
  std::pair<Game, std::vector<WebsocketSession*>> pair(game, sessions);
  registry_.insert({game.GetId(), pair});
}

void GamesRegistry::InsertSession(const std::string& id, WebsocketSession* session) {
  if (registry_.find(id) == registry_.end()) {
    throw NoGameRegisteredException(id);
  }
  registry_.at(id).second.push_back(session);
}

void GamesRegistry::UpdateGameAndBroadcast(const std::string& id, const std::string& msg) {
  if (registry_.find(id) == registry_.end()) {
    throw NoGameRegisteredException(id);
  }
  for (auto* session : registry_.at(id).second) {
    session->send(msg);
  }
}
