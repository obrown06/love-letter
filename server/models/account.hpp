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
  const std::string username_;
  const std::string password_;
  const std::string email_;
};

#endif
