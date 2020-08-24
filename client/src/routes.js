import React from 'react';
import Home from 'views/home.js';
import Login from 'views/login.js';
import NotFound from 'views/not-found.js';
import Header from 'components/header.js';
import { Route, Switch, Redirect } from 'react-router-dom';

class Routes extends React.Component {
  render() {
    return (
      <div>
        <Header />
        <Switch>
          <Route exact path="/home" component={Home} />
          <Route exact path="/">
            <Redirect to="/home" />
          </Route>
          <Route exact path="/login" component={Login} />
          <Route component={NotFound}/>
        </Switch>
      </div>
    );
  }
}

export default Routes;
