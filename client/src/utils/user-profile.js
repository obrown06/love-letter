import React from 'react';

class UserProfile {

  static login(userName, sessionId) {
    localStorage.setItem('username', userName);
    localStorage.setItem('sessionid', sessionId);
  }

  static logout() {
    localStorage.removeItem('username');
    localStorage.removeItem('sessionid');
  }

  static isLoggedIn() {
    return localStorage.getItem('username') !== null;
  }

  static getUserName() {
    return localStorage.getItem('username');
  }

  static getSessionId() {
    return localStorage.getItem('sessionid');
  }
}

export default UserProfile;
