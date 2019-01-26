#include "qcolorsel.h"
#include <QMouseEvent>
#include <QPainter>
#include <QDebug>

QColorSel::QColorSel(QWidget* parent)
    :QFrame(parent) {
    color_ = Qt::red;
}

SkColor QColorSel::skcolor() const {
    return SkColorSetARGB(color_.alpha(), color_.red(), color_.green(), color_.blue());
}

void QColorSel::setColor(QColor c) {
    color_ = c;
}

void QColorSel::mouseReleaseEvent(QMouseEvent *evt) {
    dlg_.open(this, SLOT(onColorSelected(QColor)));
}

void QColorSel::paintEvent(QPaintEvent *evt) {
    QPainter pt(this);
    pt.fillRect(rect(), color());
}

void QColorSel::onColorSelected(const QColor &color) {
    color_ = color;

    emit colorSelected(color);
}
