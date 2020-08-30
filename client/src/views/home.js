import React from 'react';
import CreateGameForm from "components/create-game-form.js"
import UserProfile from 'utils/user-profile.js';
import "common.module.css";

class Home extends React.Component {
  render() {
    return (
      <div>
       Welcome, {UserProfile.getUserName()}!
       <CreateGameForm />
       </div>
    );
  }
}

export default Home;
