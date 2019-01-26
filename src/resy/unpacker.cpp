#include "unpacker.h"
#include <cstdint>
#include "minizip/unzip.h"

namespace sia {
namespace resy {

struct ZipFileInfo {
    std::string szFileName;
    // char szFileName[MAX_COMMENT + 1];
    // char szComment[MAX_COMMENT + 1];

    int32_t dwVersion = 0;
    int32_t dwVersionNeeded = 0;
    int32_t dwFlags = 0;
    int32_t dwCompressionMethod = 0;
    int32_t dwDosDate = 0;
    int32_t dwCRC = 0;
    int32_t dwCompressedSize = 0;
    int32_t dwUncompressedSize = 0;
    int32_t dwInternalAttrib = 0;
    int32_t dwExternalAttrib = 0;
    bool bFolder = false;
};


Unpacker::~Unpacker() {
    unload();
}

bool Unpacker::unload() {
    // files_.clear();

    unzCloseCurrentFile(hzip_);

    int nRet = unzClose(hzip_);
    hzip_ = nullptr;

    return (nRet == UNZ_OK);
}

bool Unpacker::loaded() const {
    return hzip_ != nullptr;
}

bool Unpacker::release2File(const std::string& name, const std::string& out) {

    return false;
}

bool Unpacker::extract2Mem(const std::string& name, std::string& out) {
    return readFile(name.c_str(), out);
}

bool Unpacker::load(const std::string& file) {
    unload();
    hzip_ = unzOpen(file.c_str());

    // 遍历所有的文件
//     unz_global_info info;
//     if (unzGetGlobalInfo(hzip_, &info) != UNZ_OK) {
//         return false;
//     }
//
//     int file_count = info.number_entry;
//     if (unzGoToFirstFile(hzip_) != UNZ_OK) {
//         return false;
//     }
//
//     while (unzGoToNextFile(hzip_) == UNZ_OK){
//         ZipFileInfo info;
//         if (!getFileInfo(info)) {
//             return false;
//         }
//
//         // 存储
//         std::string key = info.szFileName;
//         files_.insert({ key, info });
//     }
    return (hzip_ != nullptr);
}

bool Unpacker::getFileInfo(ZipFileInfo& info) {
    if (!hzip_) {
        return false;
    }

    unz_file_info uzfi;
    // ZeroMemory(&uzfi, sizeof(uzfi));
    memset(&uzfi, 0, sizeof(uzfi));

    const int32_t MAX_COMMENT = 255;
    char name[MAX_COMMENT + 1] = { 0 };
    char comment[MAX_COMMENT + 1] = { 0 };
    if (UNZ_OK != unzGetCurrentFileInfo(hzip_, &uzfi, name, MAX_COMMENT, nullptr, 0, comment, MAX_COMMENT)) {
        return false;
    }

    // copy across
    info.dwVersion = uzfi.version;
    info.dwVersionNeeded = uzfi.version_needed;
    info.dwFlags = uzfi.flag;
    info.dwCompressionMethod = uzfi.compression_method;
    info.dwDosDate = uzfi.dosDate;
    info.dwCRC = uzfi.crc;
    info.dwCompressedSize = uzfi.compressed_size;
    info.dwUncompressedSize = uzfi.uncompressed_size;
    info.dwInternalAttrib = uzfi.internal_fa;
    info.dwExternalAttrib = uzfi.external_fa;

    // replace filename forward slashes with backslashes
    int nLen = strlen(name);
    while (nLen--) {
        if (name[nLen] == '\\') {
            name[nLen] = '/';
        }
    }
    info.szFileName = name;

    // is it a folder?
    // info.bFolder = ((info.dwExternalAttrib & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY);

    return true;
}

bool Unpacker::readFile(const std::string& file, std::string& out) {
    // 先定位到这个文件
    if (!gotoFile(file)) {
        return false;
    }

    // 打开这个文件
    if (unzOpenCurrentFile(hzip_) != UNZ_OK) {
        return false;
    }

    // 读取内容
    int nRet = UNZ_OK;
    const int32_t BUFFERSIZE = 2048;
    char pBuffer[BUFFERSIZE];
    do {
        nRet = unzReadCurrentFile(hzip_, pBuffer, BUFFERSIZE);
        if (nRet > 0) {
            // 存到字符串中
            out.append(pBuffer, nRet);
        }
    } while (nRet > 0);

    unzCloseCurrentFile(hzip_);
    return (nRet == UNZ_OK);
}

bool Unpacker::gotoFile(const std::string& file) {
    if (!hzip_)
        return false;

    // 简单的定位下看看，能否找到
    if (unzLocateFile(hzip_, file.c_str(), 2) == UNZ_OK)
        return true;

    // 如果不能，则遍历所有的目录
    if (unzGoToFirstFile(hzip_) != UNZ_OK)
        return false;

    ZipFileInfo info;
    do {
        if (!getFileInfo(info))
            return false;

        if (file.compare(info.szFileName) == 0) {
            return true;
        }
    } while (unzGoToNextFile(hzip_) == UNZ_OK);

    // else
    return false;
}


}
}

