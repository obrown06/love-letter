#include "json-api/games.hpp"
#include "json-api/exceptions.hpp"

#include <json/json.h>

Game JSONToGame(const std::string& json) {
  Json::Value root;
  Json::CharReaderBuilder builder;
  Json::CharReader * reader = builder.newCharReader();
  std::string errors;
  bool parsingSuccessful = reader->parse(json.c_str(), json.c_str() + json.size(), &root, &errors);
  if (!parsingSuccessful || !root.isMember("game_id") || !root.isMember("creator")) {
    throw InvalidJsonException(json);
  }
  return Game(root["game_id"].asString(), root["creator"].asString());
}

std::string GameToJSON(const Game& game) {
  Json::Value root;
  root["game_id"] = game.GetId();
  root["creator"] = game.GetCreator();
  root["state"] = game.GetState();
  if (game.GetState() == Game::IN_PROGRESS) {
    root["tokens_to_win"] = game.GetTokensToWin();
    const auto& round = game.GetLatestRound();
    Json::Value latest_round;
    latest_round["deck_size"] = round.GetDeckSize();
    for (const auto& player : round.GetPlayers()) {
      Json::Value player_node;
      player_node["player_id"] = player.player_id;
      for (const auto& discarded_card : player.discarded_cards) {
        player_node["discarded_cards"].append(static_cast<int>(discarded_card.GetType()));
      }
      for (const auto& held_card : player.held_cards) {
        player_node["held_cards"].append(static_cast<int>(held_card.GetType()));
      }
      player_node["still_in_round"] = player.still_in_round;
      latest_round["players"].append(player_node);
    }

    const auto turn = round.GetLatestTurn();
    Json::Value turn_node;
    turn_node["player_id"] = turn.player_id;
    const auto next_move_type = turn.GetNextMoveType();
    turn_node["next_move_type"] = next_move_type;
    latest_round["current_turn"] = turn_node;

    root["rounds"].append(latest_round);
  }
  for (const auto& player : game.GetPlayers()) {
    Json::Value player_node;
    player_node["player_id"] = player.player_id;
    root["players"].append(player_node);
  }
  Json::StreamWriterBuilder builder;
  return Json::writeString(builder, root);
}
