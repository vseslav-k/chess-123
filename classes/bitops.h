#pragma once
#include <cstdint>
inline uint64_t setBit(uint64_t x, const unsigned int pos, const bool value) {
    /*if (pos >= 64) {
        log(Error, "setBit Bit position out of range");
        return x;
    }*/

    // 0 = MSB, 63 = LSB
    const unsigned shift = 63 - pos;
    const uint64_t mask = 1ULL << shift;

    if (value) {
        x |= mask;   // set to 1
    } else {
        x &= ~mask;  // set to 0
    }
    return x;
}

inline void setBitInPlace(uint64_t& x, const unsigned int pos, const bool value) {
    /*if (pos >= 64) {
        log(Error, "setBitInPlace Bit position out of range");
        return;
    }*/

    // 0 = MSB, 63 = LSB
    const unsigned shift = 63 - pos;
    const uint64_t mask = 1ULL << shift;

    x = (x & ~mask) | (static_cast<uint64_t>(value) << shift);
}

inline int getBit(uint64_t x, const unsigned int pos) {
    /*if (pos >= 64) {
        log(Error, "getBit Bit position out of range");
        return 0;
    }*/
    return static_cast<int>((x >> (63 - pos)) & 1ULL);
}