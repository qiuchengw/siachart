#pragma once

#include "graphyapi.h"
#include "resy/resitem.h"

// paintman 用于从配置文件解析出来paint对象
// 可用于实现配置式的换肤
namespace sia {
namespace resy {
class ResItem;
}
namespace graphy {

class ComposedPaint;
class PaintMan;

enum PaintResType {
    kGeometry = resy::ResType::kFramework + 1,
};

class GRAPHY_API GeometryItem : public resy::ResItem {
public:
    GeometryItem(const SkString& res_id = SkString(""))
        :ResItem(PaintResType::kGeometry, SkString("geometry"), res_id) {
    }

    virtual bool parse(const SkString& res) override;

    const SkRect& rect()const {
        return rc_;
    }

private:
    SkRect rc_;
};

/*!
 * \class PaintGroup
 *
 * \brief 一组SkPaint和geometry的集合管理。每个Group管理一个@paint下面的多个PaintItem 和一个geometry。
 *
 *      例如：graphy::Widget可以使用PaintMan/PaintGroup里面的SkPaint，来定制自己的外观显示。
 *      注意：PaintGroup和graphy::Widget并没有必然的联系。graphy上层的应用来设计他们之间的关联。
 *
 *      通常情况下，我们可以将一个Widget和一个PaintGroup对应起来。
 *      使用继承的方式，重写必要的虚函数，可以在paint发生变化的时候得到通知。
 *      如果不需要在使用运行期动态改变外观显示，那么组合使用更合理点。
 *
 *      group_id:   @paint/group/item
 *
 */
class GRAPHY_API PaintGroup {

    friend class PaintMan;
public:
    PaintGroup(const SkString& group_id);
    ~PaintGroup();

    SkPaint* getPaint(const SkString& paint_item_name, SkPaint* default_p = nullptr);
    bool applyRes(const SkString& paint_item_name, my_sp<sia::resy::ResItem> res);

    bool getGeometry(SkRect& out, const SkString& paint_item_name = SkString("geometry"));

    bool adjustRawRes(SkString& res_typ, SkString& key, SkString& res_val);

    // 预处理
#ifdef DEBUG_CODE
    void dumpInfo();
#endif

private:
    my_sp<ComposedPaint> composedPaint(const SkString& paint_item_name, bool create_if_not_exist = false);

protected:
    // 资源列表
    // key: paint_item_name
    std::unordered_map<SkString, my_sp<ComposedPaint>> paints_;
    SkString group_id_;
};
}
}
