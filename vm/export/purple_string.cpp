#include "include/purple_string.h"

static constexpr size_t LEN_OFFSET = -1;

/******************************/

C_LINKAGE const char* purGetSzFromPurStr(const PurString* pstr) {
	return reinterpret_cast<const char*>(pstr);
}

/******************************/

C_LINKAGE size_t purGetLenFromPurStr(const PurString* pstr) {
	return *(reinterpret_cast<const size_t*>(pstr) + LEN_OFFSET);
}

/******************************/

C_LINKAGE void purStrRelease(PurString* pstr) {
}

/******************************/