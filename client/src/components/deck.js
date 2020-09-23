import React from 'react';
import styles from "components/public-player.module.css";

class Deck extends React.Component {
  render() {
    return (
      <div>
        DECK: {this.props.size}
      </div>
    );
  }
}

export default Deck;
