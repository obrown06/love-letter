import React from 'react';
import cookieClient from 'react-cookie';
import UserProfile from 'utils/user-profile.js';
import { myaxios } from 'utils/axios.js';
import { Redirect } from 'react-router-dom';
import styles from "components/login-form.module.css";


class LoginForm extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
      username: '',
      password: '',
    }
    this.handleSubmit = this.handleSubmit.bind(this);
    this.handleChange = this.handleChange.bind(this);
  }

  handleChange = ({ target }) => {
    this.setState({ [target.name]: target.value });
  };

  handleSubmit(e) {
    e.preventDefault();
    myaxios.post('http://localhost:3000/api/login/',
      {
        username: this.state.username,
        password: this.state.password
      },
      { withCredentials: true }
    ).then(response => {
      if (response.status === 200) {
        this.props.loginCallback(this.state.username, response.headers['set-cookie']);
      }
    });
  }

  render() {
    return (
      <div className={styles.container}>
      <div className={styles.loginFormContainer}>
      <div className={styles.loginFormTitle}>
        LOGIN
      </div>
      <br></br>
      <form onSubmit={this.handleSubmit} className={styles.form}>
        <input
          type="text"
          name="username"
          placeholder="Username"
          autocomplete="off"
          value={this.state.username}
          onChange={this.handleChange}
          className={styles.input}
        />

        <input
          type="password"
          name="password"
          placeholder="Password"
          value={this.state.password}
          onChange={this.handleChange}
          className={styles.input}
        />

        <br></br><br></br>

        <input type="submit" value="Log In" className={styles.submitButton}/>
      </form>
      </div>
      </div>
    );
  }
}

export default LoginForm;
