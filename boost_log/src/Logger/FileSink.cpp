#include "FileSink.h"
/* Boost */
#include <boost/make_shared.hpp>
#include <boost/log/common.hpp>
// backend
#include <boost/log/sinks/text_file_backend.hpp>
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
namespace keys = boost::log::keywords;
namespace Log
{
	namespace File
	{
		using backend_t = boost::log::sinks::text_file_backend;
		using frontend_t = sink::synchronous_sink<backend_t>;

		/**
		 * Create back sink
		 *
		 * configure output path, max size, rotation trigger etc...
		 * @return backend
		 */
		static boost::shared_ptr<backend_t> CreateBacksink()
		{
			auto backend = boost::make_shared<backend_t>(
				keys::file_name = "log/[%y-%m%d]%H-%M.log",
				keys::rotation_size = 10 * 1024 /*MB*/ * 1024 /*KB*/,
				keys::time_based_rotation = sink::file::rotation_at_time_point(0, 0, 0));
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

			frontend->set_formatter(expr::format("[%1%][%2%:%3%:%4%][%5%] %6% at %7%(%8%:%9%)") %
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
			frontend->set_filter(Log::severity >= Log::Lv::Debug);
#else
			frontend->set_filter(Log::severity >= Log::Lv::Info);
#endif
			return frontend;
		}

		void ConfigureSink()
		{
			auto backsink = CreateBacksink();
			auto frontsink = CreateFrontsink(backsink);
			boost::log::core::get()->add_sink(frontsink);
		}
	} // namespace File
} // namespace Log
