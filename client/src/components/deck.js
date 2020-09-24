import React from 'react';
import styles from "components/public-player.module.css";

class Deck extends React.Component {
  render() {
    const selectable = !this.props.selectable ? null : "SELECTABLE";
    return (
      <div onClick={selectable ? this.props.selectCallback : undefined} style={{border: '2px solid black'}}>
        DECK: {this.props.size}
        {selectable}
      </div>
    );
  }
}

export default Deck;
