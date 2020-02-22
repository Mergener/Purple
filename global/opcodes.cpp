#include "include/opcodes.h"

namespace purple {

const char* getOpcodeName(Opcode op) {
	switch (op) {
		case Opcode::CallNative: return "CallNative";
		case Opcode::CallExtern: return "CallExtern";
		case Opcode::CallLocal: return "CallLocal";
		case Opcode::Pushconst: return "Pushconst";
		case Opcode::Pushframe: return "Pushframe";
		case Opcode::Return: return "Return";
		case Opcode::Exit: return "Exit";
		default: return "Unknown Opcode";
	}
}

} // Purple