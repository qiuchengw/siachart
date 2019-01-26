#include "formulaman.h"
#include "talib.h"
#include "taformula.h"

namespace sia {
namespace formula {

struct _GroupMethods {
public:
    bool exist(const SkString& group, const SkString& method) {
        if (auto g = getGroup(group, false)) {
            return g->exist(method);
        }
        return false;
    }

    inline bool add(const SkString& group, const SkString& method) {
        return getGroup(group, true)->add(method);
    }

    void groups(SkTArray<SkString>& out) const {
        for (auto i : groups_) {
            out.push_back(i->groupName());
        }
    }

    void methods(const SkString& group, SkTArray<SkString>& out)  {
        if (auto g = getGroup(group, false)) {
            out = g->methods();
        }
    }

protected:
    my_sp<GroupMethod> getGroup(const SkString& group, bool create) {
        for (auto g : groups_) {
            if (group.equals(g->groupName())) {
                return g;
            }
        }
        if (create) {
            auto g = std::make_shared<TAFuncGroup>(group);
            groups_.push_back(g);
            return g;
        }
        return nullptr;
    }
private:
    SkTArray<my_sp<GroupMethod>> groups_;
};

struct FormulaManPrivate {
    // talib公式
    TaLib	talib;

    // 已注册的公式：<group, method>
    _GroupMethods gmethods;
};

FormulaMan::FormulaMan() {
    d = new FormulaManPrivate();

    // 注册已知的Talib
    for (auto& g : d->talib.groups()) {
        for (auto &s : g->methods()) {
            regFormula<TaFormula>(g->groupName(),s);
        }
    }
}

FormulaMan::~FormulaMan() {
    delete d;
}

IFormula* FormulaMan::create(const SkString& name) {
    return ff_.create(name);
}

void FormulaMan::destroy(const SkString& name, IFormula* indi) {
    ff_.destory(name, indi);
}

void FormulaMan::groups(SkTArray<SkString>& out) const {
    d->gmethods.groups(out);
}

void FormulaMan::methods(const SkString& group, SkTArray<SkString>& out) const {
    d->gmethods.methods(group, out);
}

void FormulaMan::_regFormula(const SkString& group, const SkString& name) {
    d->gmethods.add(group, name);
}

}
}

