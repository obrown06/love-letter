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
