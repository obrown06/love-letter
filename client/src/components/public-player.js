import React from 'react';
import Avatar from "components/avatar.js"
import Card from "components/card.js"
import TokenIcon from "components/token-icon.js"
import commonStyles from "components/common.module.css"
import styles from "components/public-player.module.css";
import arrow from "img/arrow.jpg"

class PublicPlayer extends React.Component {

  constructor(props) {
    super(props);
    this.state = {
      isHovered: false,
    };

    this.toggleHover = this.toggleHover.bind(this);
  }

  renderCard(type, visible) {
    return (
      <div className={styles.playerElementContainer}>
        <Card type={type} visible={visible} />
      </div>
    );
  }

  renderCards(types, visible) {
    if (!types) {
      return null;
    }

    return (
      <div className={styles.cardsContainer}>
        {types.map((type) => this.renderCard(type, visible))}
      </div>
    );
  }

  toggleHover() {
    this.setState(prevState => ({isHovered: !prevState.isHovered}));
  }

  renderTurnArrow() {
    return (
      <img
        className={styles.img}
        src={arrow}>
      </img>
    );
  }

  render() {
    const is_turn = !this.props.has_turn ? null : <div> TURN </div>;
    const callback = this.props.selectable ?
                       () => { this.props.selectCallback(this.props.id) } :
                       () => { this.props.viewCallback(this.props.id) };
    const turnArrow = !this.props.has_turn ? null : this.renderTurnArrow();
    let hasCallback = this.props.selectable || this.props.viewable;
    return (
      <div
        className={[hasCallback && !this.state.isHovered ? commonStyles.selectable : "",
                    hasCallback && this.state.isHovered ? commonStyles.hovered : "",
                    !this.props.active_in_round ? styles.faded : ""].join(" ")}
        onMouseEnter={this.toggleHover}
        onMouseLeave={this.toggleHover}
        onClick={hasCallback ? callback : undefined}>
      <div className={styles.turnArrowContainer}>
        {turnArrow}
      </div>
      <div className={styles.playerElementContainer}>
        <Avatar id={this.props.id} />
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
