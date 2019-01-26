#include "talib.h"
#include "deps/talib/include/ta_libc.h"
#include "../base/datasource.h"
#include "taitem.h"
#include "iformula.h"

namespace sia {
namespace formula {
GroupMethod::GroupMethod(const SkString& name)
    :group_(name) {

}

void GroupMethod::dumpInfo() const {
    std::cout << "------------------" << std::endl;
    std::cout << group_.c_str() << std::endl;
    for (auto& s : methods_) {
        std::cout << s.c_str() << std::endl;
    }
}

//////////////////////////////////////////////////////////////////////////

TAFuncGroup::TAFuncGroup(const SkString& group)
    :GroupMethod(group) {
    TA_StringTable *table = nullptr;
    TA_RetCode result = TA_FuncTableAlloc(group.c_str(), &table);
    if (result == TA_SUCCESS) {
        for (int j = 0; j < (int)table->size; j++) {
            add(SkString(table->string[j]));
        }
        TA_FuncTableFree(table);
    }
}

//////////////////////////////////////////////////////////////////////////
TaLib::TaLib(const chartbase::DataSource* ds)
    :ds_(ds) {
    TA_Initialize();
    initMethods();
}

TaLib::~TaLib() {
    TA_Shutdown();
}

void TaLib::dumpInfo() const {
    for (auto& s : methods_) {
        s->dumpInfo();
    }
}

void TaLib::initMethods() {
    methods_.reset();

    TA_StringTable *table = nullptr;
    if (TA_GroupTableAlloc(&table) == TA_SUCCESS) {
        for (int i = 0; i < (int)table->size; i++) {
            SkString group_name(table->string[i]);
            auto p = std::make_shared<TAFuncGroup>(group_name);
            if (p->count() > 0) {
                methods_.push_back(p);
            }
        }
        TA_GroupTableFree(table);
    }
}

}
}

