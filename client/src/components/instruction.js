import React from 'react';
import styles from "components/instruction.module.css";

class Instruction extends React.Component {
  render() {
    return (
      <div className={styles.text}>
        {this.props.data}
      </div>
    );
  }
}

export default Instruction;
