import React from 'react';
import UserProfile from 'utils/user-profile.js';
import { myaxios } from 'utils/axios.js';
import { Redirect } from 'react-router-dom';
import GameLobby from 'components/game-lobby.js'
import GameInProgress from 'components/game-in-progress.js'
import { apiEndpoint } from 'utils/axios.js'

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
  heartbeatIntervalId = 0;
  constructor(props) {
    super(props);
    this.handleStartGame = this.handleStartGame.bind(this);
    console.log("apiEndpoint: " + apiEndpoint);
    this.state = {
      data: {},
      gameNotFound: false,
      gameLocked: false,
      leavingPlayerId: null,
    };

    this.heartbeat = this.heartbeat.bind(this);
    this.setupWebsocket = this.setupWebsocket.bind(this);
  }

  setupWebsocket() {
    console.log("setting up websocket");
    this.ws = new WebSocket('wss://' + apiEndpoint + '/' + this.props.match.params.gameId);
    this.ws.onopen = () => {
      console.log('connected');
      this.ws.send(JSON.stringify({
        game_id: this.props.match.params.gameId,
        player_id: UserProfile.getUserName(),
        update_type: UpdateType.JOIN_GAME_REQUEST
      }));
      this.heartbeatIntervalId = setInterval(this.heartbeat, 5000);
    }

    this.ws.onmessage = evt => {
      console.log('recieved ping: ');
      console.log(evt.data);
      let parsed = JSON.parse(evt.data);
      if (parsed.game_found !== undefined && parsed.game_found === false) {
        this.setState({ gameNotFound: true });
      } else if (parsed.game_already_started !== undefined) {
        this.setState({ gameLocked: true });
      } else if (parsed.player_left !== undefined) {
        this.setState({ leavingPlayerId: parsed.player_id });
      } else {
        this.setState({
          data: parsed,
        });
      }
    }

    this.ws.onclose = () => {
      console.log('disconnected');
    }
  }

  componentDidMount() {
    myaxios.get('https://' + apiEndpoint + '/api/games/',
      {},
      { withCredentials: true }
    ).then(response => {
      console.log("processing response");
      if (response.status === 200) {
        console.log("status ok");
        this.setupWebsocket();
      }
    });
  }

  componentWillUnmount() {
    this.ws.close();
    clearInterval(this.heartbeatIntervalId);
  }

  heartbeat() {
    console.log("sending heartbeat");
    if (this.ws.readyState !== 1) return;
    this.ws.send(JSON.stringify({
      is_ping: true,
    }));
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
    if (this.state.gameNotFound) {
      return (
        <Redirect to="/not-found" />
      );
    }
    if (!this.props.loggedIn) {
      return (
        <Redirect to="/login" />
      );
    } else if (this.state.gameLocked || !this.state.data.state || this.state.data.state == GameState.WAITING) {
      return (
        <GameLobby
          gameLocked={this.state.gameLocked}
          gameId={this.props.match.params.gameId}
          players={this.state.data.players}
          handleStartGame={this.handleStartGame} />
      );
    } else {
      return (
        <GameInProgress
          ws={this.ws}
          data={this.state.data}
          gameId={this.props.match.params.gameId}
          leavingPlayerId={this.state.leavingPlayerId}/>
      );
    }
  }
}

export { UpdateType };
export default Game;
