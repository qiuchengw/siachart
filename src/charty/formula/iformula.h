#pragma once

#include "formulapi.h"
#include "tadata.h"
#include "taparam.h"

namespace sia {
namespace chartbase {
class DataSource;
}
namespace formula {

/*!
 * \class IFormula
 *
 * \brief 封装公式的基础接口类！
 *	用法：
 *		1，创建IFormula对象 m
 *		2，查询参数，看需要灌入什么数据：queryParam
 *		3，根据2查询灌入数据：m.data() = mydata;
 *			3.1 或者使用数据源的便捷方式：m.useDataSource
 *		4，传入参数并计算：m.calc(myparam);
 *		5，从公式中拿出数据并使用：m.data().out1();
 */
class FORMULA_API IFormula {
public:
    IFormula(const SkString& name);
    virtual ~IFormula();

    const TaData& data()const {
        return data_;
    }

    /*! \brief 获取数据结构
     *		可以通过此方法，向内部灌入数据。
     *
     *    @param
     *    @return
     */
    inline TaData& data() {
        return data_;
    }

    /*! \brief 使用此方法向data()中灌入必要的参数。
     *		此方法会自动从提供的数据源中寻找以下名称的数据序列：
     *			open/high/low/clow/oi/volume
     *
     *    @param	ds	数据源
     *    @param	idx_begin	数据源的数据所以开始
     *    @param	idx_end	数据源的数据索引结束
     *    @return
     */
    virtual bool useDataSource(const chartbase::DataSource *ds, int idx_begin, int idx_end);

    /*! \brief 重写此方法实现公式的计算
     *
     *    @param	para	公式计算所需参数
     *    @return
     */
    virtual bool calc(const TaParam& para) = 0;

    /*! \brief 查询参数
     *
     *    @param	para [out] 输出参数
     *    @return
     */
    virtual bool queryParam(SkTArray<TaParamItem>& para);

    /*! \brief 获取默认的参数
     *
     *    @param	para	[out]	输出的默认参数
     *    @return	true	查询成功
     */
    virtual bool defaultParam(TaParam& para);

    /*! \brief 查询建议的图表输出。
     *
     *    @param	series [out]
     *    @return
     */
    virtual bool suggestOutSeries(SkTArray<TaSeries>& series);

    inline TaParam& param() {
        return para_;
    }

    const SkString& name()const {
        return name_;
    }

protected:
    //! in
    TaParam para_;

    //! 公式名
    SkString name_;

    //! 公式数据，包括输入和输出
    TaData data_;
};
}
}
