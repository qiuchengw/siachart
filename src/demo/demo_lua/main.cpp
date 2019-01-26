#include <iostream>
#include <deps/sol2/sol.hpp>
#include "SkString.h"

bool parseCommon(sol::table& t) {
    for (auto i : t) {
        // std::cout << i;
    }
    return false;
}

bool loadFile(const SkString& file) {
    sol::state lua;

    lua.open_libraries(sol::lib::base);
    // 加载/检查/执行/检查结果
    sol::load_result code = lua.load_file(file.c_str());
    if (!code.valid() || !code().valid()) {
        return false;
    }

    // 先读全局的配置
    sol::table global = lua["global"];
    if (global.valid()) {
        if (!parseCommon(global)) {
            return false;
        }
    }

    // 读窗口自己的配置
    return true;
}


int main() {

    // loadFile(SkString("./paint.lua"));

    sol::state lua;
    lua.open_libraries(sol::lib::base);
    lua.script("myval='hello world! good!'");
    lua.script("print(myval)");

    std::string tmp;
    std::cin >> tmp;
    return 0;
}
