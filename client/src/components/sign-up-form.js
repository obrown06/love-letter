import React from 'react';
import cookieClient from 'react-cookie';
import UserProfile from 'utils/user-profile.js';
import { myaxios } from 'utils/axios.js';
import { Redirect } from 'react-router-dom';
import styles from "components/login-form.module.css";
import { apiEndpoint } from 'utils/axios.js'

const kEmailPoorlyFormed = "Not a valid email address!";
const kUsernameTooShort = "Username must be at least five characters long!";
const kPasswordTooShort = "Password must be at least five characters long!";
const kUsernameOnlyAlphanumericCharacters = "Username must only contain alphanumeric characters!";
const kPasswordNoSpaces = "Password cannot have spaces!";
const kPreexistingAccount = "Another account already exists with that username!";
const kMinimumUsernameLength = 5;

class SignupForm extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
      username: '',
      password: '',
      email: '',
      error: ''
    }
    this.handleSubmit = this.handleSubmit.bind(this);
    this.handleChange = this.handleChange.bind(this);
  }

  handleChange = ({ target }) => {
    this.setState({ [target.name]: target.value });
  };

  validateUsernameForErrors(username) {
    if (username.length < kMinimumUsernameLength) {
      return kUsernameTooShort;
    }
    let reg = /[^A-Za-z0-9]/;
    if (reg.test(username)) {
      return kUsernameOnlyAlphanumericCharacters;
    }
  }

  validateEmailForErrors(email) {
    const re = /^(([^<>()\[\]\\.,;:\s@"]+(\.[^<>()\[\]\\.,;:\s@"]+)*)|(".+"))@((\[[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\])|(([a-zA-Z\-0-9]+\.)+[a-zA-Z]{2,}))$/;
    if (!re.test(String(email).toLowerCase())) {
      return kEmailPoorlyFormed;
    }
  }

  validatePasswordForErrors(password) {
    if (password.length < kMinimumUsernameLength) {
      return kPasswordTooShort;
    }
    let reg = /[ ]/;
    if (reg.test(password)) {
      return kPasswordNoSpaces;
    }
  }



  validateCredentialsForErrors() {
    return this.validateUsernameForErrors(this.state.username) || this.validateEmailForErrors(this.state.email) ||this.validatePasswordForErrors(this.state.password);
  }

  handleSubmit(e) {
    e.preventDefault();
    this.setState({ error: '' });
    let foundError = this.validateCredentialsForErrors()
    if (foundError) {
      this.setState({ error: foundError });
      return;
    }
    myaxios.post('https://' + apiEndpoint + '/api/accounts/',
      {
        username: this.state.username,
        password: this.state.password,
        email: this.state.email
      }
    ).then((response) => {
      if (response.status === 200) {
        this.props.signupCallback(this.state.username, response.headers['set-cookie']);
      }
    }).catch((error) => {
      if (error.response.data.preexisting_account) {
        this.setState({ error: kPreexistingAccount });
      }
    });
  }

  renderErrorMessage() {
    return (
      <div className={styles.errorNotice}>
        {this.state.error}
      </div>
    );
  }

  render() {
    const errorMessage = (this.state.error == '') ? null : this.renderErrorMessage();
    return (
      <div className={styles.container}>
      <div className={styles.loginFormContainer}>
      <div className={styles.loginFormTitle}>
        Sign Up
      </div>
      <br></br>
      <form onSubmit={this.handleSubmit} className={styles.form}>

        {errorMessage}
        <input
          type="text"
          name="email"
          placeholder="E-mail"
          autocomplete="off"
          value={this.state.email}
          onChange={this.handleChange}
          className={styles.input}
        />
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

        <input type="submit" value="Sign Up" className={styles.submitButton}/>
      </form>
      </div>
      </div>
    );
  }
}

export default SignupForm;
