#include "server/websocket-session.hpp"
#include "models/exceptions.hpp"
#include "json-api/game-updates.hpp"
#include "json-api/games.hpp"

#include <iostream>

WebsocketSession::~WebsocketSession() {
  try {
    registry_->RemoveSession(game_id_, this);
  } catch (NoGameRegisteredException& e){
    fail("destructor", e.what());
  }
}

void
WebsocketSession::on_accept(beast::error_code ec)
{
  if (ec) {
    return fail(ec, "accept");
  }

  try {
    registry_->InsertSession(game_id_, this);
  } catch (NoGameRegisteredException& e){
    ws_.write(net::buffer(GetNoGameWithIdJson(game_id_)));
    return fail("accept", e.what());
  } catch (GameAlreadyStartedException& e) {
    ws_.write(net::buffer(GetGameAlreadyStartedJson(game_id_)));
    return fail("accept", e.what());
  }

  do_read();
}

void
WebsocketSession::do_read()
{
  ws_.async_read(
    buffer_,
    beast::bind_front_handler(
      &WebsocketSession::on_read,
      shared_from_this()));
}

void WebsocketSession::send(const std::string& msg)
{
  queue_.push_back(msg);
  if (queue_.size() > 1) {
    return;
  }

  ws_.async_write(
    net::buffer(queue_.front()),
    beast::bind_front_handler(
      &WebsocketSession::on_write,
      shared_from_this()));
}

void
WebsocketSession::on_read(
  beast::error_code ec,
  std::size_t bytes_transferred)
{
  boost::ignore_unused(bytes_transferred);

  if (ec) {
    return fail(ec, "read");
  }

  try {
    std::string data = beast::buffers_to_string(buffer_.data());
    if (!IsPing(data)) {
      GameUpdate update = JSONToGameUpdate(data);
      std::cout << "before calling UpdateGameAndBroadcast" << std::endl;
      registry_->UpdateGameAndBroadcast(update);
      std::cout << "after calling UpdateGameAndBroadcast" << std::endl;
      if (update.update_type == GameUpdate::UpdateType::JOIN_GAME_REQUEST) {
        player_id_ = update.player_id;
      }
    }
    buffer_.consume(buffer_.size());
  } catch (GameAlreadyStartedException& e) {
    registry_->RemoveSession(game_id_, this);
    ws_.write(net::buffer(GetGameAlreadyStartedJson(game_id_)));
  } catch (std::exception& e) {
    fail("read", e.what());
  }

  ws_.async_read(
    buffer_,
    beast::bind_front_handler(
      &WebsocketSession::on_read,
      shared_from_this()));
}

void
WebsocketSession::on_write(
  beast::error_code ec,
  std::size_t bytes_transferred)
{
  boost::ignore_unused(bytes_transferred);

  if (ec) {
    return fail(ec, "write");
  }

  queue_.erase(queue_.begin());

  if (!queue_.empty()) {
    ws_.async_write(net::buffer(queue_.front()),
    beast::bind_front_handler(
      &WebsocketSession::on_write,
      shared_from_this()));
  }
}

bool WebsocketSession::HasPlayerId() const {
  return player_id_ != boost::none;
}

std::string WebsocketSession::GetPlayerId() const {
  return player_id_.get();
}
