import React from 'react';
import styles from "components/avatar.module.css";
import avatar from "img/avatar.png";

class Avatar extends React.Component {
  render() {
    return (
      <div className={styles.container}>
      <img
        className={styles.img}
        src={avatar}>
      </img>
      <div className={styles.text}>
      {this.props.id}
      </div>
      </div>
    );
  }
}

export default Avatar;
