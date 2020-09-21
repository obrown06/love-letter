import React from 'react';
import styles from "components/public-player.module.css";

class PublicPlayer extends React.Component {
  render() {
    return (
      <div>
      ID: {this.props.id} <br></br>
      TOKENS: {this.props.ntokens} <br></br>
      ACTIVE: {this.props.active_in_round ? "TRUE" : "FALSE"} <br></br>
      SELECTABLE: {this.props.selectable ? "TRUE" : "FALSE"} <br></br>


      </div>
    );
  }
}

export default PublicPlayer;
