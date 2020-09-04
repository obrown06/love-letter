import React from 'react';
import UserProfile from 'utils/user-profile.js';
import axios from 'axios';

axios.defaults.withCredentials = true;

class Game extends React.Component {
  ws = new WebSocket('ws://localhost:8000/' + this.props.match.params.gameId);

  componentDidMount() {
    this.ws.onopen = () => {
      console.log('connected');
      this.ws.send('ping!');
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
