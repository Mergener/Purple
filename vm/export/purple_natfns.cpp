#include "include/purple_natfns.h"

#include "../callframe.h"

#include <cstring>

using namespace purple;

/******************************/

template <typename T>
static T callFrameGet(CallFrame* cf)
{
	T ret;

	static_assert(sizeof(T) <= 8, "Unsupported type requested for 'callFrameGet<T>'");

	if constexpr (sizeof(T) == 1) {
		ret = (T)cf->stackView[0].pByte;
	} else if constexpr (sizeof(T) == 2) {
		ret = (T)cf->stackView[0].pUshort;
	} else if constexpr(sizeof(T) == 4) {
		ret = (T)cf->stackView[0].pUint;
	} else if constexpr (sizeof(T) == 8) {
		ret = (T)cf->stackView[0].pUlong;
	}

	cf->stackView++;
	return ret;	
}

/******************************/

C_LINKAGE int8_t purCallFrameGetSByte(PurCallFrame* cf) {
	return callFrameGet<int8_t>(reinterpret_cast<CallFrame*>(cf));
}

/******************************/

C_LINKAGE uint8_t purCallFrameGetByte(PurCallFrame* cf) {
	return callFrameGet<uint8_t>(reinterpret_cast<CallFrame*>(cf));
}

/******************************/

C_LINKAGE int16_t purCallFrameGetShort(PurCallFrame* cf) {
	return callFrameGet<int16_t>(reinterpret_cast<CallFrame*>(cf));
}

/******************************/

C_LINKAGE uint16_t purCallFrameGetUShort(PurCallFrame* cf) {
	return callFrameGet<uint16_t>(reinterpret_cast<CallFrame*>(cf));
}

/******************************/

C_LINKAGE int32_t purCallFrameGetInt(PurCallFrame* cf) {
	return callFrameGet<int32_t>(reinterpret_cast<CallFrame*>(cf));
}

/******************************/

C_LINKAGE uint32_t purCallFrameGetUInt(PurCallFrame* cf) {
	return callFrameGet<uint32_t>(reinterpret_cast<CallFrame*>(cf));
}

/******************************/

C_LINKAGE int64_t purCallFrameGetLong(PurCallFrame* cf) {
	return callFrameGet<int64_t>(reinterpret_cast<CallFrame*>(cf));	
}

/******************************/

C_LINKAGE uint64_t purCallFrameGetULong(PurCallFrame* cf) {
	return callFrameGet<uint64_t>(reinterpret_cast<CallFrame*>(cf));
}

/******************************/

C_LINKAGE PurString_* purCallFrameGetStr(PurCallFrame* cf) {
	return callFrameGet<PurString_*>(reinterpret_cast<CallFrame*>(cf));
}

/******************************/

template <typename T>
static void callFrameWrite(CallFrame* f, T val) {
	static_assert(sizeof(T) <= sizeof(PurpleNative), "Unsupported type requested for 'callFrameWrite<T>'");

	std::memcpy(f->stackView, &val, sizeof(T));
}

/******************************/

C_LINKAGE void purCallFrameWriteSByte(PurCallFrame* cf, int8_t val) {
	callFrameWrite(reinterpret_cast<CallFrame*>(cf), val);
}

/******************************/

C_LINKAGE void purCallFrameWriteByte(PurCallFrame* cf, uint8_t val) {
	callFrameWrite(reinterpret_cast<CallFrame*>(cf), val);
}

/******************************/

C_LINKAGE void purCallFrameWriteShort(PurCallFrame* cf, int16_t val) {
	callFrameWrite(reinterpret_cast<CallFrame*>(cf), val);
}

/******************************/

C_LINKAGE void purCallFrameWriteUShort(PurCallFrame* cf, uint16_t val) {
	callFrameWrite(reinterpret_cast<CallFrame*>(cf), val);
}

/******************************/

C_LINKAGE void purCallFrameWriteInt(PurCallFrame* cf, int32_t val) {
	callFrameWrite(reinterpret_cast<CallFrame*>(cf), val);
}

/******************************/

C_LINKAGE void purCallFrameWriteUInt(PurCallFrame* cf, uint32_t val) {
	callFrameWrite(reinterpret_cast<CallFrame*>(cf), val);
}

/******************************/

C_LINKAGE void purCallFrameWriteLong(PurCallFrame* cf, int64_t val) {
	callFrameWrite(reinterpret_cast<CallFrame*>(cf), val);
}

/******************************/

C_LINKAGE void purCallFrameWriteULong(PurCallFrame* cf, uint64_t val) {
	callFrameWrite(reinterpret_cast<CallFrame*>(cf), val);
}

/******************************/

C_LINKAGE void purCallFrameWriteStr(PurCallFrame* cf, const PurString* str) {
	callFrameWrite(reinterpret_cast<CallFrame*>(cf), str);
}

/******************************/