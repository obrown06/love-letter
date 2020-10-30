import React from 'react';
import styles from 'views/not-found.module.css';
import "common.module.css";

class Home extends React.Component {
  render() {
    return (
      <div className={styles.notFoundMessage}>Not Found!</div>
    );
  }
}

export default Home;
