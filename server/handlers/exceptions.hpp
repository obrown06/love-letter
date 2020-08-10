#ifndef SERVER_HANDLERS_EXCEPTIONS_H
#define SERVER_HANDLERS_EXCEPTIONS_H

#include <exception>
#include <string>

class DuplicateRouteException : public std::exception {
 public:
   DuplicateRouteException(const std::string& route) : route_(route) {}
   const char * what () const throw ()
   {
     return ("More than one handler registered for route: " + route_).c_str();
   }
 private:
   std::string route_;
};

#endif
