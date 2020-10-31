import React from 'react';
import deck from "img/deck.png";
import commonStyles from "components/common.module.css";
import styles from "components/deck.module.css";

class Deck extends React.Component {

  render() {
    return (
      <div
        className={[this.props.selectable ? commonStyles.selectable : ""].join(" ")}
        onClick={this.props.selectable ? this.props.selectCallback : undefined}>
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
