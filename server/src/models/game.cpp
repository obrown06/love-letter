#include "models/game.hpp"
#include "models/exceptions.hpp"

#include <algorithm>
#include <iostream>
#include <random>

namespace {

  constexpr int kInitialRound = 1;

  const std::vector<Card::Type> kUnshuffledDeck = {
    Card::PRINCESS,
    Card::COUNTESS,
    Card::KING,
    Card::PRINCE,
    Card::PRINCE,
    Card::HANDMAID,
    Card::HANDMAID,
    Card::BARON,
    Card::BARON,
    Card::PRIEST,
    Card::PRIEST,
    Card::GUARD,
    Card::GUARD,
    Card::GUARD,
    Card::GUARD,
    Card::GUARD
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

Game::Round* Game::GetMutableLatestRound() {
  return &rounds_.at(rounds_.size() - 1);
}

Game::Round Game::GetLatestRound() const {
  return rounds_.at(rounds_.size() - 1);
}

int Game::GetTokensToWin() const {
  return n_tokens_to_win_;
}

std::vector<Game::GamePlayer> Game::GetPlayers() const {
  return players_;
}

Game::GamePlayer* Game::GetPlayer(const std::string& player_id) {
  auto iter = std::find_if(players_.begin(), players_.end(), [&](const GamePlayer& player) {
    return player.player_id == player_id;
  });
  if (iter != players_.end()) {
    return &(*iter);
  }
  return nullptr;
}

void Game::AddPlayer(const std::string& player_id) {
  CheckGameNotStarted();
  if (GetPlayer(player_id) != nullptr) {
    throw DuplicatePlayerException(player_id, id_);
  }
  Game::GamePlayer player;
  player.player_id = player_id;
  players_.push_back(player);
}

void Game::ProcessUpdate(const GameUpdate& update) {
  switch(update.update_type) {
    case GameUpdate::UpdateType::JOIN_GAME_REQUEST:
      return AddPlayer(update.player_id);
    case GameUpdate::UpdateType::START_GAME_REQUEST:
      return Start();
    case GameUpdate::UpdateType::MOVE_REQUEST:
      return ExecuteMove(update.move.get(), update.player_id);
  }
}

void Game::ValidateMove(const GameUpdate::Move& move, const std::string& player_id) {
  CheckGameInProgress();
  if (GetPlayer(player_id) != nullptr) {
    throw NoSuchPlayerException(player_id, id_);
  }
  GetMutableLatestRound()->ValidateMove(move, player_id);
}

void Game::ExecuteMove(const GameUpdate::Move& move, const std::string& player_id) {
  ValidateMove(move, player_id);
  GetMutableLatestRound()->ExecuteMove(move);
  MaybeUpdateGameState();
}

void Game::MaybeUpdateGameState() {
  if (!GetMutableLatestRound()->IsComplete()) {
    return;
  }
  for (const auto& winner : GetMutableLatestRound()->GetWinners()) {
    auto it = std::find_if(players_.begin(), players_.end(), [&winner](const Game::GamePlayer& player) {
      return player.player_id == winner.player_id;
    });
    it->ntokens_held++;
  }

  if (!IsComplete()) {
    AdvanceRound();
  }
}

void Game::AdvanceRound() {
  rounds_.push_back(Round(players_));
}

bool Game::IsComplete() const {
  return (std::find_if(players_.begin(), players_.end(), [this](const Game::GamePlayer& player) {
    return player.ntokens_held >= GetTokensToWin();
  }) != players_.end());
}

std::vector<Game::GamePlayer*> Game::GetWinners() {
  std::vector<Game::GamePlayer*> winners;
  for (auto& player : players_) {
    if (player.ntokens_held > GetTokensToWin()) {
      winners.push_back(&player);
    }
  }
  return winners;
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

  AdvanceRound();
}

void Game::CheckGameNotStarted() const {
  if (state_ != State::WAITING) {
    throw GameAlreadyStartedException(id_);
  }
}

void Game::CheckGameInProgress() const {
  if (state_ != State::WAITING) {
    throw GameNotInProgressException(id_);
  }
}

// Game::Round

Game::Round::Round(const std::vector<Game::GamePlayer>& round_players) {
  for (const auto& player : round_players) {
    Game::Round::RoundPlayer round_player;
    round_player.player_id = player.player_id;
    round_player.still_in_round = true;
    players_.push_back(round_player);
  }

  std::vector<Card::Type> shuffled_deck = kUnshuffledDeck;
  std::random_device rd;
  std::mt19937 g(rd());
  std::shuffle(shuffled_deck.begin(), shuffled_deck.end(), g);

  for (auto& player : players_) {
    player.held_cards.push_back(Card(shuffled_deck.back()));
    shuffled_deck.pop_back();
  }

  // Remove one extra card from deck to prevent card counting.
  shuffled_deck.pop_back();

  for (auto& card_type : shuffled_deck) {
    deck_.push_back(Card(shuffled_deck.back()));
    shuffled_deck.pop_back();
  }
}

int Game::Round::GetDeckSize() const {
  return deck_.size();
}

Game::Round::RoundPlayer* Game::Round::GetPlayer(const std::string& player_id) {
  auto iter = std::find_if(players_.begin(), players_.end(), [&](const Game::Round::RoundPlayer& player) {
    return player.player_id == player_id;
  });
  if (iter != players_.end()) {
    return &(*iter);
  }
  return nullptr;
}

Game::Round::Turn* Game::Round::GetMutableLatestTurn() {
  return &turns_.at(turns_.size() - 1);
}

void Game::Round::ValidateMove(const GameUpdate::Move& move,
                               const std::string& player_id) {
  Game::Round::RoundPlayer* player = GetPlayer(player_id);
  if (player_id != GetMutableLatestTurn()->player_id) {
    throw MoveOutOfTurnException(player_id);
  }
  // TODO implement turn-based validation
}

bool Game::Round::IsComplete() const {
  return deck_.size() == 0 || GetPlayersInRound().size() <= 1;
}

std::vector<Game::Round::RoundPlayer> Game::Round::GetPlayers() const {
  return players_;
}

std::vector<Game::Round::RoundPlayer> Game::Round::GetWinners() const {
  std::vector<Game::Round::RoundPlayer> winners;
  auto players_in_round = GetPlayersInRound();
  if (players_in_round.size() == 1) {
    winners.push_back(*players_in_round.at(0));
    return winners;
  }

  // If one player has the highest card, that player wins.
  std::vector<const Game::Round::RoundPlayer*> candidates;
  int highest_card_value = 0;
  for (auto player : players_in_round) {
    if (player->held_cards[0].GetValue() > highest_card_value) {
      candidates.clear();
    }
    if (player->held_cards[0].GetValue() >= highest_card_value) {
      candidates.push_back(player);
    }
  }

  if (candidates.size() == 1) {
    winners.push_back(*candidates.at(0));
    return winners;
  }

  // Use discard pile to break ties.
  std::vector<std::pair<const Game::Round::RoundPlayer*, int>> candidates_with_sums;
  for (size_t i = 0; i < candidates.size(); i++) {
    int sum = 0;
    for (const auto& card : candidates[i]->discarded_cards) {
      sum += card.GetValue();
    }
    if (candidates_with_sums.size() == 0 || candidates_with_sums[0].second < sum) {
      candidates_with_sums.clear();
      candidates_with_sums.push_back(std::make_pair(candidates[i], sum));
    }
  }

  for (const auto& winner : candidates_with_sums) {
    winners.push_back(*winner.first);
  }
  return winners;
}

std::vector<const Game::Round::RoundPlayer*> Game::Round::GetPlayersInRound() const {
  std::vector<const Game::Round::RoundPlayer*> players_in_round;
  for (auto& player : players_) {
    if (player.still_in_round) {
      players_in_round.push_back(&player);
    }
  }
  return players_in_round;
}

void Game::Round::ExecuteMove(const GameUpdate::Move& move) {
  switch (move.move_type) {
    case GameUpdate::Move::DRAW_CARD:
      DrawCard(GetMutableLatestTurn()->player_id);
      break;
  }
  GetMutableLatestTurn()->ExecuteMove(move);
  MaybeUpdateRoundState();
}

void Game::Round::DrawCard(const std::string& drawing_player_id) {
  Game::Round::RoundPlayer* player = GetPlayer(drawing_player_id);
  player->held_cards.push_back(deck_.back());
  deck_.pop_back();
}

void Game::Round::MaybeUpdateRoundState() {
  if (GetMutableLatestTurn()->IsComplete() && !IsComplete()) {
    AdvanceTurn();
  }
}

void Game::Round::AdvanceTurn() {
  auto it = std::find_if(players_.begin(), players_.end(), [this](const Game::Round::RoundPlayer& player) {
    return player.player_id == this->GetMutableLatestTurn()->player_id;
  });
  auto advance = [this](std::vector<Game::Round::RoundPlayer>::iterator& it) {
    it++;
    if (it == players_.end()) {
      it = players_.begin();
    }
  };
  advance(it);
  while (true) {
    if (it->still_in_round) {
      turns_.push_back(Turn(it->player_id));
      return;
    }
  }
}

// Game::Round::Turn


void Game::Round::Turn::ExecuteMove(const GameUpdate::Move& move) {
  previous_moves.push_back(move);
}

bool Game::Round::Turn::IsComplete() {
  return GetMutableLatestMove()->move_type == GameUpdate::Move::MoveType::SELECT_PLAYER;
}

GameUpdate::Move* Game::Round::Turn::GetMutableLatestMove() {
  return &previous_moves.at(previous_moves.size() - 1);
}
