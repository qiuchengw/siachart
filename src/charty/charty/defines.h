#pragma once

#include "chartapi.h"
#include "bitflag.h"
#include "../base/datasource.h"
#include "../formula/taparam.h"

//! panel 句柄handle
typedef struct __charty_panel {}*HPANEL;

namespace sia {
namespace charty {

using chart_key_t = chartbase::data_key_t;
using chart_val_t = chartbase::data_val_t;
using SeriesType = chartbase::SeriesType;

/*! \brief 将chart_key_t作为时间解析
 *
 *    @param	d	字面形式的时间值，如：（20181008162430435 = 2018-10-08 16:24:30.435）
 *    @param	dt	解析后的数据
 *    @return
 */
CHARTY_API void siaParseChartKey(chart_key_t d, std::tm& dt);

/*! \brief 生成chart_key_t
 *
 *    @param	日期/时间
 *    @return	chart_key_t
 */
CHARTY_API chart_key_t siaMakeChartKey(const std::tm& dt);

/*!
 * \enum VisualItemZValue
 *      图层等的Z轴顺序规定
 */
enum CHARTY_API VisualItemZValue : int16_t {
    kPanel = 1,
    kBackground,
    kTitle,
    kAxis,
    kBarSeries,
    kUpDownSeries,
    kCandleSeries,
    //! line 序列的显示在其他序列之上
    kLineSeries,
    kSideBoard,

    // 嵌入对象
    kObjects = 100,

    //! 用户绘图
    kTrendObjects = 300,

    // tooltip
    kTooltip,

    // 十字线
    kCrossHair,

    // 框选
    kRubberband,

    // 显示图形详细信息的视图
    kCrossHairDetail,
};

/*!
 * \enum ObjectType
 *  自定义对象（ObjectItem）类型
 */
enum class CHARTY_API ObjectType : uint8_t {
    kNotSet = 0,

    //! 水平线
    kHLine = 1,

    //! 文字
    kText = 2,

    //! 图像
    kPicture = 3,

    // 动态图片
    kMovie = 4,

    //! 图文混排
    kRichText = 5,

    //! 用户自定义，需要用户提供绘图函数
    Custom = 0xFF,
};

/*!
 * \enum ObjectPosition
 *
 *   自定义组件的位置
 */
enum class CHARTY_API PosType {
    kNotSet = 0,

    //! 在图表MainView中的相对坐标位置（只支持相对于父容器：MainView）。
    // 参考CSS的position:relate
    kRelated,

    //! 依赖指定的MainView中的子组件（Series/Object等）
    kAttached,

    //! 在MainView中，根据Y值计算位置
    kValued,
};

/*!
 * \enum PanelItemType
 *  面板区域
 */
enum /*class*/ CHARTY_API PanelItemType {
    //! value
    kPanelValueAxis = 0x1,

    //! ratio
    kPanelRatioAxis = 0x2,

    //! 日历（key）
    kPanelCalendar = 0x4,

    //! 标题
    kPanelTitle = 0x8,

    //! all
    kPanelAll = 0xFFFF,
};

/*!
 * \struct SeriesStyle
 *
 * \brief 序列的风格显示
 *
 */
struct CHARTY_API SeriesStyle {
    SeriesStyle() = default;

    // line
    SeriesStyle(SkColor c)
        :color(c) {
    }

    // line
    SeriesStyle(SkScalar w_stroke, SkColor c)
        :color(c), stroke_width(w_stroke) {
    }

    // bar
    SeriesStyle(bool fill, SkColor c, SkScalar w_bar = SkIntToScalar(0))
        :color(c), fill_bar(fill), bar_width(w_bar) {
    }

    // candle
    SeriesStyle(SkColor up, SkColor down, bool fill = false)
        :up_color(up), down_color(down), fill_bar(fill) {
    }

    //! 反锯齿
    bool antialias = false;

    //! line style
    SkScalar stroke_width = SkIntToScalar(1);
//     SkPaint::Cap line_cap = SkPaint::kDefault_Cap;
//     SkPaint::Join line_join = SkPaint::kDefault_Join;

    //! bar/candle style
    bool fill_bar = false;

    //! bar的宽度。 0 : 由图表智能决定。
    SkScalar bar_width = SkIntToScalar(0);

    //! 正常的颜色
    SkColor color = SK_ColorCYAN;

    //! candle/updown/axis series color
    SkColor up_color = SK_ColorRED;
    SkColor down_color = SK_ColorGREEN;
};
extern CHARTY_API const SeriesStyle& siaSeriesStyle();

/*!
 * \class ChartFormater
 *
 * \brief 提供数据的格式化功能
 *
 */
struct CHARTY_API ChartFormater {
public:
    /*! \brief 按照ChartPanel设置的精度和放大倍数来格式化数据
     *
     *    @param	val		格式化数据
     *    @return	格式化化后的数据字符串形式。
     */
    SkString formatVal(chart_val_t val) const;

    /*! \brief 格式化时间
     *
     *    @param	key		时间值
     *    @param	format	时间格式，遵循std::strftime格式。如果为空，则使用预设的格式
     *    @return
     */
    SkString formatKey(chart_key_t key, const SkString& format = SkString("")) const;

    /*! \brief 设置所在的panel句柄
     *
     *    @param	panel	面板
     *    @return
     */
    void setPanel(HPANEL panel);

private:
    HPANEL panel_ = nullptr;
};

/*!
 * \class SeriesTooltipData
 *
 * \brief 序列tooltip的数据。
 *	当鼠标移动到序列上的时候，将会弹出一个tooltip展示鼠标指示位置处的序列详细信息。
 *
 */
struct CHARTY_API SeriesTooltipData : public ChartFormater {
    //! 序列名字
    SkString series_name;

    //! 序列类型
    SeriesType typ;

    //! 鼠标所在位置对应的key
    chart_key_t key;

    //! <data_name, data_val>
    std::map<SkString, chart_val_t> datas;

    // ----------------
    //! 输出title
    SkString title;

    //! 显示到Tooltip上的项目
    struct Item {
        SkString cont;
        SkColor color;
    };
    SkTArray<Item> items;
};

/*! \brief 序列的tooltip提示：
 *
 *    @param	[in|out] SeriesTooltipData
 *    @return	返回false，则不显示tooltip。
 */
typedef std::function<bool(SeriesTooltipData&)> SeriesTooltipCallback;

enum class CHARTY_API PanelTitleItemType {
    //! 使用字面常量
    kConstant,

    // 使用图表symbol
    kChartSymbol,

    //! 使用序列名
    kSeriesName,
};

/*!
 * \class PanelTitleItem
 *
 * \brief 面板Title的item定制显示。
 *	默认情况下，title显示当前面板上的所有序列名。且不响应十字线的移动变化
 *
 */
struct CHARTY_API PanelTitleItem {
    //! 显示名，具体含义需要使用PanelTitleItemType解析
    SkString item;

    //! 类型
    PanelTitleItemType typ = PanelTitleItemType::kConstant;

    //! 如果是Sk_InvalidColor，使用默认的颜色，否则使用提供的颜色
    SkColor color = Sk_InvalidColor;
};

namespace ChartEventType {
//! 数据变化
static const char* kDataChanged = u8"sia::charty::DataChanged";

// 显示范围变化
static const char* kDomainChanged = u8"sia::charty::DomainChanged";

// viewitem选中状态变化
// data：setString("item", (ViewItem*))
static const char* kViewItemSeletChanged = u8"sia::charty::ViewItemSelected";

// viewitem删除
// data：setString("item", (ViewItem*))
static const char* kViewItemDeleted = u8"sia::charty::ViewItemDeleted";

// 全局序列风格变化
static const char* kSiaSeriesStyleChanged = u8"sia::charty::SiaSeriesStyleChanged";

// 图表主题变化
static const char* kSiaThemeChanged = u8"sia::charty::SiaThemeChanged";

// 序列即将删除
// data：setString("name", $series_name)
static const char* kSeriesBeforeDelete = u8"sia::charty::SeriesBeforeDelete";

// 序列已删除
// data：setString("name", $series_name)
static const char* kSeriesDeleted = u8"sia::charty::SeriesDeleted";

};

}
}

