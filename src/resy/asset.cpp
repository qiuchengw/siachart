#include "asset.h"
#include "unpacker.h"

namespace sia {
namespace resy {
static Unpacker _unpacker;
static const std::string SIA_RES_DAT = "./lua.zip";

bool Asset::read(const std::string& file, std::string& cont) {
    if (!_unpacker.loaded()) {
        if (!_unpacker.load(SIA_RES_DAT)) {
            return false;
        }
    }

    return _unpacker.extract2Mem(file, cont);
}

}
}
