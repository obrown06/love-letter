import React from 'react';
import shield from "img/shield.jpg";
import styles from "components/round-icon.module.css";

class RoundIcon extends React.Component {
  render() {
    return (
      <div className={styles.container}>
      <img
        className={styles.img}
        src={shield}>
      </img>
      <div className={styles.text}>
      {this.props.number}
      </div>
      </div>
    );
  }
}

export default RoundIcon;
