#include "models/games-registry.hpp"
#include "models/exceptions.hpp"

#include "json-api/games.hpp"

#include <set>
#include <utility>
#include <iostream>

void GamesRegistry::InsertGame(const Game& game) {
  if (registry_.find(game.GetId()) != registry_.end()) {
    throw DuplicateGameIdException(game.GetId());
  }
  Game copy = game;
  std::unordered_set<WebsocketSession*> sessions;
  std::pair<Game, std::unordered_set<WebsocketSession*>> pair(game, sessions);
  registry_.insert({game.GetId(), pair});
}

void GamesRegistry::InsertSession(const std::string& game_id, WebsocketSession* session) {
  if (registry_.find(game_id) == registry_.end()) {
    throw NoGameRegisteredException(game_id);
  }
  registry_.at(game_id).second.insert(session);
}

void GamesRegistry::RemoveSession(const std::string& game_id, WebsocketSession* session) {
  if (registry_.find(game_id) == registry_.end()) {
    throw NoGameRegisteredException(game_id);
  }
  std::cout << "REMOVING SESSION FOR GAME: " << game_id << std::endl;
  registry_.at(game_id).second.erase(session);
  if (!session->HasPlayerId()) {
    std::cout << "SESSION HAD NO PLAYER ID " << std::endl;
    return;
  }
  if (registry_.at(game_id).second.empty()) {
    std::cout << "REMOVING GAME: " << game_id << std::endl;
    registry_.erase(game_id);
  } else {
    std::string json = GetPlayerLeftGameJson(game_id, session->GetPlayerId());
    for (auto* registered_session : registry_.at(game_id).second) {
      registered_session->send(json);
    }
  }
}

void GamesRegistry::UpdateGameAndBroadcast(const GameUpdate& game_update) {
  if (registry_.find(game_update.game_id) == registry_.end()) {
    throw NoGameRegisteredException(game_update.game_id);
  }
  try {
    Game& game = registry_.at(game_update.game_id).first;
    std::cout << "before calling ProcessUpdate" << std::endl;
    game.ProcessUpdate(game_update);
    std::cout << "after calling ProcessUpdate" << std::endl;
    if (game.IsComplete() && !game.) {
      std::cout << "calling UpdateAccountsWithGameResults " << std::endl;
      UpdateAccountsWithGameResults(game);
    }
  } catch (DuplicatePlayerException& e) {
    // we shouldn't update accounts for re-entry requests
  }
  std::cout << "before calling GameToJSON" << std::endl;
  std::string json = GameToJSON(game);
  std::cout << "about to broadcast\n";
  for (auto* session : registry_.at(game.GetId()).second) {
    session->send(json);
  }
}

void GamesRegistry::UpdateAccountsWithGameResults(const Game& game) {
  auto players = game.GetPlayers();
  std::vector<int> tokens;
  for (const auto& player : players) {
    if (std::find(tokens.begin(), tokens.end(), player.ntokens_held) == tokens.end()) {
      tokens.push_back(player.ntokens_held);
    }
  }
  std::sort(tokens.begin(), tokens.end());
  for (const auto& player : players) {
    Account account = storage_->LoadAccount(player.player_id);
    std::cout << "player: " << account.GetUsername() << std::endl;
    account.AddGamePerformance(
      players.size(),
      std::find(tokens.begin(), tokens.end(), player.ntokens_held) - tokens.begin() + 1);
    storage_->InsertOrUpdateAccount(account);
  }
}
