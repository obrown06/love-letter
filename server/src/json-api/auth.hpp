#ifndef SERVER_JSON_API_AUTH_H
#define SERVER_JSON_API_AUTH_H

#include <string>

std::string GetNotLoggedInJson();

std::string GetIncorrectPasswordJson();

std::string GetNoMatchingUsernameJson();

std::string GetPreexistingAccountJson();

#endif
