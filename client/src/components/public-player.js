import React from 'react';
import Card from "components/card.js"
import TokenIcon from "components/token-icon.js"
import styles from "components/public-player.module.css";

class PublicPlayer extends React.Component {

  renderCard(type) {
    return (
      <div className={styles.playerCard}>
        <Card type={type} />
      </div>
    );
  }
  render() {
    const held_cards = !this.props.held_cards ? null : this.props.held_cards.map((type) => this.renderCard(type));
    const discarded_cards = !this.props.discarded_cards ? null : this.props.discarded_cards.map((type) => this.renderCard(type));
    const is_turn = !this.props.has_turn ? null : <div> TURN </div>;
    const callback = this.props.selectable ?
                       () => { this.props.selectCallback(this.props.id) } :
                       () => { this.props.viewCallback(this.props.id) };
    let hasCallback = this.props.selectable || this.props.viewable;
    return (
      <div style={{border: '2px solid black'}} onClick={hasCallback ? callback : undefined}>
      ID: {this.props.id} <br></br>
      <TokenIcon number={this.props.ntokens} /> <br></br>
      ACTIVE: {this.props.active_in_round ? "TRUE" : "FALSE"} <br></br>
      SELECTABLE: {this.props.selectable ? "TRUE" : "FALSE"} <br></br>
      VIEWABLE: {this.props.viewable ? "TRUE" : "FALSE"} <br></br>
      HELD CARDS: {held_cards} <br></br>
      DISCARDED_CARDS: {discarded_cards} <br></br>
      {is_turn}
      </div>
    );
  }
}

export default PublicPlayer;
