import React from 'react';
import { myaxios } from 'utils/axios.js';
import styles from "components/header.module.css";
import logo from "img/logo.jpg";
import authbutton from "img/auth-button.jpg";
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

  renderLink(text) {
    return (
      <div className={styles.headerLink}>
        <span className={styles.underline}>{text}</span>
      </div>
    );
  }

  render() {
    let authButton = this.props.loggedIn ?
      <div onClick={this.handleLogout}>{this.renderLink("Log Out")}</div> :
      <Link to="/login">{this.renderLink("Log In")}</Link>;
    let leaderboardButton = this.props.loggedIn ?
      <Link to="/leaderboard">{this.renderLink("Leaderboard")}</Link> :
      null;
    return (
      <div className={styles.container}>
        <div className={styles.logoContainer}>
          <Link to="/">
            <img
              className={styles.img}
              src={logo}>
            </img>
          </Link>
        </div>
        <div className={styles.leaderboardContainer}>
          {leaderboardButton}
        </div>
        <div className={styles.authContainer}>
          {authButton}
        </div>
      </div>
    );
  }
}

export default Header;
