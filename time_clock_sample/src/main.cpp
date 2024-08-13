// STL
#include <chrono>
#include <cstdio>
#include <ctime>
#include <thread>

using namespace std::literals::chrono_literals;

/// @brief itWorks
/// https://www.reddit.com/r/ProgrammerHumor/comments/1d1pbn9/itworks/
/// @return
int
main()
{
  int minutes_ar[] = { 1, 2, 3, 4, 5, 0 };
  int minute_arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 9 };
  int hours[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 19, 11, 0 };

  int hour = 12;
  int minute = 9;
  int minutes = 5;

  while (true) {
    while (minute == true || minutes) {
      do {
        std::printf("%02d:%i%d\n", hour, minutes, minute);
        std::this_thread::sleep_for(10ms);
        minute = minute_arr[minute];
      } while (minute != 0);
      minutes = minutes_ar[minutes];
    }
    hour = hours[hour];
    std::printf("%02d:%i%i\n", hour, minutes, minute);
    minute = minute_arr[minute];
    std::this_thread::sleep_for(10ms);
  }
  return 0;
}
