import React from 'react';
import styles from "components/card.module.css";
import commonStyles from "components/common.module.css"
import princess from "img/cards/princess.jpg";
import countess from "img/cards/countess.jpg";
import king from "img/cards/king.jpg";
import prince from "img/cards/prince.jpg";
import handmaid from "img/cards/handmaid.jpg";
import baron from "img/cards/baron.jpg";
import priest from "img/cards/priest.jpg";
import guard from "img/cards/guard.jpg";
import back from "img/cards/back.png";

const TypesToSrcs = {
  8: princess,
  7: countess,
  6: king,
  5: prince,
  4: handmaid,
  3: baron,
  2: priest,
  1: guard
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

  constructor(props) {
    super(props);

    this.onMouseOver = this.onMouseOver.bind(this);
    this.onMouseLeave = this.onMouseLeave.bind(this);
  }

  onMouseOver() {
    if (this.props.onMouseOver !== undefined && this.props.visible) {
      this.props.onMouseOver(this.props.type);
    }
  }

  onMouseLeave() {
    if (this.props.onMouseLeave !== undefined) {
      this.props.onMouseLeave();
    }
  }

  componentWillUnmount() {
    if (this.props.onMouseLeave !== undefined) {
      this.props.onMouseLeave();
    }
  }

  render() {
    const callback = () => { this.props.selectCallback(this.props.type) };
    return (
      <img
        className={[styles.img,
                    this.props.selectable ? commonStyles.selectable : ""].join(" ")}
        src={this.props.visible ? TypesToSrcs[this.props.type] : back}
        onClick={this.props.selectable ? callback : undefined}
        onMouseOver={this.onMouseOver}
        onMouseLeave={this.onMouseLeave} />
    );
  }
}

export { CardTypes };
export default Card;
