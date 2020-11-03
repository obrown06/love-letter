import React from 'react';
import cookieClient from 'react-cookie';
import UserProfile from 'utils/user-profile.js';
import { myaxios } from 'utils/axios.js';
import { Redirect } from 'react-router-dom';
import { apiEndpoint } from 'utils/axios.js'
import styles from "components/login-form.module.css";


class LoginForm extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
      username: '',
      password: '',
      incorrect_password: false,
      incorrect_Username: false
    }
    this.handleSubmit = this.handleSubmit.bind(this);
    this.handleChange = this.handleChange.bind(this);
  }

  handleChange = ({ target }) => {
    this.setState({ [target.name]: target.value });
  };

  handleSubmit(e) {
    this.setState({ incorrect_password: false, incorrect_username: false })
    e.preventDefault();
    myaxios.post('https://' + apiEndpoint + '/api/login/',
      {
        username: this.state.username,
        password: this.state.password
      }
    ).then((response) => {
      if (response.status === 200) {
        this.props.loginCallback(this.state.username, response.headers['set-cookie']);
      }
    }).catch((error) => {
      if (error.response.data.incorrect_password) {
        this.setState({ incorrect_password: true });
      } else if (error.response.data.no_matching_username) {
        this.setState({ incorrect_username: true });
      }
    });
  }

  renderIncorrectUsernameNotice() {
    return (
      <div className={styles.errorNotice}>
        No account with matching username!
      </div>
    );
  }

  renderIncorrectPasswordNotice() {
    return (
      <div className={styles.errorNotice}>
        Incorrect Password!
      </div>
    );
  }

  render() {
    const incorrectUsernameNotice = !this.state.incorrect_username ? null : this.renderIncorrectUsernameNotice();
    const incorrectPasswordNotice = !this.state.incorrect_password ? null : this.renderIncorrectPasswordNotice();
    return (
      <div className={styles.container}>
      <div className={styles.loginFormContainer}>
      <div className={styles.loginFormTitle}>
        LOGIN
      </div>
      <br></br>
      <form onSubmit={this.handleSubmit} className={styles.form}>
      {incorrectUsernameNotice}
        <input
          type="text"
          name="username"
          placeholder="Username"
          autocomplete="off"
          value={this.state.username}
          onChange={this.handleChange}
          className={styles.input}
        />

        {incorrectPasswordNotice}

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
