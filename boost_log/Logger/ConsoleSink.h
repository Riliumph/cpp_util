#ifndef INCLUDE_CONSOLE_SINK_H
#define INCLUDE_CONSOLE_SINK_H
namespace Log
{
	namespace Console
	{
		/**
		 * Concifgure Boost.Logger
		 *
		 * Create backsink & frontsink
		 * Add sink to boost log core
		 */
		void ConfigureSink();
	} // namespace Console
} // namespace Log
#endif // INCLUDE_CONSOLE_SINK_H
