#ifndef SERVER_MODELS_GAME_UPDATE_HPP
#define SERVER_MODELS_GAME_UPDATE_HPP

#include <boost/optional.hpp>
#include "models/card.hpp"

#include <string>

struct GameUpdate {
  enum UpdateType {
    UPDATE_UNSPECIFIED,
    JOIN_GAME_REQUEST,
    START_GAME_REQUEST,
    MOVE_REQUEST
  };

  struct Move {
    enum MoveType {
      MOVE_UNSPECIFIED,
      DRAW_CARD,
      DISCARD_CARD,
      SELECT_PLAYER,
      VIEW_CARD
    };
    static std::string GetMoveTypeString(const MoveType& move_type);
    static MoveType GetNextMoveType(const MoveType& move_type);
    MoveType move_type;
    boost::optional<std::string> selected_player_id;
    boost::optional<Card::Type> discarded_card_type;
    boost::optional<std::string> viewed_player_id;
    boost::optional<Card::Type> predicted_card_type;
  };

  UpdateType update_type;
  std::string player_id;
  std::string game_id;
  boost::optional<Move> move;
};

#endif
