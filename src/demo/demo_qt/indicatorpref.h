#ifndef INDICATORPREF_H
#define INDICATORPREF_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "../../charty/formula/formulaman.h"

using namespace sia;

class PrefItem : public QWidget {
    Q_OBJECT

public:
    PrefItem(QWidget* p);

    void setParam(const formula::TaParamItem& item);

    bool param(formula::TaParam &pa);

protected:

private:
    formula::TaParamItem item_;
    QHBoxLayout* layout_;
    QWidget* w_input_ = nullptr;
};

class IndicatorPref : public QWidget {
    Q_OBJECT
public:
    explicit IndicatorPref(QWidget *parent = nullptr);

    void setIndicator(const QString& method);

    SkString taName()const {
        return SkString(method_.toUtf8().data());
    }

    bool getParam(formula::TaParam &pa);

protected:
    void setParam(const SkTArray<formula::TaParamItem>& paras);

signals:

public slots:

private:
    formula::FormulaMan* man_ = nullptr;
    QString method_;
    QVBoxLayout* layout_;
    QList<PrefItem*> items_;
};

#endif // INDICATORPREF_H
