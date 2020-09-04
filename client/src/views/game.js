import React from 'react';
import UserProfile from 'utils/user-profile.js';
import axios from 'axios';

axios.defaults.withCredentials = true;

const UpdateType = {
  JOIN_GAME_REQUEST: 1
};

class Game extends React.Component {
  ws = new WebSocket('ws://localhost:8000/' + this.props.match.params.gameId);

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

  render() {
    return (
      <div>
      THIS IS A GAME
      </div>
    );
  }
}

export default Game;
