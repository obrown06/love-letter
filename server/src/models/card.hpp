#ifndef SERVER_MODELS_CARD_HPP
#define SERVER_MODELS_CARD_HPP

#include <string>
#include <vector>

#include <boost/optional.hpp>

class Card {
public:
  enum Type {
    CARD_UNSPECIFIED,
    GUARD,
    PRIEST,
    BARON,
    HANDMAID,
    PRINCE,
    KING,
    COUNTESS,
    PRINCESS
  };

  Card(const Card::Type& type) : type_(type) {}

  static std::string GetCardTypeString(const Card::Type& type);
  static std::string GetActionString(const Card::Type& card_type,
                                     const std::string& selected_player_id,
                                     const boost::optional<Card>& selected_card);

  static std::vector<Card> GetDiscardableCards(const std::vector<Card>& cards);

  Type GetType() const;
  int GetValue() const;
  bool RequiresSelectMove(bool exists_another_player_to_select) const;
  bool AllowsSelfSelection() const;
  bool RequiresPrediction() const;
  int RequiredViewMovesCount() const;

private:
  Type type_;
};

#endif
