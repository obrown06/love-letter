#include "server/handlers/util.hpp"

const char kDelineator = '/';

size_t GetLastIdxOfDelineator(const std::string& url, char delineator) {
  size_t last_idx = 0;
  for (size_t idx = 0; idx < url.size(); idx++) {
    if (url[idx] == delineator) {
      last_idx = idx;
    }
  }
  return last_idx;
}

std::string GetRoute(const std::string& url) {
  size_t last_idx = GetLastIdxOfDelineator(url, kDelineator);
  return url.substr(0, last_idx + 1);
}

std::string GetTarget(const std::string& url) {
  size_t last_idx = GetLastIdxOfDelineator(url, kDelineator);
  return url.substr(last_idx + 1, url.size() - (last_idx + 1));
}
