#include "models/game.hpp"
#include "models/exceptions.hpp"

#include <iostream>

std::string Game::GetId() const {
  return id_;
}

std::string Game::GetCreator() const {
  return creator_;
}

Game::State Game::GetState() const {
  return state_;
}

void Game::ProcessUpdate(const GameUpdate& update) {
  switch(update.update_type) {
    case GameUpdate::UpdateType::JOIN_GAME_REQUEST:
      return AddPlayer(update.player_id);
    case GameUpdate::UpdateType::START_GAME_REQUEST:
      return Start();
  }
}

void Game::AddPlayer(const std::string& player_id) {
  CheckGameNotStarted();
  if (std::find(players_.begin(), players_.end(), player_id) != players_.end()) {
    throw DuplicatePlayerException(player_id, id_);
  }
  players_.push_back(player_id);
}

void Game::Start() {
  CheckGameNotStarted();
  state_ = State::IN_PROGRESS;
}

void Game::CheckGameNotStarted() const {
  if (state_ != State::WAITING) {
    throw GameAlreadyStartedException(id_);
  }
}
