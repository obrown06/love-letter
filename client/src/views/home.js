import React from 'react';
import CreateGameForm from "components/create-game-form.js"
import JoinGameForm from "components/join-game-form.js"
import SignupForm from "components/sign-up-form.js"
import UserProfile from 'utils/user-profile.js';
import styles from 'views/home.module.css';
import "common.module.css";

class Home extends React.Component {
  render() {
    if (!this.props.loggedIn) {
      return <SignupForm signupCallback={this.props.signupCallback} />;
    }
    return (
      <div>
       <div className={styles.welcome}>
         Welcome, { UserProfile.getUserName()}...
       </div>
       <div className={styles.formsContainer}>
       <div className={styles.createGameFormContainer}>
         <CreateGameForm />
        </div>
       <div className={styles.dividerMessage}>
         or
       </div>
       <div className={styles.createGameFormContainer}>
         <JoinGameForm />
        </div>
      </div>
      </div>
    );
  }
}

export default Home;
