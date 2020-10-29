import React from 'react';
import ExitButton from 'components/exit-button.js'
import styles from "components/game-lobby.module.css";
import lobby from "img/lobby.jpg";

class GameLobby extends React.Component {
  render() {
    const players = !this.props.players ? null : this.props.players.map((player) =>
      <div className={styles.playerIdContainer} key={player.player_id}>
        -- {player.player_id}
      </div>
    );
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
        <div className={styles.exitButtonContainer}>
          <ExitButton />
        </div>
      </div>
    );
  }
}

export default GameLobby;
