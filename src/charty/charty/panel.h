#pragma once

#include "panelayout.h"
#include "visualitem.h"
#include "valueaxis.h"
#include "ratioaxis.h"
#include "mainview.h"
#include "title.h"

namespace sia {

namespace chartbase {
class TaParam;
}

namespace formula {
class IFormula;
}

namespace charty {

// class ValueAxis;
// class RatioAxis;
// class Title;
// class MainView;
class CalendarAxis;
class Series;

/*!
 * \class VisualItem
 *
 * \brief 图表内所有可见的区域元素都要继承于此类
 *
 */
class Panel : public VisualItem {

    struct _Formula {
        // 在哪个view中显示的
        my_sp<MainView> view = nullptr;

        // 公式
        formula::IFormula* fml = nullptr;

        //! 指标名
        SkString formula_name;

        //! title名（Brief Param）
        SkString brief_name;

        //! 公式实例id
        int32_t id = 0;

        //! 输出到图表的数据序列名
        SkTArray<SkString> data_names;

        //! 输出到图表的序列名
        SkTArray<SkString> series_names;

        //! 公式参数
        formula::TaParam param;

        inline bool hasSeries(const SkString& series_name)const {
            for (auto& s : series_names) {
                if (series_name.equals(s)) {
                    return true;
                }
            }
            return false;
        }
    };

public:
    Panel(graphy::Widget* parent);
    ~Panel();

    virtual void recalc(Bits flags) override;

    inline MainView* mainView() {
        return mainview_.get();
    }

    /*! \brief 获取所有的视图（主和副图）
     *
     *    @param
     *    @return
     */
    void views(SkTArray<my_sp<MainView>>& out)const;

    /*! \brief 所有的view中（主和副图）所有的序列
     *
     *    @param
     *    @return
     */
    std::vector<my_sp<Series>> serieses()const;

    /*! \brief 在panel中，根据名字查找序列
     *
     *    @param    series_name 序列名
     *    @param    view [out] 所在view
     *    @return   序列指针
     */
    my_sp<Series> series(const SkString& series_name, my_sp<MainView>& view)const;

    /*! \brief 创建一个公式，并输出到图表
     *
     *    @param	ta_name	公式名
     *    @param	para	公式计算/输出参数
     *    @return	公式在图表内的ID：>0，创建公式成功。
     */
    int32_t showFormula(const SkString& ta_name, const formula::TaParam& para);

    /*! \brief 销毁一个公式
     *
     *    @param
     *    @return
     */
    void removeFormula(int32_t id);

    /*! \brief 显示或者隐藏控件
     *
     *    @param	show	true显示
     *    @return
     */
    template <typename _Item>
    void showPanelItem(my_sp<_Item>& item, PanelItemType typ, bool show) {
        flags_.set(typ, show);
        if (show) {
            if (!item) {
                item = std::make_shared<_Item>(this);
                initPanelItem(item.get());
            }
        } else {
            if (item) {
                // 从父亲控件中移除
                removeChild(item.get());
                item.reset();
            }
        }
    }

    inline bool hasItem(PanelItemType typ)const {
        return flags_.test(typ);
    }

    inline GridLine& grid() {
        return mainview_->grid();
    }

    inline Title* title() {
        return &title_;
    }

    /*! \brief 显示隐藏控件
     *
     *    @param    item    参见PanelItemType
     *    @return
     */
    void showItem(int item, bool show);

    void seriesRemoved(const SkString& series_name);

    inline const SkTArray<my_sp<MainView>>& subViews()const {
        return subviews_;
    }

    /*! \brief 将sub面板合并进来
     *
     *    @param
     *    @return
     */
    bool mergePanel(Panel* sub);

    /*! \brief 移除视图item
     *
     *    @param    item    可能是series或者object等
     *    @return
     */
    void removeViewitem(ViewItem* item);

    inline const std::unordered_map<int32_t, _Formula>& formulas()const {
        return formulas_;
    }

protected:
    /*! \brief 初始化控件
     *
     *    @param
     *    @return
     */
    void initItems();

    void initPanelItem(VisualItem* w);

    virtual bool onMouse(int x, int y, sia::InputState state, uint32_t modifiers) override;

    // 刷新公式
    void refreshFormulas();

    /*! \brief 创建公式输出
     *
     *    @param
     *    @return
     */
    bool createFormulaSeries(const SkString& formula_name, const int32_t id, _Formula& fml,
                             const SkTArray<formula::TaSeries>* p_series, const formula::TaParam& user_param);

    /*! \brief 根据序列名，查找指标实例
     *
     *    @param
     *    @return   -1 没有此指标
     */
    int32_t findFormulaBySeriesName(const SkString& series_name)const;

private:
    BitFlags flags_;

    // 控件
    PanelLayout layout_;

    //! 主图
    my_sp<MainView> mainview_ = nullptr;

    //! 副图
    SkTArray<my_sp<MainView>> subviews_;

    Title title_;// = nullptr;
    my_sp<ValueAxis> value_ = nullptr;
    my_sp<RatioAxis> ratio_ = nullptr;
    my_sp<CalendarAxis> calendar_ = nullptr;

    //! 图表中的公式
    std::unordered_map<int32_t, _Formula> formulas_;
};

#define ToPanel(hPanel) reinterpret_cast<Panel*>(hPanel)
#define HPanel(panel) reinterpret_cast<HPANEL>(panel)

}
}
