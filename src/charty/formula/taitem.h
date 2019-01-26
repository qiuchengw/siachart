#pragma once

#include "tadata.h"
#include "taparam.h"

namespace sia {
namespace formula {

/*!
 * \class TaItem
 *
 * \brief Talib 的公式项
 *	1，init
 *	2，queryParam
 *	3，calc
 */
class TaItem {
public:
    TaItem(const SkString& name = SkString(""));

    ~TaItem();

    void setName(const SkString& name);

    bool init();

    /*! \brief 查询参数
     *
     *    @param	para [out]
     *    @return
     */
    bool queryParam(SkTArray<TaParamItem>& para);

    /*! \brief 获取默认的参数
     *
     *    @param	para	[out]	输出的默认参数
     *    @return	true	查询成功
     */
    bool defaultParam(TaParam& para);

    /*! \brief 建议的图表序列输出
     *
     *    @param	series [out]
     *    @return
     */
    bool suggestOutSeries(SkTArray<TaSeries>& series);

    bool calc(const TaParam& para);

    void setData(TaData* d);

    bool valid() const;

protected:
    bool initFunc();

    bool initInputParam();

    bool initOutParam();

    bool evalCall();

private:
    //! 可选输入参数 optional input
    TaParam opt_in_;

    //! 数据源
    TaData* d_ = nullptr;

    //! TA 函数名
    SkString ta_name_;

    //! 参数
    TA_ParamHolder *param_holder_ = nullptr;

    //! TA 函数信息
    const TA_FuncInfo *func_info_ = nullptr;
};
}
}
