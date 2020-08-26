import React from 'react';
import cookieClient from 'react-cookie';
import UserProfile from 'utils/user-profile.js';
import axios from 'axios';
import { Redirect } from 'react-router-dom';

axios.defaults.withCredentials = true;

class LoginForm extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
      username: '',
      password: '',
      isLoggedIn: UserProfile.isLoggedIn(),
    }
    this.handleSubmit = this.handleSubmit.bind(this);
    this.handleChange = this.handleChange.bind(this);
  }

  handleChange = ({ target }) => {
    this.setState({ [target.name]: target.value });
  };

  handleSubmit(e) {
    e.preventDefault();
    axios.post('http://localhost:3000/api/login/',
      {
        username: this.state.username,
        password: this.state.password
      },
      { withCredentials: true }
    ).then(response => {
      if (response.status === 200) {
        UserProfile.login(this.state.username, response.headers['set-cookie']);
        this.setState({ isLoggedIn: UserProfile.isLoggedIn() });
      }
    });
  }

  render() {
    if (this.state.isLoggedIn) {
      return <Redirect to="/home" />;
    }

    return (
      <div>
      <form onSubmit={this.handleSubmit}>
        <label htmlFor="username">username</label>
        <input
          type="text"
          name="username"
          value={this.state.username}
          onChange={this.handleChange}
        />

        <label htmlFor="password">password</label>
        <input
          type="password"
          name="password"
          value={this.state.password}
          onChange={this.handleChange}
        />

        <input type="submit" value="Log In" />
      </form>
      </div>
    );
  }
}

export default LoginForm;
