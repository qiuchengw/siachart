#ifndef QCOLORSEL_H
#define QCOLORSEL_H

#include <QObject>
#include <QFrame>
#include <QColorDialog>
#include "SkColor.h"

class QColorSel : public QFrame {
    Q_OBJECT

public:
    QColorSel(QWidget* parent);

    QColor color()const {
        return color_;
    }

    SkColor skcolor()const;

    void setColor(QColor c);

protected:
    virtual void mouseReleaseEvent(QMouseEvent * evt) override;

    virtual void paintEvent(QPaintEvent *evt) override;

signals:
    void colorSelected(const QColor& color);

protected slots:
    void onColorSelected(const QColor& color);

private:
    QColor color_;
    QColorDialog dlg_;
};

#endif // QCOLORSEL_H
