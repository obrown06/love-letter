import React from 'react';
import Avatar from "components/avatar.js"
import Card from "components/card.js"
import TokenIcon from "components/token-icon.js"
import commonStyles from "components/common.module.css"
import styles from "components/public-player.module.css";
import arrow from "img/arrow.png"

class PublicPlayer extends React.Component {

  constructor(props) {
    super(props);
    this.executeCallback = this.executeCallback.bind(this);
  }

  renderCard(type, visible) {
    return (
      <div className={styles.playerElementContainer}>
        <Card
          type={type}
          visible={visible}
          onMouseOver={this.props.onHoverCard}
          onMouseLeave={this.props.onLeaveCard}/>
      </div>
    );
  }

  renderCards(types, visible) {
    if (!types) {
      return null;
    }

    const reversed = types.slice().reverse();

    return (
      <div className={styles.cardsContainer}>
        {reversed.map((type) => this.renderCard(type, visible))}
      </div>
    );
  }

  renderTurnArrow() {
    return (
      <img
        className={styles.img}
        src={arrow}>
      </img>
    );
  }

  executeCallback() {
    if (this.props.selectable) {
      this.props.selectCallback(this.props.id)
    } else {
      this.props.viewCallback(this.props.id)
    }
  }

  render() {
    const is_turn = !this.props.has_turn ? null : <div> TURN </div>;
    const turnArrow = !this.props.has_turn ? null : this.renderTurnArrow();
    let hasCallback = this.props.selectable || this.props.viewable;
    return (
      <div
        className={[hasCallback ? commonStyles.selectable : "",
                    !this.props.active_in_round ? styles.faded : ""].join(" ")}
        onMouseEnter={this.toggleHover}
        onMouseLeave={this.toggleHover}
        onClick={hasCallback ? this.executeCallback : undefined}>
      <div className={styles.turnArrowContainer}>
        {turnArrow}
      </div>
      <div className={styles.avatarContainer}>
        <Avatar id={this.props.id} chars_size={this.props.avatar_chars_size} />
      </div>
      <div className={styles.playerElementContainer}>
        <TokenIcon number={this.props.ntokens} />
      </div>
      {this.renderCards(this.props.held_cards, this.props.viewable)}
      {this.renderCards(this.props.discarded_cards, true)}
      </div>
    );
  }
}

export default PublicPlayer;
