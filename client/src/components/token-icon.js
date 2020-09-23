import React from 'react';
import styles from "components/public-player.module.css";

class TokenIcon extends React.Component {
  render() {
    return (
      <div>
        TOKENS: {this.props.number}
      </div>
    );
  }
}

export default TokenIcon;
