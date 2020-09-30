import React from 'react';
import deck from "img/deck.png";
import commonStyles from "components/common.module.css";
import styles from "components/deck.module.css";

class Deck extends React.Component {

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
    const selectable = !this.props.selectable ? null : "SELECTABLE";
    return (
      <div
        className={[this.props.selectable && !this.state.isHovered ? commonStyles.selectable : "",
                    this.props.selectable && this.state.isHovered ? commonStyles.hovered : ""].join(" ")}
        onClick={selectable ? this.props.selectCallback : undefined}
        onMouseEnter={this.toggleHover}
        onMouseLeave={this.toggleHover}>
        <div className={styles.annotation}>
          {this.props.size}
        </div>
        <img
          className={styles.img}
          src={deck}>
        </img>
      </div>
    );
  }
}

export default Deck;
