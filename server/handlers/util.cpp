#include "server/handlers/util.hpp"

const char kDelineator = '/';

std::string GetRoute(const std::string& target) {
  size_t last_idx = 0;
  for (size_t idx = 0; idx < target.size(); idx++) {
    if (target[idx] == kDelineator) {
      last_idx = idx;
    }
  }
  return target.substr(0, last_idx + 1);
}
