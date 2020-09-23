import React from 'react';
import styles from "components/public-player.module.css";

const Types = {
  8: "PRINCESS",
  7: "COUNTESS",
  6: "KING",
  5: "PRINCE",
  4: "HANDMAID",
  3: "BARON",
  2: "PRIEST",
  1: "GUARD"
};

class Card extends React.Component {
  render() {
    return (
      <div>
        {Types[this.props.type]}
      </div>
    );
  }
}

export default Card;
