#pragma once

#include "siapi.h"
#include "unpacker.h"

namespace sia {
namespace resy {

// 提供原始的资源解包接口
class RESY_API Asset {
public:
    static bool read(const std::string& file, std::string& cont);
};

}
}
