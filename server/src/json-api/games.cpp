#include "json-api/exceptions.hpp"
#include "json-api/games.hpp"

#include <iostream>

std::string GetCreatedGameResponse(const std::string& game_id) {
  Json::Value root;
  root["game_id"] = game_id;
  Json::StreamWriterBuilder builder;
  return Json::writeString(builder, root);
}

std::string GetGameAlreadyStartedJson(const std::string& game_id) {
  Json::Value root;
  root["game_already_started"] = true;
  root["game_id"] = game_id;
  Json::StreamWriterBuilder builder;
  return Json::writeString(builder, root);
}

std::string GetPlayerLeftAndGameEndedJson(const std::string& game_id, const std::string& player_id) {
  Json::Value root;
  root["player_left_and_game_ended"] = true;
  root["game_id"] = game_id;
  root["player_id"] = player_id;
  Json::StreamWriterBuilder builder;
  return Json::writeString(builder, root);
}

std::string GetNoGameWithIdJson(const std::string& game_id) {
  Json::Value root;
  root["game_found"] = false;
  root["game_id"] = game_id;
  Json::StreamWriterBuilder builder;
  return Json::writeString(builder, root);
}

Game JSONToGame(const std::string& json) {
  Json::Value root;
  Json::CharReaderBuilder builder;
  Json::CharReader * reader = builder.newCharReader();
  std::string errors;
  bool parsingSuccessful = reader->parse(json.c_str(), json.c_str() + json.size(), &root, &errors);
  if (!parsingSuccessful || !root.isMember("game_id") || !root.isMember("creator")) {
    throw InvalidJsonException(json);
  }
  return Game(root["game_id"].asString(), root["creator"].asString());
}

std::string GameToJSON(const Game& game) {
  std::cout << "start of GameToJSON" << std::endl;
  Json::Value root;
  std::cout << "before GetId()" << std::endl;
  root["game_id"] = game.GetId();
  std::cout << "before GetCreator()" << std::endl;
  root["creator"] = game.GetCreator();
  std::cout << "before GetState()" << std::endl;
  root["state"] = game.GetState();
  if (game.GetState() != Game::WAITING) {
    std::cout << "before GetTokensToWin()" << std::endl;
    root["tokens_to_win"] = game.GetTokensToWin();
    std::cout << "before GetRounds()" << std::endl;
    for (const auto& round : game.GetRounds()) {
      std::cout << "before RoundToJSON" << std::endl;
      root["rounds"].append(RoundToJSON(round));
    }
    if (game.GetState() == Game::COMPLETE) {
      for (const auto& winner : game.GetWinners()) {
        Json::Value winner_node;
        winner_node["player_id"] = winner.player_id;
        root["winners"].append(winner_node);
      }
      root["summary"] = game.GetSummary();
    }
  }
  for (const auto& player : game.GetPlayers()) {
    Json::Value player_node;
    player_node["player_id"] = player.player_id;
    if (game.GetState() != Game::WAITING) {
      player_node["tokens_held"] = player.ntokens_held;
    }
    root["players"].append(player_node);
  }
  std::cout << "returning from GameToJSON\n";
  Json::StreamWriterBuilder builder;
  return Json::writeString(builder, root);
}

Json::Value RoundToJSON(const Game::Round& round) {
  std::cout << "in RoundToJSON" << std::endl;
  Json::Value round_node;
  round_node["deck_size"] = round.GetDeckSize();
  for (const auto& player : round.GetPlayers()) {
    std::cout << " examining player " << player.player_id << std::endl;
    Json::Value player_node;
    player_node["player_id"] = player.player_id;
    for (const auto& discarded_card : player.discarded_cards) {
      player_node["discarded_cards"].append(static_cast<int>(discarded_card.GetType()));
    }
    for (const auto& held_card : player.held_cards) {
      player_node["held_cards"].append(static_cast<int>(held_card.GetType()));
    }
    player_node["still_in_round"] = player.still_in_round;
    round_node["players"].append(player_node);
  }

  for (const auto& turn : round.GetTurns()) {
    round_node["turns"].append(TurnToJSON(round, turn));
  }

  if (round.IsComplete()) {
    round_node["summary"] = round.GetSummary();
    for (const auto& winner : round.GetWinners()) {
      Json::Value winner_node;
      winner_node["player_id"] = winner.player_id;
      round_node["winners"].append(winner_node);
    }
  }
  std::cout << "returning from RoundToJSON\n";
  return round_node;
}

Json::Value TurnToJSON(const Game::Round& round, const Game::Round::Turn& turn) {
  std::cout << "in TurnToJSON" << std::endl;
  Json::Value turn_node;
  std::cout << "before GetPlayerId" << std::endl;
  turn_node["player_id"] = turn.GetPlayerId();
  std::cout << "after GetPlayerId" << std::endl;
  for (const auto& move : turn.GetMoves()) {
    std::cout << "about to call MoveToJSON" << std::endl;
    turn_node["moves"].append(MoveToJSON(move));
  }
  if (turn.IsComplete()) {
    std::cout << "turn was complete" << std::endl;
    turn_node["summary"] = turn.GetSummary();
  } else {
    std::cout << "turn wasn't complete" << std::endl;
    const auto next_move_type = turn.GetNextMoveType();
    turn_node["next_move_type"] = next_move_type;
    turn_node["instruction"] = turn.GetNextMoveInstruction();
    if (next_move_type == GameUpdate::Move::MoveType::DISCARD_CARD) {
      std::cout << "next move type is discard" << std::endl;
      for (const auto& discardable_card : turn.GetDiscardableCards()) {
        turn_node["discardable_cards"].append(static_cast<int>(discardable_card.GetType()));
      }
    } else if (next_move_type == GameUpdate::Move::MoveType::SELECT_PLAYER) {
      std::cout << "next move is select" << std::endl;
      for (const auto& selectable_player : round.GetSelectablePlayers()) {
        turn_node["selectable_players"].append(selectable_player.player_id);
      }
      turn_node["requires_prediction"] = turn.NextMoveRequiresPrediction();
    } else if (next_move_type == GameUpdate::Move::MoveType::VIEW_CARD) {
      std::cout << "next move is view" << std::endl;
      for (const auto& [viewer, viewed] : round.GetViewPlayerPairs()) {
        Json::Value view_node;
        view_node["viewer_id"] = viewer.player_id;
        view_node["viewed_id"] = viewed.player_id;
        turn_node["viewed_players"].append(view_node);
      }
    }
  }
  std::cout << "returning from TurnToJSON" << std::endl;
  return turn_node;
}

Json::Value MoveToJSON(const GameUpdate::Move& move) {
  std::cout << "in MoveToJSON" << std::endl;
  Json::Value move_node;
  move_node["move_type"] = static_cast<int>(move.move_type);
  if (move.selected_player_id) {
    move_node["selected_player_id"] = *move.selected_player_id;
  }
  if (move.discarded_card_type) {
    move_node["discarded_card_type"] = static_cast<int>(move.discarded_card_type.get());
  }
  if (move.predicted_card_type) {
    move_node["predicted_card_type"] = static_cast<int>(move.predicted_card_type.get());
  }
  if (move.viewed_player_id) {
    move_node["viewed_player_id"] = move.viewed_player_id.get();
  }
  return move_node;
}
