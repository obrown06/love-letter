#ifndef SERVER_MODELS_EXCEPTIONS_H
#define SERVER_MODELS_EXCEPTIONS_H

#include "models/game-update.hpp"
#include "models/card.hpp"

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

class GameNotInProgressException : public std::exception {
 public:
   GameNotInProgressException(const std::string& id) : id_(id) {}
   const char * what () const throw ()
   {
     return ("Game " + id_ + " is not in progress!").c_str();
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

class NoSuchPlayerException : public std::exception {
 public:
   NoSuchPlayerException(const std::string& name, const std::string& game_id) : game_id_(game_id), name_(name) {}
   const char * what () const throw ()
   {
     return ("Player " + name_ + " is not registered for game: " + game_id_).c_str();
   }
 private:
   std::string game_id_;
   std::string name_;
};

class MoveOutOfTurnException : public std::exception {
 public:
   MoveOutOfTurnException(const std::string& name) : name_(name) {}
   const char * what () const throw ()
   {
     return ("Player " + name_ + " is not allowed to move!").c_str();
   }
 private:
   std::string name_;
};

class IllegalMoveException : public std::exception {
 public:
   IllegalMoveException(const std::string& name,
                          const GameUpdate::Move::MoveType& move_type)
                            : move_type_(move_type),
                              name_(name) {}
   const char * what () const throw ()
   {
     return ("Player " + name_ + " is not allowed to take move: " + GameUpdate::Move::GetMoveTypeString(move_type_)).c_str();
   }
 private:
   GameUpdate::Move::MoveType move_type_;
   std::string name_;
};

class OutOfTurnException : public std::exception {
 public:
   OutOfTurnException(const std::string& name) : name_(name) {}
   const char * what () const throw ()
   {
     return ("Player " + name_ + " tried to act out of turn!").c_str();
   }
 private:
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

class PlayerSizeException : public std::exception {
 public:
   PlayerSizeException(const std::string& id) : id_(id) {}
   const char * what () const throw ()
   {
     return ("Game " + id_ + " has too few or too many players. Games must have at least 2 players and no more than 8!").c_str();
   }
 private:
   std::string id_;
};

class NoNextMoveException : public std::exception {
 public:
   NoNextMoveException(const GameUpdate::Move::MoveType& type) : type_(type) {}
   const char * what () const throw ()
   {
     return ("MoveType " + GameUpdate::Move::GetMoveTypeString(type_) + " has no next move").c_str();
   }
 private:
   GameUpdate::Move::MoveType type_;
};

class NoCardException : public std::exception {
 public:
   NoCardException(const std::string& player_id, const Card::Type& type) : player_id_(player_id), type_(type) {}
   const char * what () const throw ()
   {
     return ("Player " + player_id_ + " doesn't have a card with type: " + Card::GetCardTypeString(type_)).c_str();
   }
 private:
   Card::Type type_;
   std::string player_id_;
};

#endif
