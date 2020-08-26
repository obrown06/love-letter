import React from 'react';
import LoginForm from "components/login-form.js";
import { Redirect } from 'react-router-dom';
import "common.module.css";


class Login extends React.Component {
  render() {
    return (
      <LoginForm />
    );
  }
}

export default Login;
