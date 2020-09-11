#include "models/card.hpp"

#include <map>
#include <set>

namespace {
  const std::map<Card::Type, int> kCardTypesToValues = {
    { Card::PRINCESS, 8 },
    { Card::COUNTESS, 7 },
    { Card::KING, 6 },
    { Card::PRINCE, 5 },
    { Card::HANDMAID, 4 },
    { Card::BARON, 3 },
    { Card::PRIEST, 2 },
    { Card::GUARD, 1 },
  };

  const std::set<Card::Type> kCardTypesRequiringPlayerSelection = {
    Card::KING,
    Card::PRINCE,
    Card::BARON,
    Card::PRIEST,
    Card::GUARD
  };

  const std::set<Card::Type> kCardTypesDiscardPreventedByCountess = {
    Card::KING,
    Card::PRINCE,
  };

  const Card::Type kCardTypeRequiringPlayerSelectionIncludingSelf = Card::PRINCE;

  const std::string kCardStrings[] = {
    "CARD_UNSPECIFIED",
    "PRINCESS",
    "COUNTESS",
    "KING",
    "PRINCE",
    "HANDMAID",
    "BARON",
    "PRIEST",
    "GUARD",
  };
};

Card::Type Card::GetType() const {
  return type_;
}

int Card::GetValue() const {
  return kCardTypesToValues.at(type_);
}

bool Card::RequiresSelectMove(bool exists_another_player_to_select) const {
  if (exists_another_player_to_select) {
    return kCardTypesRequiringPlayerSelection.find(type_) != kCardTypesRequiringPlayerSelection.end();
  }
  return type_ == kCardTypeRequiringPlayerSelectionIncludingSelf;
}

std::string Card::GetCardTypeString(const Card::Type& card_type) {
  return kCardStrings[static_cast<int>(card_type)];
}

std::vector<Card> Card::GetDiscardableCards(const std::vector<Card>& cards) {
  std::vector<Card> discardable_cards = cards;
  if (std::find_if(discardable_cards.begin(), discardable_cards.end(), [](const Card& card) {
    return card.GetType() == Card::COUNTESS;
  }) != discardable_cards.end()) {
    discardable_cards.erase(std::remove_if(discardable_cards.begin(), discardable_cards.end(), [](const Card& card) {
      return kCardTypesDiscardPreventedByCountess.find(card.GetType()) != kCardTypesDiscardPreventedByCountess.end();
    }));
  }
  return discardable_cards;
}
