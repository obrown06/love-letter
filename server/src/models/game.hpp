#ifndef SERVER_MODELS_GAME_HPP
#define SERVER_MODELS_GAME_HPP

#include "models/game-update.hpp"

#include <string>
#include <vector>

class Game {
public:
  Game(const std::string& id, const std::string& creator) :
    id_(id),
    creator_(creator),
    state_(State::WAITING) {}

  enum State {
    WAITING,
    IN_PROGRESS,
    COMPLETE,
  };

  std::string GetId() const;
  std::string GetCreator() const;
  Game::State GetState() const;
  void ProcessUpdate(const GameUpdate& update);
private:
  void AddPlayer(const std::string& name);
  void Start();
  void CheckGameNotStarted() const;
  std::string id_;
  std::string creator_;
  std::vector<std::string> players_;
  State state_;
};

#endif
