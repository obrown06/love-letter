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
    };
    static std::string GetMoveTypeString(const MoveType& move_type);
    MoveType move_type;
    std::string selected_player_id;
    boost::optional<Card> selected_card;
  };

  UpdateType update_type;
  std::string player_id;
  std::string game_id;
  boost::optional<Move> move;
};

#endif
