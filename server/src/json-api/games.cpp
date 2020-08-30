#include "json-api/games.hpp"
#include "json-api/exceptions.hpp"

#include <json/json.h>

Game JSONToGame(const std::string& json) {
  Json::Value root;
  Json::CharReaderBuilder builder;
  Json::CharReader * reader = builder.newCharReader();
  std::string errors;
  bool parsingSuccessful = reader->parse(json.c_str(), json.c_str() + json.size(), &root, &errors);
  if (!parsingSuccessful || !root.isMember("game_id") || !root.isMember("username")) {
    throw InvalidJsonException(json);
  }
  return Game(root["game_id"].asString(), root["username"].asString());
}
