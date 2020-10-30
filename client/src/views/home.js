import React from 'react';
import CreateGameForm from "components/create-game-form.js"
import SignupForm from "components/sign-up-form.js"
import UserProfile from 'utils/user-profile.js';
import "common.module.css";

class Home extends React.Component {
  render() {
    if (!this.props.loggedIn) {
      return <SignupForm signupCallback={this.props.signupCallback} />;
    }
    return (
      <div>
       Welcome, { UserProfile.getUserName()}!
       <CreateGameForm />
       </div>
    );
  }
}

export default Home;
