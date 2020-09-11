#ifndef SERVER_MODELS_CARD_HPP
#define SERVER_MODELS_CARD_HPP

#include <string>
#include <vector>

class Card {
public:
  enum Type {
    CARD_UNSPECIFIED,
    PRINCESS,
    COUNTESS,
    KING,
    PRINCE,
    HANDMAID,
    BARON,
    PRIEST,
    GUARD
  };

  Card(const Card::Type& type) : type_(type) {}

  static std::string GetCardTypeString(const Card::Type& type);
  static std::vector<Card> GetDiscardableCards(const std::vector<Card>& cards);

  Type GetType() const;
  int GetValue() const;
  bool RequiresSelectMove(bool exists_another_player_to_select) const;

private:
  Type type_;
};

#endif
