
#pragma once

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <algorithm>

namespace imascs {
namespace capture {

static std::string FilenameSafeDatetimeString() {
  auto time_point = std::chrono::system_clock::now();
  auto time = std::chrono::system_clock::to_time_t(time_point);
  ::tm local_time = {};
  ::localtime_s(&local_time, &time);
  std::stringstream ss;
  // F "%Y-%m-%d" (the ISO 8601 date format)
  // T "%H:%M:%S" (the ISO 8601 time format)
  ss << std::put_time(&local_time, "%F_%T");
  std::string str = ss.str();
  std::replace(str.begin(), str.end(), ':', '-');
  return str;
}

}  // namespace capture
}  // namespace imascs
