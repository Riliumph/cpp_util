#include "System.h"
/* Original */
#include "ConsoleSink.h"
#include "FileSink.h"

namespace Log {
void
BootLogSystem()
{
  Console::ConfigureSink();
  File::ConfigureSink();
}
} // namespace Log
