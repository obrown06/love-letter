#include "server/server/util.hpp"
#include <iostream>

namespace beast = boost::beast;

void fail(beast::error_code ec, const std::string& action) {
  std::cerr << action << " failed with the following message: " << ec.message() << "\n";
}
