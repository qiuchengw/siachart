#pragma once

#include "resyapi.h"

namespace sia {
namespace resy {

enum /*class*/ RESY_API ResType : int16_t {
    kResInvalid = 0,
    //! 字体：不包含颜色
    kResFont = 1,
    //! 图像
    kResImage,
    //! 颜色
    kResColor,
    //! 线段
    kResStroke,
    //! 效果，如线段的风格（dashed ...）
    kResEffect,

    // 基于resy开发的底层应用框架可以基于此值累加
    kFramework = 100,

    // 更高的应用层基于此值累加
    kApplication = 1000,
};

class RESY_API ResGroup;
class RESY_API ResItem {
    enum Flags {
        kRefRes = 0x1,
        kValid = 0x2,
    };

public:
    ResItem() = delete;
    virtual ~ResItem() {
    }

    /*! \brief 类型必须是在创建的时候就确定，且是不能更改的！
     *
     *    @param    typ 整形的类型标识
     *              styp    更容易识别的类型（人易读）
     *              res_path  资源的路径（或者叫id）
     */
    explicit ResItem(int16_t typ, const SkString& styp, const SkString& res_path);

    /*! \brief 解析资源
     *
     *    @param    res 以字符串标识的资源
     *              要经过解析才能知道里面的内容（但是类型一定是已确定的)
     *    @return
     */
    virtual bool parse(const SkString& res) {
        return false;
    }

    void releaseRes() {
        cmn_res_ = nullptr;
        global_res_ = nullptr;
        res_ref_ = nullptr;
    }

    inline SkString resTypeStr()const {
        return sTyp_;
    }

    void setCommonResRef(my_sp<ResGroup> cmn_res, my_sp<ResGroup> global_res) {
        cmn_res_ = cmn_res;
        global_res_ = global_res;
    }

    inline SkString respath()const {
        return respath_;
    }

    inline SkString key()const {
        return key_;
    }

    inline bool isRefrence()const {
        return flags_ & Flags::kRefRes;
    }

    inline int16_t resType()const {
        return kTyp_;
    }

    inline bool isValid()const {
        return is_valid_;
    }

protected:
    // 检测是否是引用的资源类型
    bool detectRefrence(const SkString& id);
    my_sp<ResItem> commonRefrence(const SkString& id);
    my_sp<ResItem> globalRefrence(const SkString& id);

protected:
    //! 类型是不能更改的
    const int16_t kTyp_ /*= ResType::kResInvalid*/;
    int16_t flags_ = Flags::kValid;
    const SkString sTyp_;

    // 引用类似 @global/font/f1
    // 或者资源类似：“#ff00ff” / "bold 15 宋体"
    SkString respath_;
    SkString key_;

    // 引用的别处的资源
    my_sp<ResItem> res_ref_ = nullptr;

private:
    bool is_ref_res_ = false;
    bool is_valid_ = true;
    my_sp<ResGroup> cmn_res_ = nullptr;
    my_sp<ResGroup> global_res_ = nullptr;
};

}
}
