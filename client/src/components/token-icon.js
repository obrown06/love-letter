import React from 'react';
import styles from "components/token-icon.module.css";
import token from "img/token.png";

class TokenIcon extends React.Component {
  render() {
    return (
      <div className={styles.container}>
      <img
        className={styles.img}
        src={token}>
      </img>
      <div className={styles.text}>
      {this.props.number}
      </div>
      </div>
    );
  }
}

export default TokenIcon;
