// Copyright (c) 2020 GGGGITFKBJG
//
// Date: 2020-05-09 16:36:50
// Description:

#include "src/utils/Timestamp.h"

#include <sys/time.h>
#include <time.h>

using std::string;
using wethands::Timestamp;

static_assert(sizeof(Timestamp) == sizeof(int64_t), "Unexpected size.");

Timestamp Timestamp::Now() {
  struct timespec ts;
  ::clock_gettime(CLOCK_REALTIME, &ts);
  return Timestamp(ts.tv_sec * kMicrosecondsPerSecond +
                   ts.tv_nsec / kNanosecondsPerMicrosecond);
}

Timestamp Timestamp::RoundByDay() const {
  // gmtime()后再mktime(), 比较低效.
  time_t seconds = SecondsSinceEpoch();
  struct tm tm_time;
  ::localtime_r(&seconds, &tm_time);  // mktime的参数必须是本地时间.
  tm_time.tm_hour = 0;
  tm_time.tm_min = 0;
  tm_time.tm_sec = 0;
  return Timestamp(kMicrosecondsPerSecond *
                   static_cast<int64_t>(::mktime(&tm_time)));
}

string Timestamp::ToFormattedString(bool showMicroseconds) const {
  time_t seconds = SecondsSinceEpoch();
  struct tm tm_time;
  // ::gmtime_r(&seconds, &tm_time);  // UTC时间
  ::localtime_r(&seconds, &tm_time);  // 日历时间转为本地时间

  char buf[32];
  if (showMicroseconds) {
    int microseconds = static_cast<int>(microsecondsSinceEpoch_ %
                                        kMicrosecondsPerSecond);
    snprintf(buf, sizeof(buf), "%4d%02d%02d-%02d%02d%02d.%06d",
             tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
             tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec,
             microseconds);
  } else {
    snprintf(buf, sizeof(buf), "%4d%02d%02d-%02d%02d%02d",
             tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
             tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
  }
  return buf;
}
