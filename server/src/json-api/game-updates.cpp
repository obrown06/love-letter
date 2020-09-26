#include "json-api/games.hpp"
#include "json-api/exceptions.hpp"

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
    if (move.move_type == GameUpdate::Move::DISCARD_CARD) {
      move.selected_card = Card(static_cast<Card::Type>(root["move"]["selected_card"].asInt()));
    }
    if (move.move_type == GameUpdate::Move::SELECT_PLAYER) {
      move.selected_player_id = root["move"]["selected_player_id"].asString();
      if (root["move"].isMember("predicted_card")) {
        move.selected_card = Card(static_cast<Card::Type>(root["move"]["predicted_card"].asInt()));
      }
    }
    if (move.move_type == GameUpdate::Move::VIEW_CARD) {
      move.viewed_player_id = root["move"]["viewed_player_id"].asString();
    }
    update.move = move;
  }
  return update;
}
