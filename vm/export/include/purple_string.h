#ifndef PURPLE_STRING_H
#define PURPLE_STRING_H

#include "purple_libconf.h"

#include <stdlib.h>
#include <stdbool.h>

EXTERN_C_BEGIN

/*
	Represents the string data type that exists within Purple.
	Purple strings memory are automatically managed by the Purple GC. However,
	when references to Purple strings are exchanged between 
*/
typedef struct PurString_ PurString;

/*
	Generates a PurpleString from the specified C style string (null terminated).
	The contents of the given string are copied to a new buffer.
*/
PURPLE_API PurString* PURPLE_CALL purGetPurStrFromSz(const char* cstr);

/*
	Returns a C-usable null terminated string from a Purple_String.

	The returned string *is not a copy* and modifying it might cause undefined
	behaviour. If you want a copy, it is always possible to strcpy from the pointer
	returned by this function.
*/
PURPLE_API const char* PURPLE_CALL purGetSzFromPurStr(const PurString* pstr);

/*
	Allows the Purple GC to eventually release resources from the specified string.
	Note that the memory might not be released right after this call, but still, released
	PurpleStrings should not be used.
*/
PURPLE_API void PURPLE_CALL purStrRelease(PurString* pstr);

/*
	Returns the length of a Purple string.

	Has constant time complexity.
*/
PURPLE_API size_t PURPLE_CALL purGetLenFromPurStr(const PurString* pstr);

EXTERN_C_END

#endif // PURPLE_STRING_H