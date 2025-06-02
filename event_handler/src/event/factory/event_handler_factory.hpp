#ifndef EVENT_FACTORY_EVENT_HANDLER_FACTORY_HPP
#define EVENT_FACTORY_EVENT_HANDLER_FACTORY_HPP
// STL
#include <memory>
#include <string>
// event
#include "event/abc/event_handler.hpp"
#include "event/epoll/epoll_handler.hpp"
#include "event/select/select_handler.hpp"

namespace event {
namespace factory {
/// @brief イベントハンドラのインスタンスを生成するファクトリクラス
/// @details ファクトリパターンの設計方針を原則クラスとする。
/// 現状、関数でも問題はないが将来に生成条件をDBから探す事になった場合、DBの接続クライアントを引数でもらうことになる。
/// 引数はインスタンスの生成を行う本質的な情報のみとしたいため、クラスの内部で接続クライアントを持たせることとする。
class EventHandlerFactory
{
public:
  using event_handler_t = event::abc::EventHandler;
  using epoll_handler_t = event::epoll::EpollHandler;
  using select_handler_t = event::select::SelectHandler;

public:
  EventHandlerFactory() = default;
  ~EventHandlerFactory() = default;

  std::shared_ptr<event_handler_t> MakeEventHandler(std::string event_type);
};
} // namespace factory
} // namespace event
#endif // EVENT_FACTORY_EVENT_HANDLER_FACTORY_HPP
