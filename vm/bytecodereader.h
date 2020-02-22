#ifndef BYTECODEREADER_H
#define BYTECODEREADER_H

#include <cstdint>
#include <cstring>

namespace purple {

class BytecodeReader {
public:
	BytecodeReader() = default;
	BytecodeReader(const std::uint8_t* buf, std::size_t bufSize);

	template <typename T>
	void readUnsafe(T* outBuf) {
		*outBuf = *reinterpret_cast<const T*>(m_CurrPos);
		m_CurrPos += sizeof(T);
	}

	void readBlockUnsafe(void* outBuf, std::size_t n);

	void seekposUnsafe(std::intptr_t pos);

	void addposUnsafe(std::ptrdiff_t off);

private:
	const std::uint8_t* m_Buf = nullptr;
	const std::uint8_t* m_CurrPos = nullptr;
	std::size_t m_BufSize = 0;
};

} // Purple

#endif // BYTECODEREADER_H