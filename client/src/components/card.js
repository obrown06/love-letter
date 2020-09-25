import React from 'react';
import styles from "components/public-player.module.css";

const TypesToNames = {
  8: "PRINCESS",
  7: "COUNTESS",
  6: "KING",
  5: "PRINCE",
  4: "HANDMAID",
  3: "BARON",
  2: "PRIEST",
  1: "GUARD"
};

const CardTypes = {
  PRINCESS: 8,
  COUNTESS: 7,
  KING: 6,
  PRINCE: 5,
  HANDMAID: 4,
  BARON: 3,
  PRIEST: 2,
  GUARD: 1
};

class Card extends React.Component {
  render() {
    const selectable = !this.props.selectable ? null : <div>SELECTABLE</div>;
    const callback = () => { this.props.selectCallback(this.props.type) };
    return (
      <div onClick={this.props.selectable ? callback : undefined}>
        {TypesToNames[this.props.type]}
        {selectable}
      </div>
    );
  }
}

export { CardTypes };
export default Card;
