import React from 'react';
import UserProfile from 'utils/user-profile.js';
import axios from 'axios';
import { Redirect } from 'react-router-dom';
import GameLobby from 'components/game-lobby.js'
import GameInProgress from 'components/game-in-progress.js'

axios.defaults.withCredentials = true;

const UpdateType = {
  JOIN_GAME_REQUEST: 1,
  START_GAME_REQUEST: 2,
  MOVE_REQUEST: 3,
};

const GameState = {
  WAITING: 1,
  IN_PROGRESS: 2,
  COMPLETE: 3
};

class Game extends React.Component {
  constructor(props) {
    super(props);
    this.handleStartGame = this.handleStartGame.bind(this);
    this.ws = new WebSocket('ws://localhost:8000/' + this.props.match.params.gameId);
    this.state = {
      data: {},
    };
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
      console.log('recieved ping: ');
      console.log(evt.data);
      this.setState({
        data: JSON.parse(evt.data),
      });
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
      update_type: UpdateType.START_GAME_REQUEST
    }));
  }

  render() {
    if (!this.props.loggedIn) {
      return (
        <Redirect to="/login" />
      );
    } else if (!this.state.data.state || this.state.data.state == GameState.WAITING) {
      return (
        <GameLobby players={this.state.data.players} handleStartGame={this.handleStartGame} />
      );
    } else {
      return (
        <GameInProgress
          ws={this.ws}
          data={this.state.data}
          gameId={this.props.match.params.gameId}/>
      );
    }
  }
}

export { UpdateType };
export default Game;
