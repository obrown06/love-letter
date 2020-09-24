import React from 'react';
import Card from "components/card.js"
import Deck from 'components/deck.js'
import GameLobby from 'components/game-lobby.js'
import PublicPlayer from 'components/public-player.js'
import RoundIcon from 'components/round-icon.js'
import TokenIcon from 'components/token-icon.js'
import UserProfile from 'utils/user-profile.js';
import UpdateType from 'views/game.js'
import styles from "components/game-in-progress.module.css";

const MoveTypes = {
  DRAW_CARD: 1,
  DISCARD_CARD: 2,
  SELECT_PLAYER: 3,
  VIEW_PLAYER: 4
};

class GameInProgress extends React.Component {
  constructor(props) {
    super(props);
    this.drawCard = this.drawCard.bind(this);
    this.discardCard = this.discardCard.bind(this);
  }

  getCurrentRound() {
    return this.props.data.rounds[this.props.data.rounds.length - 1];
  }

  drawCard() {
    this.props.ws.send(JSON.stringify({
      game_id: this.props.gameId,
      player_id: UserProfile.getUserName(),
      update_type: 3,
      move: {
        move_type: MoveTypes.DRAW_CARD
      }
    }));
  }

  discardCard(cardType) {
    this.props.ws.send(JSON.stringify({
      game_id: this.props.gameId,
      player_id: UserProfile.getUserName(),
      update_type: 3,
      move: {
        move_type: MoveTypes.DISCARD_CARD,
        selected_card: cardType
      }
    }));
  }

  isPlayerSelectable(player) {
    return this.userHasTurn() &&
           this.getCurrentTurn().selectable_players &&
           this.getCurrentTurn().selectable_players.find((selectable_player) => {
             return selectable_player.player_id === player.player_id;
           });
  }

  isCardSelectable(cardType) {
    return this.userHasTurn() &&
           this.getCurrentTurn().discardable_cards &&
           this.getCurrentTurn().discardable_cards.find((currType) => {
             return currType === cardType;
           });
  }

  isDeckSelectable() {
    return this.userHasTurn() &&
           this.getCurrentTurn().next_move_type === MoveTypes.DRAW_CARD;
  }

  userHasTurn() {
    return this.getCurrentTurn().player_id === UserProfile.getUserName();
  }

  getRoundPlayerForUser() {
    return this.getCurrentRound().players.find(player => player.player_id === UserProfile.getUserName());
  }

  userInRound() {
    return this.getRoundPlayerForUser().still_in_round;
  }

  getCurrentTurn() {
    const current_round = this.getCurrentRound();
    return current_round.turns[current_round.turns.length - 1];
  }

  renderPlayer(player) {
    const round_player_info = this.getCurrentRound().players.find((info) => {
      return info.player_id === player.player_id;
    });
    return (
      <PublicPlayer
        id={player.player_id}
        ntokens={player.tokens_held}
        active_in_round={round_player_info.still_in_round}
        held_cards={round_player_info.held_cards}
        discarded_cards={round_player_info.discarded_cards}
        selectable={this.isPlayerSelectable(player)}
        has_turn={this.getCurrentTurn().player_id === player.player_id}
      />
    );
  }

  renderCard(type) {
    return (
      <Card
        selectCallback={this.discardCard}
        selectable={this.isCardSelectable(type)}type={type} />
    );
  }

  render() {
    const players = this.props.data.players.map((player) =>
      <div key={player.player_id}>
        <br></br>
        {this.renderPlayer(player)}
        <br></br>
      </div>
    );
    const hand = !this.userInRound() ? null :
                    this.getRoundPlayerForUser().held_cards.map((type) => this.renderCard(type));
    return (
      <div>
        GAME IN PROGRESS
        {players}
        <Deck
          selectable={this.isDeckSelectable()}
          selectCallback={this.drawCard}
          size={this.getCurrentRound().deck_size} />
        <RoundIcon number={this.props.data.rounds.length} />
        <TokenIcon number={this.props.data.tokens_to_win} />
        <div style={{border: '2px solid black'}}>
          HAND: {hand}
        </div>
      </div>
    );
  }
}

export default GameInProgress;
