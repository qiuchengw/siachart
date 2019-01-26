#pragma once

#include "siapi.h"
#include "defines.h"
#include "chart.h"

namespace sia {
namespace charty {

/*!
 * \class CrosshairDetail
 *
 * \brief 十字线详情面板内容生成。
 *	继承此类，重写detailItems方法，返回内容列表（DetailItem）
 */
class CHARTY_API CrosshairDetail : public ChartFormater {
    friend class CrossHair;
public:
    struct DetailItem {
        DetailItem() = default;
        DetailItem(const SkString& stitle, const SkString& sval, SkColor color_val = SK_ColorRED)
            :title(stitle), val(sval), val_color(color_val) {
        }

        SkColor val_color = SK_ColorRED;
        SkString title;
        SkString val;
    };

public:
    /*! \brief 十字线detail面板上显示的项目信息
     *
     *    @param	data	当前十字线所在index索引的数据
     *    @return	detail面板上需要显示的项目
     */
    virtual std::vector<DetailItem> detailItems(chartbase::data_key_t k, const std::map<SkString, chartbase::data_val_t>& data) = 0;
};

}
}
