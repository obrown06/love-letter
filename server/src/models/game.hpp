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
    bool still_in_round;
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
  void PerformAction(const GameUpdate::Action& action, const std::string& actor_id);
  void ValidateAction(const GameUpdate::Action& action, const std::string& actor_id);
  void AddPlayer(const std::string& name);
  void Start();
  void Draw(const std::string& player_id);
  void CheckGameInProgress() const;
  void CheckGameNotStarted() const;
  void ShuffleCardsAndDeal();
  Game::PlayerInfo* GetPlayer(const std::string& player_id);
  std::string id_;
  std::string creator_;
  std::vector<Game::PlayerInfo> players_;
  State state_;
  int n_tokens_to_win_;
  int round_;
  std::string player_with_turn_;
  std::string next_actor_;
  GameUpdate::Action::ActionType next_action_type_;
  std::vector<Card> deck_;
};

#endif
