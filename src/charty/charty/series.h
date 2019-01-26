#pragma once
#include "visualitem.h"
#include "sensorwidget.h"
#include "viewitemtooltip.h"
#include "viewitem.h"

namespace sia {
namespace charty {

class MainView;

/*!
 * \class Series
 *
 * \brief 序列用于展示图表的数据内容
 *  序列具有两个名字：widget_type_name 和 series_name
 *  widget_type_name：指代序列的类型，如lineSeries/CandleSeries等固定名
 *  series_name：指代用户给予序列的名字，比如test，myline等，任意名
 */
class Series : public ViewItem {
    enum SeriesFlag {
        //! 是否使用的定制的外观？通过调用setStyle设置的外观，就是自定制外观
        kUseCustomStyle = 1 << (ViewItemFlag::kMaxFlagShift + 1),

        //! 是否显示max/min的提示
        kShowMinMaxTip = 1 << (ViewItemFlag::kMaxFlagShift + 2),

        //! 在删除的时候，是否同步清空其使用的数据序列？
        kRemoveDatasourceOnDelete = 1 << (ViewItemFlag::kMaxFlagShift + 3),
    };

public:
    Series(const SkString& widget_name, const SkString& series_name, SeriesType typ);

    virtual ~Series();

    inline SkString seriesName()const {
        return series_name_;
    }

    /*! \brief 根据类型创建一个实例
     *
     *    @return   实例指针，调用者负责释放
     */
    static my_sp<Series> create(const SkString& series_name, SeriesType typ);

    /*! \brief 每个序列至少需要一个类型的数据来显示
     *
     *    @param datas 数据名称序列
     *    @return
     */
    bool setDataNames(const SkTArray<SkString>& datas);

    /*! \brief 设置默认的数据名，在调用valAt的时候，取此数据列的值
     *
     *    @param	dataname 数据名
     *    @return
     */
    void setDefaultDataName(const SkString& dataname);

    /*! \brief 在序列销毁的时候，同时删除关联的数据源数据
     *		!!!WARNING: 请小心使用！如果有多个序列同时关联了这个数据名，则会导致其他的序列显示错误！
     *
     *    @param on 是否开启标志
     *    @return
     */
    void setRemoveDataOnDelete(bool on);

    /*! \brief 是否需要删除数据源？
     *
     *    @param
     *    @return
     */
    bool isRemoveDatasource()const;

    /*! \brief 计算序列中的每个数据所在的点位置
     *
     *    @param
     *    @return
     */
    bool calcPoints();

    /*! \brief 计算在x轴上，一个序列点最大的可视化宽度
     *          比如bar/candle的可视宽度（在图表上眼睛看到的宽度）
     *
     *    @param    desire_width    期望宽度，函数会根据此值计算出一个合理的宽度
     *                              不会最终结果小于0，也不会大于整个合理的宽度
     *    @return
     */
    SkScalar barWidth(SkScalar desire_width = SkIntToScalar(0));

    std::set<SkString> datanames() const;

    void setColor(SkColor c);

    void setStyle(const SeriesStyle& ss);

    inline SkColor color()const {
        return style().color;
    }

    inline bool isValidPoint(const SkPoint& pt)const {
        return (pt.x() != chartbase::kInvalidCoord
                && pt.y() != chartbase::kInvalidCoord);
    }

    const SeriesStyle& style()const;

    /*! \brief 设置tooltip显示内容的回调
     *
     *    @param	cb	回调函数。如果为nullptr，则会设置一个默认的
     *    @return
     */
    void setTooltipCallback(SeriesTooltipCallback cb = nullptr);

    /*! \brief 显示最大最小值提示
     *
     *    @param	show
     *    @return
     */
    void showMinMaxTip(bool show);

    /*! \brief 序列在key处的值。
     *
     *    @param
     *    @return
     */
    chart_val_t valAt(int visual_idx) const;

protected:
    virtual void onSensed(int idx, const SkPoint& pt) override;

    // 序列需要的数据名
    struct _DataPos {
        SkString dataname;

        SkTArray<chart_key_t> keys;
        SkTArray<chart_val_t> vals;
        SkTArray<SkPoint> points;

        inline bool isValid()const {
            return !dataname.isEmpty();
        }

        inline int itemCount()const {
            return keys.count();
        }
    };
    const _DataPos& dataPosItem(const SkString& dataname) const;

    virtual void onDraw(SkCanvas* canvas) override;

    virtual void onStyleChanged();

protected:
    // 序列名，和widget name是不同的
    SkString series_name_;

    //! 默认取值的数据名(data_name)
    SkString default_dataname_;

    // 序列类型
    SeriesType typ_ = SeriesType::kBlank;

    // 序列数据
    SkTArray<_DataPos> datapos_;

    //! 外观定制
    SeriesStyle style_;

    // tooltip
    SeriesTooltipCallback tooltip_cb_ = nullptr;

    //! 最大最小值的位置
    struct _DataItem {
        chart_key_t key;
        chart_val_t val;
        SkPoint point;

        void set(chart_key_t k, chart_val_t v, const SkPoint& p) {
            key = k;
            val = v;
            point = p;
        }
    };
    _DataItem min_item_;
    _DataItem max_item_;
};

}
}
