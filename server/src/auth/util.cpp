#include "auth/util.hpp"

const std::string kHTTPParamStart = ";";

std::string SanitizeHTTPParam(const std::string& param) {
  if (param.size() > 0 && param[0] == kHTTPParamStart[0]) {
    return param;
  }
  return kHTTPParamStart + param;
}
