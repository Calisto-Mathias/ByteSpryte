#ifndef CHIP8_HPP
#define CHIP8_HPP

#include <cstdint>
        // The cstdint header file holds the required definitions for portable typedefs
#include "Constants.hpp"
        // Contains all the constants related to the Chip-8 Interpreter

class Chip8 {
public:
        Chip8();
        ~Chip8();

private:
        void op_00e0(const uint16_t);
                // Clear the display
        void op_00ee(const uint16_t);
                // Return from a subroutine
        void op_1nnn(const uint16_t);
                // Jump to an instruction giving by the three bytes 0xnnn
        void op_2nnn(const uint16_t);
                // Call a subroutine at location given by the three bytes 0xnnn
        void op_3xkk(const uint16_t);
                // Skip next instruction if register x (0-based indexing) is equal to the bytes 0xkk
        void op_4xkk(const uint16_t);
                // Skip next instruction if register x (0-based indexing) is not equal to the bytes 0xkk
        void op_5xy0(const uint16_t);
                // skip next instruction if register x (0-based indexing) is equal to the register no. y
        void op_6xkk(const uint16_t);
                // Set register x (0-based indexing) to bytes 0xkk
        void op_7xkk(const uint16_t);
                // Add byte 0xkk to register x (0-based indexing)
        void op_8xy0(const uint16_t);
                // Store the value of register y in register x
        void op_8xy1(const uint16_t);
                // Store the bitwise OR of the two registers x and y in register x
        void op_8xy2(const uint16_t);
                // Store the bitwise AND of the two registers x and y in the register x
        void op_8xy3(const uint16_t);
                // Store the bitwise XOR of the two registers x and y in the register x
        void op_8xy4(const uint16_t);
                // Store the addition of the two registers x and y in the register x
                // in register 15
        void op_8xy5(const uint16_t);
                // Set register x to the difference between the registers x and y
        void op_8xy6(const uint16_t);
                // Shift register x to the right by one bit and take the carry into the carry register
        void op_8xy7(const uint16_t);
                // Subtract register y from register x and store it in register x. Carry bit should also be set
        void op_8xye(const uint16_t);
                // Shift the register x to the left by one bit and store the necessary information in the carry
                // bit
        void op_9xy0(const uint16_t);
                // Skip the next instruction if register x is not equal to register y
        void op_annn(const uint16_t);
                // Set the index register to the value given by 0xnnn
        void op_bnnn(const uint16_t);
                // Jump to location given by 0xnnn + register 0
        void op_cxkk(const uint16_t);
                // Set the register x to be the bitwise AND operation between a random number and 0xkk
        void op_dxyn(const uint16_t);
        void op_ex9e(const uint16_t);
                // Checks the current key and compares its value to register x. If its the same, then it skips
                // the next instruction
        void op_exa1(const uint16_t);
                // Checks the current key and compares its value to register x. If it is not the same, then it
                // skips the next instruction.
        void op_fx07(const uint16_t);
                // Sets the register x to the delay timer value.
        void op_fx0a(const uint16_t);
                // Waits for a key press, and then stores the value in register x.
        void op_fx15(const uint16_t);
                // Sets the delay timer to the value of the register x.
        void op_fx18(const uint16_t);
                // Sets the sound timer to the value of the register x.
        void op_fx1e(const uint16_t);
                // Add the index register to the register x and store the resulting value in the index register.
        void op_fx29(const uint16_t);
        void op_fx33(const uint16_t);
                // Stores the BCD representation of the register x in locations I, I + 1, I + 2
        void op_fx55(const uint16_t);
                // Stores the registers from register 0 to register x in locations starting from I. Then the 
                // index register is moved correspondingly.
        void op_fx65(const uint16_t);
                // loads the registers from register 0 to register x starting with memory location indicated
                // by the index register. The index register is also moved correspondingly.


private:
        // Registers
        uint8_t registers[NUMBER_OF_REGISTERS];
        uint16_t index_register;
        uint8_t stack_pointer;
                // We only really need 6 bits out of this 8 bit register.
        uint16_t program_counter;
                // We only really need 12 bits out of this 16 bit register.

        // Timers (to be decremented at a rate of 60 Hz)
        uint8_t delay_timer;
        uint8_t sound_timer;

        // Memory
        uint8_t memory[MEMORY_SIZE];
        uint8_t stack[STACK_SIZE];
                // Since the stack size is 64 bytes and each instruction address contains 2 bytes,
                // We can store 32 addresses but most programs only store 16 levels deep, although in this
                // setup, we can have upto 32 levels deep of subroutine-nesting.

        // Display
        bool display_buffer[SCREEN_WIDTH][SCREEN_HEIGHT];
                // A position given by (x, y) where x belongs to [0, SCREEN_WIDTH - 1] and y belongs to
                // [0, SCREEN_HEIGHT - 1] indicates that the pixel is on or off.

};

#endif