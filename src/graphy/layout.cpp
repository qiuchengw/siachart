#include "layout.h"
#include <iostream>
#include "widget.h"

namespace sia {
namespace graphy {

void Layout::setDecoratedWidget(Widget* w) {
    decorated_ = w;
    doLayout();
}

Layout::~Layout()
{

}

SkRect Layout::geometry() const {
    if (decorated_) {
        return decorated_->geometry();
    }
    return SkRect::MakeEmpty();
}

SkRect Layout::rect() const {
    if (decorated_) {
        return decorated_->rect();
    }
    return SkRect::MakeEmpty();
}

Widget* Layout::widget(const SkString& name) const {
    Widget* ret = nullptr;
    if (decorated_) {
        decorated_->forEachChild([&](Widget* w)->bool {
            if (w->name() == name) {
                ret = w;
                return false;
            }
            return true;
        });
    }
    return ret;
}

std::vector<Widget*> Layout::childrenWidget() const {
    if (decorated_) {
        return decorated_->children();
    }
    return std::vector<Widget*>();
}

bool Measure::parse(const SkString& s) {
    std::cout << "Measure::parse [" << s.c_str() << "] ";
    try {
        if (s.startsWith('*')) {
            if (s.equals("*")) {
                val_ = SkIntToScalar(1);
            } else {
                val_ = std::stof(s.c_str() + 1);
            }
            type_ = MeasureType::kPercent;
        } else if (s.equals("#")) {
            val_ = SkIntToScalar(1);
            type_ = MeasureType::kAll;
        } else {
            val_ = std::stof(s.c_str());
            type_ = MeasureType::kFixed;
        }

        std::cout<< " type:" << (int)type_ << " | val:" << val_ << std::endl;
        return true;
    } catch (...) {
        std::cout << " failed" << std::endl;

        val_ = 0.f;
        type_ = MeasureType::kInvalid;
        return false;
    }
}

void Measure::set(SkScalar v, MeasureType t) {
    val_ = v;
    type_ = t;
}

SkScalar Measure::calc(SkScalar all, SkScalar max_v, SkScalar min_v) const {
    SkScalar ret = 0.f;
    switch (type_) {
    case MeasureType::kFixed:
        ret = val_;
        break;

    case MeasureType::kPercent:
        ret = val_ * all;
        break;

    case MeasureType::kAll:
        ret = all;
        break;

    default:
        return ret;
    }

    if (ret > max_v) {
        ret = max_v;
    }

    if (ret < min_v) {
        ret = min_v;
    }
    return ret;
}

}
}
