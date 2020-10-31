#include "models/account.hpp"

std::string Account::GetUsername() const {
  return username_;
}

std::string Account::GetEmail() const {
  return email_;
}

std::string Account::GetPassword() const {
  return password_;
}

int Account::GetWins() const {
  return wins_;
}

int Account::GetLosses() const {
  return losses_;
}

int Account::GetPoints() const {
  return points_;
}

void Account::AddGamePerformance(int game_size, int rank) {
  if (rank == game_size) {
    wins_++;
  } else {
    losses_++;
  }

  int points = rank - game_size / 2;


  if (game_size % 2 != 0 || rank <= game_size / 2) {
    points--;
  }

  points_ += points;
}
