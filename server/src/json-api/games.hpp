#ifndef SERVER_JSON_API_GAMES_H
#define SERVER_JSON_API_GAMES_H

#include "models/account.hpp"
#include "models/game.hpp"

#include <string>
#include <vector>

Game JSONToGame(const std::string& json);

std::string GameToJSON(const Game& game);

#endif
