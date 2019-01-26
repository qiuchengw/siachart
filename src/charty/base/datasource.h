#pragma once

#include "baseapi.h"
#include "bitflag.h"
#include "base.h"

/*! \brief 数据源中的数据按照时间值顺序（升序）索引。每一个时间值上关联了多个value。
 *	    在图表使用的时候通常是按照时间取多个序列连续的值。这个时候需要遍历！
 */
namespace sia {
namespace chartbase {

class DataSource;
/*!
 * \class ColumnData
 *
 * \brief 每一个时间值索引一个数据
 *
 */
class BASE_API ColumnData { /* : public std::enable_shared_from_this<ColumnData>*/
    friend class DataSource;
public:
    ColumnData(data_key_t k)
        :k_(k) {
    }

    inline bool operator()(const data_key_t& k, const ColumnData& t)const {
        return k == t.key();
    }

    void set(const SkString& data_name, data_val_t v);

    data_val_t get(const SkString& data_name)const;

    inline data_key_t key()const {
        return k_;
    }

    /*! \brief 移除序列名指的数据
     *
     *    @param	data_name	数据名
     *    @return
     */
    void remove(const SkString& data_name);

    std::vector<SkString> datanames() const;

    inline bool empty()const {
        return d_.empty();
    }

private:
    // 通常是时间值
    data_key_t k_;

    // 序列数据
    // key: data name
    std::unordered_map<SkString, data_val_t> d_;
};

/*!
 * \class IDataSource
 *
 * \brief 数据源接口
 *
 */
class BASE_API DataSource {
    using Elem = my_sp<ColumnData>;
    using Containter = SkTArray<Elem>;

    enum Flags {
        kDataReady = 0x1,
    };

public:
    /*! \brief 添加数据 / 编辑数据
     *		如果key不存在则添加，已存在，则修改对应的value
     *
     *    @param	data_name	数据名，确保唯一！
     *    @param	datas	数据
     *    @return
     */
    void addData(const SkString&data_name, const std::map<data_key_t, data_val_t>& datas);

    /*! \brief 移除数据
     *		如果对应的列中数据已空，则column将会删除
     *
     *    @param	data_name	数据名
     *    @return
     */
    void removeData(const SkString&data_name);
    void removeData(const SkString&data_name, const SkTArray<data_key_t>& keys);

    /*! \brief 遍历指定的区间数据: [idx_begin, idx_end]
     *
     *    @param idx_begin >= 0 区间开始
     *    @param idx_end >= -1 区间结束: -1: 遍历到最后一个
     *    @param cb 数据回调
     *    @return
     */
    bool forIndexData(int32_t idx_begin, int32_t idx_end, std::function<bool(int32_t, const DataSource::Elem)> cb) const;

    bool keys(int32_t idx_begin, int32_t idx_end, SkTArray<data_key_t>& out) const;
    data_key_t key(int32_t idx)const;

    /*! \brief 查找某个key的index
     *	    传入索引的begin和end，可以有助于缩小查找范围
     *      如果传入了index范围，如果key在传入的index范围之外，则无法查找到！
     *
     *    @param
     *    @return   如果没有找到，返回-1，否则 >= 0
     */
    int keyIndex(data_key_t k, int32_t idx_begin = 0, int32_t idx_end = -1)const;

    inline bool ready()const {
        return flags_.test(Flags::kDataReady);
    }

    /*! \brief 查询dataname数据序列 idx索引处的值
     *
     *    @param    idx [-1, x] 如果为-1，代表取最后一个值。如果x大于total()，返回无效值
     *    @return   可能为无效值，调用者需要判断
     */
    data_val_t value(int32_t idx, const SkString& dataname)const;

    void beginTransaction();
    void endTransaction();

    inline int32_t total()const {
        return int32_t(all_.count());
    }

    /*! \brief 在下标区域内的names对应的data序列最大最小值
     *
     *    @param    names   数据序列
     *    @param    idx_begin，idx_end   数据序列区间
     *    @param    min,max [out]   数据序列区间的最大，最小值
     *    @return
     */
    bool getIndexMinMaxVal(const std::set<SkString>& names,
                           int32_t idx_begin, int32_t idx_end, data_val_t& min, data_val_t& max) const;

    const Elem get(int idx)const;

protected:
    Elem _get(data_key_t k, bool create);

    /*! \brief 删除ColumnData::empty() == true的元素
     *
     *    @param
     *    @return
     */
    void clearEmptyData();

    // 排序
    void sortData();

    /*! \brief 下标区间矫正
     *
     *    @param idx_begin,idx_end [in|out]
     *    @return   false 有无法矫正的错误值
     */
    bool validateIndex(int32_t &idx_begin, int32_t &idx_end)const;

private:
    // 按key顺序排序
    Containter all_;
    BitFlags flags_;
};

}
}
