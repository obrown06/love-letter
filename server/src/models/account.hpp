#ifndef SERVER_MODELS_ACCOUNT_H
#define SERVER_MODELS_ACCOUNT_H

#include <string>

class Account {
public:
  Account(const std::string& username,
          const std::string& password,
          const std::string& email)
          : username_(username),
            password_(password),
            email_(email) {}

  std::string GetUsername() const;
  std::string GetEmail() const;
  std::string GetPassword() const;

private:
  std::string username_;
  std::string password_;
  std::string email_;
};

#endif
