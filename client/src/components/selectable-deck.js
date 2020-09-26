import React from 'react';
import Card from 'components/card.js'
import styles from "components/public-player.module.css";

class Deck extends React.Component {

  constructor(props) {
    super(props);
    this.renderCard = this.renderCard.bind(this);
  }

  renderCard(type) {
    return (
      <Card
        selectCallback={this.props.selectCallback}
        selectable={true}
        type={type} />
    );
  }

  render() {
    const cardTypes = [1, 2, 3, 4, 5, 6, 7, 8];
    const cards = cardTypes.map(type => this.renderCard(type));
    return (
      <div style={{border: '2px solid black'}}>
        SELECTABLE DECK
        {cards}
      </div>
    );
  }
}

export default Deck;
