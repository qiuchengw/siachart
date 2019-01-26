#pragma once

#include "formulapi.h"
#include "deps/talib/include/ta_libc.h"
#include "../base/datasource.h"

namespace sia {
namespace formula {


enum FORMULA_API TaInputDataIndex {
    kOpen = 0,
    kHigh = 1,
    kLow = 2,
    kClose = 3,
    kVolume = 4,
    kOi = 5,	// open interest
    // kStamp = 6,	// 暂时不支持
    kMaxInputIndex = kOi + 1,
};

enum FORMULA_API TaOutputDataIndex {
    kOut1 = 0,
    kOut2 = 1,
    kOut3 = 2,
    kOutInt = 3,
    kMaxOutputIndex = kOutInt,
};

enum class TaParamType {
    //
    kReal,
    kInt,

    //-------------------------
    // 数组数
    // open / high / low / close
    kPriceList,

    kIntList,

    // 某一个list，如 open 或 volume
    kRealList,
};

struct TaParamItem {
    //! 参数名
    SkString name;

    //! 显示名
    SkString display_name;

    //! 参数类型
    TaParamType typ;

    //! 可选参数
    bool opt = true;

    // TA_OptInputFlags         flags;

    // const void *dataSet;
    //! 默认值
    double     default_val;

    //! 提示
    SkString hint;

    //! 帮助
    SkString help_file;
};

class FORMULA_API TaData {
public:
    template<typename T>
    struct _NamedData {
        //! 数据名
        SkString name;

        //! 数据
        SkTArray<T> data;

        //! 公式执行完成后，输出的数据从哪个key开始有效
        // 输出的有效数据在data中是从0开始的。有效数据个数为[$count]
        // 数据和TaData::keys的对应关系是 TaData::keys[i+key_offset] = _NamedData[i]
        int key_offset = -1;

        //! 有效数据个数
        int count = 0;

        inline bool valid()const {
            return (-1 < key_offset) && !name.isEmpty() && (count > 0);
        }
    };

    const TA_Real* open()const {
        return open_.begin();
    }

    const TA_Real* high()const {
        return high_.begin();
    }

    const TA_Real* close()const {
        return close_.begin();
    }

    const TA_Real* low()const {
        return low_.begin();
    }

    // open interest ： 未平仓量
    const TA_Real* oi()const {
        return oi_.begin();
    }

    const TA_Real* volume() const {
        return volume_.begin();
    }

    /*! \brief 数组数据：open/high/low/close/oi/volume
     *
     *    @param	idx	请参考TaInputDataIndex值
     *    @return	数据
     */
    const TA_Real* indexData(uint16_t idx) const;

    int dataLength()const {
        return (int)open_.count();
    }

    TA_Integer* outInt() {
        return out_int_.data.begin();
    }

    TA_Real* outReal(TaOutputDataIndex idx);

    TA_Real* out1() {
        return out1_.data.begin();
    }

    TA_Real* out2() {
        return out2_.data.begin();
    }

    TA_Real* out3() {
        return out3_.data.begin();
    }

    /*! \brief 设置公式输出数据的名字和有效数据索引。
     *	        当公式运算完成后，会输出数据/信息，
     *          参考：TaItem::evalCall
     *          参考：TA_GetOutputParameterInfo/TA_OutputParameterInfo
     *
     *    @param    out_idx = TaOutputDataIndex, 指示out1/out2/out3/int
     *    @param    name    数据命名
     *    @param    valid_idx_begin 有效的key索引位置
     *    @param    valid_count 有效数据的个数
     *    @return
     */
    void setOut(TaOutputDataIndex out_idx, const SkString& name, int valid_idx_begin, int valid_count);

    /*! \brief 在真正的插入数据之前，调用此函数初始化内存空间，并记录key（时间）值
     *	        key 序列要和open/high... 等序列要完全的一一对应起来
     *
     *    @param    total   总的数据大小
     *              keys    时间序列数据
     *    @return
     */
    void beginInsert(int total, const SkTArray<chartbase::data_key_t>& keys);
    void setItem(TaInputDataIndex idx, int i, TA_Real val);
    void endInsert();

    /*! \brief 遍历有效的输出数据
     *
     *    @param    std::function<bool(const SkString& data_name, const std::map<data_key_t, data_val_t>& data)
     *    @return
     */
    void forValidOutData(std::function<bool(
                             const SkString&, const std::map<chartbase::data_key_t, chartbase::data_val_t>&)> )const;

protected:
    SkTArray<TA_Real>& indexData(TaInputDataIndex idx);

private:
    // input
    SkTArray<TA_Real> open_;
    SkTArray<TA_Real> high_;
    SkTArray<TA_Real> low_;
    SkTArray<TA_Real> close_;
    SkTArray<TA_Real> volume_;
    SkTArray<TA_Real> oi_;

    // output
    // 目前输出最多支持3个输出数组，其中只有一个int数组输出
    _NamedData<TA_Real> out1_;
    _NamedData<TA_Real> out2_;
    _NamedData<TA_Real> out3_;
    _NamedData<TA_Integer> out_int_;

    //! 时间轴数据
    SkTArray<chartbase::data_key_t> keys_;
};

}
}
