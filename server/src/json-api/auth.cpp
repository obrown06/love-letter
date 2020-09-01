#include "json-api/auth.hpp"

#include <json/json.h>

std::string GetNotLoggedInJson() {
  Json::Value root;
  root["authenticated"] = false;
  Json::StreamWriterBuilder builder;
  return Json::writeString(builder, root);
}
