import React from 'react';
import { myaxios } from 'utils/axios.js';
import styles from "components/header.module.css";
import { Link } from 'react-router-dom';

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
    let authButton = this.props.loggedIn ?
      <button onClick={this.handleLogout}>Logout</button> :
      <Link to="/login"><button>Login</button></Link>;
    return (
      <div className={styles.container}>
        I am a header.
        {authButton}
      </div>
    );
  }
}

export default Header;
