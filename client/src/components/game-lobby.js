import React from 'react';
import ExitButton from 'components/exit-button.js'
import styles from "components/game-lobby.module.css";
import lobby from "img/lobby.jpg";
import padlock from "img/padlock.png";

class GameLobby extends React.Component {

  renderPlayersAndStartGame() {
    const players = !this.props.players ? null : this.props.players.map((player) =>
      <div className={styles.playerIdContainer} key={player.player_id}>
        -- {player.player_id}
      </div>
    );
    return (
      <div>
      <div className={styles.playersTitle}>
        The Players
      </div>
      <div
        className={styles.players}>
        {players}
      </div>
      <br></br><br></br>
      <div className={styles.startButton} onClick={this.props.handleStartGame}>
        Start Game
      </div>
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

    const players = !this.props.gameLocked ? this.renderPlayersAndStartGame() : null;
    const lockedSymbol = this.props.gameLocked ? this.renderLockedSymbol() : null;
    return (
      <div className={styles.backgroundImageContainer}>

        <img
          className={styles.backgroundImage}
          src={lobby}>
        </img>
        <div
          className={styles.textContainer}>
          <div
            className={styles.title}>
            Game: {this.props.gameId}
          </div>
          <br></br>
          {players}
        </div>
        {lockedSymbol}
        <div className={styles.exitButtonContainer}>
          <ExitButton />
        </div>
      </div>
    );
  }
}

export default GameLobby;
