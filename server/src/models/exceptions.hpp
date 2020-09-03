#ifndef SERVER_MODELS_EXCEPTIONS_H
#define SERVER_MODELS_EXCEPTIONS_H

#include <exception>
#include <string>

class DuplicateGameIdException : public std::exception {
 public:
   DuplicateGameIdException(const std::string& id) : id_(id) {}
   const char * what () const throw ()
   {
     return ("More than one game registered for id: " + id_).c_str();
   }
 private:
   std::string id_;
};

class GameAlreadyStartedException : public std::exception {
 public:
   GameAlreadyStartedException(const std::string& id) : id_(id) {}
   const char * what () const throw ()
   {
     return ("Game " + id_ + " is already started!").c_str();
   }
 private:
   std::string id_;
};

class DuplicatePlayerException : public std::exception {
 public:
   DuplicatePlayerException(const std::string& name, const std::string& game_id) : game_id_(game_id), name_(name) {}
   const char * what () const throw ()
   {
     return ("Player " + name_ + " is already registered for game: " + game_id_).c_str();
   }
 private:
   std::string game_id_;
   std::string name_;
};

class NoGameRegisteredException : public std::exception {
 public:
   NoGameRegisteredException(const std::string& id) : game_id_(id) {}
   const char * what () const throw ()
   {
     return ("No game is registered with id: " + game_id_).c_str();
   }
 private:
   std::string game_id_;
};

#endif
