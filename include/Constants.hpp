#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <cstdint>
        // The cstdint library contains the necessary typedefs for portable code.

// Size Constants
const uint8_t NUMBER_OF_REGISTERS = 16;
const uint8_t MEMORY_SIZE = 4096;
const uint8_t SCREEN_WIDTH = 64;
const uint8_t SCREEN_HEIGHT = 32;
const uint8_t STACK_SIZE = 64;

// Important Registers
const uint8_t CARRY_REGISTER = 0xF;

#endif