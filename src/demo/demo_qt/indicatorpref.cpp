#include "indicatorpref.h"
#include "../../charty/charty/chartman.h"
#include <QLabel>
#include <QSpinBox>
#include <QDoubleSpinBox>

PrefItem::PrefItem(QWidget *p)
    :QWidget(p) {

    layout_ = new QHBoxLayout(this);
}

void PrefItem::setParam(const formula::TaParamItem &item) {
    item_ = item;

    QString txt = item.display_name.c_str();
    if (txt.isEmpty()) {
        txt = item.name.c_str();
    }
    if (!item.opt) {
        txt.append("(*)");
    }
    auto lbl = new QLabel(txt, this);

    QString tip = item.help_file.c_str();
    lbl->setToolTip(tip);
    layout_->addWidget(lbl, 0);

    switch (item.typ) {
    case formula::TaParamType::kReal: {
        auto box = new QDoubleSpinBox(this);
        box->setRange(-99999999999, 999999999999);
        box->setValue(item.default_val);
        w_input_ = box;
    }
    break;

    case formula::TaParamType::kInt: {
        auto box = new QSpinBox(this);
        box->setRange(-99999999, 999999999);
        box->setValue(int(item.default_val));
        w_input_ = box;
    }
    break;

    default:
        break;
    }

    if (w_input_) {
        w_input_->setToolTip(item.hint.c_str());
        layout_->addWidget(w_input_, 1);
    }
}

bool PrefItem::param(formula::TaParam& pa) {
    if (!w_input_) {
        return false;
    }

    switch (item_.typ) {
    case formula::TaParamType::kReal: {
        if (auto box = dynamic_cast<QDoubleSpinBox*>(w_input_)) {
            pa.setParam(item_.name, box->value());
            return true;
        }
    }
    break;

    case formula::TaParamType::kInt: {
        if (auto box = dynamic_cast<QSpinBox*>(w_input_)) {
            pa.setParam(item_.name, box->value());
            return true;
        }
    }
    break;
    }
    return false;
}

////////////////////////////////////////////////////////////////

IndicatorPref::IndicatorPref(QWidget *parent) : QWidget(parent) {
    layout_ = new QVBoxLayout(this);

    man_ = charty::ChartMan::instance()->formulaMan();
}

void IndicatorPref::setIndicator(const QString &method) {
    method_ = method;

    SkString str(method.toUtf8().data());
    if (auto indi = man_->create(str)) {
        SkTArray<formula::TaParamItem> paras;
        if (indi->queryParam(paras)) {
            setParam(paras);
        }
    }
}

bool IndicatorPref::getParam(formula::TaParam& pa) {
    for (PrefItem* w : items_) {
        if (!w->param(pa)) {
            return false;
        }
    }
    return true;
}

void IndicatorPref::setParam(const SkTArray<formula::TaParamItem> &paras) {
    // 当前所有的控件都删掉
    for (PrefItem* w : items_) {
        delete w;
    }
    items_.clear();

    // 创建控件
    for (const formula::TaParamItem& item : paras) {
        if (!item.opt) {
            continue;
        }

        PrefItem * w = new PrefItem(this);
        items_.push_back(w);
        w->setParam(item);
        layout_->addWidget(w);
    }
    layout_->addStretch(100);
}

