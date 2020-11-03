import React from 'react';
import CreateGameForm from "components/create-game-form.js"
import JoinGameForm from "components/join-game-form.js"
import SignupForm from "components/sign-up-form.js"
import UserProfile from 'utils/user-profile.js';
import { myaxios } from 'utils/axios.js';
import { Redirect } from 'react-router-dom';
import styles from 'views/leaderboard.module.css';
import { apiEndpoint } from 'utils/axios.js'
import "common.module.css";

class LeaderBoard extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
      players: [],
    };
  }

  componentDidMount() {
    console.log("requesting");
    myaxios.get('https://' + apiEndpoint + '/api/accounts/',
      {},
      { withCredentials: true }
    ).then(response => {
      if (response.status === 200) {
        let players = response.data.accounts;
        let sortedPlayers = players.sort(function(first, second) {
          if (first.points < second.points) {
            return 1;
          } else if (first.points > second.points) {
            return -1;
          }
          return 0;
        });
        this.setState({ players: sortedPlayers });
      }
    });
  }

  renderRow(player) {
    return (
      <tr>
        <td> {player.username} </td>
        <td> {player.points} </td>
        <td> {player.wins}   </td>
        <td> {player.losses} </td>
       </tr>
    );
  }

  render() {
    if (!this.props.loggedIn) {
      return (
        <Redirect to="/home" />
      );
    }
    const players = this.state.players.map((player) => this.renderRow(player));
    return (
      <div className={styles.container}>
       <div className={styles.title}>
         Ye Olde Leaderboard
       </div>
       <div className={styles.tableContainer}>
       <table>
         <tr>
           <th> Player </th>
           <th> Points </th>
           <th> Wins   </th>
           <th> Losses </th>
          </tr>
          {players}
        </table>
        </div>
      </div>
    );
  }
}

export default LeaderBoard;
