import React from 'react';
import Home from 'views/home.js';
import Login from 'views/login.js';
import Game from 'views/game.js';
import NotFound from 'views/not-found.js';
import Header from 'components/header.js';
import UserProfile from 'utils/user-profile.js';
import { checkAuthenticated } from 'utils/auth.js';
import { Route, Switch, Redirect } from 'react-router-dom';
import { myaxios } from 'utils/axios.js';

class Routes extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
      loggedIn: UserProfile.isLoggedIn(),
    };

    this.handleLogin = this.handleLogin.bind(this);
    this.handleLogout = this.handleLogout.bind(this);

    myaxios.interceptors.response.use(res => res, error => {
      console.log("logging out!");
      if (!checkAuthenticated(error.response)) {
        this.handleLogout();
      }
      return error;
    });
  }

  handleRedirectToLogin() {
    this.setState({ redirectToLogin: true });
  }

  handleLogin(username, sessionId) {
    UserProfile.login(username, sessionId);
    this.setState({ loggedIn : UserProfile.isLoggedIn() });
  }

  handleLogout() {
    UserProfile.logout();
    this.setState({ loggedIn: UserProfile.isLoggedIn() });
  }

  render() {
    return (
      <div>
        <Header
          loggedIn = {this.state.loggedIn}
          logoutCallback={this.handleLogout}/>
        <Switch>
          <Route exact path="/home" render={props => (
            <Home
              {...props}
              loggedIn={this.state.loggedIn}/>
          )} />
          <Route exact path="/">
            <Redirect to="/home" />
          </Route>
          <Route exact path="/login" render={(props) => (
            <Login
              {...props}
              loggedIn={this.state.loggedIn}
              loginCallback={this.handleLogin} />
          )} />
          <Route path="/games/:gameId" render={(props) => (
            <Game
              {...props}
              loggedIn={this.state.loggedIn}/>
          )} />
          <Route component={NotFound}/>
        </Switch>
      </div>
    );
  }
}

export default Routes;
