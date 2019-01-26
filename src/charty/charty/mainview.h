#pragma once

#include "stacklayout.h"
#include "panelitem.h"
#include "valueslice.h"
#include "viewitemtooltip.h"
#include "viewobject.h"
#include "precision.h"
#include "gridline.h"

namespace sia {
namespace charty {
class Series;
class ViewItem;

/*!
 * \class MainView
 *
 * \brief 用于图表Panel::Series的背景显示和主要数据计算
 *
 */
class MainView : public PanelItem {
public:
    enum Flags {
        //! 有中间值的时候要将界面分为上下等分的两部分
        kMiddleVal = 0x2,

        //! 是否是副图
        kSubView = 0x4,
    };

    MainView(Panel* parent = nullptr);
    ~MainView();

    inline const SkTArray<SkScalar>& indexXPos()const {
        return xpos_;
    }

    /*! \brief 设置是否作为副图
     *	    副图没有网格线和背景！
     *    @param
     *    @return
     */
    void setAsSubview(bool on);

    my_sp<Series> addSeries(const SkString& name, SeriesType typ, const SkTArray<SkString>& datas);
    bool removeSeries(const SkString& name);

    bool addObject(const SkString& name, const ObjectParam&para, my_sp<IVisualObject> obj);
    bool removeObject(const SkString& name);

    my_sp<Series> series(const SkString& name)const;
    std::vector<my_sp<Series>> serieses()const;

    inline chart_val_t minY()const {
        return vslice_.minY();
    }

    inline chart_val_t maxY()const {
        return vslice_.maxY();
    }

    virtual void recalc(Bits flags) override;

    void setValueSlice(int count);

    /*! \brief 设置序列风格
     *
     *    @param	styles
     *    @return
     */
    void setSeriesColor(const std::map<SkString, SkColor>& colors);
    void setSeriesStyle(const std::map<SkString, SeriesStyle>& styles);

    /*! \brief 设置序列的提示内容回调
     *
     *    @param	callbacks <series_name, callback> 回调
     *    @return
     */
    void setSeriesTooltipCallback(std::map<SkString, SeriesTooltipCallback> callbacks);

    const ValueSlice& valSlice()const {
        return vslice_;
    }

    void setMiddleVal(chart_val_t val);

    // 显示最大最小值的提示
    void showSeriesMinMaxTip(const SkString&series_name, bool show);

    /*! \brief 每个序列都可以要求显示一个提示框
     *
     *    @param
     *    @return
     */
    inline ViewItemTooltip* seriesTip() {
        return &tip_;
    }

    ViewItem* viewItem(const SkString& name)const;

    /*! \brief 整个视图有效宽度(contentWidth)被等分（等分为ChartContext::curIndexCount）
     *	    每一份占用的宽度
     *    @param
     *    @return
     */
    SkScalar indexWidth()const;

    Precision& precision() {
        return precision_;
    }

    GridLine& grid() {
        return grid_;
    }

    //////////////////////////////////////////////////////////////////////////
    // 坐标转换

    /*! \brief 根据x坐标位置，计算出其对应的数据索引位置
     *
     *    @param
     *    @return   序列索引（已加上ChartContext::indexBegin偏移量）
     */
    int dataIndexOfPos(SkScalar x)const;

    /*! \brief 根据x坐标位置，计算出其对应的数据索引位置，不带ChartContext::indexBegin偏移
     *
     *    @param
     *    @return 序列索引（不带ChartContext::indexBegin偏移量）
     */
    int visualIndexOfPos(SkScalar x)const;

    /*! \brief 检查key是否在当前的view显示区内
     *	    因为我们的key都是排序的（从小到大），所以，只需要检测
     *      key是否大于indexBegin的value值，且小于indexEnd的value值即可
     *
     *    @return   如果没有在显示区内，返回-1
     */
    int visualIndexOfKey(chart_key_t key) const;

    /*! \brief 根据y坐标，计算出其对应的value值
     *		我们数值是从上到下减小，但是坐标是从上到下增大的！
     */
    double valueOfYPos(SkScalar y) const;

    /*! \brief 值在panel中的y坐标位置
     *
     *    @param    v
     *    @return
     */
    SkScalar yPosOfValue(chart_val_t v) const;

    /*! \brief 计算dataname的idx索引处的坐标
     *
     */
    SkPoint posOfDataname(int idx, const SkString& dataname)const;

    /*! \brief 根据idx计算其对应的x坐标
     *
     *    @param    visual_idx （减掉DataSource::indexBegin的偏移）
     *    @return   x坐标，
     *				< 0 :	参数index不在当前的显示返回，返回小于0.
     *				>= 0:	正确返回
     */
    SkScalar xPosOfVisualIndex(int visual_idx) const;

    /*! \brief 计算key所在的x坐标
     *		如果key不在当前显示的返回，
     *    @param key
     *    @return 参考xPosOfVisualIndex返回值
     */
    SkScalar xPosOfKey(chart_key_t key) const;

    /*! \brief 数据序列对应的时间值
     *
     *    @param    idx 已加上ChartContext::indexBegin的偏移量
     *    @return
     */
    chart_key_t datetimeOfIndex(int idx) const;

    /*! \brief 视图是不是没有任何可视化的项目了？
     *
     *    @param
     *    @return
     */
    bool viewEmpty()const;

public:
    virtual bool onMouse(int x, int y, sia::InputState state, uint32_t modifiers) override;

protected:
    virtual void onDraw(SkCanvas* canvas) override;

    virtual void onSizeChange() override;

    virtual void preparePaints() override;

    void calcYRange();
    void calcXPoss();

    std::set<SkString> datanames() const;

private:
    StackLayout stacklay_;

    BitFlags flags_;

    //! 序列index的x坐标
    SkTArray<SkScalar> xpos_;

    // 序列
    std::unordered_map<SkString, my_sp<Series>> series_;

    // objects
    std::unordered_map<SkString, my_sp<ObjectItem>> objects_;

    //! 将value划为等分的几个档位，每个档位上对应value/ratio
    // 并根据此值显示横线
    ValueSlice vslice_;

    // paints
    SkPaint* bk_ = nullptr;
    SkPaint middle_grid_;

    //! 提示框
    ViewItemTooltip tip_;

    // 显示精度
    Precision precision_;

    //! 网格线
    GridLine grid_;
};
}
}

