#include "crosshair.h"
#include "panel.h"
#include "crosshairdetail.h"
#include "seriesname.h"
#include "SkSurface.h"

namespace sia {

namespace charty {

CrossHair::CrossHair(/*Chart* parent*/)
    :VisualItem(SkString("crosshair"), /*(Widget*)parent*/ nullptr) {
    setZIndex(VisualItemZValue::kCrossHair);
    setVisibleP(true);
}

void CrossHair::onDraw(SkCanvas* canvas) {
    if (!canvas) {
        return;
    }

    if (flags_.test(Flags::kHasRubber)) {
        if (mouse_state_.dragging()) {
            SkRect rc = SkRect::MakeLTRB(pt_lt_.x(), pt_lt_.y(), pt_cur_.x(), pt_cur_.y());
            canvas->drawRect(rc, *rubber_);
        }
    }

    if (flags_.test(Flags::kHasCross) && !mouse_state_.dragging()) {
        // 画十字线
        SkString key, val;
        int data_idx = 0;
        if (curKeyVal(data_idx, key, val)) {
            // 每个panel-title的绘制
            chartCtx()->foreachPanel([=](HPANEL hp)->bool {
                if (Panel* p = ToPanel(hp)) {
                    if (Title *tt = p->title()) {
                        if (tt->hasDynamicItem()) {
                            tt->draw(canvas, p->clientToWindow(tt->pos()));
                        }
                    }
                }
                return true;
            });

            // 画水平线 + tag
            static const SkScalar TAG_WIDTH = 60, TAG_HEIGHT = 20;
            canvas->drawLine(0, pt_cur_.y(), width(), pt_cur_.y(), *line_); //
            SkRect rc = SkRect::MakeXYWH(width() - TAG_WIDTH, pt_cur_.y() - TAG_HEIGHT, TAG_WIDTH, TAG_HEIGHT);
            drawTag(canvas, rc, val);

            // 画垂直线 + tag
            canvas->drawLine(pt_cur_.x(), 0, pt_cur_.x(), height(), *line_);
            rc = SkRect::MakeXYWH(pt_cur_.x() - TAG_WIDTH, height() - TAG_HEIGHT, TAG_WIDTH, TAG_HEIGHT);
            drawTag(canvas, rc, key);

            // 画detail
            drawLineDetail(canvas, data_idx);
        }
    }
}

void CrossHair::preparePaints() {
    // 十字线
    line_ = declarePaint(SkString("line"), SK_ColorWHITE);
    tag_back_ = declarePaint(SkString("tag_background"), SK_ColorCYAN);
    tag_text_ = declarePaint(SkString("tag_text"), SK_ColorRED);
    tag_border_ = declarePaint(SkString("tag_border"), SK_ColorCYAN);
    tag_border_->setStyle(SkPaint::kStroke_Style);

    // 十字线的detail显示
    detail_border_ = declarePaint(SkString("detail_border"), SK_ColorWHITE);
    detail_border_->setStyle(SkPaint::kStroke_Style);

    detail_background_ = declarePaint(SkString("detail_background"), SK_ColorBLACK);
    detail_title_ = declarePaint(SkString("detail_title"), SK_ColorWHITE);
    detail_title_->setAntiAlias(true);
    detail_value_ = declarePaint(SkString("detail_value"), SK_ColorCYAN);
    detail_value_->setAntiAlias(true);

    // 框选
    rubber_ = declarePaint(SkString("rubber"), SK_ColorCYAN);

    // detail panel 宽度
    SkRect rc;
    if (getConfigGeometry(rc)) {
        detail_size_.set(rc.width(), rc.height());
    } else {
        detail_size_.set(SkIntToScalar(0), SkIntToScalar(0));
    }
}

void CrossHair::doRubberSelect() {
    // 获取当前选区的对应序列索引
    if (!cur_panel_) {
        return;
    }

    MainView* view = cur_panel_->mainView();
    const SkScalar MAX_X = view->width();
    // 获取对应的序列
    // 先转换到panel的坐标
    SkScalar x_1 = view->windowToClient(pt_lt_).x();
    SkScalar x_2 = view->windowToClient(pt_cur_).x();
    if ((x_1 < 0 && x_2 < 0)
            || (x_1 > MAX_X && x_2 > MAX_X)) {
        return;
    }

    // corret
    x_1 = std::min(MAX_X, std::max(x_1, SkIntToScalar(0)));
    x_2 = std::min(MAX_X, std::max(x_2, SkIntToScalar(0)));

    int idx1 = view->dataIndexOfPos(x_1);
    int idx2 = view->dataIndexOfPos(x_2);
    if (idx1 > idx2) {
        std::swap(idx1, idx2);
    }

    // 如果缩放级别已经太高的话，就不要再放大了！
    if (idx2 - idx1 >= 3) {
        chartCtx()->setCurrentDomain(idx1, idx2);
    }
}

void CrossHair::drawTag(SkCanvas* canvas, const SkRect& rc, const SkString& str) const {
    SkRect rc2 = rc;
    SkScalar w_str = tag_text_->measureText(str.c_str(), str.size()) + 10;
    if (rc.width() < w_str) {
        rc2.fLeft = rc.right() - w_str;
        if (rc2.fLeft < 0) {
            rc2.offsetTo(0, rc2.top());
        }
    }

    canvas->drawRect(rc2, *tag_back_);
    canvas->drawRect(rc2, *tag_border_);

    SkTextUtils::DrawString(canvas, str, rc2.left() + rc2.width() / 2,
                            rc2.top() + textVerticalMiddle(*tag_text_, rc2.height()), *tag_text_, SkTextUtils::kCenter_Align);
}

void CrossHair::drawLineDetail(SkCanvas* canvas, int idx) {
    if (!cross_detail_ || !canvas || !cur_panel_) {
        return;
    }

    auto dict = dictionary();
    // 需要绘制的
    cross_detail_->setPanel(HPanel(cur_panel_));

    std::map<SkString, chartbase::data_val_t> kvs;
    auto data = datasource()->get(idx);
    for (const SkString &dname : data->datanames()) {
        // 先不翻译，只是修正数据名
        SkString trans_dname = SeriesName::translateDataName(dname, nullptr);
        kvs.insert({trans_dname, data->get(dname)});
    }
    auto items = cross_detail_->detailItems(data->key(), kvs);

    const SkScalar padding = SkIntToScalar(5);
    const SkScalar wid = ((detail_size_.width() > 0) ? detail_size_.width() : SkIntToScalar(60));
    const SkScalar ITEM_HEIGHT = 15;
    const SkScalar hgt = ((detail_size_.height() > 0)
                          ? detail_size_.height()
                          : (items.size() * 2 * ITEM_HEIGHT + padding * 2));
    SkRect rc;
    if (pt_cur_.x() < wid) {
        // 放在右上角
        rc.setXYWH(width() - wid, 0, wid, hgt);
    } else {
        // 放在左上角
        rc.setXYWH(0, 0, wid, hgt);
    }

    // 画背景
    int sc = canvas->save();
    canvas->clipRect(rc);
    canvas->drawRect(rc, *detail_background_);
    canvas->drawRect(rc, *detail_border_);

    // detail 信息
    SkScalar y = rc.top() + padding + 10;
    for (const auto& hd : items) {
        canvas->drawString(dict->get(hd.title), rc.left() + padding, y, *detail_title_);
        y += ITEM_HEIGHT;

        SkPaint paint_val = *detail_value_;
        paint_val.setColor(hd.val_color);
        SkTextUtils::DrawString(canvas, hd.val, rc.width() - padding, y, paint_val, SkTextUtils::kRight_Align);
        y += ITEM_HEIGHT;
    }
    canvas->restoreToCount(sc);
}

bool CrossHair::onMouse(int x, int y, sia::InputState state, uint32_t modifiers) {
    mouse_state_.onMouse(x, y, state, modifiers);

    bool handled = false;
    switch (state) {
    case InputState::kDoubleClick_InputState:
        pt_lt_.set(SkIntToScalar(x), SkIntToScalar(y));
        pt_cur_ = pt_lt_;
        handled = true;
        break;

    case InputState::kDown_InputState:
        pt_lt_.set(SkIntToScalar(x), SkIntToScalar(y));
        pt_cur_ = pt_lt_;
        handled = true;
        break;

    case InputState::kMove_InputState:
        if (mouse_state_.dragging()) {
            // 如果不是拖动过程中的话，我们还是把消息传递给上层！让别人再处理
            handled = true;
        }
        pt_cur_.set(SkIntToScalar(x), SkIntToScalar(y));
        break;

    case InputState::kUp_InputState:
        pt_cur_.set(SkIntToScalar(x), SkIntToScalar(y));
        if (flags_.test(Flags::kHasRubber) && mouse_state_.dragged()) {
            handled = true;
            doRubberSelect();
        }

        if (!flags_.test(Flags::kHasCross)) {
            // 主动隐藏掉自己
            hide();
        }
        break;
    }
    return handled;
}

bool CrossHair::onKey(sia::KeyBoard key, sia::InputState state, uint32_t modifiers) {
    switch (key) {
    case KeyBoard::kLeft:
        move(-1);
        break;

    case KeyBoard::kRight:
        move(1);
        break;
    }

    if (modifiers & ModifierKey::kFirstPress_ModifierKey) {
        if (key == KeyBoard::kUp) {
            pt_cur_.set(pt_cur_.x(), pt_cur_.y() - 1);
        } else if (key == KeyBoard::kDown) {
            pt_cur_.set(pt_cur_.x(), pt_cur_.y() + 1);
        }
    }
    return true;
}

void CrossHair::setPanel(Panel* main_p, Panel* cur_p) {
    main_panel_ = main_p;
    cur_panel_ = cur_p;
}

bool CrossHair::curKeyVal(int& idx, SkString& key, SkString& val) {
    if (!cur_panel_) {
        return false;
    }

    MainView* view = cur_panel_->mainView();
    SkPoint pt_client = view->windowToClient(pt_cur_);
    if (!view->rect().contains(pt_client.x(), pt_client.y())) {
        // std::cout << "not in view!" << std::endl;
        return false;
    }

    // 获取对应的序列
    idx = view->dataIndexOfPos(pt_client.x());
    if (-1 == idx) {
        return false;
    }
//     std::cout << "current key val pos:" << pt_client.x() << "," << pt_client.y()
//               << " - index：" << idx << std::endl;

    if (auto ctx = chartCtx()) {
        ctx->setCurVisualIndex(idx - ctx->indexBegin());
        key = ctx->formatCalendar(view->datetimeOfIndex(idx), ChartContext::CalendarFormatType::kCalendarCrossHair);
        double v = view->valueOfYPos(pt_client.y());
        if (!chartbase::validVal(v)) {
            v = 0.f;
        }
        val = view->precision().chartValFormat(v);
        return true;
    }
    return false;
}

bool CrossHair::move(int offset) {
    auto ctx = chartCtx();
    if (!main_panel_ || !ctx || dataname_.isEmpty()) {
        return false;
    }

    MainView* view = main_panel_->mainView();
    SkPoint pt_client = view->windowToClient(pt_cur_);
    // std::cout << "move pos:[x,y] =" << pt_client.x() << "," << pt_client.y() << std::endl;

    int idx = view->dataIndexOfPos(pt_client.x());
    if (-1 == idx) {
        // 没有可移动的了
        idx = (offset > 0) ? ctx->indexEnd() : ctx->indexBegin();
    } else {
        int old_idx = idx;
        idx += offset;
        if (offset < 0) {
            idx = std::max(ctx->indexBegin(), idx);
        }
        if (offset > 0) {
            idx = std::min(ctx->indexEnd(), idx);
        }
        if (idx == old_idx) {
            // 无法移动了！
            return false;
        }
    }

    // 仅能在当前显示的index范围内移动
    if (idx < ctx->indexBegin() || idx > ctx->indexEnd()) {
        return false;
    }

    // 使用方向键导航的时候，十字线始终是在主panel上的
    cur_panel_ = main_panel_;

    // 移动到序列上
    pt_cur_ = view->clientToWindow(view->posOfDataname(idx, dataname_));
    return true;
}

void CrossHair::show(Bits bits) {
    flags_.set(bits, true);
}

}
}

