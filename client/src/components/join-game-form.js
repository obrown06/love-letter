import React from 'react';
import UserProfile from 'utils/user-profile.js';
import { myaxios } from 'utils/axios.js';
import { Redirect } from 'react-router-dom';
import styles from 'components/join-game-form.module.css';
var hri = require('human-readable-ids').hri;

class JoinGameForm extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
      gameId: null,
      gameJoined: false,
    }
    this.handleJoinGame = this.handleJoinGame.bind(this);
    this.handleChange = this.handleChange.bind(this);
  }

  handleJoinGame(e) {
    e.preventDefault();
    this.setState({ gameJoined: true });
  }

  handleChange = ({ target }) => {
    this.setState({ [target.name]: target.value });
  };

  render() {

    if (this.state.gameJoined) {
      return ( <Redirect to={"/games/" + this.state.gameId} /> );
    }

    return (
      <div>
      <form onSubmit={this.handleJoinGame}>

        <input type="submit" value="Join Existing Game" className={styles.submitButton}/>
        <input
          type="username"
          name="gameId"
          placeholder="Game ID"
          value={this.state.gameId}
          onChange={this.handleChange}
          className={styles.input}
        />
      </form>
      </div>
    );
  }
}

export default JoinGameForm;
