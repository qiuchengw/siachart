#include "tadata.h"

namespace sia {
namespace formula {


const TA_Real* TaData::indexData(uint16_t idx) const {
    if (idx >= TaInputDataIndex::kMaxInputIndex) {
        return nullptr;
    }
    return const_cast<TaData*>(this)->indexData((TaInputDataIndex)idx).begin();
}

SkTArray<TA_Real>& TaData::indexData(TaInputDataIndex idx) {
    switch (idx) {
    case TaInputDataIndex::kOpen:
        return open_;
    case TaInputDataIndex::kHigh:
        return high_;
    case TaInputDataIndex::kLow:
        return low_;
    case TaInputDataIndex::kClose:
        return close_;
    case TaInputDataIndex::kVolume:
        return volume_;
    case TaInputDataIndex::kOi:
        return oi_;
    }
}

TA_Real* TaData::outReal(TaOutputDataIndex idx) {
    switch (idx) {
    case TaOutputDataIndex::kOut1:
        return out1();
    case TaOutputDataIndex::kOut2:
        return out2();
    case TaOutputDataIndex::kOut3:
        return out3();
    }
    return nullptr;
}

void TaData::setOut(TaOutputDataIndex out_idx, const SkString& name, int valid_idx_begin, int valid_count) {
    switch (out_idx) {
    case TaOutputDataIndex::kOut1:
        out1_.name = name;
        out1_.key_offset = valid_idx_begin;
        out1_.count = valid_count;
        break;
    case TaOutputDataIndex::kOut2:
        out2_.name = name;
        out2_.key_offset = valid_idx_begin;
        out2_.count = valid_count;
        break;
    case TaOutputDataIndex::kOut3:
        out3_.name = name;
        out3_.key_offset = valid_idx_begin;
        out3_.count = valid_count;
        break;
    case TaOutputDataIndex::kOutInt:
        out_int_.name = name;
        out_int_.key_offset = valid_idx_begin;
        out_int_.count = valid_count;
        break;
    }
}

void TaData::beginInsert(int total, const SkTArray<chartbase::data_key_t>& keys) {
    keys_ = keys;

    for (int i = 0; i < TaInputDataIndex::kMaxInputIndex; i++) {
        indexData((TaInputDataIndex)i).reset(total);
    }

    // 出参也必须init
    out1_.data.reset(total);
    out2_.data.reset(total);
    out3_.data.reset(total);
    out_int_.data.reset(total);
}

void TaData::setItem(TaInputDataIndex idx, int i, TA_Real val) {
    indexData(idx)[i] = val;
}

void TaData::endInsert() {

}

template<typename T>
bool forEachOutItem(const TaData::_NamedData<T>& data, const SkTArray<chartbase::data_key_t>& keys,
                    std::function<bool(const SkString&, const std::map<chartbase::data_key_t, chartbase::data_val_t>&)> on_data) {

    std::map<chartbase::data_key_t, chartbase::data_val_t> items;
    const auto& dd = data.data;
    // std::cout << "TaData dump: " << data.name.c_str() << "\n";
    for (int i = 0; i < data.count; ++i) {
        // FIX: 这儿的数据是不能这样强转的。比如double/int64强转就丢失小数了！
        // std::cout << dd[i] << " ";
        items[keys[i+ data.key_offset]] = (chartbase::data_val_t)(dd[i]);
    }
    // std::cout << std::endl;
    return on_data(data.name, items);
}

void TaData::forValidOutData(
    std::function<bool(const SkString&, const std::map<chartbase::data_key_t, chartbase::data_val_t>&)> on_data)const {
    if (out1_.valid()) {
        if (!forEachOutItem(out1_, keys_, on_data)) {
            return;
        }
    }

    if (out_int_.valid()) {
        if (!forEachOutItem(out_int_, keys_, on_data)) {
            return;
        }
    }

    if (out2_.valid()) {
        if (!forEachOutItem(out2_, keys_, on_data)) {
            return;
        }
    }

    if (out3_.valid()) {
        if (!forEachOutItem(out3_, keys_, on_data)) {
            return;
        }
    }
}

}
}

