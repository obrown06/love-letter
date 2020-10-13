import React from 'react';
import styles from "components/scroll.module.css";
import scroll from "img/scroll.jpg";


class Scroll extends React.Component {

  constructor(props) {
    super(props);
    this.renderMessages.bind(this);
    this.renderRoundSummary.bind(this);
    this.renderTurnSummary.bind(this);
  }

  renderTurnSummary(turn) {
    return !turn.summary ? null : (
      <div className={styles.turnSummary}>
        {turn.summary}
      </div>
    );
  }

  renderRoundSummary(round, roundNumber) {
    let roundSummary = !round.summary ? null : <div className={styles.roundSummary}> {round.summary} </div>;
    let turnSummaries = !round.turns ? null : round.turns.map((turn) => this.renderTurnSummary(turn)).reverse();
    return (
      <div className={styles.roundContainer}>
        <div className={styles.roundID}>
          ROUND {roundNumber}
        </div>
        {roundSummary}
        <div className={styles.turnHeader}>
          TURNS
        </div>
        {turnSummaries}
      </div>
    );
  }

  renderMessages(game) {
    let gameSummary = !game.summary ? null : <div className={styles.gameSummary}> {game.summary} </div>;
    let roundSummaries = game.rounds.map((round, idx) => this.renderRoundSummary(round, idx + 1)).reverse();
    return (
      <div>
        <div className={styles.title}>
          Ye Olde Game History
        </div>
        {gameSummary}
        {roundSummaries}
      </div>
    );
  }

  render() {
    let messages = this.renderMessages(this.props.data);
    return (
      <div style= {{ backgroundImage: "url(" + scroll + ")" }} className={styles.scroll}>
        <div className={styles.textContainer}>
          {messages}
       </div>
      </div>
    );
  }
}

export default Scroll;
