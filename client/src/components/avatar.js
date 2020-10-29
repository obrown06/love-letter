import React from 'react';
import styles from "components/avatar.module.css";
import avatar from "img/avatar.png";

class Avatar extends React.Component {
  render() {
    let width = this.props.chars_size * 8;
    return (
      <div className={styles.container} style={{width: `${width}px`}}>
      <div className={styles.text}>
        {this.props.id}
      </div>
      </div>
    );
  }
}

export default Avatar;
