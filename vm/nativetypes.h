#ifndef NATIVETYPES_H
#define NATIVETYPES_H

#include <cstdint>

namespace purple {

union PurpleNative {

	std::uint8_t	pByte;
	std::uint16_t	pUshort;
	std::uint32_t	pUint;
	std::uint64_t	pUlong;
	std::int8_t		pSbyte;
	std::int16_t	pShort;
	std::int32_t	pInt;
	std::int64_t	pLong;
	std::intptr_t	pObj;
	float			pFloat;
	double			pDouble;
	
};

} // Purple

#endif // NATIVETYPES_H