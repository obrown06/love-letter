#include "models/game.hpp"
#include "models/exceptions.hpp"

#include <algorithm>
#include <iostream>
#include <map>
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

  const std::map<GameUpdate::Move::MoveType, std::string> kMoveTypesToInstructions = {
    { GameUpdate::Move::MoveType::DRAW_CARD, "Draw a card from the center." },
    { GameUpdate::Move::MoveType::DISCARD_CARD, "Play one of the cards in your hand." },
    { GameUpdate::Move::MoveType::SELECT_PLAYER, "Select a player." },
    { GameUpdate::Move::MoveType::VIEW_CARD, "View the other player's hand." }
  };

  const std::string kInstructionPrefix = "NEXT MOVE: ";
  const std::string kPredictAndSelectInstruction = "Select a player and predict their hand.";

  const std::string kGameSummaryMessage = "Game won by: ";
  const std::string kRoundSummaryMessage = "Round won by: ";
};

std::string Game::GetId() const {
  return id_;
}

bool Game::HasPlayer(const std::string& player_id) const {
  return GetPlayer(player_id) != nullptr;
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

const Game::Round& Game::GetLatestRound() const {
  return rounds_.at(rounds_.size() - 1);
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

const Game::GamePlayer* Game::GetPlayer(const std::string& player_id) const {
  auto iter = std::find_if(players_.begin(), players_.end(), [&](const GamePlayer& player) {
    return player.player_id == player_id;
  });
  if (iter != players_.end()) {
    return &(*iter);
  }
  return nullptr;
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
  std::cout << "ADDING PLAYER: " << player_id << std::endl;
  if (HasPlayer(player_id)) {
    throw DuplicatePlayerException(player_id, id_);
  }
  CheckGameNotStarted();
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
  if (!HasPlayer(player_id)) {
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
  std::cout << "starting MaybeUpdateGameState\n";
  if (!GetLatestRound().IsComplete()) {
    std::cout << "round wasn't complete\n";
    return;
  }
  std::cout << "round was complete\n";
  for (const auto& winner : GetLatestRound().GetWinners()) {
    auto it = std::find_if(players_.begin(), players_.end(), [&winner](const Game::GamePlayer& player) {
      return player.player_id == winner.player_id;
    });
    it->ntokens_held++;
  }

  if (std::find_if(players_.begin(), players_.end(), [this](const Game::GamePlayer& player) {
    return player.ntokens_held >= GetTokensToWin();
  }) != players_.end()) {
    state_ = Game::State::COMPLETE;
  }

  if (!IsComplete()) {
    AdvanceRound();
  }
}

void Game::AdvanceRound() {
  boost::optional<std::string> previous_winner_id;
  if (!rounds_.empty()) {
    previous_winner_id = GetLatestRound().GetWinners()[0].player_id;
  }
  rounds_.push_back(Round(players_, previous_winner_id));
}

bool Game::IsComplete() const {
  return state_ == Game::State::COMPLETE;
}

std::vector<Game::GamePlayer> Game::GetWinners() const {
  std::vector<Game::GamePlayer> winners;
  for (auto& player : players_) {
    if (player.ntokens_held >= GetTokensToWin()) {
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
  if (state_ != State::IN_PROGRESS) {
    throw GameNotInProgressException(id_);
  }
}

// Game::Round

Game::Round::Round(const std::vector<Game::GamePlayer>& round_players,
                   const boost::optional<std::string>& previous_winner_id) {

  for (const auto& player : round_players) {
    Game::Round::RoundPlayer round_player;
    round_player.player_id = player.player_id;
    round_player.still_in_round = true;
    round_player.immune = false;
    players_.push_back(round_player);
  }

  std::random_device rd;
  std::mt19937 g(rd());
  std::vector<Card::Type> shuffled_deck = kUnshuffledDeck;
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

  auto player_id = previous_winner_id ? previous_winner_id.get() : players_[0].player_id;
  std::cout << "player id is : " << player_id << "\n";
  MakeNewTurn(player_id);
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

const Game::Round::RoundPlayer& Game::Round::GetPlayer(const std::string& player_id) const {
  auto iter = std::find_if(players_.begin(), players_.end(), [&](const Game::Round::RoundPlayer& player) {
    return player.player_id == player_id;
  });
  if (iter == players_.end()) {
    throw std::exception();
  }
  return *iter;
}

Game::Round::Turn* Game::Round::GetMutableLatestTurn() {
  std::cout << "in GetMutableLatestTurn\n";
  return &turns_.at(turns_.size() - 1);
}

const Game::Round::Turn& Game::Round::GetLatestTurn() const {
  return turns_.at(turns_.size() - 1);
}

bool Game::Round::IsComplete() const {
  std::cout << "starting Round::IsComplete\n";
  bool is_complete = GetLatestTurn().IsComplete() && (deck_.size() == 0 || GetPlayersInRound().size() <= 1);
  std::cout << "returning from Round::IsComplete\n";
  return is_complete;
}

const std::vector<Game::Round::Turn>& Game::Round::GetTurns() const {
  return turns_;
}

std::string Game::Round::GetSummary() const {
  std::cout << "in Round::GetSummary\n";
  auto winners = GetWinners();
  std::stringstream ss;
  ss << kRoundSummaryMessage;
  for (size_t i = 0; i < winners.size(); i++) {
    ss << winners[i].player_id;
    if (i < winners.size() - 1) {
      ss << ", ";
    }
  }
  std::cout << "returning from Round::GetSummary\n";
  return ss.str();
}

std::vector<Game::Round::RoundPlayer> Game::Round::GetPlayers() const {
  return players_;
}

std::vector<const Game::Round::RoundPlayer*> Game::Round::GetPlayersInRound() const {
  std::cout << "in GetPlayersInRound\n";
  std::vector<const Game::Round::RoundPlayer*> players_in_round;
  for (auto& player : players_) {
    if (player.still_in_round) {
      players_in_round.push_back(&player);
    }
  }
  std::cout << "returning from GetPlayersInRound\n";
  return players_in_round;
}

std::vector<Game::Round::RoundPlayer> Game::Round::GetSelectablePlayers() const {
  std::vector<Game::Round::RoundPlayer> selectable_players;
  std::vector<const Game::Round::RoundPlayer*> players_in_round = GetPlayersInRound();
  const auto& latest_turn = GetLatestTurn();
  if (latest_turn.GetLatestMove().move_type != GameUpdate::Move::MoveType::DISCARD_CARD) {
    return selectable_players;
  }
  auto discarded_card = latest_turn.GetDiscardedCard();
  for (auto& player : players_in_round) {
    if (!player->immune &&
        (player->player_id != latest_turn.GetPlayerId() || discarded_card.AllowsSelfSelection())) {
      selectable_players.push_back(*player);
    }
  }
  if (selectable_players.empty()) {
    selectable_players.push_back(GetPlayer(latest_turn.GetPlayerId()));
  }
  return selectable_players;
}

std::vector<std::pair<Game::Round::RoundPlayer, Game::Round::RoundPlayer>>
Game::Round::GetViewPlayerPairs() const {
  std::cout << "in GetViewPlayerPairs\n";
  const auto latest_turn = GetLatestTurn();
  const auto& discarded_card = latest_turn.GetDiscardedCard();
  std::cout << "discarded card was: " << Card::GetCardTypeString(discarded_card.GetType());
  std::vector<std::pair<Game::Round::RoundPlayer, Game::Round::RoundPlayer>> pairs;
  if (discarded_card.RequiredViewMovesCount() == 0) {
    std::cout << "no required view moves!\n";
    return pairs;
  }
  auto selecting_player = GetPlayer(latest_turn.GetPlayerId());
  auto selected_player = GetPlayer(latest_turn.GetSelectedPlayerId());

  if (!latest_turn.WasPlayerViewed(selected_player.player_id)) {
    auto pair = std::pair<Game::Round::RoundPlayer, Game::Round::RoundPlayer>(selecting_player, selected_player);
    pairs.push_back(pair);
  }

  if (discarded_card.RequiredViewMovesCount() == 2 && !latest_turn.WasPlayerViewed(selecting_player.player_id)) {
    const auto reciprocal_pair = std::pair<Game::Round::RoundPlayer, Game::Round::RoundPlayer>(selected_player, selecting_player);
    pairs.push_back(reciprocal_pair);
  }
  return pairs;
}

std::vector<Game::Round::RoundPlayer> Game::Round::GetWinners() const {
  std::cout << "in Round::GetWinners\n";
  std::vector<Game::Round::RoundPlayer> winners;
  auto players_in_round = GetPlayersInRound();
  if (players_in_round.size() == 1) {
    winners.push_back(*players_in_round.at(0));
    std::cout << "returning from Round::GetWinners\n";
    return winners;
  }

  std::cout << "multiple players in round... \n";

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
    std::cout << "returning from Round::GetWinners\n";
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
  std::cout << "returning from Round::GetWinners\n";
  return winners;
}

void Game::Round::ValidateMove(const GameUpdate::Move& move,
                               const std::string& player_id) {
  // TODO implement turn-based validation
}

void Game::Round::ExecuteMove(const GameUpdate::Move& move) {
  std::cout << "in ExecuteMove\n";
  const auto& latest_turn = GetLatestTurn();
  const auto& player = GetPlayer(latest_turn.GetPlayerId());
  switch (move.move_type) {
    case GameUpdate::Move::DRAW_CARD:
      DrawCard(player.player_id);
      break;
    case GameUpdate::Move::DISCARD_CARD:
      DiscardCard(move.discarded_card_type.get(), player.player_id);
      ApplyDiscardEffect(move.discarded_card_type.get(), player.player_id);
      break;
    case GameUpdate::Move::SELECT_PLAYER: {
      ApplySelectEffect(latest_turn.GetDiscardedCard().GetType(),
                        player.player_id,
                        move.selected_player_id.get(),
                        move.predicted_card_type);
      std::cout << "after ApplyEffect\n";
      break;
    }
    case GameUpdate::Move::VIEW_CARD: {
      ApplyViewEffect(latest_turn.GetDiscardedCard());
      break;
    }

  }
  GetMutableLatestTurn()->ExecuteMove(move, GetPlayersInRound());
  MaybeUpdateRoundState();
}

void Game::Round::DrawCard(const std::string& drawing_player_id) {
  std::cout << "DRAWING CARD for player: " << drawing_player_id << "\n";
  Game::Round::RoundPlayer* player = GetMutablePlayer(drawing_player_id);
  if (deck_.empty()) {
    player->held_cards.push_back(extra_card_.get());
    extra_card_ = boost::none;
  } else {
    player->held_cards.push_back(deck_.back());
    deck_.pop_back();
  }
  std::cout << "FINISHED DRAWING CARD\n";
}

void Game::Round::DiscardCard(const Card::Type& card_type, const std::string& discarding_player_id) {
  std::cout << "Trying to discard card: " << Card::GetCardTypeString(card_type) << "\n";
  Game::Round::RoundPlayer* player = GetMutablePlayer(discarding_player_id);
  auto it = std::find_if(player->held_cards.begin(), player->held_cards.end(), [&card_type] (const Card& held_card) {
    return held_card.GetType() == card_type;
  });
  if (it == player->held_cards.end()) {
    throw NoCardException(discarding_player_id, card_type);
  }
  player->held_cards.erase(it);
  player->discarded_cards.push_back(Card(card_type));
}

void Game::Round::ApplyDiscardEffect(const Card::Type& card_type,
                                     const std::string& discarding_player_id)
{
  switch (card_type) {
    case Card::Type::PRINCESS:
      return ApplyEffectPRINCESS(discarding_player_id);
    case Card::Type::HANDMAID:
      return ApplyEffectHANDMAID(discarding_player_id);
  }
}

void Game::Round::ApplySelectEffect(const Card::Type& card_type,
                                    const std::string& discarding_player_id,
                                    const std::string& selected_player_id,
                                    const boost::optional<Card::Type>& predicted_card_type) {
  std::cout << "in ApplySelectEffect\n";
  switch (card_type) {
    case Card::Type::KING:
      return ApplyEffectKING(discarding_player_id, selected_player_id);
    case Card::Type::PRINCE:
      return ApplyEffectPRINCE(selected_player_id);
    case Card::Type::GUARD:
      return ApplyEffectGUARD(selected_player_id, predicted_card_type.get());
  }
}

void Game::Round::ApplyViewEffect(const Card& discarded_card)
{
  const auto& latest_turn = GetLatestTurn();
  if (latest_turn.GetNumViewMoves() != discarded_card.RequiredViewMovesCount() - 1) {
    return;
  }
  switch (discarded_card.GetType()) {
    case Card::Type::BARON:
      return ApplyEffectBARON(latest_turn.GetPlayerId(), latest_turn.GetSelectedPlayerId());
  }
}

void Game::Round::ApplyEffectPRINCESS(const std::string& discarding_player_id) {
  RemoveFromRound(discarding_player_id);
}

void Game::Round::ApplyEffectKING(const std::string& discarding_player_id,
                                  const std::string& selected_player_id)
{
  std::cout << "in ApplyEffectKING\n";
  std::cout << "selected_player_id: " << selected_player_id << "\n";
  Game::Round::RoundPlayer* discarding_player = GetMutablePlayer(discarding_player_id);
  Game::Round::RoundPlayer* selected_player = GetMutablePlayer(selected_player_id);
  std::vector<Card> tmp = discarding_player->held_cards;
  discarding_player->held_cards = selected_player->held_cards;
  selected_player->held_cards = tmp;
}

void Game::Round::ApplyEffectPRINCE(const std::string& selected_player_id)
{
  auto player = GetMutablePlayer(selected_player_id);
  auto held_card = player->held_cards.front();
  DiscardCard(held_card.GetType(), selected_player_id);
  if (held_card.GetType() == Card::Type::PRINCESS) {
    player->still_in_round = false;
  } else {
    DrawCard(selected_player_id);
  }
}

void Game::Round::ApplyEffectHANDMAID(const std::string& discarding_player_id)
{
  Game::Round::RoundPlayer* player = GetMutablePlayer(discarding_player_id);
  player->immune = true;
}

void Game::Round::ApplyEffectBARON(const std::string& selecting_player_id,
                                   const std::string& selected_player_id)
{
  const auto& selector = GetPlayer(selecting_player_id);
  const auto& selected = GetPlayer(selected_player_id);
  if (selector.held_cards.front().GetValue() > selected.held_cards.front().GetValue()) {
    RemoveFromRound(selected_player_id);
  } else if (selector.held_cards.front().GetValue() < selected.held_cards.front().GetValue()) {
    RemoveFromRound(selecting_player_id);
  }
}

void Game::Round::ApplyEffectGUARD(const std::string& selected_player_id,
                                   const Card::Type& predicted_card_type)
{
  const auto& selected_player = GetPlayer(selected_player_id);
  if (selected_player.held_cards.front().GetType() == predicted_card_type) {
    RemoveFromRound(selected_player_id);
  }
}

void Game::Round::RemoveFromRound(const std::string& player_id) {
  auto* player = GetMutablePlayer(player_id);
  player->still_in_round = false;
}

void Game::Round::MaybeUpdateRoundState() {
  std::cout << "Starting MaybeUpdateRoundState\n";
  if (GetLatestTurn().IsComplete() && !IsComplete()) {
    std::cout << "advancing turn\n";
    AdvanceTurn();
  }
  std::cout << "ending MaybeUpdateRoundState\n";
}

void Game::Round::AdvanceTurn() {
  std::cout << "in AdvanceTurn\n";
  auto it = std::find_if(players_.begin(), players_.end(), [this](const Game::Round::RoundPlayer& player) {
    return player.player_id == this->GetLatestTurn().GetPlayerId();
  });
  auto advance = [this](std::vector<Game::Round::RoundPlayer>::iterator& it) {
    it++;
    if (it == players_.end()) {
      it = players_.begin();
    }
  };

  do {
    advance(it);
  }
  while (!it->still_in_round);

  MakeNewTurn(it->player_id);
}

void Game::Round::MakeNewTurn(const std::string& player_id) {
  Game::Round::RoundPlayer* player = GetMutablePlayer(player_id);

  // Satisfy Handmaid rule
  player->immune = false;
  turns_.push_back(Turn(player));
}

// Game::Round::Turn

const Card Game::Round::Turn::GetDiscardedCard() const {
  std::cout << "in GetDiscardedCard" << std::endl;
  auto discard_move = std::find_if(previous_moves.begin(),
                                   previous_moves.end(),
                                   [] (const GameUpdate::Move& move) {
      return move.move_type == GameUpdate::Move::MoveType::DISCARD_CARD;
  });
  std::cout << "discard move found? " << (discard_move != previous_moves.end()) << std::endl;
  std::cout << "looking for card type" << std::endl;
  discard_move->discarded_card_type.get();
  std::cout << "constructing Card" << std::endl;
  return Card(discard_move->discarded_card_type.get());
}

std::string Game::Round::Turn::GetSelectedPlayerId() const {
  auto select_move = std::find_if(previous_moves.begin(),
                                   previous_moves.end(),
                                   [] (const GameUpdate::Move& move) {
      return move.move_type == GameUpdate::Move::MoveType::SELECT_PLAYER;
  });
  return select_move->selected_player_id.get();
}

std::string Game::Round::Turn::GetPlayerId() const {
  return player->player_id;
}

int Game::Round::Turn::GetNumViewMoves() const {
  return std::count_if(previous_moves.begin(),
                       previous_moves.end(),
                       [] (const GameUpdate::Move& move) {
                          return move.move_type == GameUpdate::Move::MoveType::VIEW_CARD;
                        });
}

const GameUpdate::Move& Game::Round::Turn::GetLatestMove() const {
  return previous_moves.at(previous_moves.size() - 1);
}

GameUpdate::Move::MoveType Game::Round::Turn::GetNextMoveType() const {
  std::cout << "in GetNextMoveType\n";
  if (previous_moves.size() == 0) {
    return GameUpdate::Move::MoveType::DRAW_CARD;
  }
  GameUpdate::Move::MoveType latest_move_type = previous_moves.at(previous_moves.size() - 1).move_type;
  GameUpdate::Move::MoveType type = GameUpdate::Move::GetNextMoveType(latest_move_type);
  std::cout << "returning from GetNextMoveType\n";
  return type;
}

std::string Game::Round::Turn::GetNextMoveInstruction() const {
  if (NextMoveRequiresPrediction()) {
    return kInstructionPrefix + kPredictAndSelectInstruction;
  }
  return kInstructionPrefix + kMoveTypesToInstructions.at(GetNextMoveType());
}

bool Game::Round::Turn::NextMoveRequiresPrediction() const {
  if (GetNextMoveType() != GameUpdate::Move::MoveType::SELECT_PLAYER) {
    return false;
  }
  return GetNextMoveType() == GameUpdate::Move::MoveType::SELECT_PLAYER &&
         GetDiscardedCard().RequiresPrediction();
}

const std::vector<GameUpdate::Move>& Game::Round::Turn::GetMoves() const {
  return previous_moves;
}

std::string Game::Round::Turn::GetSummary() const {
  std::cout << "In GetSummary" << std::endl;
  std::cout << "getting player id: " << std::endl;
  std::cout << player->player_id << std::endl;
  std::cout << "getting card type string " << std::endl;
  std::cout << Card::GetCardTypeString(GetDiscardedCard().GetType()) << std::endl;
  std::stringstream ss;
  ss << player->player_id
     << " played the "
     << Card::GetCardTypeString(GetDiscardedCard().GetType())
     << " card";
  std::cout << "looking for select move" << std::endl;
  auto select_move = std::find_if(previous_moves.begin(),
                                   previous_moves.end(),
  [](const GameUpdate::Move& move) {
    return move.move_type == GameUpdate::Move::SELECT_PLAYER;
  });
  if (select_move != previous_moves.end()) {
    std::cout << "Getting SelectedPlayerId" << std::endl;
    GetSelectedPlayerId();
    std::cout << "Getting selected card type" << std::endl;
    GetDiscardedCard().GetType();
    std::cout << "Getting Action String" << std::endl;
    ss << " and "
       << Card::GetActionString(GetDiscardedCard().GetType(),
                                      GetSelectedPlayerId(),
                                      select_move->predicted_card_type);
  }
  ss << ".";
  std::cout << "End GetSummary" << std::endl;
  return ss.str();
}

bool Game::Round::Turn::IsComplete() const {
  return is_complete;
}

bool Game::Round::Turn::WasPlayerViewed(const std::string& player_id) const {
  return std::find_if(previous_moves.begin(), previous_moves.end(), [=](const GameUpdate::Move& move) {
    return (move.move_type == GameUpdate::Move::MoveType::VIEW_CARD) && (move.viewed_player_id.get() == player_id);
  }) != previous_moves.end();
}

std::vector<Card> Game::Round::Turn::GetDiscardableCards() const {
  return Card::GetDiscardableCards(player->held_cards);
}

void Game::Round::Turn::ExecuteMove(const GameUpdate::Move& move,
                                    std::vector<const Game::Round::RoundPlayer*> players_in_round) {
  std::cout << "in Turn::ExecuteMove\n";
  previous_moves.push_back(move);
  MaybeUpdateTurnState(players_in_round);
}

void Game::Round::Turn::MaybeUpdateTurnState(std::vector<const Game::Round::RoundPlayer*> players_in_round) {
  std::cout << "in MaybeUpdateTurnState\n";
  if (previous_moves.size() == 0) {
    return;
  }
  GameUpdate::Move::MoveType latest_move_type = GetLatestMove().move_type;

  if (latest_move_type == GameUpdate::Move::MoveType::DRAW_CARD) {
    return;
  }

  auto discarded_card = GetDiscardedCard();
  std::cout << "discarded_card: " << Card::GetCardTypeString(discarded_card.GetType()) << "\n";
  if (latest_move_type == GameUpdate::Move::MoveType::DISCARD_CARD) {
    bool another_non_immune_player_in_round = (std::find_if(players_in_round.begin(), players_in_round.end(),
     [this](const Game::Round::RoundPlayer* player) {
       std::cout << "immunity of player " << player->player_id << " : " << player->immune << "\n";
      return (player->player_id != this->player->player_id && !player->immune);
    }) != players_in_round.end());
    std::cout << "another non immune player in round? " << another_non_immune_player_in_round << "\n";
    is_complete = !discarded_card.RequiresSelectMove(another_non_immune_player_in_round);
  } else {
    is_complete = discarded_card.RequiredViewMovesCount() == GetNumViewMoves();
  }
}
