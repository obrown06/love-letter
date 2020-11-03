import React from 'react';
import UserProfile from 'utils/user-profile.js';
import { myaxios } from 'utils/axios.js';
import { Redirect } from 'react-router-dom';
import styles from 'components/create-game-form.module.css';
import { apiEndpoint } from 'utils/axios.js'
var hri = require('human-readable-ids').hri;

class CreateGameForm extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
      game_id: null,
    }
    this.handleCreateGame = this.handleCreateGame.bind(this);
  }

  handleCreateGame(e) {
    e.preventDefault();
    myaxios.post('https://' + apiEndpoint + '/api/games/',
      {
        game_id: hri.random(),
        creator: UserProfile.getUserName(),
      }
    ).then(response => {
      if (response.status === 200) {
        this.setState({ game_id: response.data.game_id });
      }
    });
  }

  render() {

    if (this.state.game_id) {
      return ( <Redirect to={"/games/" + this.state.game_id} /> );
    }

    return (
      <div>
      <form onSubmit={this.handleCreateGame}>
        <input type="submit" value="Create New Game" className={styles.submitButton}/>
      </form>
      </div>
    );
  }
}

export default CreateGameForm;
