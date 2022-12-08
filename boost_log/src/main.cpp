#include "Logger/System.h"

int main()
{
	// Boot up log system
	Log::BootLogSystem();

	LOG(Log::Lv::Debug) << "Debug!!";
	LOG(Log::Lv::Info) << "Info!!";
	LOG(Log::Lv::Warn) << "Warn!!";
	LOG(Log::Lv::Error) << "Error!!";
	LOG(Log::Lv::Fatal) << "Fatal!!";
	return 0;
}
