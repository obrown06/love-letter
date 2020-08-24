import React from 'react';
import styles from "components/header.module.css";

class Header extends React.Component {
  render() {
    return (
      <div className={styles.container}>
        I am a header.
      </div>
    );
  }
}

export default Header;
