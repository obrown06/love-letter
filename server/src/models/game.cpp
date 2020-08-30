#include "models/game.hpp"
#include "models/exceptions.hpp"

std::string Game::GetId() const {
  return id_;
}

std::string Game::GetCreator() const {
  return creator_;
}

void Game::AddPlayer(const std::string& name) {
  CheckGameNotStarted();
  if (std::find(players_.begin(), players_.end(), name) != players_.end()) {
    throw DuplicatePlayerException(name, id_);
  }
  players_.push_back(name);
}

void Game::CheckGameNotStarted() const {
  if (state_ != State::WAITING) {
    throw GameAlreadyStartedException(id_);
  }
}
