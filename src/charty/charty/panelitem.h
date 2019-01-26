#pragma once

#include "defines.h"
#include "visualitem.h"

namespace sia {
namespace charty {

class Panel;
class MainView;
class Series;

/*!
 * \class PanelItem
 *
 * \brief 所有Panel的直接孩子都要继承此类
 *
 *
 */
class PanelItem : public VisualItem {
public:
    PanelItem(const SkString& name, Panel* parent);

    inline void setParent(Panel* p) {
        VisualItem::setParent((Widget*)p);
    }

    Panel* parent()const;

    inline Panel* panel()const {
        return parent();
    }

    MainView* mainView()const;

    std::vector<my_sp<Series>> serieses()const;

    /*! \brief PanelItem 真正绘制的时候，都是有padding的。
     *
     *    @param
     *    @return
     */
    void setContentArea(SkRect rc) {
        content_rect_ = rc;
    }

    const SkRect& contentRect()const {
        return content_rect_;
    }

    inline SkScalar contentHeight()const {
        return content_rect_.height();
    }

    inline SkScalar contentWidth()const {
        return content_rect_.width();
    }

    inline SkScalar contentX()const {
        return content_rect_.left();
    }

    inline SkScalar contentY()const {
        return content_rect_.right();
    }

private:
    SkRect content_rect_;
};

}
}
