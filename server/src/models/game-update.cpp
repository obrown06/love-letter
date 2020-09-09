#include "models/game-update.hpp"

namespace {
  const std::string kMoveStrings[] = {
    "MOVE_UNSPECIFIED",
    "DRAW CARD",
    "DISCARD CARD",
    "SELECT PLAYER"
  };
};

std::string GameUpdate::Move::GetMoveTypeString(const MoveType& move_type) {
  return kMoveStrings[static_cast<int>(move_type)];
}
