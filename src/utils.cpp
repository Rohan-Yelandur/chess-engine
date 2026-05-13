#include "utils.h"

uint64_t reverse_bits(uint64_t b) {
    b = ((b & 0x5555555555555555ULL) << 1)  | ((b >> 1)  & 0x5555555555555555ULL);
    b = ((b & 0x3333333333333333ULL) << 2)  | ((b >> 2)  & 0x3333333333333333ULL);
    b = ((b & 0x0F0F0F0F0F0F0F0FULL) << 4) | ((b >> 4)  & 0x0F0F0F0F0F0F0F0FULL);
    return __builtin_bswap64(b);
}