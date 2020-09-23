import React from 'react';
import Card from "components/card.js"
import styles from "components/public-player.module.css";

class PublicPlayer extends React.Component {

  renderCard(type) {
    return (
      <Card type={type} />
    );
  }
  render() {
    const held_cards = !this.props.held_cards ? null : this.props.held_cards.map((type) => this.renderCard(type));
    const discarded_cards = !this.props.discarded_cards ? null : this.props.discarded_cards.map((type) => this.renderCard(type));
    return (
      <div>
      ID: {this.props.id} <br></br>
      TOKENS: {this.props.ntokens} <br></br>
      ACTIVE: {this.props.active_in_round ? "TRUE" : "FALSE"} <br></br>
      SELECTABLE: {this.props.selectable ? "TRUE" : "FALSE"} <br></br>
      HELD CARDS: {held_cards} <br></br>
      DISCARDED_CARDS: {discarded_cards} <br></br>
      </div>
    );
  }
}

export default PublicPlayer;
