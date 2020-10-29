import React from 'react';
import { Link } from 'react-router-dom';
import exit from "img/exit-button.png";
import styles from "components/exit-button.module.css";

class ExitButton extends React.Component {
  render() {
    return (
      <Link to="/">
        <div className={styles.close}>
        </div>
      </Link>
    );
  }
}

export default ExitButton;
