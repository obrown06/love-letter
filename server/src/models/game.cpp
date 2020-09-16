#include "models/game.hpp"
#include "models/exceptions.hpp"

#include <algorithm>
#include <iostream>
#include <random>
#include <sstream>

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

  const std::string kGameSummaryMessage = "Game won by: ";
  const std::string kRoundSummaryMessage = "Round won by: ";
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

const std::vector<Game::Round>& Game::GetRounds() const {
  return rounds_;
}

int Game::GetTokensToWin() const {
  return n_tokens_to_win_;
}

std::vector<Game::GamePlayer> Game::GetPlayers() const {
  return players_;
}

Game::GamePlayer* Game::GetMutablePlayer(const std::string& player_id) {
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
  if (GetMutablePlayer(player_id) != nullptr) {
    throw DuplicatePlayerException(player_id, id_);
  }
  Game::GamePlayer player(player_id);
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
  if (GetMutablePlayer(player_id) != nullptr) {
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
  } else {
    state_ = Game::State::COMPLETE;
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

std::vector<Game::GamePlayer> Game::GetWinners() const {
  std::vector<Game::GamePlayer> winners;
  for (auto& player : players_) {
    if (player.ntokens_held > GetTokensToWin()) {
      winners.push_back(player);
    }
  }
  return winners;
}

std::string Game::GetSummary() const {
  auto winners = GetWinners();
  std::stringstream ss;
  ss << kGameSummaryMessage;
  for (size_t i = 0; i < winners.size(); i++) {
    ss << winners[i].player_id;
    if (i < winners.size() - 1) {
      ss << ", ";
    }
  }
  return ss.str();
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
  extra_card_ = shuffled_deck.back();
  shuffled_deck.pop_back();

  for (auto& card_type : shuffled_deck) {
    deck_.push_back(Card(shuffled_deck.back()));
    shuffled_deck.pop_back();
  }

  turns_.push_back(Turn(&players_[0]));
}

int Game::Round::GetDeckSize() const {
  return deck_.size();
}

Game::Round::RoundPlayer* Game::Round::GetMutablePlayer(const std::string& player_id) {
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

bool Game::Round::IsComplete() const {
  return deck_.size() == 0 || GetPlayersInRound().size() <= 1;
}

const std::vector<Game::Round::Turn>& Game::Round::GetTurns() const {
  return turns_;
}

std::string Game::Round::GetSummary() const {
  auto winners = GetWinners();
  std::stringstream ss;
  ss << kRoundSummaryMessage;
  for (size_t i = 0; i < winners.size(); i++) {
    ss << winners[i].player_id;
    if (i < winners.size() - 1) {
      ss << ", ";
    }
  }
  return ss.str();
}

std::vector<Game::Round::RoundPlayer> Game::Round::GetPlayers() const {
  return players_;
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

std::vector<Game::Round::RoundPlayer> Game::Round::GetSelectablePlayers() const {
  std::vector<Game::Round::RoundPlayer> selectable_players;
  std::vector<const Game::Round::RoundPlayer*> players_in_round = GetPlayersInRound();
  for (auto& player : players_in_round) {
    if (!player->immune) {
      selectable_players.push_back(*player);
    }
  }
  return selectable_players;
}

std::vector<std::pair<Game::Round::RoundPlayer, Game::Round::RoundPlayer>>
Game::Round::GetViewPlayerPairs() const {
  const auto& turns = GetTurns();
  const auto& latest_turn = turns.at(turns.size() - 1);
  const auto& discarded_card = latest_turn.GetDiscardedCard();
  std::vector<std::pair<Game::Round::RoundPlayer, Game::Round::RoundPlayer>> pairs;
  if (discarded_card.RequiredViewMovesCount() == 0) {
    return pairs;
  }
  auto viewed_player = std::find_if(players_.begin(),
                                    players_.end(),
                                    [&latest_turn](const Game::Round::RoundPlayer& player) {
    return player.player_id == latest_turn.GetSelectedPlayerId();
  });
  auto pair = std::pair<Game::Round::RoundPlayer, Game::Round::RoundPlayer>(*latest_turn.player, *viewed_player);
  pairs.push_back(pair);

  if (discarded_card.RequiredViewMovesCount() == 2) {
    const auto reciprocal_pair = std::pair<Game::Round::RoundPlayer, Game::Round::RoundPlayer>(pair.second, pair.first);
    pairs.push_back(reciprocal_pair);
  }
  return pairs;
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
      highest_card_value = player->held_cards[0].GetValue();
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
    if (candidates_with_sums.size() > 0 && candidates_with_sums[0].second < sum) {
      candidates_with_sums.clear();
    }
    if (candidates_with_sums.size() == 0 || candidates_with_sums[0].second <= sum) {
      candidates_with_sums.push_back(std::make_pair(candidates[i], sum));
    }
  }

  for (const auto& winner : candidates_with_sums) {
    winners.push_back(*winner.first);
  }
  return winners;
}

void Game::Round::ValidateMove(const GameUpdate::Move& move,
                               const std::string& player_id) {
  Game::Round::RoundPlayer* player = GetMutablePlayer(player_id);
  if (player_id != GetMutableLatestTurn()->player->player_id) {
    throw MoveOutOfTurnException(player_id);
  }
  // TODO implement turn-based validation
}

void Game::Round::ExecuteMove(const GameUpdate::Move& move) {
  auto latest_turn = GetMutableLatestTurn();
  switch (move.move_type) {
    case GameUpdate::Move::DRAW_CARD:
      DrawCard(latest_turn->player->player_id);
      break;
    case GameUpdate::Move::DISCARD_CARD:
      DiscardCardAndApplyEffect(latest_turn->player->player_id, move.selected_card.get());
      break;
    case GameUpdate::Move::SELECT_PLAYER:
      ApplyEffect(latest_turn->player->player_id,
                  latest_turn->GetMutableLatestMove()->selected_card.get(),
                  move.selected_player_id,
                  move.selected_card->GetType());
  }
  GetMutableLatestTurn()->ExecuteMove(move);
  MaybeUpdateRoundState();
}

void Game::Round::DrawCard(const std::string& drawing_player_id) {
  Game::Round::RoundPlayer* player = GetMutablePlayer(drawing_player_id);
  if (deck_.empty()) {
    player->held_cards.push_back(extra_card_.get());
    extra_card_ = boost::none;
  } else {
    player->held_cards.push_back(deck_.back());
    deck_.pop_back();
  }
}

void Game::Round::DiscardCardAndApplyEffect(const std::string& discarding_player_id, const Card& card) {
  DiscardCard(discarding_player_id, card);
  boost::optional<std::string> unused_selected_player_id;
  boost::optional<Card::Type> unused_predicted_card_type;
  ApplyEffect(discarding_player_id, card, unused_selected_player_id, unused_predicted_card_type);
}

void Game::Round::DiscardCard(const std::string& discarding_player_id, const Card& card) {
  Game::Round::RoundPlayer* player = GetMutablePlayer(discarding_player_id);
  auto it = std::find_if(player->held_cards.begin(), player->held_cards.end(), [&card] (const Card& held_card) {
    return held_card.GetType() == card.GetType();
  });
  if (it == player->held_cards.end()) {
    throw NoCardException(discarding_player_id, card.GetType());
  }
  player->held_cards.erase(it);
  player->discarded_cards.push_back(card);
}

void Game::Round::ApplyEffect(const std::string& discarding_player_id,
                              const Card& card,
                              const boost::optional<std::string>& selected_player_id,
                              const boost::optional<Card::Type>& predicted_card_type) {
  switch (card.GetType()) {
    case Card::Type::PRINCESS:
      return ApplyEffectPRINCESS(discarding_player_id);
    case Card::Type::KING:
      return ApplyEffectKING(discarding_player_id, selected_player_id);
    case Card::Type::PRINCE:
      return ApplyEffectPRINCE(selected_player_id);
    case Card::Type::HANDMAID:
      return ApplyEffectHANDMAID(discarding_player_id);
    case Card::Type::BARON:
      return ApplyEffectBARON(discarding_player_id, selected_player_id);
    case Card::Type::GUARD:
      return ApplyEffectGUARD(selected_player_id, predicted_card_type);
    case Card::Type::PRIEST:
    case Card::Type::COUNTESS:
      return;
  }
}

void Game::Round::ApplyEffectPRINCESS(const std::string& discarding_player_id) {
  Game::Round::RoundPlayer* player = GetMutablePlayer(discarding_player_id);
  player->still_in_round = false;
}

void Game::Round::ApplyEffectKING(const std::string& discarding_player_id,
                                  const boost::optional<std::string>& selected_player_id)
{
  if (!selected_player_id) {
    return;
  }
  Game::Round::RoundPlayer* discarding_player = GetMutablePlayer(discarding_player_id);
  Game::Round::RoundPlayer* selected_player = GetMutablePlayer(selected_player_id.get());
  std::vector<Card> tmp = discarding_player->held_cards;
  discarding_player->held_cards = selected_player->held_cards;
  selected_player->held_cards = tmp;
}

void Game::Round::ApplyEffectPRINCE(const boost::optional<std::string>& selected_player_id)
{
  if (!selected_player_id) {
    return;
  }
  auto player = GetMutablePlayer(selected_player_id.get());
  auto held_card = player->held_cards.front();
  DiscardCard(selected_player_id.get(), held_card);
  if (held_card.GetType() == Card::Type::PRINCESS) {
    player->still_in_round = false;
  } else {
    DrawCard(selected_player_id.get());
  }
}

void Game::Round::ApplyEffectHANDMAID(const std::string& discarding_player_id)
{
  Game::Round::RoundPlayer* player = GetMutablePlayer(discarding_player_id);
  player->immune = true;
}

void Game::Round::ApplyEffectBARON(const std::string& discarding_player_id,
                                  const boost::optional<std::string>& selected_player_id)
{
  if (!selected_player_id) {
    return;
  }
  Game::Round::RoundPlayer* discarding_player = GetMutablePlayer(discarding_player_id);
  Game::Round::RoundPlayer* selected_player = GetMutablePlayer(selected_player_id.get());
  if (discarding_player->held_cards.front().GetValue() > selected_player->held_cards.front().GetValue()) {
    selected_player->still_in_round = false;
  } else if (discarding_player->held_cards.front().GetValue() < selected_player->held_cards.front().GetValue()) {
    discarding_player->still_in_round = false;
  }
}

void Game::Round::ApplyEffectGUARD(const boost::optional<std::string>& selected_player_id,
                                   const boost::optional<Card::Type>& predicted_card_type)
{
  if (!selected_player_id) {
    return;
  }
  Game::Round::RoundPlayer* selected_player = GetMutablePlayer(selected_player_id.get());
  if (selected_player->held_cards.front().GetType() == predicted_card_type.get()) {
    selected_player->still_in_round = false;
  }
}

void Game::Round::MaybeUpdateRoundState() {
  if (GetMutableLatestTurn()->IsComplete(GetPlayersInRound()) && !IsComplete()) {
    AdvanceTurn();
  }
}

void Game::Round::AdvanceTurn() {
  auto it = std::find_if(players_.begin(), players_.end(), [this](const Game::Round::RoundPlayer& player) {
    return player.player_id == this->GetMutableLatestTurn()->player->player_id;
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
      return MakeNewTurn(it->player_id);
    }
    advance(it);
  }
}

void Game::Round::MakeNewTurn(const std::string& player_id) {
  Game::Round::RoundPlayer* player = GetMutablePlayer(player_id);

  // Satisfy Handmaid rule
  player->immune = false;
  turns_.push_back(Turn(player));
}

// Game::Round::Turn

const Card& Game::Round::Turn::GetDiscardedCard() const {
  auto discard_move = std::find_if(previous_moves.begin(),
                                   previous_moves.end(),
                                   [] (const GameUpdate::Move& move) {
      return move.move_type == GameUpdate::Move::MoveType::DISCARD_CARD;
  });
  return discard_move->selected_card.get();
}

std::string Game::Round::Turn::GetSelectedPlayerId() const {
  auto select_move = std::find_if(previous_moves.begin(),
                                   previous_moves.end(),
                                   [] (const GameUpdate::Move& move) {
      return move.move_type == GameUpdate::Move::MoveType::SELECT_PLAYER;
  });
  return select_move->selected_player_id.get();
}

bool Game::Round::Turn::IsComplete(std::vector<const Game::Round::RoundPlayer*> players_in_round) const {
  if (previous_moves.size() == 0) {
    return false;
  }
  GameUpdate::Move::MoveType latest_move_type = GetLatestMove().move_type;

  if (latest_move_type == GameUpdate::Move::MoveType::DRAW_CARD) {
    return false;
  }

  auto discarded_card = GetDiscardedCard();

  if (latest_move_type == GameUpdate::Move::MoveType::DISCARD_CARD) {
    bool another_non_immune_player_in_round = (std::find_if(players_in_round.begin(), players_in_round.end(),
     [this](const Game::Round::RoundPlayer* player) {
      return player->player_id != this->player->player_id && !player->immune;
    }) != players_in_round.end());
    return !discarded_card.RequiresSelectMove(another_non_immune_player_in_round);
  } else if (latest_move_type == GameUpdate::Move::MoveType::SELECT_PLAYER) {
    return discarded_card.RequiredViewMovesCount() == 0;
  } else {
    return discarded_card.RequiredViewMovesCount() == std::count_if(previous_moves.begin(),
                                                                 previous_moves.end(),
                                          [] (const GameUpdate::Move& move) {
                                              return move.move_type == GameUpdate::Move::MoveType::VIEW_CARD;
                                          });
  }
}

GameUpdate::Move* Game::Round::Turn::GetMutableLatestMove() {
  return &previous_moves.at(previous_moves.size() - 1);
}

const GameUpdate::Move& Game::Round::Turn::GetLatestMove() const {
  return previous_moves.at(previous_moves.size() - 1);
}

GameUpdate::Move::MoveType Game::Round::Turn::GetNextMoveType() const {
  if (previous_moves.size() == 0) {
    return GameUpdate::Move::MoveType::DRAW_CARD;
  }
  GameUpdate::Move::MoveType latest_move_type = previous_moves.at(previous_moves.size() - 1).move_type;
  return GameUpdate::Move::GetNextMoveType(latest_move_type);
}

const std::vector<GameUpdate::Move>& Game::Round::Turn::GetMoves() const {
  return previous_moves;
}

std::string Game::Round::Turn::GetSummary() const {
  auto discard_move = std::find_if(previous_moves.begin(),
                                   previous_moves.end(),
  [](const GameUpdate::Move& move) {
    return move.move_type == GameUpdate::Move::DISCARD_CARD;
  });
  std::stringstream ss;
  ss << player->player_id
     << " played the "
     << Card::GetCardTypeString(discard_move->selected_card->GetType())
     << " card";
  auto select_move = std::find_if(previous_moves.begin(),
                                   previous_moves.end(),
  [](const GameUpdate::Move& move) {
    return move.move_type == GameUpdate::Move::SELECT_PLAYER;
  });
  if (select_move != previous_moves.end()) {
    ss << " and "
       << Card::GetActionString(discard_move->selected_card->GetType(),
                                      *select_move->selected_player_id,
                                      select_move->selected_card);
  }
  ss << ".";
  return ss.str();
}

void Game::Round::Turn::ExecuteMove(const GameUpdate::Move& move) {
  previous_moves.push_back(move);
}

// Game::Round::RoundPlayer

std::vector<Card> Game::Round::RoundPlayer::GetDiscardableCards() const {
  return Card::GetDiscardableCards(held_cards);
}
