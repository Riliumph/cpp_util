#include <spdlog/spdlog.h>

int
main()
{
  spdlog::info("Hello, {}!", "world");
  spdlog::warn("This is a warning!");
  spdlog::error("Something went wrong!");

  return 0;
}
