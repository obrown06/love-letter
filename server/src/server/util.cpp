#include "server/util.hpp"
#include <iostream>

namespace beast = boost::beast;

void fail(beast::error_code ec, const std::string& action) {
  std::cerr << action << " failed with the following message: " << ec.message() << "\n";
}

void fail(const std::string& action, const std::string& reason) {
  std::cerr << action << " failed with the following message: " << reason << "\n";
}
