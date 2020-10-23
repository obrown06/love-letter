import React from 'react';
import { Link } from 'react-router-dom';
import exit from "img/exit-button.png";
import styles from "components/exit-button.module.css";

class ExitButton extends React.Component {
  render() {
    return (
      <div>
        <Link to="/">
          <img className={styles.image}
          src={exit}>
        </img>
        </Link>
      </div>
    );
  }
}

export default ExitButton;
