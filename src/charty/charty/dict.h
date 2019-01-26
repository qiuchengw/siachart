#pragma once

#include <unordered_map>
#include "chartapi.h"

namespace sia {
namespace charty {
/*!
 * \class Dict
 *
 * \brief 图表内部的序列数据名字含义字典
 *      字典是全局共享的，可以用于：
 *          1，序列数据名字解析（human readable）
 *          2，多语言
 */
class CHARTY_API Dict final {
    friend struct ChartManPrivate;

private:
    ~Dict();

public:
    /*! \brief 添加一个字典映射
     *
     *    @param    k  english key
     *    @param    v  human readable text
     */
    void add(const SkString& k, const SkString& v);
    void add(const std::map<SkString, SkString>& dict);

    /*! \brief 移除字典项目
     *
     *    @param	k	字典key
     *    @return
     */
    void remove(const SkString& k);

    /*! \brief 获取可读的文本
     *
     *    @param k  key
     *    @return human readable text
     */
    SkString get(const SkString& k)const;

private:
    // key: english name
    // value: human readable text
    std::unordered_map<SkString, SkString> dict_;
};
}
}
