#if defined vsnprintf
#undef vsnprintf
#endif

#include <vector>
#include <iostream>
#include "resy/unpacker.h"
#include "resy/resman.h"
#include "SkTSort.h"
#include "SkTSearch.h"

using namespace sia::resy;
int test_unpacker() {
    Unpacker u;
    if (!u.load("./test.zip")) {
        return -1;
    }

    // json
    std::string j1;
    if (u.extract2Mem("a.json", j1)) {
        std::cout << "a.json: \n-------------\n" << j1;
    }

    // lua/b.lua
    std::string lu;
    if (u.extract2Mem("lua/b.lua", lu)) {
        std::cout << "/lua/b.lua: \n-------------\n" << lu;
    }

    // big
    if (u.extract2Mem("lua/big.lua", lu)) {
        std::cout << "/lua/big.lua: \n-------------\n" << lu;
    }
    return 0;
}

void test_res() {
    ResMan man;
}

template<typename T>
void print_array(const SkTArray<T>& it) {
    for (auto i : it) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

void test_skia_array() {
    SkTArray<int> it;
    it.push_back(1);
    it.push_back(3);
    it.push_back(2);
    it.push_back(3);
    it.push_back(4);
    it.push_back(3);

    print_array(it);

    // sort
    SkTQSort(it.begin(), (it.begin() + it.count() - 1), [](const int& i1, const int& i2)->bool {
        return i1 < i2;
    });
    print_array(it);

    // search
    int k = 4;
    int idx = SkTSearch(it.begin(), it.count(), k, sizeof(int), []( const int& elem, const int& k)->bool {
        return elem < k;
    });
    std::cout << "find: " << k << ", idx:" << idx << " , elem:" << ((idx>0)? it[idx] : 0) << std::endl;

    // remove
    auto i_end = std::remove_if(it.begin(), it.end(), [](int i)->bool {
        return i == 3;
    });
    it.pop_back_n(it.end() - i_end);

    print_array(it);
}

int main() {

    test_res();

    test_skia_array();

    return 0;
}

