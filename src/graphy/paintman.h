#pragma once

#include "siapi.h"
#include "paintgroup.h"
#include "resy/resman.h"

// paintman 用于从配置文件解析出来paint对象
// 可用于实现配置式的换肤
namespace sia {
namespace graphy {

class Widget;
class PaintGroup;

/*!
 * \class PaintMan
 *
 * \brief SkPaint配置的解析和管理
 *      例如：graphy::Widget可以使用PaintMan/PaintGroup里面的SkPaint，来定制自己的外观显示。
 *      注意：PaintGroup和graphy::Widget并没有必然的联系。graphy上层的应用来设计他们之间的关联。
 *
 *      通常情况下，我们可以将一个Widget和一个PaintGroup对应起来。
 *      使用继承的方式，重写必要的虚函数，可以在paint发生变化的时候得到通知。
 *      如果不需要在使用运行期动态改变外观显示，那么组合使用更合理点。
 */
class GRAPHY_API PaintMan :
    public sia::resy::ResMan,
    public std::enable_shared_from_this<PaintMan> {

public:
    PaintMan();
    ~PaintMan();

    // 对象创建完成后，要立即调用这个函数进行初始化！
    void init();

    bool load(const SkString& file);

    my_sp<PaintGroup> paintGroup(const SkString& group_name, bool create_if_not_exist = false);

    static my_sp<PaintMan> instance();

#ifdef DEBUG_CODE
    void dumpInfo()const;
#endif

protected:
    virtual void onLuaOpen(sol::state& lua) override;

    // paint_item: @paint/group/item
    void buildPaint(const SkString& paint_item, const SkString& res_typ, const SkString& res_val);

private:
    // 当前系统中注册的painters
    // key -- paint_group_name
    std::unordered_map<SkString, my_sp<PaintGroup>> paints_;

    // 实例指针
    static my_sp<PaintMan> inst_;
};

}
}
