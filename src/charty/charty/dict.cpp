#include "dict.h"

namespace sia {
namespace charty {

Dict::~Dict() {

}

void Dict::add(const SkString& k, const SkString& v) {
    dict_[k] = v;
}

void Dict::add(const std::map<SkString, SkString>& dict) {
    for (auto i : dict) {
        dict_[i.first] = i.second;
    }
}

void Dict::remove(const SkString& k) {
    dict_.erase(k);
}

SkString Dict::get(const SkString& k)const {
    auto i = dict_.find(k);
    if (i != dict_.end()) {
        return i->second;
    }
    return k;
}

}

}

