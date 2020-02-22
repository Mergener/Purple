#ifndef PURPLE_NATIVEFNS_H
#define PURPLE_NATIVEFNS_H

#include "purple_libconf.h"

#include <stdint.h>

#include "purple_string.h"
#include "purple_obj.h"

EXTERN_C_BEGIN

/*
	Serves as a stream for reading parameters passed to a native function from the Purple VM
	as well as for inputting return values to the Purple Stack.
*/
typedef struct PurCallFrame_ PurCallFrame;

/*
	All functions below of type 'purCallFrameGet' extract values from a Purple Call Frame and return them,
	while all functions below of type 'purCallFrameWrite' write values to the Purple Call Frame.
	Purple Call Frame should be used like a stream of values.
	
	Important note: You should not use purCallFrameGet functions after using 
	purCallFrameWrite.
*/

PURPLE_API int8_t      PURPLE_CALL purCallFrameGetSByte(PurCallFrame* cf);
PURPLE_API uint8_t     PURPLE_CALL purCallFrameGetByte(PurCallFrame* cf);
PURPLE_API int16_t     PURPLE_CALL purCallFrameGetShort(PurCallFrame* cf);
PURPLE_API uint16_t    PURPLE_CALL purCallFrameGetUShort(PurCallFrame* cf);
PURPLE_API int32_t     PURPLE_CALL purCallFrameGetInt(PurCallFrame* cf);
PURPLE_API uint32_t    PURPLE_CALL purCallFrameGetUInt(PurCallFrame* cf);
PURPLE_API int64_t     PURPLE_CALL purCallFrameGetLong(PurCallFrame* cf);
PURPLE_API uint64_t    PURPLE_CALL purCallFrameGetULong(PurCallFrame* cf);
PURPLE_API PurString_* PURPLE_CALL purCallFrameGetStr(PurCallFrame* cf);

PURPLE_API void        PURPLE_CALL purCallFrameWriteSByte(PurCallFrame* cf, int8_t val);
PURPLE_API void        PURPLE_CALL purCallFrameWriteByte(PurCallFrame* cf, uint8_t val);
PURPLE_API void        PURPLE_CALL purCallFrameWriteShort(PurCallFrame* cf, int16_t val);
PURPLE_API void        PURPLE_CALL purCallFrameWriteUShort(PurCallFrame* cf, uint16_t val);
PURPLE_API void        PURPLE_CALL purCallFrameWriteInt(PurCallFrame* cf, int32_t val);
PURPLE_API void        PURPLE_CALL purCallFrameWriteUInt(PurCallFrame* cf, uint32_t val);
PURPLE_API void        PURPLE_CALL purCallFrameWriteLong(PurCallFrame* cf, int64_t val);
PURPLE_API void        PURPLE_CALL purCallFrameWriteULong(PurCallFrame* cf, uint64_t val);
PURPLE_API void        PURPLE_CALL purCallFrameWriteStr(PurCallFrame* cf, const PurString* str);

/*
	Function pointer type for functions that can be called from within the Purple VM.
*/
typedef void (*PurNativeFn)(PurCallFrame* cf);

EXTERN_C_END

#endif // PURPLE_NATIVEFNS_H