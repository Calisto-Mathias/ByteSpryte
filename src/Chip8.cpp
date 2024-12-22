#include "Chip8.hpp"
        // For Header Definitions
#include "Constants.hpp"
        // For Required Constants

#include <array>
#include <cstring>
#include <fstream>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

Chip8::Chip8(std::string_view filePath)
        :
        index_register(0u),
        stack_pointer(0u),
        program_counter(0x200u),
        instruction(0u),
        delay_timer(0u),
        sound_timer(0u),
        keypad(0u) 
{
        InitializeMemory();
        LoadFonts();
        LoadRom(filePath);
}

void
Chip8::InitializeMemory()
{
        registers.fill(0u);
        memory.fill(0u);
        stack.fill(0u);
        display_buffer.fill(0u);
}

void
Chip8::LoadFonts()
{
        memcpy(memory.data(), Chip8::fontset.data(), FONTSET_SIZE);
}

void
Chip8::LoadRom(std::string_view filePath)
{
	// Open the file as a stream of binary and move the file pointer to the end
	std::ifstream file(filePath.data(), std::ios::binary | std::ios::ate);

	if (file.is_open())
	{
		// Get size of file and allocate a buffer to hold the contents
		std::streampos size = file.tellg();
		std::vector<char> buffer_vector;

		// Go back to the beginning of the file and fill the buffer
		file.seekg(0, std::ios::beg);
		file.read(buffer_vector.data(), size);
		file.close();

                memcpy(memory.data() + MEMORY_START_ADDRESS, buffer_vector.data(), size);
	}
}

void
Chip8::call_corresponding_function_for_opcode_starting_with_0()
{
        (this->*function_ptrs_starting_with_0.at(instruction))();
}

void
Chip8::call_corresponding_function_for_opcode_starting_with_e()
{
        (this->*function_ptrs_starting_with_8.at(instruction & 0x000Fu))();
}

void
Chip8::call_corresponding_function_for_opcode_starting_with_8()
{
        (this->*function_ptrs_starting_with_e.at(instruction & 0x00FFu))();
}

void
Chip8::call_corresponding_function_for_opcode_starting_with_f()
{
        (this->*function_ptrs_starting_with_f.at(instruction & 0x00FFu))();
}

typedef void (Chip8::*Chip8_Opcode_Function_Ptr)();
const std::array<Chip8_Opcode_Function_Ptr, NUMBER_OF_OPCODE_GROUPS> Chip8::function_ptrs = {
        &Chip8::call_corresponding_function_for_opcode_starting_with_0,
        &Chip8::op_1nnn,
        &Chip8::op_2nnn,
        &Chip8::op_3xkk,
        &Chip8::op_4xkk,
        &Chip8::op_5xy0,
        &Chip8::op_6xkk,
        &Chip8::op_7xkk,
        &Chip8::call_corresponding_function_for_opcode_starting_with_8,
        &Chip8::op_9xy0,
        &Chip8::op_annn,
        &Chip8::op_bnnn,
        &Chip8::op_cxkk,
        &Chip8::op_dxyn,
        &Chip8::call_corresponding_function_for_opcode_starting_with_e,
        &Chip8::call_corresponding_function_for_opcode_starting_with_f
};

const std::unordered_map<uint8_t, Chip8_Opcode_Function_Ptr> Chip8::function_ptrs_starting_with_0 = {
        std::pair<uint8_t, Chip8_Opcode_Function_Ptr>(0x00e0, &Chip8::op_00e0),
        std::pair<uint8_t, Chip8_Opcode_Function_Ptr>(0x00ee, &Chip8::op_00ee)
};

const std::unordered_map<uint8_t, Chip8_Opcode_Function_Ptr> Chip8::function_ptrs_starting_with_8 = {
        std::pair<uint8_t, Chip8_Opcode_Function_Ptr>(0x0, &Chip8::op_8xy0),
        std::pair<uint8_t, Chip8_Opcode_Function_Ptr>(0x1, &Chip8::op_8xy1),
        std::pair<uint8_t, Chip8_Opcode_Function_Ptr>(0x2, &Chip8::op_8xy2),
        std::pair<uint8_t, Chip8_Opcode_Function_Ptr>(0x3, &Chip8::op_8xy3),
        std::pair<uint8_t, Chip8_Opcode_Function_Ptr>(0x4, &Chip8::op_8xy4),
        std::pair<uint8_t, Chip8_Opcode_Function_Ptr>(0x5, &Chip8::op_8xy5),
        std::pair<uint8_t, Chip8_Opcode_Function_Ptr>(0x6, &Chip8::op_8xy6),
        std::pair<uint8_t, Chip8_Opcode_Function_Ptr>(0x7, &Chip8::op_8xy7),
        std::pair<uint8_t, Chip8_Opcode_Function_Ptr>(0xe, &Chip8::op_8xye),
};

const std::unordered_map<uint8_t, Chip8_Opcode_Function_Ptr> Chip8::function_ptrs_starting_with_e = {
        std::pair<uint8_t, Chip8_Opcode_Function_Ptr>(0xa1, &Chip8::op_exa1),
        std::pair<uint8_t, Chip8_Opcode_Function_Ptr>(0x9e, &Chip8::op_ex9e),
};

const std::unordered_map<uint8_t, Chip8_Opcode_Function_Ptr> Chip8::function_ptrs_starting_with_f = {
        std::pair<uint8_t, Chip8_Opcode_Function_Ptr>(0x07, &Chip8::op_fx07),
        std::pair<uint8_t, Chip8_Opcode_Function_Ptr>(0x0a, &Chip8::op_fx0a),
        std::pair<uint8_t, Chip8_Opcode_Function_Ptr>(0x15, &Chip8::op_fx15),
        std::pair<uint8_t, Chip8_Opcode_Function_Ptr>(0x18, &Chip8::op_fx18),
        std::pair<uint8_t, Chip8_Opcode_Function_Ptr>(0x1e, &Chip8::op_fx1e),
        std::pair<uint8_t, Chip8_Opcode_Function_Ptr>(0x29, &Chip8::op_fx29),
        std::pair<uint8_t, Chip8_Opcode_Function_Ptr>(0x33, &Chip8::op_fx33),
        std::pair<uint8_t, Chip8_Opcode_Function_Ptr>(0x55, &Chip8::op_fx55),
        std::pair<uint8_t, Chip8_Opcode_Function_Ptr>(0x65, &Chip8::op_fx65), 
};

const std::array<uint8_t, FONTSET_SIZE> fontset = {
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};