#include <iostream>
#include <fstream>
#include <filesystem>

#include "deserializer/deserializer.hpp"
#include "deserializer/datatypes.hpp"
#include "deserializer/chunk.hpp"
#include "deserializer/instruction.hpp"
#include "deserializer/constant.hpp"

namespace fs = std::filesystem;


const char* instruction_names[] {
	"MOVE",
	"LOADK",
	"LOADBOOL",
	"LOADNIL",
	"GETUPVAL",
	"GETGLOBAL",
	"GETTABLE",
	"SETGLOBAL",
	"SETUPVAL",
	"SETTABLE",
	"NEWTABLE",
	"SELF",
	"ADD",
	"SUB",
	"MUL",
	"DIV",
	"MOD",
	"POW",
	"UNM",
	"NOT",
	"LEN",
	"CONCAT",
	"JMP",
	"EQ",
	"LT",
	"LE",
	"TEST",
	"TESTSET",
	"CALL",
	"TAILCALL",
	"RETURN",
	"FORLOOP",
	"FORPREP",
	"TFORLOOP",
	"SETLIST",
	"CLOSE",
	"CLOSURE",
	"VARARG",
};

inline void indent( uint16_t depth ) {
	for ( uint16_t i = 0; i < depth; ++i ) {
		std::cout << "    "; // why the fuck is a tab character 8 spaces
	}
}

inline void output_chunk( const chunk_t& chunk, uint16_t depth = 0 ) {
	indent( depth );
	std::cout << "Constants:\n";
	for ( l_int i = 0; i < chunk.constant_cnt; ++i ) {
		indent( depth );
		std::cout << '[' << i << "]: ";
		switch ( chunk.constants[i].type ) {
			case const_t::K_NIL:
				std::cout << "nil";
				break;

			case const_t::K_BOOLEAN:
				std::cout << ( chunk.constants[i].data_boolean ? "true" : "false" );
				break;

			case const_t::K_NUMBER:
				std::cout << chunk.constants[i].data_number;
				break;

			case const_t::K_STRING:
				std::cout << chunk.constants[i].data_string.data;
				break;
		}
		std::cout << '\n';
	}
	std::cout << '\n';

	indent( depth );
	std::cout << "Functions:\n";
	for ( l_int i = 0; i < chunk.function_cnt; ++i ) {
		indent( depth + 1 );
		std::cout << '[' << i << "]\n";
		output_chunk( chunk.functions[i], depth + 1 );
	}
	std::cout << '\n';

	indent( depth );
	std::cout << "Instructions:\n";
	for ( l_int i = 0; i < chunk.instruction_cnt; ++i ) {
		indent( depth );
		std::cout << '[' << i << "] (" << chunk.instructions[i].line << "): ";
		std::cout << instruction_names[ chunk.instructions[i].opcode ] << ' ';
		// fuck you lua
		switch ( chunk.instructions[i].type ) {
			case instr_t::i_ABC:
				std::cout
					<< chunk.instructions[i].a << ' '
					<< chunk.instructions[i].b << ' '
					<< chunk.instructions[i].c;
				break;

			case instr_t::i_ABx:
			case instr_t::i_AsBx:
				std::cout
					<< chunk.instructions[i].a << ' '
					<< chunk.instructions[i].b;
				break;
		}
		std::cout << '\n';
	}
	std::cout << '\n';
}

int main( int argc, char** argv ) {
	if ( argc != 2 ) {
		std::cerr << "Usage: luac_viewer.exe <input>" << std::endl;
		return 1;
	}

	fs::path infile_p = argv[1];
	if ( !fs::is_regular_file( infile_p ) ) {
		std::cerr << "Provided path is not a file" << std::endl;
		throw "INVALID_PATH";
	}

	std::ifstream infile( infile_p.c_str(), std::ios::in | std::ios::binary );
	if ( !infile ) {
		std::cerr << "Could not open file for reading" << std::endl;
		throw "ACCESS_DENIED";
	}

	std::cout << "Deserializing...\n\n";
	chunk_t tl_chunk = decode_file( infile );

	output_chunk( tl_chunk );
}
