import React from 'react';
import UserProfile from 'utils/user-profile.js';
import axios from 'axios';
import { Redirect } from 'react-router-dom';

axios.defaults.withCredentials = true;

const UpdateType = {
  JOIN_GAME_REQUEST: 1,
  START_GAME_REQUEST: 2
};

class Game extends React.Component {
  constructor(props) {
    super(props);
    this.handleStartGame = this.handleStartGame.bind(this);
    this.ws = new WebSocket('ws://localhost:8000/' + this.props.match.params.gameId);
  }

  componentDidMount() {
    this.ws.onopen = () => {
      console.log('connected');
      this.ws.send(JSON.stringify({
        game_id: this.props.match.params.gameId,
        player_id: UserProfile.getUserName(),
        update_type: UpdateType.JOIN_GAME_REQUEST
      }));
    }

    this.ws.onmessage = evt => {
      console.log(evt.data);
    }

    this.ws.onclose = () => {
      console.log('disconnected');
    }
  }

  handleStartGame(e) {
    e.preventDefault();
    this.ws.send(JSON.stringify({
      game_id: this.props.match.params.gameId,
      player_id: UserProfile.getUserName(),
      update_type: 2
    }));
  }

  render() {
    if (!this.props.loggedIn) {
      return (
        <Redirect to="/login" />
      );
    }
    return (
      <div>
      THIS IS A GAME
      <form onSubmit={this.handleStartGame}>

        <input type="submit" value="Start Game" />
      </form>
      </div>
    );
  }
}

export default Game;
