#include "json-api/games.hpp"
#include "json-api/exceptions.hpp"

#include <json/json.h>

GameUpdate JSONToGameUpdate(const std::string& json) {
  Json::Value root;
  Json::CharReaderBuilder builder;
  Json::CharReader * reader = builder.newCharReader();
  std::string errors;
  bool parsingSuccessful = reader->parse(json.c_str(), json.c_str() + json.size(), &root, &errors);
  if (!parsingSuccessful || !root.isMember("game_id") || !root.isMember("player_id") || !root.isMember("update_type")) {
    throw InvalidJsonException(json);
  }
  GameUpdate update;
  update.game_id = root["game_id"].asString();
  update.player_id = root["player_id"].asString();
  update.update_type = static_cast<GameUpdate::UpdateType>(root["update_type"].asInt());
  if (update.update_type == GameUpdate::MOVE_REQUEST) {
    GameUpdate::Move move;
    move.move_type = static_cast<GameUpdate::Move::MoveType>(root["move"]["move_type"].asInt());
    update.move = move;
  }
  return update;
}
