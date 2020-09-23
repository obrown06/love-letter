import React from 'react';
import styles from "components/public-player.module.css";

class RoundIcon extends React.Component {
  render() {
    return (
      <div>
        ROUND: {this.props.number}
      </div>
    );
  }
}

export default RoundIcon;
