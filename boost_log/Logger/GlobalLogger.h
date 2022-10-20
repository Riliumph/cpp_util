#ifndef INCLUDE_GLOBAL_LOGGER_H
#define INCLUDE_GLOBAL_LOGGER_H
/* Boost */
#include <boost/log/sources/global_logger_storage.hpp> // for BOOST_LOG_GLOBAL_LOGGER
#include <boost/log/sources/severity_logger.hpp>	   // for severity_logger
/* Original */
#include "SeverityType.h"

namespace Log
{
	/**
	 * Declare logger
	 *
	 * @param g_logger [i] struct name
	 * @param severity_logger_mt [i] logger type
	 */
	BOOST_LOG_GLOBAL_LOGGER(g_logger, boost::log::sources::severity_logger_mt<Log::Lv>)

	/**
	 * output operator
	 *
	 * @param os [i] logger stream
	 * @param level [i] log level
	 * @return os
	 */
	std::ostream &operator<<(std::ostream &os, const Log::Lv &level);
} // namespace Log
#endif // INCLUDE_GLOBAL_LOGGER_H
