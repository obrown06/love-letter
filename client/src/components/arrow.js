import React from 'react';
import styles from "components/avatar.module.css";
import arrow from "img/arrow.png";

class Avatar extends React.Component {
  render() {
    return (
      <div className={styles.container}>
      <img
        className={styles.img}
        src={arrow}>
      </img>
      </div>
    );
  }
}

export default Avatar;
