#include "json-api/auth.hpp"

#include <jsoncpp/json/json.h>

std::string GetNotLoggedInJson() {
  Json::Value root;
  root["authenticated"] = false;
  Json::StreamWriterBuilder builder;
  return Json::writeString(builder, root);
}

std::string GetIncorrectPasswordJson() {
  Json::Value root;
  root["incorrect_password"] = true;
  Json::StreamWriterBuilder builder;
  return Json::writeString(builder, root);
}

std::string GetNoMatchingUsernameJson() {
  Json::Value root;
  root["no_matching_username"] = true;
  Json::StreamWriterBuilder builder;
  return Json::writeString(builder, root);
}

std::string GetPreexistingAccountJson() {
  Json::Value root;
  root["preexisting_account"] = true;
  Json::StreamWriterBuilder builder;
  return Json::writeString(builder, root);
}
