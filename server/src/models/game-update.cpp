#include "models/game-update.hpp"

#include "models/exceptions.hpp"

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

GameUpdate::Move::MoveType GameUpdate::Move::GetNextMoveType(const MoveType& move_type) {
  if (move_type == MoveType::SELECT_PLAYER) {
    throw NoNextMoveException(move_type);
  }
  return static_cast<MoveType>(static_cast<int>(move_type) + 1);
}