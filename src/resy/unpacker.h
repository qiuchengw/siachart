#pragma once

#include <string>
#include "resyapi.h"

namespace sia {
namespace resy {

struct ZipFileInfo;

// 资源解包
class RESY_API Unpacker {
public:
    ~Unpacker();

    bool load(const std::string& file);
    bool unload();
    bool loaded()const;

    // 释放name 到 out路径
    bool release2File(const std::string& name, const std::string& out);

    // 解压到内存out中
    bool extract2Mem(const std::string& name, std::string& out);

private:
    bool getFileInfo(ZipFileInfo& info);
    bool readFile(const std::string& file, std::string& out);
    bool gotoFile(const std::string& file);

protected:
    void* hzip_ = nullptr;

    // 文件信息
    // std::unordered_map<std::string, ZipFileInfo> files_;
};
}
}
