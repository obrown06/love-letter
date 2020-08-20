#include "handlers/dispatcher.hpp"
#include "handlers/exceptions.hpp"

#include <iostream>

void HandlerDispatcher::RegisterHandler(std::shared_ptr<BaseHandler> handler) {
  if (routes_to_handlers_.find(handler->GetRoute()) != routes_to_handlers_.end()) {
    throw DuplicateRouteException(handler->GetRoute());
  }
  routes_to_handlers_.emplace(std::make_pair(handler->GetRoute(), handler));
}
