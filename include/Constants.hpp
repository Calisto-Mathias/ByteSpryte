#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <cstdint>
        // The cstdint library contains the necessary typedefs for portable code.

// Size Constants
const uint8_t NUMBER_OF_REGISTERS = 16u;
const uint8_t MEMORY_SIZE = 4096u;
const uint8_t SCREEN_WIDTH = 64u;
const uint8_t SCREEN_HEIGHT = 32u;
const uint8_t STACK_SIZE = 64u;
const uint8_t NUMBER_OF_KEYS = 16u;

// Important Registers
const uint8_t CARRY_REGISTER = 0xFu;

// Other Important Sizes
const uint8_t FONTSET_SIZE = 80u;
const uint8_t SPRITE_WIDTH = 8u;
const uint8_t MAXIMUM_HORIZONTAL_INDEX_FOR_SCREEN_UPDATE = SCREEN_WIDTH - SPRITE_WIDTH;

// Miscellaneous
const uint8_t NUMBER_OF_OPCODES = 34u;
const uint8_t UNIQUE_FUNCTIONS = 12u;
const uint8_t FUNCTIONS_STARTING_WITH_0 = 2u;
const uint8_t FUNCTIONS_STARTING_WITH_8 = 9u;
const uint8_t FUNCTIONS_STARTING_WITH_E = 2u;
const uint8_t FUNCTIONS_STARTING_WITH_F = 9u;
const uint8_t NUMBER_OF_OPCODE_GROUPS = 16u;

// Memory Index Points
const uint16_t FONTSET_START_ADDRESS = 0u;
const uint16_t FONTSET_END_ADDRESS = 80u;
const uint16_t MEMORY_START_ADDRESS = 0x200u;

#endif