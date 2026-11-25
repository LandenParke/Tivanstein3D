// stb_bitwise.h
// Landen Parke

#ifndef INCLUDE_STB_BITWISE
#define INCLUDE_STB_BITWISE

#ifdef STB_BITWISE_IMPLEMENTATION
/************************************Includes***************************************/
#include <stdbool.h>
#include <stdint.h>
/************************************Includes***************************************/

/********************************Public Functions***********************************/
bool stb_bit_set(uint32_t data, uint8_t bit);
bool stb_bit_cleared(uint32_t data, uint8_t bit);
/********************************Public Functions***********************************/

// stb_bit_set
// Checks if bit in data is set
// Starts from 0, last possible bit is 31
// Returns: true [1] == set, false [0] == cleared
bool stb_bit_set(uint32_t data, uint8_t bit) {
    if (bit > 31) return false;
    uint32_t set = 0x0001 << bit;
    if (data & set) return true;
    return false;
}

// stb_bit_cleared
// Checks if bit in data is cleared
// Starts from 0, last possible bit is 31
// Returns: true [1] == cleared, false [0] == set
bool stb_bit_cleared(uint32_t data, uint8_t bit) {
    if (bit > 31) return false;
    uint32_t set = 0x0001 << bit;
    if (!(data & set)) return true;
    return false;
}

#endif // STB_BITWISE_IMPLEMENTATION

#endif // INCLUDE_STB_BITWISE
