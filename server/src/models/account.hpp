#ifndef SERVER_MODELS_ACCOUNT_H
#define SERVER_MODELS_ACCOUNT_H

#include <string>

class Account {
public:
  Account(const std::string& username,
          const std::string& password,
          const std::string& email,
          int wins, int losses, int points)
          : username_(username),
            password_(password),
            email_(email),
            wins_(wins),
            losses_(losses),
            points_(points) {}

  std::string GetUsername() const;
  std::string GetEmail() const;
  std::string GetPassword() const;
  int GetWins() const;
  int GetLosses() const;
  int GetPoints() const;
  void AddGamePerformance(int game_size, int rank);

private:
  std::string username_;
  std::string password_;
  std::string email_;
  int wins_;
  int losses_;
  int points_;
};

#endif
