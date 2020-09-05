import React from 'react';
import { myaxios } from 'utils/axios.js';
import styles from "components/header.module.css";

class Header extends React.Component {

  constructor(props) {
    super(props);
    this.handleLogout = this.handleLogout.bind(this);
  }

  handleLogout(e) {
    e.preventDefault();
    myaxios.post('http://localhost:3000/api/logout/', {},
      { withCredentials: true }
    ).then(response => {
      if (response.status === 200) {
        this.props.logoutCallback();
      }
    });
  }

  render() {
    return (
      <div className={styles.container}>
        I am a header.
        <button onClick={this.handleLogout}>Logout</button>
      </div>
    );
  }
}

export default Header;
