#ifndef SERVER_HANDLERS_UTIL_H
#define SERVER_HANDLERS_UTIL_H

#include <string>

std::string GetRoute(const std::string& url);

std::string GetTarget(const std::string& url);

#endif
