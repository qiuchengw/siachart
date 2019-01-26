#pragma once

#include "siapi.h"
#include "resitem.h"
#include "deps/sol2/sol.hpp"
#include "resfactory.h"

#define GLOBAL_GROUP_NAME "@global"
#define PAINT_GROUP_NAME "@paint"

namespace sia {
namespace resy {

/*!
 * \class ResGroup
 *
 * \brief 一组资源集合管理
 *      为了便于资源的管理，可以人为的把资源归为一组（看使用者的用法，注意并不是资源的类型分组）。
 *      比如，一个窗口拥有自己独特的资源（包括字体设置，颜色设置，图片等），同时它也会引用一些全局的资源。
 *      这种情况下，我们可以设计一组全局资源：@global，将一些全局的字体/图片等放进去。
 *      再设计一组@window，保存自己的图片/字体等。这样就可以将资源有效/清晰的管理起来。
 *
 *      资源的命名要统一，简洁！相互不可以名字冲突。
 */
class RESY_API ResGroup { /*: public util::enable_shared_from_this_virtual<ResGroup>*/
public:
    virtual ~ResGroup();

    // void add(const SkTArray<my_sp<ResItem>>& a);
    void add(my_sp<ResItem> a);

    /*! \brief 清空所有的Resource
     *
     *		ResItem 和 ResGroup是相互引用的关系。必须手动清理内存
     */
    void clearRes();

    // 此用法不规范，请不要保留返回的指针值！
    // TODO: 将ResItem 继承自enable_shared_from_this，以使用shared_from_this()
    template<typename ResType>
    ResType* get(const SkString& id) {
        if (auto p = rawRes(id)) {
            return reinterpret_cast<ResType*>(p.get());
        }
        return nullptr;
    }

    my_sp<ResItem> rawRes(const SkString& id);

#ifdef DEBUG_CODE
    void dumpInfo();
#endif

private:
    // key - resource id （全路径id，类似@global/font/h1）
    std::unordered_map<SkString, my_sp<ResItem>> res_;
};

/*!
 * \class ResManBase
 *
 * \brief 基础资源管理器，可以组合/继承此类以实现自己的资源管理/解析器。
 *      比如：一个应用可能需要使用lua定制一种脚本，以配置自己的窗口颜色/大小等（例如实现一个类CSS的脚本）
 *      则可以继承此类，提供自己的资源解析器。可以参考sia::graphy::PaintMan的实现
 *
 *      本类已实现了几种基本的资源解析/管理，包括
 *      ** font/image/stroke/color
 *
 *      此管理器导出了两个lua函数，可以供lua代码使用（使用实例可以参考：doc/lua/paint_parser.lua）：
 *      1，host_reg_res : 注册资源
 *      2，host_raise_error: 显示lua解析错误
 *      通过parseFile，可以执行/解析用户自定义的资源语法，以达使用脚本扩展程序功能的目的
 *
 */
class RESY_API ResMan {
public:
    ResMan();

    virtual ~ResMan();

    /*! \brief 清空当前所有的资源（资源组和资源）
     *
     *    @param
     *    @return
     */
    void clearRes();

    /*! \brief 解析用户自定义的资源文件
     *
     *    @param    lua_res_file 用户自己设计的资源文件（遵循lua语法）
     *              res_parser_code 用户针对自己设计的资源语法实现的解析代码（lua解析）
     *
     *    @return   如果出现错误，可以通过 onLuaError 输出具体的错误信息！
     */
    bool parseFile(const std::string& lua_res_file, const std::string& res_parser_code);

    /*! \brief 查找resGroup
     *
     *    @param    name    必须是以'@'开头，如果有多级，需要以 '/'分隔路径，如：@paint/title
     *              目前有两种已知/保留的group名字(自定义的group，不可和这两个重复)：
     *              1，@global  代表全局的资源group，所有其他的资源都可以引用它
     *              2，@paint   sia::graphy自定义的，用于生成SkPaint
     *
     *              create_if_not_exist true 如果没有就创建一个group
     *    @return
     */
    my_sp<ResGroup> findGroup(const SkString& name) {
        return getGroup(name, false);
    }

    /*! \brief 注册一个资源解析器
     *	        如果已有同类型的资源解析器，则新的将替代前面的！
     *
     *    @param    factory 资源解析器
     *    @return
     */
    void regResFactory(my_sp<IResFactory> factory);

#ifdef DEBUG_CODE
    void dumpInfo()const;
#endif

    inline SkString errorMsg()const {
        return error_msg_;
    }

protected:
    // 在lua打开的时候可以注册一些自定义的函数
    virtual void onLuaOpen(sol::state& lua) {}

    /*! \brief 注册资源
     *
     *    @param	group_name	资源所属组，参考ResGroup
     *    @param	res_typ	资源类型，参考ResItem(color/font ...)
     *    @param	key	资源名，参考ResItem::id
     *    @param	val	资源值
     *    @return	资源对象指针，如果资源解析错误，返回nullptr
     */
    virtual my_sp<ResItem> regResItem(const SkString& group_name, const SkString& res_typ, const SkString& key, const SkString& val);

    /*
     *	lua错误
     */
    virtual void onLuaError(const SkString& err) {
        std::cout << err.c_str() << std::endl;
    }

    /*! \brief 查找一个资源解析器
     *
     *    @param
     *    @return
     */
    my_sp<IResFactory> factory(const SkString& res_type)const;

protected:
    bool splitGroupPath(const SkString& prefix, SkString& name, SkString& item) const;
    bool extractResType(const SkString& resval, SkString& typ) const;

    /*! \brief 设置错误信息
     *
     *    @param	err	错误信息
     *    @return
     */
    void setError(const SkString& err);
private:
    /*! \brief 查找resGroup
     *
     *    @param    name    必须是以'@'开头，如果有多级，需要以 '/'分隔路径，如：@paint/title
     *              目前有两种已知/保留的group名字(自定义的group，不可和这两个重复)：
     *              1，@global  代表全局的资源group，所有其他的资源都可以引用它
     *              2，@paint   sia::graphy自定义的，用于生成SkPaint
     *
     *              create_if_not_exist true 如果没有就创建一个group
     *    @return
     */
    my_sp<ResGroup> getGroup(const SkString& name, bool create_if_not_exist = false);

protected:
    // key - 资源组id，参考getGroup定义
    std::unordered_map<SkString, my_sp<ResGroup>> all_;

    // 用于解析各类资源，可以自定义自己的资源解析器
    std::unordered_map<SkString, my_sp<IResFactory>> factory_;

    // 错误信息
    SkString error_msg_;
};

//////////////////////////////////////////////////////////////////////////
template <typename Res>
class KnownResFactory : public IResFactory {
public:
    KnownResFactory(ResMan* man)
        :IResFactory(Res().resTypeStr()), man_(man) {
    }

    virtual my_sp<ResItem> parseRes(const SkString& group_name, const SkString& key, const SkString& val) override {
        my_sp<ResItem> p(new Res(key));
        p->setCommonResRef(man_->findGroup(group_name), man_->findGroup(SkString(GLOBAL_GROUP_NAME)));

        if (!p->parse(val)) {
            return nullptr;
        }
        return p;
    }

private:
    ResMan* man_ = nullptr;
};

}
}
