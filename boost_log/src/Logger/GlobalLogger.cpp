#include "GlobalLogger.h"
/* STL */
#include <ostream>
#include <string>
/* Boost */
#include <boost/log/attributes/current_process_name.hpp>
#include <boost/log/core.hpp>                          // for sink
#include <boost/log/sources/global_logger_storage.hpp> //for BOOST_LOG_GLOBAL_LOGGER
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
/* Original */
#include "Utility/e_cast.hpp"

namespace src = boost::log::sources;
namespace attr = boost::log::attributes;
namespace Log {
namespace {
const std::string LV_NAME[enum_cast(Log::Lv::NUM)] = { "Debug",
                                                       "Info",
                                                       "Warn",
                                                       "Error",
                                                       "Fatal" };
}

/**
 * Construct logger
 *
 * @param g_logger [i] class name
 */
BOOST_LOG_GLOBAL_LOGGER_INIT(g_logger, src::severity_logger_mt<Log::Lv>)
{
  auto r = src::severity_logger_mt<Log::Lv>();
#ifdef DEFAULT_SETTING
  boost::log::add_common_attributes();
#else
  r.add_attribute("TimeStamp", attr::local_clock());
  r.add_attribute("ProcessID", attr::current_process_id());
  r.add_attribute("Process", attr::current_process_name());
  r.add_attribute("ThreadID", attr::current_thread_id());
#endif
  return r;
}

std::ostream&
operator<<(std::ostream& os, const Log::Lv& level)
{
  assert(level != Log::Lv::NUM && "Detect illegal log level");
  os << LV_NAME[static_enum_cast(level)];
  return os;
}
} // namespace Log
