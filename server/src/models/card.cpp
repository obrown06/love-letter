#include "models/card.hpp"

#include <map>

namespace {
  const std::map<Card::Type, int> card_types_to_values = {
    { Card::PRINCESS, 8 },
    { Card::COUNTESS, 7 },
    { Card::KING, 6 },
    { Card::PRINCE, 5 },
    { Card::HANDMAID, 4 },
    { Card::BARON, 3 },
    { Card::PRIEST, 2 },
    { Card::GUARD, 1 },
  };
};

Card::Type Card::GetType() const {
  return type_;
}

int Card::GetValue() const {
  return card_types_to_values.at(type_);
}
