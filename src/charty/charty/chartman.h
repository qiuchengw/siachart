#pragma once

#include "deps/singleton.h"
#include "chart.h"
#include "dict.h"

namespace sia {
namespace formula {
class IFormula;
class FormulaMan;
}

namespace charty {

struct ChartManPrivate;
class Chart;

/*!
 * \class ChartMan
 *
 * \brief 图表实例管理器
 *
 */
class CHARTY_API ChartMan final: public Singleton<ChartMan> {
    friend class Singleton<ChartMan>;
    friend class Chart;
public:
    ChartMan();
    ~ChartMan();

    /*! \brief 根据ID查询图表的对象
     *
     *    @param	chart_id	图表ID
     *    @return	图表对象
     */
    Chart* chart(const SkString& chart_id) const;

    /*! \brief 对所有的图表实例广播事件
     *
     *    @param	evt	事件
     *    @return
     */
    void broadcast(const SkEvent& evt) const;

    /*! \brief 应用主题配置
     *
     *    @param	config_file	主题配置文件
     *    @param	error_msg [out]	错误信息
     *    @return	false： 加载失败，错误信息参见参数error
     */
    bool loadThemeConfig(const SkString& config_file, SkString& error_msg);

    /*! \brief 字典对象
     *
     *    @param
     *    @return
     */
    Dict* dict();

    /*! \brief 创建公式实例
     *
     *    @param	name	创建公式
     *    @return
     */
    formula::IFormula* createFormula(const SkString& name);

    /*! \brief 销毁公式实例
     *
     *    @param	inst	公式实例指针，由createFormula创建
     *    @return
     */
    void destroyFormula(formula::IFormula* inst);

    /*! \brief 公式管理器实例
     *
     *    @param
     *    @return
     */
    formula::FormulaMan* formulaMan();

protected:
    /*! \brief 注册新的图表实例
     *
     *    @param	c	图表对象实例
     *    @return
     */
    void regInst(Chart* c);

    /*! \brief 取消注册图表实例
     *
     *    @param	c	图表对象实例
     *    @return
     */
    void unregInst(Chart* c);

private:
    ChartManPrivate* d = nullptr;
};
}
}
