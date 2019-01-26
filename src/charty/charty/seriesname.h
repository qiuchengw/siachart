#pragma once

#include "dict.h"

namespace sia {
namespace charty {

/*!
 * \class SeriesName
 *
 * \brief 序列名/数据名等的修整
 *
 */
class SeriesName {
public:
    /*! \brief
     *		对于指标公式的数据输出，其dataname要和用户手动添加的数据名区分开。
     *		我们对于指标公式生成的数据，加前缀: fml:$formula_name:$id:$dataname
     *    @param
     *    @return
     */
    static SkString formulaDataname(const SkString& formula_name, int32_t id, const SkString& dataname) {
        SkString ret;
        ret.appendf("fml:%s:%d:%s", formula_name.c_str(), id, dataname.c_str());
        return ret;
    }

    static SkString formulaSeriesName(const SkString& formula_name, const SkString& series_name, int32_t id) {
        SkString ret;
        ret.appendf("fml:%s:%s:%d", formula_name.c_str(), series_name.c_str(), id);
        return ret;
    }

    // 翻译序列的名字
    static SkString translateSeriesName(const SkString& name, Dict* dict) {
        SkString real_name = name;
        if (name.startsWith("fml:")) {
            // 我们要把指标序列的名字给提取出来
            SkTArray<SkString> out;
            SkStrSplit(name.c_str(), ":", &out);
            if (out.count() >= 4) {
                real_name = out[2];
            }
        }

        if (dict) {
            dict->get(real_name);
        }
        return real_name;
    };

    // 翻译数据的名字
    static SkString translateDataName(const SkString& name, Dict* dict) {
        SkString real_name = name;
        if (name.startsWith("fml:")) {
            // 我们要把指标序列的名字给提取出来
            SkTArray<SkString> out;
            SkStrSplit(name.c_str(), ":", &out);
            if (out.count() >= 4) {
                real_name = out[3]; // 第三个是
            }
        }

        if (dict) {
            dict->get(real_name);
        }
        return real_name;
    };

};

}
}
