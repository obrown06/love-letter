import React from 'react';
import UserProfile from 'utils/user-profile.js';
import "common.module.css";

class Home extends React.Component {
  render() {
    return (
      <div>
       Welcome, {UserProfile.getUserName()}!
       </div>
    );
  }
}

export default Home;
