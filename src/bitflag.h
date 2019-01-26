#pragma once

typedef int32_t Bits;

class BitFlags final {
public:
    inline bool test(Bits b) const {
        return !!(flags & b);
    }

    inline bool testReset(Bits b) {
        bool ret = test(b);
        set(b, false);
        return ret;
    }

    inline void toggle(Bits b) {
        set(b, !test(b));
    }

    inline Bits set(Bits b, bool on = true) {
        if (on) {
            flags |= b;
        } else {
            flags &= ~b;
        }
        return flags;
    }

    void clear() {
        flags = 0;
    }

private:
    Bits flags = 0;
};
