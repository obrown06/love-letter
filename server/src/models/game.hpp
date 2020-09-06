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
    STATE_UNSPECIFIED,
    WAITING,
    IN_PROGRESS,
    COMPLETE,
  };

  enum Card {
    CARD_UNSPECIFIED,
    PRINCESS,
    COUNTESS,
    KING,
    PRINCE,
    HANDMAID,
    BARON,
    PRIEST,
    GUARD
  };

  struct PlayerInfo {
    std::string player_id;
    std::vector<Card> discarded_cards;
    std::vector<Card> held_cards;
    int ntokens_held;
  };

  std::string GetId() const;
  std::string GetCreator() const;
  Game::State GetState() const;
  std::vector<Game::PlayerInfo> GetPlayers() const;
  int GetRound() const;
  int GetTokensToWin() const;
  int GetNCardsInCenter() const;
  
  void ProcessUpdate(const GameUpdate& update);
private:
  void AddPlayer(const std::string& name);
  void Start();
  void CheckGameNotStarted() const;
  void ShuffleCardsAndDeal();
  std::string id_;
  std::string creator_;
  std::vector<Game::PlayerInfo> players_;
  State state_;
  int n_tokens_to_win_;
  int round_;
  std::string player_with_turn_;
  std::vector<Card> deck_;
};

#endif
