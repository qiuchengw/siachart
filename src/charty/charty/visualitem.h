#pragma once

#include "siapi.h"
#include "graphy/widget.h"
#include "defines.h"
#include "chartctx.h"
#include "dict.h"

namespace sia {
namespace graphy {
class PaintGroup;
}

namespace charty {

/*!
 * \class VisualItem
 *
 * \brief 图表内所有可见的区域元素都要继承于此类
 *
 */
class VisualItem : public graphy::Widget {
public:
    VisualItem(const SkString& name, graphy::Widget* parent);

    inline const chartbase::DataSource* datasource() const {
        if (ctx_) {
            return ctx_->datasource();
        }
        return nullptr;
    }

    /*! \brief 是否准备好，能够绘制自己的内容了
     *		会检查两个方面，一个是数据内容准备好了，一个是paint内容准备好了
     *
     *    @param
     *    @return	true	可以绘制了。
     */
    bool ready() const;

    void setChartContext(ChartContext* t);

    inline ChartContext* chartCtx()const {
        return ctx_;
    }

    Dict* dictionary()const;

    /*! \brief 重新计算数据相关的参数
     *
     */
    virtual void recalc(Bits flags) {

    }

    static SkScalar textVerticalMiddle(const SkPaint& paint, SkScalar height = 0);

    /*! \brief 检测字符串占用的矩形大小
     *		注意：字符串不会断行
     *    @param	str
     *    @return
     */
    static SkRect textRect(const SkString& str, const SkPaint& paint,
                           const SkPoint& left_top = SkPoint::Make(SkIntToScalar(0), SkIntToScalar(0)));

    /*! \brief 根据\n 断行绘制
     *      当前仅支持水平绘制
     *
     *    @param    text 如果没有 \n 换行符，则绘制到同一行内
     *    @param    rc 文本绘制到此矩形内    （目前还没有实现一行自动wrap的功能）
     *    @param    spacing 行间距，同时兼具rc的padding。需要细心设置。
     *    @return
     */
    static void drawMultilineText(SkCanvas* canvas, const SkPaint& paint,
                                  const SkString& text, const SkRect& rc, SkScalar spacing = 5);

protected:
    void forVisualChildren(std::function<bool(VisualItem*)> cb);

    /*! \brief 从painters中查找用户设置的paint，如果找不到，就自己创建一个
     *
     *    @param    name paint名字索引
     *    @return
     */
    SkPaint* declarePaint(const SkString& name, SkColor def_color = SK_ColorWHITE, bool antialias = false);

    /*! \brief 配置文件中要求的geometry
     *
     *    @param
     *    @return   false 如果外部没有配置
     */
    bool getConfigGeometry(SkRect& rc);

    /*! \brief 处理事件
     *
     *    @param
     *    @return
     */
    virtual bool onEvent(const SkEvent&) override;

    /*! \brief 准备paints
     *
     *    @param
     *    @return
     */
    virtual void preparePaints();

protected:
    my_sp<graphy::PaintGroup> painters_;

    void refreshPaints();

private:
    ChartContext* ctx_ = nullptr;

    //! 本地声明的变量
    std::unordered_map<SkString, my_sp<SkPaint>> paints_;

    //! 绘制paint是否已经准备好
    bool paint_ready_ = false;
};

}
}
