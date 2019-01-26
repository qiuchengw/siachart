#pragma once

#include "visualitem.h"
#include "mousestate.h"

namespace sia {
namespace charty {
class Panel;
class CrosshairDetail;

/*!
 * \class CrossHair
 *
 * \brief 图表级别的十字线显示
 *      一定要把本实例完全覆盖到图表上才可以正确运行
 *
 *	使用key（左右方向键）导航的时候，十字线始终是贴合在主panel上！
 */
class CrossHair : public VisualItem {
public:
    enum Flags {
        //! 显示十字线
        kHasCross = 0x4,

        //! 显示框选
        kHasRubber = 0x8,
    };

    CrossHair(/*Chart* parent = nullptr*/);

    virtual bool onMouse(int x, int y, sia::InputState state, uint32_t modifiers) override;

    virtual bool onKey(sia::KeyBoard key, sia::InputState state, uint32_t modifiers) override;

    void setPanel(Panel* main_p, Panel* cur_p);

    bool curKeyVal(int& idx, SkString& key, SkString& val);

    /*! \brief 移动十字线
     *
     *    @param    offset    >0 向右移动， <0 向左移动, abs(offset) 移动的index数量
     *    @return false 已经无法移动了！
     */
    bool move(int offset);

    /*! \brief 十字线移动的时候应该贴合到哪个数据上？
     *
     *    @param
     *    @return
     */
    void attachDataname(const SkString& name) {
        dataname_ = name;
    }

    void setDetail(CrosshairDetail* d) {
        cross_detail_ = d;
    }

    void hide() {
        flags_.clear();
    }

    //! CrossHair::Flags
    void show(Bits bits);

    inline bool flag(Bits b)const {
        return flags_.test(b);
    }

    inline bool showing()const {
        return flags_.test(Flags::kHasCross | Flags::kHasRubber);
    }

protected:
    virtual void onDraw(SkCanvas* canvas) override;

    virtual void preparePaints() override;

    void doRubberSelect();

    void drawTag(SkCanvas* canvas, const SkRect& rc, const SkString& str) const;

    void drawLineDetail(SkCanvas* canvas, int idx);

private:
    SkPaint *line_ = nullptr;
    SkPaint *rubber_ = nullptr;
    SkPaint *tag_back_ = nullptr;
    SkPaint *tag_text_ = nullptr;
    SkPaint *tag_border_ = nullptr;

    SkPaint *detail_border_ = nullptr;
    SkPaint *detail_background_ = nullptr;
    SkPaint *detail_title_ = nullptr;
    SkPaint *detail_value_ = nullptr;

    // detail 面板的大小
    SkSize detail_size_;

    bool pressed_ = false;
    SkPoint pt_lt_;
    SkPoint pt_cur_;

    BitFlags flags_;

    //! 当前十字线在哪个panel上
    Panel* cur_panel_ = nullptr;

    //! chart的主panel
    Panel* main_panel_ = nullptr;

    //! 十字线左右导航的时候，贴合到哪个data上
    SkString dataname_;

    //! 十字线所在位置的详细描述
    CrosshairDetail* cross_detail_ = nullptr;

    // 鼠标状态检测
    util::MouseStateMachine mouse_state_;
};
}
}
