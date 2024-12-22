#include "Chip8.hpp"
// Include the header file
#include "Constants.hpp"
// Include the constants

#include <cstdint>
#include <cstring>
#include <random>

void
Chip8::op_00e0(const uint16_t instruction)
{
        display_buffer.fill(0u);
}

void
Chip8::op_00ee(const uint16_t instruction)
{
        // The stack pointer stores the current available position
        stack_pointer = stack_pointer - 2 < 0 ? 0 : stack_pointer - 2;
        program_counter = stack.at(stack_pointer) + stack.at(stack_pointer + 1) << 8;
}

void
Chip8::op_1nnn(const uint16_t instruction)
{
        // Each instruction is 2 Bytes
        program_counter = instruction & 0x0FFFu;
}

void
Chip8::op_2nnn(const uint16_t instruction)
{
        // Push Program Counter onto the stack
        stack.at(stack_pointer++) = static_cast<uint8_t>(program_counter & 0x00FFu);
        stack.at(stack_pointer++) = static_cast<uint8_t>((program_counter & 0xFF00u) >> 8);

        program_counter = instruction & 0x0FFFu;
}

void
Chip8::op_3xkk(const uint16_t instruction)
{
        const uint8_t register_number = static_cast<uint8_t>((instruction & 0x0F00u) >> 8);
        const uint8_t comparison_byte = static_cast<uint8_t>(instruction & 0x00FFu);

        if (registers.at(register_number) == comparison_byte)
                program_counter += 2;

        // We do not need to handle the other case as it will be handled in the function that describes
        // Each Clock Cycle
}

void
Chip8::op_4xkk(const uint16_t instruction)
{
        const uint8_t register_number = static_cast<uint8_t>((instruction & 0x0F00u) >> 8);
        const uint8_t comparison_byte = static_cast<uint8_t>(instruction * 0x00FFu);

        if (registers.at(register_number) != comparison_byte)
                program_counter += 2;
}

void
Chip8::op_5xy0(const uint16_t instruction)
{
        const uint8_t first_register_number = static_cast<uint8_t>((instruction & 0x0F00u) >> 8u);
        const uint8_t second_register_number = static_cast<uint8_t>((instruction & 0x00F0u) >> 4u);

        if (registers.at(first_register_number) == registers.at(second_register_number))
                program_counter += 2;
}

void
Chip8::op_6xkk(const uint16_t instruction)
{
        const uint8_t register_number = static_cast<uint8_t>((instruction & 0x0F00u) >> 8u);
        const uint8_t bytes = static_cast<uint8_t>(instruction & 0x00FFu);

        registers.at(register_number) = bytes;
}

void
Chip8::op_7xkk(const uint16_t instruction)
{
        const uint8_t register_number = static_cast<uint8_t>((instruction & 0x0F00u) >> 8u);
        const uint8_t bytes = static_cast<uint8_t>(instruction & 0x00FFu);

        registers.at(register_number) += bytes;
}

void
Chip8::op_8xy0(const uint16_t instruction)
{
        const uint8_t first_register = static_cast<uint8_t>((instruction & 0x0F00u) >> 8u);
        const uint8_t second_register = static_cast<uint8_t>((instruction & 0x00F0u) >> 4u);

        registers.at(first_register) = registers.at(second_register);
}

void
Chip8::op_8xy1(const uint16_t instruction)
{
        const uint8_t first_register = static_cast<uint8_t>((instruction & 0x0F00u) >> 8u);
        const uint8_t second_register = static_cast<uint8_t>((instruction & 0x00F0u) >> 4u);

        registers.at(first_register) |= registers.at(second_register);
}

void
Chip8::op_8xy2(const uint16_t instruction)
{
        const uint8_t first_register = static_cast<uint8_t>((instruction & 0x0F00u) >> 8u);
        const uint8_t second_register = static_cast<uint8_t>((instruction & 0x00F0u) >> 4u);

        registers.at(first_register) &= registers.at(second_register);
}

void
Chip8::op_8xy3(const uint16_t instruction)
{
        const uint8_t first_register = static_cast<uint8_t>((instruction & 0x0F00u) >> 8u);
        const uint8_t second_register = static_cast<uint8_t>((instruction & 0x00F0u) >> 4u);

        registers.at(first_register) ^= registers.at(second_register);
}

void
Chip8::op_8xy4(const uint16_t instruction)
{
        const uint8_t first_register = static_cast<uint8_t>((instruction & 0x0F00u) >> 8u);
        const uint8_t second_register = static_cast<uint8_t>((instruction & 0x00F0u) >> 4u);

        const uint16_t sum = registers.at(first_register) + registers.at(second_register);

        registers.at(CARRY_REGISTER) = sum > UINT8_MAX;
        registers.at(first_register) = static_cast<uint8_t>(sum & 0x00FFu);
}

void
Chip8::op_8xy5(const uint16_t instruction)
{
        const uint8_t first_register = static_cast<uint8_t>((instruction & 0x0F00u) >> 8u);
        const uint8_t second_register = static_cast<uint8_t>((instruction & 0x00F0u) >> 4u);

        registers.at(CARRY_REGISTER) = registers.at(first_register) > registers.at(second_register);
        registers.at(first_register) -= registers.at(second_register);
}

void
Chip8::op_8xy6(const uint16_t instruction)
{
        const uint8_t first_register = static_cast<uint8_t>((instruction & 0x0F00u) >> 8u);

        registers.at(CARRY_REGISTER) = registers.at(first_register) & 1;
        registers.at(first_register) >>= 1;
}

void
Chip8::op_8xy7(const uint16_t instruction)
{
        const uint8_t first_register = static_cast<uint8_t>((instruction & 0x0F00u) >> 8u);
        const uint8_t second_register = static_cast<uint8_t>((instruction & 0x00F0u) >> 4u);

        registers.at(CARRY_REGISTER) = registers.at(second_register) > registers.at(first_register);
        registers.at(first_register) = registers.at(second_register) - registers.at(first_register);
}

void
Chip8::op_8xye(const uint16_t instruction)
{
        const uint8_t register_number = static_cast<uint8_t>((instruction & 0x0F00u) >> 8u);

        const uint8_t NUMBER_OF_BITS = 16u;
        registers.at(CARRY_REGISTER) = registers.at(register_number) & (1 << (NUMBER_OF_BITS - 1));
        registers.at(register_number) <<= 1u;
}

void
Chip8::op_9xy0(const uint16_t instruction)
{
        const uint8_t first_register = static_cast<uint8_t>((instruction & 0x0F00u) >> 8u);
        const uint8_t second_register = static_cast<uint8_t>((instruction & 0x00F0u) >> 4u);

        if (registers.at(first_register) != registers.at(second_register))
                program_counter += 2;
}

void
Chip8::op_annn(const uint16_t instruction)
{
        index_register = instruction & 0x0FFFu;
}

void
Chip8::op_bnnn(const uint16_t instruction)
{
        const uint8_t REGISTER_NUMBER_FOR_INSTRUCTION = 0;
        program_counter = instruction & 0x0FFF + registers.at(REGISTER_NUMBER_FOR_INSTRUCTION);
}

void
Chip8::op_cxkk(const uint16_t instruction)
{
        const uint8_t register_number = static_cast<uint8_t>((instruction & 0x0F00u) >> 8u);
        const uint8_t bytes = static_cast<uint8_t>((instruction & 0x00FFu));

        const std::mt19937 mt{std::random_device{}()};
        std::uniform_int_distribution<uint8_t> dist{0, 255};
        const uint8_t random_byte = dist(mt);

        registers.at(register_number) = random_byte & bytes;
}

void
Chip8::op_ex9e(const uint16_t instruction)
{
        const uint8_t register_number = static_cast<uint8_t>((instruction & 0x0F00u) >> 8u);
        if (keypad & (1 << register_number) != 0)
                program_counter += 2;
}

void
Chip8::op_exa1(const uint16_t instruction)
{
        const uint8_t register_number = static_cast<uint8_t>((instruction && 0x0F00u) >> 8u);
        if (keypad & (1 << register_number) == 0)
                program_counter += 2;
}

void
Chip8::op_fx07(const uint16_t instruction)
{
        const uint8_t register_number = static_cast<uint8_t>((instruction && 0x0F00u) >> 8u);

        registers.at(register_number) = delay_timer;
}

void
Chip8::op_fx0a(const uint16_t instruction)
{
        const uint8_t register_number = static_cast<uint8_t>((instruction && 0x0F00u) >> 8u);

        bool key_pressed = false;

        for (int i = 0; i < 16; ++i)
        {
                if (keypad & (1 << i))
                {
                        registers.at(register_number) = i;
                        key_pressed = true;
                        break;
                }
        }

        if (!key_pressed)
                program_counter -= 2;
}

void
Chip8::op_fx15(const uint16_t instruction)
{
        const uint8_t register_number = static_cast<uint8_t>((instruction && 0x0F00u) >> 8u);

        delay_timer = registers.at(register_number);
}

void
Chip8::op_fx18(const uint16_t instruction)
{
        const uint8_t register_number = static_cast<uint8_t>((instruction && 0x0F00u) >> 8u);

        sound_timer = registers.at(register_number);
}

void
Chip8::op_fx1e(const uint16_t instruction)
{
        const uint8_t register_number = static_cast<uint8_t>((instruction && 0x0F00u) >> 8u);

        index_register += registers.at(register_number);
}

void
Chip8::op_fx29(const uint16_t instruction)
{
        const uint8_t register_number = static_cast<uint8_t>((instruction && 0x0F00u) >> 8u);

        index_register = registers.at(register_number) * 5;
}

void
Chip8::op_fx33(const uint16_t instruction)
{
        const uint8_t register_number = static_cast<uint8_t>((instruction && 0x0F00u) >> 8u);

        uint8_t value = registers.at(register_number);
        memory.at(index_register) = value / 100;
        memory.at(index_register + 1) = (value / 10) % 10;
        memory.at(index_register + 2) = value % 10;
}

void
Chip8::op_fx55(const uint16_t instruction)
{
        const uint8_t register_number = static_cast<uint8_t>((instruction && 0x0F00u) >> 8u);

        for (int i = 0; i <= register_number; ++i) {
                memory.at(index_register + i) = registers.at(i);
        }

        index_register += register_number + 1;
}

void
Chip8::op_fx65(const uint16_t instruction)
{
        const uint8_t register_number = static_cast<uint8_t>((instruction && 0x0F00u) >> 8u);

        for (int i = 0; i <= register_number; ++i) {
                registers.at(i) = memory.at(index_register + i);
        }

        index_register += register_number + 1;
}
