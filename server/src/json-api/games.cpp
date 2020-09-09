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
  }
  for (const auto& player : game.GetPlayers()) {
    Json::Value player_node;
    player_node["player_id"] = player.player_id;
    root["players"].append(player_node);
  }
  Json::StreamWriterBuilder builder;
  return Json::writeString(builder, root);
}
