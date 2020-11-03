#include "models/account.hpp"

#include <iostream>

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
  std::cout << "AddGamePerformance for player " << username_ << " game_size: " << game_size << " rank " << rank << std::endl;
  if (rank == game_size) {
    std::cout << "incrementing wins " << std::endl;
    wins_++;
  } else {
    std::cout << "incrementing losses " << std::endl;
    losses_++;
  }

  int points = rank - game_size / 2;


  if (game_size % 2 != 0 || rank <= game_size / 2) {
    points--;
  }

  std::cout << "adding the following points: " << points << std::endl;

  points_ += points;
}
