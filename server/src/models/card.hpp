#ifndef SERVER_MODELS_CARD_HPP
#define SERVER_MODELS_CARD_HPP

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

  Type GetType() const;
  int GetValue() const;

private:
  Type type_;
};

#endif
