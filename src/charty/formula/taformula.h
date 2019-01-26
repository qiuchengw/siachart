#pragma once
#include "iformula.h"
#include "taitem.h"

namespace sia {
namespace chartbase {
class DataSource;
}
namespace formula {

/*!
 * \class TaFormula
 *
 * \brief 封装Talib的公式
 *
 */
class TaFormula : public IFormula {
public:
    TaFormula(const SkString& name);

    virtual bool calc(const TaParam& para) override;

    virtual bool queryParam(SkTArray<TaParamItem>& para) override;

    virtual bool defaultParam(TaParam& para) override;

    virtual bool suggestOutSeries(SkTArray<TaSeries>& series) override;

private:
    TaItem  ta_item_;
};
}
}
