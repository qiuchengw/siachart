#pragma once

#include "chartapi.h"
#include "defines.h"
#include "../base/base.h"

namespace sia {
namespace charty {

class ObjectItem;

/*!
 * \class RelPos
 *
 * \brief 相对坐标（relate position）参数
 *
 */
struct CHARTY_API RelPos {
    SkScalar l = chartbase::kInvalidCoord;
    SkScalar t = chartbase::kInvalidCoord;
    SkScalar r = chartbase::kInvalidCoord;
    SkScalar b = chartbase::kInvalidCoord;
};

/*!
 * \class ObjectParam
 *
 * \brief 自定义对象的大小/位置参数
 *
 */
struct CHARTY_API ObjectParam {
    //! 位置
    PosType postyp;

    //! 相对坐标(目前只支持相对于父容器border的位置) （PosType::kRelated 有效）
    RelPos relpos;

    //! 对象大小，可选参数
    SkSize sz;

    //! 用于x坐标上的定位
    chart_key_t x;

    //! attach到此组件 （PosType::kAttached 有效）
    SkString attach;

    //! 根据value计算，（PosType::kValued 有效）
    chart_val_t val;

    //! 提示内容，为空则不提示信息
    SkString tooltip;
};

/*!
 * \class IVisualObject
 *
 * \brief 图表的可视化Object
 *
 */
class CHARTY_API IVisualObject {
    friend class ObjectItem;
    // friend class std::_Ref_count_obj<IVisualObject>;
public:
    static my_sp<IVisualObject> create(ObjectType typ);

    explicit IVisualObject(ObjectType typ)
        :typ_(typ) {
    }

    virtual ~IVisualObject() {

    }

protected:
    virtual void draw(SkCanvas*, const SkRect& rc) = 0;
    virtual void buildPaint();

public:
    SkColor color;

protected:
    ObjectType typ_ = ObjectType::kNotSet;
    PosType pos_ = PosType::kNotSet;
    RelPos rel_;
    SkPaint paint_;
};

class CHARTY_API TextObject : public IVisualObject {
    // friend class std::_Ref_count_obj<TextObject>;
public:
    SkString text;
    bool wrap;
    SkString font_face;
    SkScalar text_size = SkIntToScalar(0);
    SkFontStyle font_style = SkFontStyle::Normal();

    TextObject();

protected:
    virtual void draw(SkCanvas*, const SkRect& rc) override;

    virtual void buildPaint() override;

private:
    sk_sp<SkTypeface> face_ = nullptr;
};

class CHARTY_API PictrueObject : public IVisualObject {
    // friend class std::_Ref_count_obj<PictrueObject>;

public:
    SkString path;
    PictrueObject();

protected:

    virtual void draw(SkCanvas*, const SkRect& rc) override;

    virtual void buildPaint() override;

private:
    sk_sp<SkImage> img_ = nullptr;
};

class CHARTY_API LineObject : public IVisualObject {
    // friend class std::_Ref_count_obj<LineObject>;
public:
    chart_val_t val;
    LineObject();

protected:
    virtual void draw(SkCanvas*, const SkRect& rc) override;
    virtual void buildPaint() override;

};

class CHARTY_API RichTextObject : public IVisualObject {
    // friend class std::_Ref_count_obj<RichTextObject>;

public:
    RichTextObject();

protected:
    virtual void draw(SkCanvas*, const SkRect& rc) override;
    virtual void buildPaint() override;
};

class CHARTY_API MovieObject : public IVisualObject {
    // friend class std::_Ref_count_obj<MovieObject>;

public:
    MovieObject();

protected:
    virtual void draw(SkCanvas*, const SkRect& rc) override;
    virtual void buildPaint() override;
};

class CHARTY_API CustomObject : public IVisualObject {
    // friend class std::_Ref_count_obj<CustomObject>;
public:
    typedef std::function<void(SkCanvas*, const SkRect&)> DrawCallback;

    void setDrawer(DrawCallback cb);
    CustomObject();

protected:
    virtual void draw(SkCanvas*, const SkRect& rc) override;

private:
    DrawCallback cb_;
};

}
}