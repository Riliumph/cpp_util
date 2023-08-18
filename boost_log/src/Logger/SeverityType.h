#ifndef INCLUDE_SEVERITY_TYPE_H
#define INCLUDE_SEVERITY_TYPE_H
/* Boost */
#include <boost/log/expressions/keyword.hpp> // for BOOST_LOG_ATTRIBUTE_KEYWORD

namespace Log {
/// Log level type
typedef enum struct SEVERITY_TYPE
{
  /*note,*/ Debug,
  Info,
  Warn,
  Error,
  /*critical,*/ Fatal,
  NUM
} Lv;

/**
 * Declare keyword
 *
 * define struct severity
 * @param severity [i] declare struct name
 * @param "severity" [i] struct attribute name
 * @param Log::Lv [i] instance tag
 */
BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", Log::Lv);
} // namespace Log
#endif // INCLUDE_SEVERITY_TYPE_H
