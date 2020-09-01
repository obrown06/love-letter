import React from 'react';
import LoginForm from "components/login-form.js";
import { Redirect } from 'react-router-dom';
import "common.module.css";


class Login extends React.Component {
  render() {
    if (this.props.loggedIn) {
      return <Redirect to="/home" />;
    }
    return (
      <LoginForm loginCallback = {this.props.loginCallback}/>
    );
  }
}

export default Login;
