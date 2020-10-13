import React from 'react';
import Card from 'components/card.js'
import styles from "components/selectable-deck.module.css";

class SelectableDeck extends React.Component {

  constructor(props) {
    super(props);
    this.renderCard = this.renderCard.bind(this);
  }

  renderCard(type) {
    return (
      <div className={styles.cardContainer}>
        <Card
          selectCallback={this.props.selectCallback}
          selectable={true}
          visible={true}
          type={type} />
      </div>
    );
  }

  render() {
    const cardTypes = [1, 2, 3, 4, 5, 6, 7, 8];
    const cards = cardTypes.map(type => this.renderCard(type));
    return (
      <div className={styles.cardsContainer}>
        {cards}
      </div>
    );
  }
}

export default SelectableDeck;
