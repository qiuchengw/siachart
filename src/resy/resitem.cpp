#include "resitem.h"
#include "resman.h"
#include <string>

namespace sia {
namespace resy {

ResItem::ResItem(int16_t typ, const SkString& styp, const SkString& id)
    :kTyp_(typ), sTyp_(styp), key_(id) {
    // id 必须是带有资源标识的！
    // assert(id.contains('/'));
    if (!id.contains('/')) {
        // 我们要给它在key里加上资源类型
        key_ = resTypeStr();
        key_.append("/");
        key_.append(id);
    }
}

// str必须类似：
//      @global/color/border
//      @global/border  <-- 这个时候必须传入资源类型
bool extractResPath(const SkString& str, const SkString& restyp, SkString& out) {
    int idx = str.find("/");
    if (idx > 0 && idx < (str.size() - 1)) {    // 且不是最后一个字符
        out = SkString(str.c_str() + idx + 1);
        if (!out.contains('/')) {
            // 没有带资源类型！
            SkString lead = restyp;
            lead.append("/");
            out.prepend(lead);
            return true;
        }

        // 带了资源类型了
        if (!out.startsWith(restyp.c_str())) {
            // 资源不同啊！
            return false;
        }
        return true;
    }
    return false;
}

my_sp<ResItem> ResItem::commonRefrence(const SkString& id) {
    if (!id.startsWith("@common")) {
        return nullptr;
    }

    // 找引用
    SkString item;
    if (cmn_res_ && extractResPath(id, resTypeStr(), item)) {
        return cmn_res_->rawRes(item);
    }
    return nullptr;
}

my_sp<ResItem> ResItem::globalRefrence(const SkString& id) {
    if (!id.startsWith("@global")) {
        return nullptr;
    }

    // 找引用
    SkString item;
    if (global_res_ && extractResPath(id, resTypeStr(), item)) {
        return global_res_->rawRes(item);
    }
    return nullptr;
}

bool ResItem::detectRefrence(const SkString& resval) {
    auto setRef = [&](my_sp<ResItem> p) -> bool {
        if (p && p->resType() == resType()) {
            flags_ |= Flags::kRefRes;
            res_ref_ = p;
            return true;
        } else {
            flags_ &= (~Flags::kValid);
            return false;
        }
    };

    if (resval.startsWith("@common")) {
        setRef(commonRefrence(resval));
        return true;
    }

    if (resval.startsWith("@global")) {
        setRef(globalRefrence(resval));
        return true;
    }
    return false;
}

}
}
