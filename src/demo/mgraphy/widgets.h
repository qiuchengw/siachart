#pragma once

#include <iostream>
#include "graphy/widget.h"
#include "graphy/layout.h"
#include "mylayout.h"
#include "graphy/paintman.h"

namespace test {

class WidgetCircle : public sia::graphy::Widget {
public:
    WidgetCircle(SkColor c, const SkString& name, Widget* parent)
        :sia::graphy::Widget(name, parent), c_(c) {
        setVisibleP(true);
    }

protected:
    virtual void onDraw(SkCanvas* canvas) override {
        SkPaint paint;
        paint.setAntiAlias(true);
        paint.setColor(c_);

        SkRect rc = rect();
        canvas->drawRect(rc, paint);

        canvas->translate(20, 20);
        paint.setColor(SK_ColorRED);
        canvas->drawCircle(rc.width() / 2, rc.height() / 2, rc.width() / 2, paint);

        canvas->translate(20, 20);
        paint.setColor(SK_ColorGRAY);
        canvas->drawCircle(rc.width() / 2, rc.height() / 2, rc.width() / 2, paint);
    }

    virtual bool onMouse(int x, int y, sia::InputState state, uint32_t modifiers) override {
        std::cout << "widget:[" << name().c_str() << "] - [" << x << "," << y << "]" << std::endl;
        return true;
    }

private:
    SkColor c_;
};

class SimpleLayout : public sia::graphy::Layout {
protected:
    virtual void doLayout() override {
        SkRect rc = geometry();
        auto radius = std::min(rc.width(), rc.height()) / 2;

        SkScalar dx = 0.f;
        SkScalar dy = 0.f;
        for (sia::graphy::Widget* w : childrenWidget()) {
            w->setGeometry(SkRect::MakeXYWH((rc.width() - radius) / 2 + dx, (rc.height() - radius) / 2 + dy, radius, radius));
            dx += SkIntToScalar(20);
            dy += SkIntToScalar(20);
        }
    }
};

class WidgetRect :
    public sia::graphy::Widget {

public:
    WidgetRect(SkString obj_name)
        :sia::graphy::Widget(obj_name) {
        // setClipToBounds(false);
        obj_name += "- circle - 1";
        auto p = new WidgetCircle(SK_ColorWHITE, obj_name, this);
        addChild(p);
        p->setZIndex(999);

        obj_name += " - 2222";
        auto p1 = new WidgetCircle(SK_ColorCYAN, obj_name, this);
        addChild(p1);
        p1->setZIndex(888);

        setLayout(new SimpleLayout());

        if (auto paintman = sia::graphy::PaintMan::instance()) {
            SkString group_name("@paint/");
            group_name.append(name());
            paint_group_ = paintman->paintGroup(group_name, false);
        }
    }

protected:
    virtual void onDraw(SkCanvas* canvas) override {
        if (!paint_group_) {
            return;
        }

        if (SkPaint *paint = paint_group_->getPaint(SkString("background"))) {
            // SkRect r = geometry();
            // canvas->translate(r.x(), r.y());
            // std::cout << "widget:" << name().c_str() << std::endl;
            // Draw a rectangle with red paint
            canvas->drawRect(rect(), *paint);
            sia::graphy::Widget::onDraw(canvas);

            SkPaint paint2 = *paint;
            paint2.setColor(SK_ColorWHITE);
            paint2.setAntiAlias(true);
            SkString u8str(u8"Hello! World! ÖÐÎÄ");
            canvas->drawString(u8str, 5, 25, paint2);
        } else {
            std::cout << "[" << name().c_str() << "] : can't find paint config!" << std::endl;
        }
    }

    virtual bool onMouse(int x, int y, sia::InputState state, uint32_t modifiers) override {
        std::cout << "widget:[" << name().c_str() << "] - [" << x << "," << y << "]" << std::endl;
        return false;
    }

private:
    my_sp<sia::graphy::PaintGroup> paint_group_;
};

inline void create_widgets(sia::graphy::Widget* container) {
    auto add_w = [=](SkColor c, const SkString &name) {
        auto * w = new WidgetRect(name);
        w->setVisibleP(true);
        w->setGeometry(SkRect::MakeXYWH(100, 100, 200, 200));
        container->addChild(w);
    };

    add_w(SK_ColorRED, SkString("calendar"));
    add_w(SK_ColorBLACK, SkString("title"));
    add_w(SK_ColorGREEN, SkString("price"));
    add_w(SK_ColorBLUE, SkString("panel"));
    add_w(SK_ColorYELLOW, SkString("ratio"));

    //     SkString cont;
    //     if (myutil::readFile("./layout.json", cont)) {
    //         sia::graphy::TemplateLayout *l = new sia::graphy::TemplateLayout();
    //         if (l->parse(cont)) {
    //             setLayout(l);
    //         } else {
    //             delete l;
    //         }
    //     }

    test::MyLayout * layout = new test::MyLayout();
    container->setLayout(layout);
}

}
