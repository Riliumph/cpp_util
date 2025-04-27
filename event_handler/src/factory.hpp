// STL
#include <iostream>
#include <memory>
#include <string>
// original
#include "event.hpp"

std::shared_ptr<event::interface::EventHandler>
CreateEventHandler(const std::string& type)
{
  if (type == "epoll") {
    return std::make_shared<event::EpollHandler>();
  }
  if (type == "select") {
    return std::make_shared<event::SelectHandler>();
  }
  static_assert("not supported type event handler");
  return nullptr;
}
