#include "tmplayout.h"
#include <iostream>
#include "deps/kutil/rjson.h"
#include "widget.h"

namespace sia {
namespace graphy {

    TemplateLayout::~TemplateLayout()
    {

    }

bool TemplateLayout::parse(const SkString& layout) {
//     rapidjson::RJsonDoc d;
//     if (!d.MyParse(layout.c_str(), false)) {
//         return false;
//     }
//
//     SkString tmpl(d["template"].GetString());
//     if (!matrix_.init(tmpl)) {
//         return false;
//     }
//
//     auto &items = d["items"];
//     for (auto i = items.MemberBegin(); i != items.MemberEnd(); ++i) {
//         SkString item(i->name.GetString());
//         if (item.size() != 1) {
//             return false;
//         }
//
//         LayoutItemPtr layout(new LayoutItem);
//         if (!layout->parse(item[0], i->value)) {
//             return false;
//         }
//
//         if (!matrix_.addLayoutItem(item[0], layout)) {
//             return false;
//         }
//     }
    return true;
}

void TemplateLayout::doLayout() {
    SkRect rc = geometry();
    if (rc.isEmpty()) {
        return;
    }

    if (matrix_.calc(rc)) {
        // 布局
        matrix_.forEach([=](int, int, char, LayoutItemPtr layout)->bool {
            if (Widget* w = widget(layout->widget)) {
                SkRect rc = SkRect::MakeXYWH(layout->topleft.fX, layout->topleft.fY,
                                             layout->size.width(), layout->size.height());
                w->setGeometry(rc);
            }
            return true;
        });
    }
}

bool TemplateLayout::TmplMatrix::init(const SkString& d2) {
    SkStrSplit(d2.c_str(), "|", SkStrSplitMode::kStrict_SkStrSplitMode, &tmpl_);
    if (tmpl_.empty()) {
        return false;
    }

    // 所有的元素宽度应该是一样的！
    auto len = tmpl_.front().size();
    for (SkString& s : tmpl_) {
        if (len != s.size()) {
            return false;
        }
    }

    // 检查元素布局是否合法
    return true;
}

char TemplateLayout::TmplMatrix::element(int r, int c) const {
    if (row() <= r || col() <= c) {
        return kNullElement;
    }
    return tmpl_[r][c];
}

int TemplateLayout::TmplMatrix::row() const {
    return tmpl_.count();
}

int TemplateLayout::TmplMatrix::col() const {
    if (tmpl_.empty()) {
        return 0;
    }
    return tmpl_.front().size();
}

void TemplateLayout::TmplMatrix::forEach(std::function<bool(int, int, char, LayoutItemPtr)> onItem) {
    int rows = row();
    int cols = col();
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            char item = tmpl_[r][c];
            if (!onItem(r, c, item, layoutItem(item))) {
                return;
            }
        }
    }
}

void TemplateLayout::TmplMatrix::forEachRow(std::function<bool(int, std::vector<LayoutItemPtr>)> onItem) {
    for (int i = 0; i < row(); ++i) {
        std::vector<LayoutItemPtr> all;
        SkString& line = tmpl_[i];
        for (int j = 0; j < col(); ++j) {
            const char item = line[j];
            if (auto layout = layoutItem(item)) {
                all.push_back(layout);
            }
        }

        if (!onItem(i, all)) {
            return;
        }
    }
}

void TemplateLayout::TmplMatrix::forEachCol(std::function<bool(int, std::vector<LayoutItemPtr>)> onItem) {
    for (int c = 0; c < col(); ++c) {	// 先列，再行
        std::vector<LayoutItemPtr> all;
        for (int r = 0; r < row(); ++r) {
            const char item = tmpl_[r][c];
            if (auto layout = layoutItem(item)) {
                all.push_back(layout);
            }
        }

        if (!onItem(c, all)) {
            return;
        }
    }
}

bool TemplateLayout::TmplMatrix::addLayoutItem(char item, LayoutItemPtr l) {
    for (int r = 0; r < row(); ++r) {
        auto &line = tmpl_[r];
        for (int c = 0; c < line.size(); ++c) {
            if (line[c] == item) {
                // 必须得存在对应的item，才能指定layout
                items_.insert({ item, l });
                return true;
            }
        }
        // if (tmpl_[r].find(&item) != -1) {
    }
    return false;
}

struct CalcTmp {
    SkScalar used_space;	// 已用空间
    SkScalar left_space;	// 剩余空间
    int used_items;	// 有多少个item参与了计算，占用掉了空间
    int left_items;	// 还剩下多少个item没有参与计算

    // 对于行，代表的是高，对于列，代表的是宽
    SkScalar min_space;

    void takeSpace(SkScalar spac) {
        used_space += spac;
        left_space -= spac;
        used_items++;
        left_items--;
    }

    // 等分剩余的空间
    inline SkScalar splitSpace() const {
        if (left_items <= 0) {
            return 0;
        }
        return left_space / left_items;
    }
};

bool TemplateLayout::TmplMatrix::calc(const SkRect& rc) {
    // 重置所有的坐标
    for (auto i : items_) {
        i.second->resetGeometry();
    }

#if 1
    auto dump_geom = [&](const char* desc) {
        std::cout << "----------------------\n" << desc << std::endl;
        for (auto i : items_) {
            i.second->dumpGeom(i.first);
        }
    };
    auto dump_measure = [&](const char* desc) {
        std::cout << "----------------------\n" << desc << std::endl;
        for (auto i : items_) {
            i.second->dumpMeasure(i.first);
        }
    };
#endif

    dump_geom("init:");
    dump_measure("measures:");

    // 先计算 * 和 fixed，最后计算 #
    SkTArray<CalcTmp> rows_calc;
    SkTArray<CalcTmp> cols_calc;
    auto init_calc_tmp = [](SkTArray<CalcTmp>& tmp, SkScalar space, int len) {
        CalcTmp cal;
        cal.left_space = space;
        cal.used_space = 0;
        cal.left_items = len;
        cal.used_items = 0;
        cal.min_space = ZERO_SCALAR;
        // cal.max_space = MAX_SCALAR;
        tmp.push_back_n(len, cal);
    };

    const int rows = row();
    init_calc_tmp(rows_calc, rc.width(), rows);
    forEachRow([&](int r, std::vector<LayoutItemPtr> all)->bool {
        // 找出这一行中的layout设置的**最小高度的最大值**
        SkScalar max_s_min = ZERO_SCALAR; // 设置的最小值的那个最大值
        for (LayoutItemPtr layout : all) {
            SkScalar sk = layout->minHeight(rc.height());
            if (sk > max_s_min) {
                max_s_min = sk;
            }
        }
        rows_calc[r].min_space = max_s_min;
        return true;
    });

    const int cols = col();
    init_calc_tmp(cols_calc, rc.height(), cols);
    forEachCol([&](int c, std::vector<LayoutItemPtr> all)->bool {
        // 找出这一列的设置的**最小宽度的最大值**
        SkScalar max_s_min = ZERO_SCALAR; // 设置的最小值的那个最大值
        for (LayoutItemPtr layout : all) {
            SkScalar sk = layout->minWidth(rc.width());
            if (sk > max_s_min) {
                max_s_min = sk;
            }
        }
        cols_calc[c].min_space = max_s_min;
        return true;
    });

    // 第一遍：计算能立即确定的宽高
    for (int r = 0; r < rows; ++r) {
        auto &line = tmpl_[r];
        // 这一行的计算
        for (int c = 0; c < line.size(); ++c) {
            const char item = line[c];
            if (LayoutItemPtr layout = layoutItem(item)) {
                SkScalar take_space = ZERO_SCALAR;
                if (layout->width.isImdiateSure()) {
                    // 可以先计算宽度了。
                    // 对于能立即确定宽高的元素，它的宽高只应该被计算一次！
                    // 但是这个元素可能会出现多次！所以我们还是要减去一些占用元素，空间不减！
                    if (layout->size.width() <= ZERO_SCALAR) {
                        take_space = layout->width.calc(rows_calc[r].left_space, layout->max_width, layout->min_width);
                        layout->size.fWidth = take_space;
                    }
                    rows_calc[r].takeSpace(take_space);
                }

                if (layout->height.isImdiateSure()) {
                    // 计算这个高度
                    if (layout->size.height() <= ZERO_SCALAR) {
                        take_space = layout->height.calc(cols_calc[c].left_space, layout->max_height, layout->min_height);
                        layout->size.fHeight = take_space;
                    }
                    cols_calc[c].takeSpace(layout->size.height());
                }
            }
        }
    }
    dump_geom("first calc:");

    // 第二遍计算：把剩余的空间平均分配下去
    for (int r = 0; r < rows; ++r) {
        auto &line = tmpl_[r];
        const SkScalar splited_width = rows_calc[r].splitSpace();

        // 这一行的计算
        for (int c = 0; c < line.size(); ++c) {
            const SkScalar splited_height = cols_calc[c].splitSpace();
            if (LayoutItemPtr layout = layoutItem(line[c])) {
                if (!layout->width.isImdiateSure()) {
                    layout->size.fWidth += splited_width;	// 注意，有可能是同一个元素占了2份的空间
                    rows_calc[r].takeSpace(splited_width);
                }

                if (!layout->height.isImdiateSure()) {
                    layout->size.fHeight += splited_height;
                    cols_calc[c].takeSpace(splited_height);
                }
            }
        }
    }
    dump_geom("second calc:");

    // 第三遍计算：位置
    SkScalar y = rc.top();
    for (int r = 0; r < rows; ++r) {
        SkScalar x = rc.left();

        // 这一行的计算
        auto &line = tmpl_[r];
        for (int c = 0; c < line.size(); ++c) {
            if (LayoutItemPtr layout = layoutItem(line[c])) {
                if (INVALID_SCALAR_VAL == layout->topleft.x()) {
                    layout->topleft.fX = x;
                    x += layout->size.width();
                }
                layout->topleft.fY = y;

                // 取一行最后一个元素的高度为下一行的y值
                if (c == line.size() - 1) {
                    y += layout->size.height();
                }
            }
        }
    }
    dump_geom("third calc:");

    return true;
}

TemplateLayout::LayoutItemPtr TemplateLayout::TmplMatrix::layoutItem(char c) const {
    auto i = items_.find(c);
    if (i != items_.end()) {
        return i->second;
    }
    return nullptr;
}

// bool TemplateLayout::LayoutItem::parse(const char item, rapidjson::Value &v) {
//     std::cout << "parse LayoutItem: [" << item << "]" << std::endl;
//     if (!v.HasMember("widget")) {
//         return false;
//     }
//     widget = v["widget"].GetString();
//
// #define MEMBER_INT(m) if (v.HasMember(#m)) { m = SkIntToScalar(v[#m].GetInt()); }
// #define MEMBER_MEASURE(m) \
// 	if (v.HasMember(#m)) {\
// 		std::cout<< "existing member:" << #m << "--->" << SkString(v[#m].GetString()).c_str() <<std::endl; \
// 		if (!m.parse(SkString(v[#m].GetString()))){ return false; }\
// 	} else { m.parse(SkString("#")); }	// 默认取剩余的所有的空间
//
//     MEMBER_INT(max_width);
//     MEMBER_INT(min_width);
//     MEMBER_MEASURE(width);
//     MEMBER_INT(max_height);
//     MEMBER_INT(min_height);
//     MEMBER_MEASURE(height);
// #undef MEMBER_MEASURE
// #undef MEMBER_INT
//
//     return true;
// }

void TemplateLayout::LayoutItem::resetGeometry() {
    topleft.set(INVALID_SCALAR_VAL, INVALID_SCALAR_VAL);
    size.setEmpty();
}

void TemplateLayout::LayoutItem::dumpGeom(char c) const {
    std::cout << c << ":" << topleft.x() << "," << topleft.y()
              << " | " << size.width() << "," << size.height() << std::endl;
}

void TemplateLayout::LayoutItem::dumpMeasure(char c) const {
    std::cout << c << ":width [type:" << (int)width.typ() << "] | [val:" << width.val() << "]"
              << " ---> height [type:" << (int)height.typ() << "] | [val:" << height.val() << "]" << std::endl;
}

void TemplateLayout::LayoutItem::adjustMaxMinWidth(SkScalar min, SkScalar max) {

}

void TemplateLayout::LayoutItem::adjustMaxMinHeight(SkScalar min, SkScalar max) {

}

}
}
