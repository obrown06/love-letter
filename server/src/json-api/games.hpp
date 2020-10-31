#ifndef SERVER_JSON_API_GAMES_HPP
#define SERVER_JSON_API_GAMES_HPP

#include "models/account.hpp"
#include "models/game.hpp"

#include <json/json.h>

#include <string>
#include <vector>

Game JSONToGame(const std::string& json);

std::string GameToJSON(const Game& game);

Json::Value RoundToJSON(const Game::Round& round);

Json::Value TurnToJSON(const Game::Round& round, const Game::Round::Turn& turn);

Json::Value MoveToJSON(const GameUpdate::Move& move);

std::string GetNoGameWithIdJson(const std::string& game_id);

std::string GetCreatedGameResponse(const std::string& game_id);

std::string GetGameAlreadyStartedJson(const std::string& game_id);

std::string GetPlayerLeftAndGameEndedJson(const std::string& game_id, const std::string& player_id);

#endif
