import React from 'react';
import styles from "components/game-lobby.module.css";

class GameLobby extends React.Component {
  render() {
    const players = !this.props.players ? null : this.props.players.map((player) =>
      <li key={player.player_id}>
        {player.player_id}
      </li>
    );
    return (
      <div>
      WELCOME TO THE LOBBY!

      PLAYERS IN WAITING: <ul>{players}</ul>

      <form onSubmit={this.props.handleStartGame}>
        <input type="submit" value="Start Game" />
      </form>
      </div>
    );
  }
}

export default GameLobby;
