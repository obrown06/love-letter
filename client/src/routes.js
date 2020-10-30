import React from 'react';
import Home from 'views/home.js';
import Login from 'views/login.js';
import Game from 'views/game.js';
import NotFound from 'views/not-found.js';
import Header from 'components/header.js';
import UserProfile from 'utils/user-profile.js';
import { checkAuthenticated } from 'utils/auth.js';
import { Route, Switch, Redirect } from 'react-router-dom';
import {withRouter} from 'react-router';
import { myaxios } from 'utils/axios.js';

class Routes extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
      loggedIn: UserProfile.isLoggedIn(),
    };

    this.handleLogin = this.handleLogin.bind(this);
    this.handleLogout = this.handleLogout.bind(this);

    myaxios.interceptors.response.use((res) => res, (error) => {
      if (!checkAuthenticated(error.response)) {
        console.log("logging out!");
        this.handleLogout();
      }
      return Promise.reject(error);
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
    const excludePath = "/games/";
    return (
      <div>
        {!this.props.location.pathname.startsWith(excludePath) && <Header
          loggedIn = {this.state.loggedIn}
          logoutCallback={this.handleLogout}/>}
        <Switch>
          <Route exact path="/home" render={props => (
            <Home
              {...props}
              loggedIn={this.state.loggedIn}
              signupCallback={this.handleLogin}/>
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

export default withRouter(Routes);
