#include "chartctrl.h"

#include <QTimer>
#include <QDebug>
#include <QPainter>
#include <QPaintEvent>
#include "../../port/qt/imgconv.h"
#include "../../port/qt/event.h"

ChartCtrl::ChartCtrl(QWidget *parent) : QWidget(parent) {
    // 需要focus接受键盘输入
    setFocusPolicy(Qt::StrongFocus);

    ctx_ = Context::create();
    chart_ = new MyChart();
    ctx_->setScene(chart_);

    QTimer *tmr = new QTimer(this);
    tmr->start(50);
    tmr->connect(tmr, &QTimer::timeout, [&]() {
        // update
        // this->setUpdatesEnabled(false);
        this->repaint();
        // this->setUpdatesEnabled(true);
    });

    setMouseTracking(true);
}

ChartCtrl::~ChartCtrl() {
    Context::release(ctx_);
}

MyChart *ChartCtrl::chart() const {
    return chart_;
}

void ChartCtrl::paintEvent(QPaintEvent *evt) {
    static int _frame = 0;
    qDebug() << "paint" << _frame++;

    QPainter paint(this);
    Frame f;
    if (ctx_->doPaint(f)) {
#ifdef SIA_OS_WIN
        QPixmap bmp = port::QtWin::fromBITMAPINFO(f.get());
        if (!bmp.isNull()) {
            paint.drawPixmap(0, 0, bmp);
            evt->accept();
        }
#elif defined(SIA_OS_LINUX)
        QImage bmp = port::QtLinux::fromXImage(f.get());
        if (!bmp.isNull()) {
            paint.drawImage(0, 0, bmp);
            evt->accept();
        }
#endif
    }
}

void ChartCtrl::resizeEvent(QResizeEvent *evt) {
    QSize sz = evt->size();
    ctx_->setSize(sz.width(), sz.height());
}

void ChartCtrl::handleMouseEvent(QMouseEvent *evt) {
    QPoint pt = evt->pos();
    qint32 modis = port::transModifier(evt->modifiers());
    sia::InputState input_state = port::transMouseInput(evt->type());
    ctx_->onMouse(pt.x(), pt.y(), input_state, modis);
//    this->repaint();
}

void ChartCtrl::mouseMoveEvent(QMouseEvent *evt) {
    handleMouseEvent(evt);
}

void ChartCtrl::mousePressEvent(QMouseEvent *evt) {
    handleMouseEvent(evt);
}

void ChartCtrl::mouseReleaseEvent(QMouseEvent *evt) {
    handleMouseEvent(evt);
}

void ChartCtrl::mouseDoubleClickEvent(QMouseEvent *evt) {
    handleMouseEvent(evt);
}

void ChartCtrl::keyPressEvent(QKeyEvent *evt) {
//    bool onKey(sia::KeyBoard k, sia::InputState state, uint32_t modifiers);
    qint32 modis = port::transModifier(evt->modifiers());
    ctx_->onKey(port::transKey(evt->key()), sia::InputState::kDown_InputState, modis);
//    this->repaint();
}
