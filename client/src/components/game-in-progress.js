import React from 'react';
import Card from "components/card.js"
import Deck from 'components/deck.js'
import ExitButton from 'components/exit-button.js'
import GameLobby from 'components/game-lobby.js'
import Instruction from 'components/instruction.js'
import PublicPlayer from 'components/public-player.js'
import Scroll from 'components/scroll.js'
import SelectableDeck from 'components/selectable-deck.js'
import TokenIcon from 'components/token-icon.js'
import UserProfile from 'utils/user-profile.js';
import UpdateType from 'views/game.js'
import styles from "components/game-in-progress.module.css";
import lobby from "img/lobby.jpg";
import padlock from "img/padlock.png";

const MoveTypes = {
  DRAW_CARD: 1,
  DISCARD_CARD: 2,
  SELECT_PLAYER: 3,
  VIEW_PLAYER: 4
};

class GameInProgress extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
      predictedCardType: 0,
    }

    this.drawCard = this.drawCard.bind(this);
    this.discardCard = this.discardCard.bind(this);
    this.selectPlayer = this.selectPlayer.bind(this);
    this.viewPlayer = this.viewPlayer.bind(this);
    this.predictCardType = this.predictCardType.bind(this);
    this.renderInstruction = this.renderInstruction.bind(this);
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
        discarded_card_type: cardType
      }
    }));
  }

  selectPlayer(player_id) {
    const predictedCardType = this.state.predictedCardType;
    this.setState({
      predictedCardType: 0
    });
    this.props.ws.send(JSON.stringify({
      game_id: this.props.gameId,
      player_id: UserProfile.getUserName(),
      update_type: 3,
      move: {
        move_type: MoveTypes.SELECT_PLAYER,
        selected_player_id : player_id,
        predicted_card_type : predictedCardType > 0 ? predictedCardType : undefined
      }
    }));
  }

  viewPlayer(player_id) {
    this.props.ws.send(JSON.stringify({
      game_id: this.props.gameId,
      player_id: UserProfile.getUserName(),
      update_type: 3,
      move: {
        move_type: MoveTypes.VIEW_PLAYER,
        viewed_player_id : player_id,
      }
    }));
  }

  isPlayerSelectable(player) {
    return this.userHasTurn() &&
           this.getCurrentTurn().selectable_players &&
           this.getCurrentTurn().selectable_players.includes(player.player_id) &&
           !this.requiresPrediction();
  }

  isPlayerViewable(player) {
    return this.getCurrentTurn().viewed_players &&
           this.getCurrentTurn().viewed_players.find((pair) => {
             return pair.viewer_id === UserProfile.getUserName() && pair.viewed_id === player.player_id;
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

  requiresPrediction() {
    return this.userHasTurn() &&
           this.getCurrentTurn().next_move_type == MoveTypes.SELECT_PLAYER &&
           this.getCurrentTurn().requires_prediction &&
           this.state.predictedCardType === 0;
  }

  predictCardType(cardType) {
    this.setState({
      predictedCardType: cardType
    });
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

  renderPlayers(players) {
    let max_id_len = 0;
    for (let i = 0; i < players.length; i++) {
      if (players[i].player_id.length > max_id_len) {
        max_id_len = players[i].player_id.length;
      }
    }
    const players_displayed = players.map((player) =>
      <div key={player.player_id} className={styles.playerContainer}>
        {this.renderPlayer(player, max_id_len)}
      </div>
    );
    return (
      <div>
      {players_displayed}
      </div>
    );
  }

  renderPlayer(player, max_id_len) {
    const round_player_info = this.getCurrentRound().players.find((info) => {
      return info.player_id === player.player_id;
    });
    return (
      <PublicPlayer
        avatar_chars_size={max_id_len}
        id={player.player_id}
        ntokens={player.tokens_held}
        active_in_round={round_player_info.still_in_round}
        held_cards={round_player_info.held_cards}
        discarded_cards={round_player_info.discarded_cards}
        selectable={this.isPlayerSelectable(player)}
        viewable={this.isPlayerViewable(player)}
        viewCallback={this.viewPlayer}
        selectCallback={this.selectPlayer}
        has_turn={this.getCurrentTurn().player_id === player.player_id}
      />
    );
  }

  renderCard(type) {
    return (
      <div className={styles.handCard}>
      <Card
        selectCallback={this.discardCard}
        selectable={this.isCardSelectable(type)}type={type}
        visible={true} />
      </div>
    );
  }

  renderSelectableDeck() {
    return (
      <div className={styles.selectableDeckContainer}>
        <SelectableDeck selectCallback={this.predictCardType} />
      </div>
    );
  }

  renderInstruction() {
    return (
      <div className={styles.instructionContainer}>
        <Instruction data={this.getCurrentTurn().instruction} />
      </div>
    );
  }

  renderLockedSymbol() {
    return (
      <div className={styles.lockedImageContainer}>
      <img
        src={padlock} className={styles.lockedImage}></img>
        </div>
    );
  }

  render() {
    const players = this.renderPlayers(this.props.data.players);

    const hand = !this.userInRound() ? null :
                    this.getRoundPlayerForUser().held_cards.map((type) => this.renderCard(type));
    const cardPredictionDeck = !this.requiresPrediction() ? null : this.renderSelectableDeck();
    const instruction = !this.userHasTurn() ? null : this.renderInstruction();
    const lockedSymbol = this.props.leavingPlayerId ? this.renderLockedSymbol() : null;
    return (
      <div className={styles.backgroundImageContainer}>
        <img
          className={styles.backgroundImage}
          src={lobby}>
        </img>
        <div className={styles.titleContainer}>
          <div
            className={styles.title}>
            Game: {this.props.gameId}
          </div>
          {lockedSymbol}
        </div>
        <div className={styles.playersContainer}>
          <div className={styles.playersTitle}>
            The Players
          </div>
          <br></br>
          {players}
        </div>
        <div className={styles.exitButtonContainer}>
          <ExitButton />
        </div>
        <div className={styles.deckContainer}>
          <Deck
            selectable={this.isDeckSelectable()}
            selectCallback={this.drawCard}
            size={this.getCurrentRound().deck_size} />
        </div>
        <div className={styles.handContainer}>
          {hand}
        </div>
        {cardPredictionDeck}
        <div className={styles.scrollContainer}>
          <Scroll data={this.props.data} leavingPlayerId={this.props.leavingPlayerId}/>
        </div>
        {instruction}
      </div>
    );
  }
}

export default GameInProgress;
