#include "viewobject.h"
#include "SkPicture.h"
#include "visualitem.h"

namespace sia {
namespace charty {

my_sp<sia::charty::IVisualObject> IVisualObject::create(ObjectType typ) {
    switch (typ) {
    case ObjectType::Custom:
        return std::make_shared<CustomObject>();
    case ObjectType::kHLine:
        return std::make_shared<LineObject>();
    case ObjectType::kText:
        return std::make_shared<TextObject>();
    case ObjectType::kPicture:
        return std::make_shared<PictrueObject>();
    case ObjectType::kMovie:
        return std::make_shared<MovieObject>();
    case ObjectType::kRichText:
        return std::make_shared<RichTextObject>();
    }
    return nullptr;
}

void IVisualObject::buildPaint() {

}

//////////////////////////////////////////////////////////////////////////
TextObject::TextObject()
    :IVisualObject(ObjectType::kText) {

}

void TextObject::draw(SkCanvas* canvas, const SkRect& rc) {
    VisualItem::drawMultilineText(canvas, paint_, text, rc, 5);
    // debug
//     paint_.setStyle(SkPaint::kStroke_Style);
//     canvas->drawRect(rc, paint_);
}

void TextObject::buildPaint() {
    face_ = SkTypeface::MakeFromName(font_face.isEmpty() ? nullptr : font_face.c_str(), font_style);
    if (face_) {
        paint_.setTypeface(face_);
    }
    if (text_size > 0) {
        paint_.setTextSize(text_size);
    }

    paint_.setColor(color);
    // paint_.setAntiAlias(true);

    // TODO: 文本换行实现
}

//////////////////////////////////////////////////////////////////////////
PictrueObject::PictrueObject()
    :IVisualObject(ObjectType::kPicture) {

}

void PictrueObject::draw(SkCanvas* canvas, const SkRect& rc) {
    if (img_) {
        canvas->drawImageRect(img_, rc, &paint_);
    }
}

void PictrueObject::buildPaint() {
    sk_sp<SkData> encoded = SkData::MakeFromFileName(path.c_str());
    img_ = SkImage::MakeFromEncoded(encoded);
}

//////////////////////////////////////////////////////////////////////////
LineObject::LineObject()
    :IVisualObject(ObjectType::kHLine) {

}

void LineObject::draw(SkCanvas*, const SkRect& rc) {

}

void LineObject::buildPaint() {

}

//////////////////////////////////////////////////////////////////////////
RichTextObject::RichTextObject()
    :IVisualObject(ObjectType::kRichText) {

}

void RichTextObject::draw(SkCanvas*, const SkRect& rc) {

}

void RichTextObject::buildPaint() {

}

//////////////////////////////////////////////////////////////////////////
MovieObject::MovieObject()
    :IVisualObject(ObjectType::kMovie) {

}

void MovieObject::draw(SkCanvas*, const SkRect& rc) {

}

void MovieObject::buildPaint() {

}

//////////////////////////////////////////////////////////////////////////
CustomObject::CustomObject()
    :IVisualObject(ObjectType::Custom) {

}

void CustomObject::setDrawer(DrawCallback cb) {
    cb_ = cb;
}

void CustomObject::draw(SkCanvas* canvas, const SkRect& rc) {
    if (cb_) {
        cb_(canvas, rc);
    }
}

}
}