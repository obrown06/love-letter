import React from 'react';
import UserProfile from 'utils/user-profile.js';
import axios from 'axios';
import { Redirect } from 'react-router-dom';

axios.defaults.withCredentials = true;

class CreateGameForm extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
      game_id: '',
      isLoggedIn: UserProfile.isLoggedIn(),
      gameCreated: false,
    }
    this.handleSubmit = this.handleSubmit.bind(this);
    this.handleChange = this.handleChange.bind(this);
  }

  handleChange = ({ target }) => {
    this.setState({ [target.name]: target.value });
  };

  handleSubmit(e) {
    e.preventDefault();
    axios.post('http://localhost:3000/api/games/',
      {
        game_id: this.state.game_id,
        username: UserProfile.getUserName(),
      },
      { withCredentials: true }
    ).then(response => {
      if (response.status === 200) {
        this.setState({ gameCreated: true});
      }
    });
  }

  render() {
    if (!this.state.isLoggedIn) {
      return;
    }

    if (this.state.gameCreated) {
      return ( <Redirect to={"/games/" + this.state.game_id} /> );
    }

    return (
      <div>
      <form onSubmit={this.handleSubmit}>
        <label htmlFor="game_id">Game ID </label>
        <input
          type="text"
          name="game_id"
          value={this.state.game_id}
          onChange={this.handleChange}
        />

        <input type="submit" value="Create New Game" />
      </form>
      </div>
    );
  }
}

export default CreateGameForm;
