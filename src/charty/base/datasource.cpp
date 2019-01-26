#include "datasource.h"
#include <algorithm>
#include "SkTSort.h"
#include "SkTSearch.h"

namespace sia {

namespace chartbase {

void ColumnData::set(const SkString& data_name, data_val_t v) {
    d_[data_name] = v;

    // !! insert 是只有在没有相同的dataname的时候才会成功！
    // d_.insert({ data_name, v });
}

data_val_t ColumnData::get(const SkString& data_name) const {
    auto i = d_.find(data_name);
    if (i != d_.end()) {
        return i->second;
    }

    return chartbase::kInvalidDataValue;
}

void ColumnData::remove(const SkString& data_name) {
    d_.erase(data_name);
}

std::vector<SkString> ColumnData::datanames() const {
    std::vector<SkString> ret;
    for (auto i : d_) {
        ret.push_back(i.first);
    }
    return ret;
}

void DataSource::addData(const SkString&data_name, const std::map<data_key_t, data_val_t>& datas) {
    beginTransaction();
    for (auto i : datas) {
        if (auto t = _get(i.first, true)) {
            t->set(data_name, i.second);
        }
    }
    endTransaction();
}

void DataSource::removeData(const SkString&data_name) {
    beginTransaction();
    for (auto i = all_.begin(); i != all_.end(); ++i) {
        (*i)->remove(data_name);
    }
    clearEmptyData();
    endTransaction();
}

void DataSource::removeData(const SkString&data_name, const SkTArray<data_key_t>& keys) {
    beginTransaction();
    for (auto k : keys) {
        if (auto cd = _get(k, false)) {
            cd->remove(data_name);
        }
    }
    clearEmptyData();
    endTransaction();
}

bool DataSource::forIndexData(int32_t idx_begin, int32_t idx_end, std::function<bool(int32_t, const Elem)> cb)const {
    if (!cb || !validateIndex(idx_begin, idx_end)) {
        return false;
    }

    for (auto i = idx_begin; i <= idx_end; ++i) {
        if (!cb(i, all_[i])) {
            break;
        }
    }
    return true;
}

bool DataSource::keys(int32_t idx_begin, int32_t idx_end, SkTArray<data_key_t>& out) const {
    if (!validateIndex(idx_begin, idx_end)) {
        return false;
    }

    for (auto i = idx_begin; i <= idx_end; ++i) {
        out.push_back(all_[i]->key());
    }
    return true;
}

data_key_t DataSource::key(int32_t idx) const {
    if (auto p = get(idx)) {
        return p->key();
    }
    return chartbase::kInvalidDataKey;
}

int DataSource::keyIndex(data_key_t k, int32_t idx_begin /*= 0*/, int32_t idx_end /*= -1*/) const {
    if (!validateIndex(idx_begin, idx_end)) {
        return -1;
    }

    static Elem k_elem = nullptr;
    if (!k_elem) {
        k_elem = std::make_shared<ColumnData>(k);
    }
    k_elem->k_ = k;

    auto cmp = [](const Elem& t, const Elem& target)->bool {
        return t->key() < target->key();
    };
    int idx = SkTSearch(all_.begin(), all_.count(), k_elem, sizeof(Elem), cmp);
    return (idx >= 0) ? idx : -1;
}

data_val_t DataSource::value(int32_t idx, const SkString& dataname) const {
    if (idx >= total()) {
        return chartbase::kInvalidDataValue;
    }

    if (idx <= -1) {
        idx = total() - 1;
    }

    return all_[idx]->get(dataname);
}

void DataSource::beginTransaction() {
    flags_.set(Flags::kDataReady, false);
}

void DataSource::endTransaction() {
    sortData();
    flags_.set(Flags::kDataReady, true);
}

bool DataSource::getIndexMinMaxVal(const std::set<SkString>& names,
                                   int32_t idx_begin, int32_t idx_end, data_val_t& min, data_val_t& max) const {
    if (names.empty()) {
        return false;
    }

    min = std::numeric_limits<data_val_t>::max();
    max = std::numeric_limits<data_val_t>::min();
    return forIndexData(idx_begin, idx_end, [&](int32_t idx, const Elem data)->bool {
        for (const SkString& name : names) {
            data_val_t v = data->get(name);
            if (chartbase::validVal(v)) {
                if (v > max) {
                    max = v;
                }
                if (v < min) {
                    min = v;
                }
            }
        }
        return true;
    });
}

DataSource::Elem DataSource::_get(data_key_t k, bool create) {
    Elem target = nullptr;
    if (!all_.empty()) {
        int idx = keyIndex(k, 0, total());
        if (idx >= 0) {
            target = all_[idx];
        }
    }

    if (!target && create) {
        target = std::make_shared<ColumnData>(k);
        all_.push_back(target);
    }
    return target;
}

void DataSource::clearEmptyData() {
    // 移除已空的元素
    auto i_end = std::remove_if(all_.begin(), all_.end(), [](Elem p)->bool {
        return p->empty();
    });
    all_.pop_back_n(int(all_.end() - i_end));
}

const DataSource::Elem DataSource::get(int idx) const {
    if (idx >= total()) {
        return nullptr;
    }

    if (idx < 0) {
        idx = total() - 1;
    }
    return all_[idx];
}

void DataSource::sortData() {
    // 第二个参数不能传入 all_.end()， 因为这个元素要求是有效的索引（参与排序比较）。
    SkTQSort(all_.begin(), (all_.begin() + all_.count() - 1), [](Elem t1, Elem t2)->bool {
        return t2->key() > t1->key();
    });
}

bool DataSource::validateIndex(int32_t &idx_begin, int32_t &idx_end)const {
    auto cnt = total();
    if (idx_begin < 0 || idx_begin >= cnt) {
        return false;
    }

    if (idx_end < 0 || idx_end >= cnt) {
        idx_end = cnt - 1;
    }

    return (idx_end >= idx_begin);
}

}
}

