#ifndef CHARTCTRL_H
#define CHARTCTRL_H

#include <QWidget>
#include "../../mode/rastery/contex.h"
#include "mychart.h"

#ifdef SIA_OS_LINUX
#undef Bool
#undef Status
#undef None
#undef CursorShape
#endif

using namespace sia;
using namespace sia::rastery;
using namespace sia::charty;

class ChartCtrl : public QWidget {
    Q_OBJECT
public:
    explicit ChartCtrl(QWidget *parent = nullptr);

    ~ChartCtrl();

    MyChart* chart()const;

signals:

public slots:

protected:
    virtual void paintEvent(QPaintEvent *evt) override;
    virtual void resizeEvent(QResizeEvent *evt) override;

    virtual void mouseMoveEvent(QMouseEvent *evt) override;
    virtual void mousePressEvent (QMouseEvent *evt) override;
    virtual void mouseReleaseEvent(QMouseEvent *evt) override;
    virtual void mouseDoubleClickEvent(QMouseEvent *evt) override;

    virtual void keyPressEvent(QKeyEvent* evt) override;

    void handleMouseEvent(QMouseEvent* evt);

private:
    Context* ctx_ = nullptr;
    MyChart* chart_ = nullptr;
};

#endif // CHARTCTRL_H
