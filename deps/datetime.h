#pragma once

#include <iostream>
#include <string>
#include <sstream>

#include "deps/date/include/date/date.h"

namespace sia {

namespace dt { // datetime

using clock = std::chrono::system_clock;
using time = std::chrono::system_clock::time_point;
using time_span = time::duration;
using time_ratio = clock::period;

// 	using hours = std::chrono::hours;
// 	using minutes = std::chrono::hours;
using secs = std::chrono::seconds;

inline time now() {
    return clock::now();
}

enum class ParseFlag {
    kFlagDate = 1,
    kFlagTime = 2,
    kFlagDateTime = 3,
};

inline bool parse(time& t, const std::string& str, ParseFlag f = ParseFlag::kFlagDate) {

    std::istringstream in(str);
    date::sys_seconds tp;
    switch (f) {
    case ParseFlag::kFlagDate:
        in >> date::parse("%F", tp);
        break;
    case ParseFlag::kFlagTime:
        // 必须要有 日期字段，否则会解析失败
        in >> date::parse("%F %T", tp);
        if (in.fail()) {
            in.clear();
            // 22:01 --> 1970-1-1 22:01:00
            mstring s;
            if (str.find(" ") == -1) {
                s = "1970-1-1 "; // 加日期
            }
            s += str;
            if (1 == std::count_if(str.begin(), str.end(), [](auto&t) {
            return t == ':';
        })) {
                s.append(":00");
            }

            in.str(s);
            in >> date::parse("%F %T", tp);
        }
        break;
    case ParseFlag::kFlagDateTime:
        in >> date::parse("%F %T", tp);
        break;
    }

    if (in.fail()) {
        return false;
    }

    t = tp;
    return true;
}

// 直接使用date::formate
using date::format;
inline std::string format_dt(const time& t) {
    return date::format("%c", t);
}

inline std::string format_date(const time& t) {
    return date::format("%F", t);
}

inline std::string format_time(const time& t, bool with_seconds = false) {
    if (with_seconds) {
        return date::format("%T", t);
    }
    return date::format("%R", t);
}

inline int64_t total_seconds(const time_span& ts) {
    return ts.count() / (time_ratio::den);
}

inline int64_t total_days(const time_span& ts) {
    return total_seconds(ts) / (24 * 60 * 60);
}

inline std::tm to_calendar_time(const time& tp) {
    auto date = date::floor<date::days>(tp);
    auto ymd = date::year_month_day(date);
    auto weekday = date::year_month_weekday(date).weekday_indexed().weekday();
    auto tod = date::make_time(tp - date);
    date::days daysSinceJan1 = date - date::sys_days(ymd.year() / 1 / 1);

    std::tm result{};
    result.tm_sec = tod.seconds().count();
    result.tm_min = tod.minutes().count();
    result.tm_hour = tod.hours().count();
    result.tm_mday = unsigned(ymd.day());
    result.tm_mon = unsigned(ymd.month());// -1u; // Zero-based!
    result.tm_year = int(ymd.year());// -1900;
    result.tm_wday = unsigned(weekday);
    result.tm_yday = daysSinceJan1.count();
    result.tm_isdst = -1; // Information not available
    return result;
}

inline std::tm __extract_uint_date(uint32_t ddate) {
    std::tm d;
    d.tm_mday = ddate & 0xFF;
    d.tm_mon = (ddate >> 8) & 0xFF;
    d.tm_year = (ddate >> 16);
    d.tm_wday = 0;
    return d;
}

inline std::tm __extract_uint_time(uint32_t dwTime) {
    std::tm t;
    t.tm_hour = ((dwTime >> 24) & 0x1F);
    t.tm_min = ((dwTime >> 16) & 0x3F);
    t.tm_sec = ((dwTime >> 10) & 0x3F);
    // + std::chrono::milliseconds((dwTime & 0x3FF));
    return t;
}

inline uint32_t make_uint_date(const time& ts) {
    std::tm st = to_calendar_time(ts);
    uint32_t r = st.tm_year;
    r <<= 8;
    r |= st.tm_mon;
    r <<= 8;
    r |= st.tm_mday;
    return r;
}

inline uint32_t make_uint_time(const time& t) {
    std::tm st = to_calendar_time(t);
    uint32_t dwTime = (st.tm_hour & 0x1F);
    dwTime <<= 8;
    dwTime |= (st.tm_min & 0x3F);
    dwTime <<= 8;
    dwTime |= (((st.tm_sec & 0x3F) << 2));
    dwTime <<= 8;
    dwTime |= 0;	// 毫秒值为0
    return dwTime;
}

inline time combine_date_time(const std::tm& ymd, const std::tm& hms) {
    auto new_ymd = date::year(ymd.tm_year) / ymd.tm_mon / ymd.tm_mday; // year_month_day type
    if (new_ymd.ok()) {
        return date::sys_days(new_ymd) + std::chrono::hours(hms.tm_hour)
               + std::chrono::minutes(hms.tm_min)
               + std::chrono::seconds(hms.tm_sec)
               + std::chrono::milliseconds(0);
    }
    throw std::runtime_error("Invalid date");
}

// convert date/time from UTC, to time_point
// note: this function does NOT need the tz library and header
inline time parse_uint_time(uint32_t dwTime, const time& ymd = now()) {
    return combine_date_time(to_calendar_time(ymd), __extract_uint_time(dwTime));
}

inline time parse_uint_date(uint32_t ddate, const time& hms = now()) {
    return combine_date_time(__extract_uint_date(ddate), to_calendar_time(hms));
}

inline time combine_date_time(uint32_t ddate, uint32_t dwTime) {
    return combine_date_time(__extract_uint_date(ddate), __extract_uint_time(dwTime));
}

inline time& set_time(time& ymd, int h, int m, int s, int mills = 0) {
    std::tm tm_t = to_calendar_time(ymd);
    auto new_ymd = date::year(tm_t.tm_year) / tm_t.tm_mon / tm_t.tm_mday; // year_month_day type
    if (new_ymd.ok()) {
        ymd = date::sys_days(new_ymd)
              + std::chrono::hours(h)
              + std::chrono::minutes(m)
              + std::chrono::seconds(s)
              + std::chrono::milliseconds(mills);
        return ymd;
    }
    throw std::runtime_error("Invalid date");
}

inline time& set_date(time& hms, int y, int m, int d) {
    auto ymd = date::year(y) / m / d; // year_month_day type
    if (ymd.ok()) {
        std::tm t = to_calendar_time(hms);
        hms = date::sys_days(ymd)
              + std::chrono::hours(t.tm_hour)
              + std::chrono::minutes(t.tm_min)
              + std::chrono::seconds(t.tm_sec)
              + std::chrono::milliseconds(0);
        return hms;
    }
    throw std::runtime_error("Invalid date");
}

inline int compare_date(const time& t1, const time& t2 ) {
    return make_uint_date(t1) - make_uint_date(t2);
}

inline int compare_time(const time& t1, const time& t2) {
    return make_uint_time(t1) - make_uint_time(t2);
}

}

}
