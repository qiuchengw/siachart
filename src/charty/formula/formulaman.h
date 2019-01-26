#pragma once

#include "deps/factory.h"
#include "formulapi.h"
#include "tadata.h"
#include "taparam.h"
#include "iformula.h"

namespace sia {
namespace formula {

struct FormulaManPrivate;

/*!
 * \class FormulaMan
 *
 * \brief 管理系统中已注册的公式
 *
 */
class FORMULA_API FormulaMan {
public:
    FormulaMan();
    ~FormulaMan();

    /*! \brief 注册公式
     *
     *    @param	group	公式分组
     *    @param	name	公式名，公式名是全局唯一，不同的分组下名字也不能相同！
     *    @return
     */
    template<typename _Typ>
    void regFormula(const SkString& group, const SkString& name) {
        ff_.add<_Typ>(name);
        _regFormula(group, name);
    }

    /*! \brief 使用公式
     *
     *    @param
     *    @return
     */
    IFormula* create(const SkString& name);

    /*! \brief 销毁已创建的实例
     *
     *    @param	name	类型名
     *    @param	indi	实例
     *    @return
     */
    void destroy(const SkString& name, IFormula* indi);

    /*! \brief 获取公式分组
     *
     *    @param	group	[out]
     *    @return
     */
    void groups(SkTArray<SkString>& group)const;

    /*! \brief 分组公式
     *
     *    @param	group
     *    @param	method [out]
     *    @return
     */
    void methods(const SkString& group, SkTArray<SkString>& method)const;

protected:
    void _regFormula(const SkString& group, const SkString& name);

private:
    FFactory<IFormula> ff_;

    //! 私有数据
    FormulaManPrivate* d = nullptr;
};
}
}
