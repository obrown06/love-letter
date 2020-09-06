#include "models/game.hpp"
#include "models/exceptions.hpp"

#include <algorithm>
#include <iostream>
#include <random>

namespace {

  constexpr int kInitialRound = 1;

  const std::vector<Game::Card> kUnshuffledDeck = {
    Game::PRINCESS,
    Game::COUNTESS,
    Game::KING,
    Game::PRINCE,
    Game::PRINCE,
    Game::HANDMAID,
    Game::HANDMAID,
    Game::BARON,
    Game::BARON,
    Game::PRIEST,
    Game::PRIEST,
    Game::GUARD,
    Game::GUARD,
    Game::GUARD,
    Game::GUARD,
    Game::GUARD
  };
};

std::string Game::GetId() const {
  return id_;
}

std::string Game::GetCreator() const {
  return creator_;
}

Game::State Game::GetState() const {
  return state_;
}

int Game::GetRound() const {
  return round_;
}

int Game::GetTokensToWin() const {
  return n_tokens_to_win_;
}

int Game::GetNCardsInCenter() const {
  return deck_.size();
}

std::vector<Game::PlayerInfo> Game::GetPlayers() const {
  return players_;
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
  if (std::find_if(players_.begin(), players_.end(), [&](const PlayerInfo& info) {
    return info.player_id == player_id;
  }) != players_.end()) {
    throw DuplicatePlayerException(player_id, id_);
  }
  PlayerInfo info;
  info.player_id = player_id;
  players_.push_back(info);
}

void Game::Start() {
  CheckGameNotStarted();
  if (players_.size() < 2) {
    throw PlayerSizeException(id_);
  }

  state_ = State::IN_PROGRESS;

  // Compute n_tokens_to_win_ based on game size
  if (players_.size() == 2) {
    n_tokens_to_win_ = 7;
  } else if (players_.size() == 3) {
    n_tokens_to_win_ = 5;
  } else {
    n_tokens_to_win_ = 4;
  }

  ShuffleCardsAndDeal();

  round_ = kInitialRound;

  player_with_turn_ = players_[0].player_id;
}

void Game::ShuffleCardsAndDeal() {
  std::vector<Game::Card> deck_to_shuffle = kUnshuffledDeck;

  std::random_device rd;
  std::mt19937 g(rd());
  std::shuffle(deck_to_shuffle.begin(), deck_to_shuffle.end(), g);

  for (auto& player_info : players_) {
    player_info.held_cards.push_back(deck_to_shuffle.back());
    deck_to_shuffle.pop_back();
  }

  // Remove one extra card from deck to prevent card counting.
  deck_to_shuffle.pop_back();

  deck_ = deck_to_shuffle;
}

void Game::CheckGameNotStarted() const {
  if (state_ != State::WAITING) {
    throw GameAlreadyStartedException(id_);
  }
}
