#ifndef SERVER_MODELS_GAME_HPP
#define SERVER_MODELS_GAME_HPP

#include "models/game-update.hpp"
#include "models/card.hpp"

#include <string>
#include <vector>

struct GameUpdate;

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

  struct GamePlayer {
    GamePlayer(const std::string& player_id) : player_id(player_id), ntokens_held() {};
    std::string player_id;
    int ntokens_held;
  };

  class Round {
    public:
      Round(const std::vector<Game::GamePlayer>& round_players);

      struct RoundPlayer {
        std::string player_id;
        std::vector<Card> discarded_cards;
        std::vector<Card> held_cards;
        bool still_in_round;
        bool immune;
        std::vector<Card> GetDiscardableCards() const;
      };

      struct Turn {
        Turn(const RoundPlayer* player):
          player(player) {}

        void ExecuteMove(const GameUpdate::Move& move);
        bool IsComplete(std::vector<const RoundPlayer*> players_in_round) const;
        const std::vector<GameUpdate::Move>& GetMoves() const;
        GameUpdate::Move* GetMutableLatestMove();
        const GameUpdate::Move& GetLatestMove() const;
        GameUpdate::Move::MoveType GetNextMoveType() const;
        const Card& GetDiscardedCard() const;
        std::string GetSelectedPlayerId() const;
        std::string GetSummary() const;
        std::vector<GameUpdate::Move> previous_moves;
        const RoundPlayer* player;
      };

      void ExecuteMove(const GameUpdate::Move& move);
      void ValidateMove(const GameUpdate::Move& move, const std::string& player_id);
      bool IsComplete() const;
      int GetDeckSize() const;
      std::vector<RoundPlayer> GetWinners() const;
      std::vector<RoundPlayer> GetPlayers() const;
      std::vector<RoundPlayer> GetSelectablePlayers() const;
      std::vector<std::pair<RoundPlayer, RoundPlayer>> GetViewPlayerPairs() const;
      const std::vector<Turn>& GetTurns() const;
      std::string GetSummary() const;
      std::vector<const RoundPlayer*> GetPlayersInRound() const;
      const Turn& GetLatestTurn() const;

    private:
      Turn* GetMutableLatestTurn();
      RoundPlayer* GetMutablePlayer(const std::string& player_id);
      void MaybeUpdateRoundState();
      void AdvanceTurn();
      void MakeNewTurn(const std::string& player_id);
      void DrawCard(const std::string& drawing_player_id);
      void DiscardCardAndApplyEffect(const std::string& discarding_player_id, const Card& card);
      void DiscardCard(const std::string& discarding_player_id, const Card& card);
      void ApplyEffect(const std::string& discarding_player_id,
                       const Card& card,
                       const boost::optional<std::string>& selected_player_id,
                       const boost::optional<Card::Type>& predicted_card_type);
      void ApplyEffectPRINCESS(const std::string& discarding_player_id);
      void ApplyEffectKING(const std::string& discarding_player_id,
                           const boost::optional<std::string>& selected_player_id);
      void ApplyEffectPRINCE(const boost::optional<std::string>& selected_player_id);
      void ApplyEffectHANDMAID(const std::string& discarding_player_id);
      void ApplyEffectBARON(const std::string& discarding_player_id,
                            const boost::optional<std::string>& selected_player_id);
      void ApplyEffectGUARD(const boost::optional<std::string>& selected_player_id,
                            const boost::optional<Card::Type>& predicted_card_type);
      std::vector<RoundPlayer> players_;
      std::vector<Card> deck_;
      boost::optional<Card> extra_card_;
      std::vector<Turn> turns_;
  };

  std::string GetId() const;
  std::string GetCreator() const;
  Game::State GetState() const;
  std::vector<Game::GamePlayer> GetPlayers() const;
  const std::vector<Game::Round>& GetRounds() const;
  int GetTokensToWin() const;
  std::vector<Game::GamePlayer> GetWinners() const;
  std::string GetSummary() const;
  bool IsComplete() const;

  void ProcessUpdate(const GameUpdate& update);
private:
  void ExecuteMove(const GameUpdate::Move& move, const std::string& player_id);
  void ValidateMove(const GameUpdate::Move& action, const std::string& player_id);
  void AddPlayer(const std::string& name);
  void Start();
  void MaybeUpdateGameState();
  void AdvanceRound();
  void CheckGameInProgress() const;
  void CheckGameNotStarted() const;
  Game::GamePlayer* GetMutablePlayer(const std::string& player_id);
  Round* GetMutableLatestRound();
  std::string id_;
  std::string creator_;
  std::vector<Game::GamePlayer> players_;
  State state_;
  int n_tokens_to_win_;
  std::vector<Round> rounds_;
};

#endif
