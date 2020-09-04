#ifndef SERVER_MODELS_GAME_UPDATE_HPP
#define SERVER_MODELS_GAME_UPDATE_HPP

#include <string>

struct GameUpdate {
  enum UpdateType {
    UPDATE_UNSPECIFIED,
    JOIN_GAME_REQUEST,
    START_GAME_REQUEST,
  };
  UpdateType update_type;
  std::string player_id;
  std::string game_id;
};

#endif
