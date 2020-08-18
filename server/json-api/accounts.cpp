#include "server/json-api/accounts.hpp"
#include "server/json-api/exceptions.hpp"

#include <json/json.h>

#include <iostream>

std::string AccountsToJSON(const std::vector<Account>& accounts) {
  Json::Value root;
  for (const auto& account : accounts) {
    Json::Value account_node;
    account_node["username"] = account.GetUsername();
    root["accounts"].append(account_node);
  }
  Json::StreamWriterBuilder builder;
  return Json::writeString(builder, root);
}

Account JSONToAccount(const std::string& json) {
  Json::Value root;
  Json::CharReaderBuilder builder;
  Json::CharReader * reader = builder.newCharReader();
  std::string errors;
  bool parsingSuccessful = reader->parse(json.c_str(), json.c_str() + json.size(), &root, &errors);
  if (!parsingSuccessful || !root.isMember("username") || !root.isMember("password") || !root.isMember("email")) {
    throw InvalidJsonException(json);
  }
  Account account(root["username"].asString(), root["password"].asString(), root["email"].asString());
  return account;
}

std::pair<std::string, std::string> JSONToUsernamePasswordPair(const std::string& json) {
  Json::Value root;
  Json::CharReaderBuilder builder;
  Json::CharReader * reader = builder.newCharReader();
  std::string errors;
  bool parsingSuccessful = reader->parse(json.c_str(), json.c_str() + json.size(), &root, &errors);
  if (!parsingSuccessful || !root.isMember("username") || !root.isMember("password")) {
    throw InvalidJsonException(json);
  }
  return std::pair<std::string, std::string>(root["username"].asString(), root["password"].asString());
}
