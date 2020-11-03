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
  registry_.at(game_id).first.CheckGameNotStarted();
  registry_.at(game_id).second.insert(session);
}

void GamesRegistry::RemoveSession(const std::string& game_id, WebsocketSession* session) {
  if (registry_.find(game_id) == registry_.end()) {
    throw NoGameRegisteredException(game_id);
  }
  if (!session->HasPlayerId()) {
    return;
  }
  std::string json = GetPlayerLeftAndGameEndedJson(game_id, session->GetPlayerId());
  for (auto* registered_session : registry_.at(game_id).second) {
    if (registered_session != session) {
      registered_session->send(json);
    }
  }
  registry_.erase(game_id);
}

void GamesRegistry::UpdateGameAndBroadcast(const GameUpdate& game_update) {
  if (registry_.find(game_update.game_id) == registry_.end()) {
    throw NoGameRegisteredException(game_update.game_id);
  }
  Game& game = registry_.at(game_update.game_id).first;
  std::cout << "before calling ProcessUpdate" << std::endl;
  game.ProcessUpdate(game_update);
  std::cout << "after calling ProcessUpdate" << std::endl;
  if (game.IsComplete()) {
    std::cout << "calling UpdateAccountsWithGameResults " << std::endl;
    UpdateAccountsWithGameResults(game);
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
