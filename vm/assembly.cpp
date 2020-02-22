#include "assembly.h"

#include <cstring>

namespace purple {

BytecodeReader Assembly::getInstructionReader() const {
	if (m_BytecodeBuf == nullptr) {
		throw std::runtime_error("");
	}

	return BytecodeReader(m_InstrStart, m_BytecodeBufSize - (m_InstrStart - m_BytecodeBuf));
}

Assembly::Assembly(const std::string& name, const std::uint8_t* buf, std::size_t bufSize, bool allocCopy) 
	: m_Name(name) {
	if (allocCopy) {
		m_BytecodeBuf = new std::uint8_t[bufSize];
		std::memcpy(const_cast<std::uint8_t*>(m_BytecodeBuf), buf, bufSize);
	} else {
		m_BytecodeBuf = buf;
	}
	m_BytecodeBufSize = bufSize;

	initialize();
}

void Assembly::initialize() {
	constexpr size_t BUF_SIZE = 4096;
	const char* BIG_SYMBOL_ERROR_MSG = "On initialization of Assembly, tried to read symbol with excedingly large name.";

	char buffer[BUF_SIZE];

	BytecodeReader reader(m_BytecodeBuf, m_BytecodeBufSize);

	// Read instruction start offset
	std::uint64_t instrStartOffset;
	reader.readUnsafe(&instrStartOffset);
	m_InstrStart = m_BytecodeBuf + instrStartOffset;

	// Read all native functions dependencies
	std::uint32_t nativeFnDepCount;
	reader.readUnsafe(&nativeFnDepCount);

	for (int i = 0; i < nativeFnDepCount; ++i) {
		NativeFnHandle nfn;
		nfn.fnPtr = nullptr;
		
		// Symbol names are stored right after their 16 bit length values.
		std::uint16_t nameLen;
		reader.readUnsafe(&nameLen);

		if (nameLen >= BUF_SIZE) {
			throw std::runtime_error(BIG_SYMBOL_ERROR_MSG);
		}

		reader.readBlockUnsafe(buffer, nameLen);
		buffer[nameLen] = '\0';
		nfn.purpleFullName = std::string(buffer);

		m_NativeFuncs.push_back(nfn);
	}
}

bool Assembly::bindNativeFunc(NativeFn fnPtr, const char* fnPurpleFullName) {
	if (fnPtr == nullptr) {
		throw std::runtime_error("Tried to bind null fnPtr to assembly.");
	}

	for (auto& f : m_NativeFuncs) {
		if (!std::strcmp(fnPurpleFullName, f.purpleFullName.c_str())) {
			f.fnPtr = fnPtr;
			return true;
		}
	}

	return false;
}

Assembly::~Assembly() {
	delete[] m_BytecodeBuf;
}

} // Purple