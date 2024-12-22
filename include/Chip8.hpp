#ifndef CHIP8_HPP
#define CHIP8_HPP

#include <cstdint>
        // The cstdint header file holds the required definitions for portable typedefs
#include <array>
#include <unordered_map>
#include <string_view>

#include "Constants.hpp"
        // Contains all the constants related to the Chip-8 Interpreter

class Chip8 {
public:
        Chip8() = delete;
        Chip8(std::string_view filePath);
        ~Chip8() = default;

private:
        // Clear the display
        void op_00e0();
        
        // Return from a subroutine
        void op_00ee();
        
        // Jump to an instruction giving by the three bytes 0xnnn
        void op_1nnn();
        
        // Call a subroutine at location given by the three bytes 0xnnn
        void op_2nnn();
        
        // Skip next instruction if register x (0-based indexing) is equal to the bytes 0xkk
        void op_3xkk();
        
        // Skip next instruction if register x (0-based indexing) is not equal to the bytes 0xkk
        void op_4xkk();
        
        // skip next instruction if register x (0-based indexing) is equal to the register no. y
        void op_5xy0();
        
        // Set register x (0-based indexing) to bytes 0xkk
        void op_6xkk();
        
        // Add byte 0xkk to register x (0-based indexing)
        void op_7xkk();
        
        // Store the value of register y in register x
        void op_8xy0();
        
        // Store the bitwise OR of the two registers x and y in register x
        void op_8xy1();
        
        // Store the bitwise AND of the two registers x and y in the register x
        void op_8xy2();
        
        // Store the bitwise XOR of the two registers x and y in the register x
        void op_8xy3();
        
        // Store the addition of the two registers x and y in the register x
        // in register 15
        void op_8xy4();
        
        // Set register x to the difference between the registers x and y
        void op_8xy5();
        
        // Shift register x to the right by one bit and take the carry into the carry register
        void op_8xy6();
        
        // Subtract register y from register x and store it in register x. Carry bit should also be set
        void op_8xy7();
        
        // Shift the register x to the left by one bit and store the necessary information in the carry
        // bit
        void op_8xye();
        
        // Skip the next instruction if register x is not equal to register y
        void op_9xy0();
        
        // Set the index register to the value given by 0xnnn
        void op_annn();
        
        // Jump to location given by 0xnnn + register 0
        void op_bnnn();
        
        // Set the register x to be the bitwise AND operation between a random number and 0xkk
        void op_cxkk();
        
        // Display related instruction
        void op_dxyn();
        
        // Checks the current key and compares its value to register x. If its the same, then it skips
        // the next instruction
        void op_ex9e();
        
        // Checks the current key and compares its value to register x. If it is not the same, then it
        // skips the next instruction.
        void op_exa1();
        
        // Sets the register x to the delay timer value.
        void op_fx07();
        
        // Waits for a key press, and then stores the value in register x.
        void op_fx0a();
        
        // Sets the delay timer to the value of the register x.
        void op_fx15();
        
        // Sets the sound timer to the value of the register x.
        void op_fx18();
        
        // Add the index register to the register x and store the resulting value in the index register.
        void op_fx1e();
        
        void op_fx29();
        
        // Stores the BCD representation of the register x in locations I, I + 1, I + 2
        void op_fx33();
        
        // Stores the registers from register 0 to register x in locations starting from I. Then the 
        // index register is moved correspondingly.
        void op_fx55();
        
        // loads the registers from register 0 to register x starting with memory location indicated
        // by the index register. The index register is also moved correspondingly.
        void op_fx65();

        // Function Pointer Related Functions

        void call_corresponding_function_for_opcode_starting_with_0();
        void call_corresponding_function_for_opcode_starting_with_e();
        void call_corresponding_function_for_opcode_starting_with_8();
        void call_corresponding_function_for_opcode_starting_with_f();

        // Helper Functions
        void InitializeMemory();
        void LoadFonts();
        void LoadRom(std::string_view filePath);
        void InstructionCycle();

private:
        // Registers
        std::array<uint8_t, NUMBER_OF_REGISTERS> registers;
        uint16_t index_register;
        uint8_t stack_pointer;          // We only really need 6 bits out of these 8 bits.
        uint16_t program_counter;       // Used to store the current instruction address.
        uint16_t instruction;           // Used to store the current instruction (2 bytes).

        // Timers (to be decremented at a rate of 60 Hz)
        uint8_t delay_timer;
        uint8_t sound_timer;

        // Memory
        std::array<uint8_t, MEMORY_SIZE> memory;
        std::array<uint8_t, STACK_SIZE> stack;
                
        // Display and Keypad Buffers
        std::array<uint64_t, SCREEN_HEIGHT> display_buffer;
        uint16_t keypad;

        typedef void (Chip8::*Chip8_Opcode_Function_Ptr)();
        static const std::array<Chip8_Opcode_Function_Ptr, NUMBER_OF_OPCODE_GROUPS> function_ptrs;
        static const std::unordered_map<uint8_t, Chip8_Opcode_Function_Ptr> function_ptrs_starting_with_0;
        static const std::unordered_map<uint8_t, Chip8_Opcode_Function_Ptr> function_ptrs_starting_with_e;
        static const std::unordered_map<uint8_t, Chip8_Opcode_Function_Ptr> function_ptrs_starting_with_f;
        static const std::unordered_map<uint8_t, Chip8_Opcode_Function_Ptr> function_ptrs_starting_with_8;
        
        static const std::array<uint8_t, FONTSET_SIZE> fontset;
};

#endif
