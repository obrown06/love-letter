#include "models/card.hpp"

#include <map>
#include <set>

#include <boost/format.hpp>
#include <iostream>

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

  const std::map<Card::Type, std::string> kCardTypesToStrings = {
    { Card::PRINCESS, "PRINCESS" },
    { Card::COUNTESS, "COUNTESS" },
    { Card::KING, "KING" },
    { Card::PRINCE, "PRINCE" },
    { Card::HANDMAID, "HANDMAID" },
    { Card::BARON, "BARON" },
    { Card::PRIEST, "PRIEST" },
    { Card::GUARD, "GUARD" },
  };

  const std::set<Card::Type> kCardTypesRequiringSelectMove = {
    Card::KING,
    Card::PRINCE,
    Card::BARON,
    Card::PRIEST,
    Card::GUARD
  };

  const std::map<Card::Type, int> kCardTypesToViewMoveCounts = {
    { Card::BARON, 2 },
    { Card::PRIEST, 1 }
  };

  const std::set<Card::Type> kCardTypesDiscardPreventedByCountess = {
    Card::KING,
    Card::PRINCE,
  };

  const Card::Type kCardTypeAllowingSelfSelection = Card::PRINCE;
  const Card::Type kCardTypeRequiringPrediction = Card::GUARD;

  const std::map<Card::Type, std::string> kActionStrings = {
    { Card::KING, " traded with %1%" },
    { Card::PRINCE, " forced %1% to discard their hand" },
    { Card::BARON, " compared hands with %1%" },
    { Card::PRIEST, " peeked at %1%'s hand" },
    { Card::GUARD, " accused %1% of being the %2% " }
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
    return kCardTypesRequiringSelectMove.find(type_) != kCardTypesRequiringSelectMove.end();
  }
  return AllowsSelfSelection();
}

bool Card::RequiresPrediction() const {
  return type_ == kCardTypeRequiringPrediction;
}

bool Card::AllowsSelfSelection() const {
  return type_ == kCardTypeAllowingSelfSelection;
}

int Card::RequiredViewMovesCount() const {
  std::cout << "in RequiredViewMovesCount\n";
  std::cout << "type is: " << type_ << "\n";
  if (kCardTypesToViewMoveCounts.find(type_) == kCardTypesToViewMoveCounts.end()) {
    return 0;
  }
  return kCardTypesToViewMoveCounts.at(type_);
}

std::string Card::GetCardTypeString(const Card::Type& card_type) {
  std::cout << "in GetCardTypeString" << std::endl;
  kCardTypesToStrings.at(card_type);
  std::cout << "could get type" << std::endl;
  return kCardTypesToStrings.at(card_type);
}

std::string Card::GetActionString(const Card::Type& card_type,
                                  const std::string& selected_player_id,
                                  const boost::optional<Card::Type>& predicted_card_type) {
  auto fmt = boost::format(kActionStrings.at(card_type)) % selected_player_id;
  if (predicted_card_type) {
    fmt % Card::GetCardTypeString(predicted_card_type.get());
  }
  return fmt.str();
}

std::vector<Card> Card::GetDiscardableCards(const std::vector<Card>& cards) {
  std::cout << "in GetDiscardableCards\n";
  std::vector<Card> discardable_cards = cards;
  if (std::find_if(discardable_cards.begin(), discardable_cards.end(), [](const Card& card) {
    return card.GetType() == Card::COUNTESS;
  }) != discardable_cards.end()) {
    std::cout << "trying to erase\n";
    discardable_cards.erase(std::remove_if(discardable_cards.begin(), discardable_cards.end(), [](const Card& card) {
      return kCardTypesDiscardPreventedByCountess.find(card.GetType()) != kCardTypesDiscardPreventedByCountess.end();
    }), discardable_cards.end());
    std::cout << "erased\n";
  }
  std::cout << "returning from GetDiscardableCard\n";
  return discardable_cards;
}
