#ifndef SERVER_JSON_API_EXCEPTIONS_H
#define SERVER_JSON_API_EXCEPTIONS_H

#include <exception>
#include <string>

class InvalidJsonException : public std::exception {
 public:
   InvalidJsonException(const std::string& json) : json_(json) {}
   const char * what () const throw ()
   {
     return ("JSON is invalid: " + json_).c_str();
   }
 private:
   std::string json_;
};

#endif
