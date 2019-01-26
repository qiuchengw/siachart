#include "taparam.h"

namespace sia {
namespace formula {

void TaVariable::set(double val) {
    vd.reset(1);
    vd[0] = val;
}

void TaVariable::push(double val) {
    vd.push_back(val);
}

//////////////////////////////////////////////////////////////////////////
void TaParam::setParam(const SkString& name, double val) {
    bool found = false;
    for (auto& tv : in_) {
        if (tv.name.equals(name)) {
            tv.set(val);
            found = true;
            break;
        }
    }

    if (!found) {
        insert(name, val);
    }
}


void TaParam::pushParam(const SkString& name, double val) {
    bool found = false;
    for (auto& tv : in_) {
        if (tv.name.equals(name)) {
            tv.push(val);
            found = true;
            break;
        }
    }

    if (!found) {
        insert(name, val);
    }
}

void TaParam::addOutSeries(const TaSeries& series) {
    out_series_.push_back(series);
}

const SkTArray<TaSeries>& TaParam::outSeries() const {
    return out_series_;
}

SkString TaParam::paramBrief(bool with_name) const {
    auto formatDouble = [](SkString& s, const SkString&name, double d) {
        if ((d - ((double)(int64_t)d)) > 1e-6) {
            // 带小数
            if (name.isEmpty()) {
                s.appendf("%.2lf,", d);
            } else {
                s.appendf("%s:%.2lf,", name.c_str(), d);
            }
        } else { // 不带小数
            if (name.isEmpty()) {
                s.appendf("%lld,", (int64_t)d);
            } else {
                s.appendf("%s:%lld,", name.c_str(), (int64_t)d);
            }
        }
    };

    SkString ret;
    for (const TaVariable& t : in_) {
        if (with_name) {
            formatDouble(ret, t.name, t.asT<double>());
        } else {
            formatDouble(ret, SkString(""), t.asT<double>());
        }
    }
    // 最后一个逗号去掉
    ret.remove(ret.size() - 1, 1);
    if (!ret.isEmpty()) {
        return SkStringPrintf("(%s)", ret.c_str());
    }
    return ret;
}

void TaParam::insert(const SkString& name, double val) {
    TaVariable tv;
    tv.name = name;
    tv.set(val);
    in_.push_back(tv);
}

}
}