#ifndef SERVER_JSON_API_GAMES_H
#define SERVER_JSON_API_GAMES_H

#include "models/game-update.hpp"
#include <json/json.h>

GameUpdate JSONToGameUpdate(const std::string& json);

#endif
