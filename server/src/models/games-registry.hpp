#ifndef SERVER_MODELS_GAMES_REGISTRY_HPP
#define SERVER_MODELS_GAMES_REGISTRY_HPP

#include "models/game.hpp"

#include <map>

class GamesRegistry {
public:
  GamesRegistry() {};

  void InsertGame(const Game& game);

private:
  std::map<std::string, Game> registry_;

};

#endif
