#include "bytecodereader.h"

namespace purple {

BytecodeReader::BytecodeReader(const std::uint8_t* buf, std::size_t bufSize) 
	: m_Buf(buf), m_CurrPos(buf), m_BufSize(bufSize) {
}

void BytecodeReader::seekposUnsafe(std::intptr_t pos) {
	m_CurrPos = m_Buf + pos;
}

void BytecodeReader::readBlockUnsafe(void* outBuf, std::size_t n) {
	std::memcpy(outBuf, m_CurrPos, n);
	m_CurrPos += n;
}

void BytecodeReader::addposUnsafe(std::ptrdiff_t off) {
	m_CurrPos += off;
}

} // Purple