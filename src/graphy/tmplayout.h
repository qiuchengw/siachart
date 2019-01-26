#pragma once

#include <vector>
#include <unordered_map>
#include "SkString.h"
#include "SkRect.h"
#include "layout.h"

namespace sia {
namespace graphy {

class GRAPHY_API TemplateLayout : public Layout {

    class LayoutItem :
        public std::enable_shared_from_this<LayoutItem> {
    public:
        // bool parse(const char item, rapidjson::Value &v);
        void resetGeometry();

        void dumpGeom(char c)const;

        void dumpMeasure(char c) const;

        // 校正同一行/列上的元素空间
        // 同一行上，最小高度应该取最大的那个（如果本身未设置的话！）
        // 同一列上，最小宽度应该取最大的那个（如果本身未设置的话！）
        void adjustMaxMinWidth(SkScalar min, SkScalar max);
        void adjustMaxMinHeight(SkScalar min, SkScalar max);

        SkScalar minWidth(SkScalar all) {
            if (width.isFixed()) {
                return width.val();
            }

            if (min_width > ZERO_SCALAR) {
                return min_width;
            }

            if (width.isPercent()) {
                return width.calc(all, max_width, min_width);
            }
            return ZERO_SCALAR;
        }

        SkScalar minHeight(SkScalar all) {
            if (height.isFixed()) {
                return height.val();
            }

            if (min_height > ZERO_SCALAR) {
                return min_width;
            }

            if (height.isPercent()) {
                return height.calc(all, max_height, min_height);
            }
            return ZERO_SCALAR;
        }

    public:
        SkString widget;

        // 左上角位置
        SkPoint topleft;
        // 大小
        SkSize size;

        // 宽的计算
        SkScalar max_width = MAX_SCALAR;
        SkScalar min_width = ZERO_SCALAR;
        Measure width;

        // 高的计算
        SkScalar max_height = MAX_SCALAR;
        SkScalar min_height = ZERO_SCALAR;
        Measure height;
    };
    typedef my_sp<LayoutItem> LayoutItemPtr;

    // 2维模板矩阵
    class TmplMatrix {
        static const char kNullElement = '\0';
    public:
        bool init(const SkString& d2);

        char element(int r, int c)const;

        int row()const;
        int col()const;

        void forEach(std::function<bool(int, int, char, LayoutItemPtr)> onItem);
        void forEachRow(std::function<bool(int, std::vector<LayoutItemPtr>)> onItem);
        void forEachCol(std::function<bool(int, std::vector<LayoutItemPtr>)> onItem);

        bool addLayoutItem(char item, LayoutItemPtr l);

        bool calc(const SkRect& rc);

    protected:
        LayoutItemPtr layoutItem(char c)const;

    private:
        // 布局模板
        SkTArray<SkString> tmpl_;

        // 需要被布局的元素
        std::unordered_map<char, LayoutItemPtr> items_;
    };

public:
    virtual ~TemplateLayout();
    virtual bool parse(const SkString& layout) override;

protected:
    virtual void doLayout() override;

private:
    TmplMatrix matrix_;
};
}
}
