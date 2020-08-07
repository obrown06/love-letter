#include "account.h"

std::string Account::GetUsername() const {
  return username_;
}

std::string Account::GetEmail() const {
  return email_;
}

std::string Account::GetPassword() const {
  return password_;
}
