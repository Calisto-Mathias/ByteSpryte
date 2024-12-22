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


void
Chip8::op_00ee()
{
        // The stack pointer stores the current available position
        stack_pointer = stack_pointer - 2 < 0 ? 0 : stack_pointer - 2;
        program_counter = stack[stack_pointer] + stack[stack_pointer + 1] << 8;
}


void
Chip8::op_1nnn(const uint16_t& instruction)
{
        // Each instruction is 2 Bytes
        program_counter = instruction & 0x0FFFu;
}


void
Chip8::op_2nnn(const uint16_t& instruction)
{
        // Push Program Counter onto the stack
        stack[stack_pointer++] = static_cast<uint8_t>(program_counter & 0x00FFu);
        stack[stack_pointer++] = static_cast<uint8_t>((program_counter & 0xFF00u) >> 8);

        program_counter = instruction & 0x0FFFu; 
}


void
Chip8::op_3xkk(const uint16_t& instruction)
{
        const uint8_t register_number = static_cast<uint8_t>((instruction & 0x0F00u) >> 8);
        const uint8_t comparison_byte = static_cast<uint8_t>(instruction & 0x00FFu);

        if (registers[register_number] == comparison_byte)
                program_counter += 2;

        // We do not need to handle the other case as it will be handled in the function that describes
        // Each Clock Cycle
}


void
Chip8::op_4xkk(const uint16_t& instruction)
{
        const uint8_t register_number = static_cast<uint8_t>((instruction & 0x0F00u) >> 8);
        const uint8_t comparison_byte = static_cast<uint8_t>(instruction * 0x00FFu);

        if (registers[register_number] != comparison_byte)
                program_counter += 2;
}