#pragma once

#include <unordered_map>
#include <random>

namespace utils {
#ifdef max
#define mymax max
#define mymin min
#else
#define mymax std::max
#define mymin std::min
#endif

template<typename T>
inline T max_4(T n1, T n2, T n3, T n4, T n5) {
    n1 = mymax(n1, n2);
    n3 = mymax(n3, n4);
    n3 = mymax(n3, n5);
    return mymax(n1, n3);
}

template<typename T>
inline T min_4(T n1, T n2, T n3, T n4, T n5) {
    n1 = mymin(n1, n2);
    n3 = mymin(n3, n4);
    n3 = mymin(n3, n5);
    return mymin(n1, n3);
}

namespace myrand {

static std::default_random_engine _rand_gen;

inline int32_t rand_v(int32_t vmin, int32_t vmax) {
    // 最新价格在 涨跌幅 [5, 100] / 10000 之间
    std::uniform_int_distribution<int> dis(vmin, vmax);
    return dis(_rand_gen);
}

inline int32_t rand_vol() {
    // 最新价格在 涨跌幅 [5, 100] / 10000 之间
    return rand_v(5, 100);
}

inline double change_ratio(bool has_negtive = true) {
    // 为了让曲线更趋向真实
    static std::unordered_map<int32_t, int16_t> _rec;

    std::uniform_int_distribution<int> rise_dis(0, 2);
    double d_ratio = (double)(rand_vol()) / 10000.f;  // 涨跌幅在 +- [0.25, 0.5]%
    if (has_negtive) {
        auto flag = rise_dis(_rand_gen);
        switch (flag) {
        case 0:
            d_ratio = 0;
            break;	// 不涨不跌
        case 1:
            d_ratio = -d_ratio;
            break; // 跌
        default:
            break;
        }
    }
    return d_ratio;
}

}

namespace timed {

const int YEAR_MONTHS = 12;
const int MONTH_DAYS = 31;
const int YEAR_DAYS = YEAR_MONTHS * MONTH_DAYS;

// 解析日期值到年月日
inline bool disamDate(int32_t date, int& y, int& m, int& d) {
    y = date / 10000; // year
    m = (date / 100) % 100; // month
    d = date % 100; // day

    return (y > 0 && m >= 1 && m <= 12 && d >= 1 && d <= 31);
}

// 生成日期，从今天向前推 days 天
// 算法并不严谨，因为我们算每个月都是31天
inline int32_t dateSub(int32_t d1, int32_t days) {
    int32_t from = d1;
    // 20180305
    int y, m, d;
    if (disamDate(d1, y, m, d)) {
        for (int i = 0; i < days; i++) {
            from = (y * 100 + m) * 100 + d;

            // 加天
            if (d-- < 1) { // 这个不用算的太严谨！
                d = 31;
                if (m-- < 1) {
                    m = 1;
                    y--;
                }
            }
        }
    }
    return from;
}

inline int32_t dateAdd(int32_t d1, int32_t days) {
    int32_t from = d1;
    // 20180305
    int y, m, d;
    if (disamDate(d1, y, m, d)) {
        for (int i = 0; i < days; i++) {
            from = (y * 100 + m) * 100 + d;

            // 加天
            if (d++ > 31) { // 这个不用算的太严谨！
                d = 1;
                if (m++ > 12) {
                    m = 1;
                    y++;
                }
            }
        }
    }
    return from;
}

// 这个是粗略的算一下日期d1 和 d2之间相隔了多少天
// 我们认为一个月就是31天且一年就是12个月以简化计算
inline int32_t daySpan(int32_t date1, int32_t date2) {
    if (date1 == date2) {
        return 0;
    }

    int y1 = 0, m1 = 0, d1 = 0;
    int y2 = 0, m2 = 0, d2 = 0;
    if (disamDate(date1, y1, m1, d1) && disamDate(date2, y2, m2, d2)) {
        return (y1 - y2)* YEAR_DAYS + (m1 - m2) * MONTH_DAYS + (d1 - d2);
    }
    return 0;
}

void forEveryMinute(int32_t start, int32_t end, std::function<void(int32_t)> cb) {
    int h = start / 10000; // hour
    int m = (start / 100) % 100; // minute
    while (true) {
        int t = (h * 100 + m) * 100; // 113000
        if (t > end) {  // 11:30:00
            break;
        }
        // 每分钟生成一个数据
        cb(t);

        // 加分钟
        ++m;
        if (m >= 60) {
            ++h;
            m = 0;
        }
    }
}

void forEveryDay(int32_t start, int32_t end, std::function<void(int32_t)> cb) {
    // 20180305
    int y, m, d;
    if (utils::timed::disamDate(start, y, m, d)) {
        while (true) {
            int t = (y * 100 + m) * 100 + d;
            if (t > end) {
                break;
            }
            // 每分钟生成一个数据
            cb(t);

            // 加天
            if (d++ >= 31) { // 这个不用算的太严谨！
                d = 1;
                if (m++ >= 13) {
                    m = 1;
                    y++;
                }
            }
        }
    }
}

}

}
