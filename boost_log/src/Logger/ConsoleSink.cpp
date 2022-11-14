#include "ConsoleSink.h"
/* Boost */
#include <boost/make_shared.hpp>
#include <boost/log/common.hpp>
// backend
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/core.hpp>
#include <boost/core/null_deleter.hpp>
// frontend
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/expressions/formatters/date_time.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/attributes/current_process_id.hpp>
#include <boost/log/attributes/current_thread_id.hpp>
#include <boost/log/attributes/current_process_name.hpp>
/* Original */
#include "GlobalLogger.h"

namespace sink = boost::log::sinks;
namespace expr = boost::log::expressions;
namespace attr = boost::log::attributes;
namespace Log
{
	namespace Console
	{
		using backend_t = boost::log::sinks::text_ostream_backend;
		using frontend_t = sink::synchronous_sink<backend_t>;

		/**
		 * Create back sink
		 *
		 * configure output destination etc...
		 * @return backend
		 */
		static boost::shared_ptr<backend_t> CreateBacksink()
		{
			auto backend = boost::make_shared<backend_t>();
			boost::shared_ptr<std::ostream> destination(&std::clog, boost::null_deleter());
			backend->add_stream(destination);
			backend->auto_flush(true);
			return backend;
		}

		/**
		 * Create front sink
		 *
		 * @param backend [i] base setting
		 * @return frontend
		 */
		static boost::shared_ptr<frontend_t> CreateFrontsink(const boost::shared_ptr<backend_t> &backend)
		{
#if (_MSC_VER == 1900) && defined(_DEBUG)
			using line_t = long;
#else
			using line_t = int;
#endif
			auto frontend = boost::make_shared<frontend_t>(backend);

			frontend->set_formatter(expr::format("1.%1%\n2.%2%\n3.%3%\n4.%4%\n5.[%5%]\n6.%6%\n7.#%7%\n8.%8%\n9.L.%9%\n") %
									expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S") %
									expr::attr<attr::current_process_id::value_type>("ProcessID") %
									expr::attr<attr::current_thread_id::value_type>("ThreadID") %
									expr::attr<attr::current_process_name::value_type>("Process") %
									Log::severity %
									expr::message %
									expr::attr<std::string>("Func") %
									expr::attr<std::string>("File") %
									expr::attr<line_t>("Line"));
#ifdef _DEBUG
			frontend->set_filter(Log::severity >= Log::Lv::Warn);
#else
			frontend->set_filter(Log::severity >= Log::Lv::Error);
#endif
			return frontend;
		}

		void ConfigureSink()
		{
			auto backsink = CreateBacksink();
			auto frontsink = CreateFrontsink(backsink);
			boost::log::core::get()->add_sink(frontsink);
		}
	} // namespace Console
} // namespace Log
