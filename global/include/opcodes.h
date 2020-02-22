#ifndef OPCODES_H
#define OPCODES_H

#include <cstdint>

namespace purple {

enum class Opcode : std::uint16_t {

	CallLocal,
	CallExtern,
	CallNative,
	Return,

	Pushframe,
	Popframe,
	Pushconst,
	
	Add,
	Sub,
	Mul,
	Umul,
	Div,
	Udiv,

	Exit

};

const char* getOpcodeName(Opcode op);

} // Purple

#endif // OPCODES_H