#ifndef SERVER_MODELS_GAME_HPP
#define SERVER_MODELS_GAME_HPP

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
  void AddPlayer(const std::string& name);
private:
  void CheckGameNotStarted() const;
  std::string id_;
  std::string creator_;
  std::vector<std::string> players_;
  State state_;
};

#endif
