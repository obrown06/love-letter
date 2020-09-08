#ifndef SERVER_MODELS_GAME_UPDATE_HPP
#define SERVER_MODELS_GAME_UPDATE_HPP

#include <string>

namespace {
  const std::string kActionStrings[] = {
    "ACTION_UNSPECIFIED",
    "DRAW",
    "DISCARD",
    "SELECT"
  };
}

struct GameUpdate {
  enum UpdateType {
    UPDATE_UNSPECIFIED,
    JOIN_GAME_REQUEST,
    START_GAME_REQUEST,
    ACTION_REQUEST
  };
  UpdateType update_type;
  std::string player_id;
  std::string game_id;
  struct Action {
    enum ActionType {
      ACTION_UNSPECIFIED,
      DRAW_ACTION,
      DISCARD_ACTION,
      SELECT_ACTION,
    };
    static std::string GetActionString(const ActionType& action) {
      return kActionStrings[static_cast<int>(action)];
    }
    ActionType action_type;
  };
  Action action;
};

#endif
