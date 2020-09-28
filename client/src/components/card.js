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
    this.state = {
      isHovered: false,
    };

    this.toggleHover = this.toggleHover.bind(this);
  }

  toggleHover() {
    this.setState(prevState => ({isHovered: !prevState.isHovered}));
  }

  render() {
    const callback = () => { this.props.selectCallback(this.props.type) };
    return (
      <img
        className={[styles.img,
                    this.props.selectable && !this.state.isHovered ? commonStyles.selectable : "",
                    this.props.selectable && this.state.isHovered ? commonStyles.hovered : ""].join(" ")}
        onMouseEnter={this.toggleHover}
        onMouseLeave={this.toggleHover}
        src={TypesToSrcs[this.props.type]}
        onClick={this.props.selectable ? callback : undefined} />
    );
  }
}

export { CardTypes };
export default Card;
