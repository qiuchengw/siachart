#pragma once

#include "panelitem.h"
#include "defines.h"

namespace sia {
namespace charty {

/*!
 * \class VisualItem
 *
 * \brief 图表内所有可见的区域元素都要继承于此类
 *
 */
class Title : public PanelItem {
    enum Flags {
        // 是否显示为空
        kBlankTitle = 0x1,

        // 是否有动态item
        kDynamicItem = 0x2,
    };

public:
    Title(Panel* parent = nullptr);

    void addTitleItem(const PanelTitleItem& item);
    void removeTitleItem(const SkString& name);

    void setItems(const SkTArray<PanelTitleItem>& items);

    void setBlank(bool blank);

    inline const SkTArray<PanelTitleItem>& titleItems()const {
        return items_;
    }

    inline bool hasCustomItem()const {
        return !flags_.test(Flags::kBlankTitle) && !items_.empty();
    }

    inline bool hasDynamicItem()const {
        return !flags_.test(Flags::kBlankTitle) && flags_.test(Flags::kDynamicItem);
    }

    /*! \brief 序列被删除了
     *
     *    @param    series_name 序列名
     *    @return
     */
    void seriesRemoved(const SkString& series_name);

    /*! \brief 在panel中，根据名字查找序列
     *
     *    @param    series_name 序列名
     *    @param    view 所在view
     *    @return   序列指针
     */
    my_sp<Series> series(const SkString& series_name, my_sp<MainView>& view)const;

protected:
    virtual void preparePaints() override;

    virtual void onDraw(SkCanvas* canvas) override;

    void checkTitleItems();

private:
    //! title显示的项目
    SkTArray<PanelTitleItem> items_;

    //! 是否显示为空
    BitFlags flags_;

    SkPaint* bk_ = nullptr;
    SkPaint* fg_ = nullptr;
    SkPaint* border_ = nullptr;
};
}
}
