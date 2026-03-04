#pragma once
#include "../imgui/imgui.h"
#include "C:\Libraries\imgui\logger\logger.h"
#include <cstdint>
inline uint64_t setBit(uint64_t x, const unsigned int pos, const bool value) {
    if (pos >= 64) {
        log(Error, "setBit Bit position out of range");
        return x;
    }

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
    if (pos >= 64) {
        log(Error, "setBitInPlace Bit position out of range");
        return;
    }

    // 0 = MSB, 63 = LSB
    const unsigned shift = 63 - pos;
    const uint64_t mask = 1ULL << shift;

    x = (x & ~mask) | (static_cast<uint64_t>(value) << shift);
}


inline int getBit(uint64_t x, const unsigned int pos) {
    if (pos >= 64) {
        log(Error, "getBit Bit position out of range");
        return 0;
    }
    return static_cast<int>((x >> (63 - pos)) & 1ULL);
}

inline uint64_t reverse(uint64_t x) {
    // Reverse bits in a 64-bit word (bit 0 <-> bit 63, etc.)
    x = ((x >> 1)  & 0x5555555555555555ULL) | ((x & 0x5555555555555555ULL) << 1);
    x = ((x >> 2)  & 0x3333333333333333ULL) | ((x & 0x3333333333333333ULL) << 2);
    x = ((x >> 4)  & 0x0F0F0F0F0F0F0F0FULL) | ((x & 0x0F0F0F0F0F0F0F0FULL) << 4);
    x = ((x >> 8)  & 0x00FF00FF00FF00FFULL) | ((x & 0x00FF00FF00FF00FFULL) << 8);
    x = ((x >> 16) & 0x0000FFFF0000FFFFULL) | ((x & 0x0000FFFF0000FFFFULL) << 16);
    x = (x >> 32) | (x << 32);
    return x;
}

inline void reverseInPlace(uint64_t &x) {
    x = reverse(x);
}





inline uint8_t setBit(uint8_t x, const unsigned int pos, const bool value) {
    if (pos >= 8) {
        log(Error, "setBit Bit position out of range");
        return x;
    }

    // 0 = MSB, 63 = LSB
    const unsigned shift = 7 - pos;
    const uint8_t mask = 1 << shift;

    if (value) {
        x |= mask;   // set to 1
    } else {
        x &= ~mask;  // set to 0
    }
    return x;
}

inline void setBitInPlace(uint8_t& x, const unsigned int pos, const bool value) {
    if (pos >= 8) {
        log(Error, "setBitInPlace Bit position out of range");
        return;
    }

    const unsigned shift = 7 - pos;
    const uint8_t mask = 1 << shift;

    x = (x & ~mask) | (static_cast<uint8_t>(value) << shift);
}


inline int getBit(uint8_t x, const unsigned int pos) {
    if (pos >= 8) {
        log(Error, "getBit Bit position out of range");
        return 0;
    }
    return static_cast<int>((x >> (7 - pos)) & 1);
}