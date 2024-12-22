#include "Chip8.hpp"
        // Include the header file
#include "Constants.hpp"
        // Include the constants

#include <cstdint>
#include <cstring>

void
Chip8::op_00e0()
{
        for (int i = 0; i < SCREEN_WIDTH; ++i) {
                memset(display_buffer[i], false, SCREEN_HEIGHT * sizeof(bool));
        }
}
