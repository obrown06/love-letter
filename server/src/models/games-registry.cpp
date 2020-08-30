#include "models/games-registry.hpp"
#include "models/exceptions.hpp"

void GamesRegistry::InsertGame(const Game& game) {
  if (registry_.find(game.GetId()) != registry_.end()) {
    throw DuplicateGameIdException(game.GetId());
  }
  registry_.insert({game.GetId(), game});
}
