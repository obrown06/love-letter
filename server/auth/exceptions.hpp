#ifndef SERVER_AUTH_EXCEPTIONS_H
#define SERVER_AUTH_EXCEPTIONS_H

#include <exception>
#include <string>

class NotLoggedInException : public std::exception {};

#endif
