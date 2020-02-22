#ifndef PURPLE_ASSEMBLY_H
#define PURPLE_ASSEMBLY_H

#include "purple_libconf.h"

#include <stdbool.h>

#include "purple_natfns.h"
#include "purple_error.h"

EXTERN_C_BEGIN

/*
	Holds all metadata and compiled bytecode for a Purple executable file or library.
*/
typedef struct PurAssembly_ PurAssembly;

/*
	Tries to load a purple assembly from the specified file.

	Parameters:
		outAsm - Pointer to a PurAssembly* variable that will receive the address of the loaded assembly.
		asmName - A name for the assembly being loaded. Can be null.
		filepath - The path to the assembly file.

	Returns:
		PUR_ERR_SUCCESS if assembly was succesfully loaded,
		PUR_ERR_NOMEMORY if the procedure ran out of memory to finish,
		PUR_ERR_FILE_NOT_FOUND if the assembly path was not found,
		PUR_ERR_UNEXPECTED_NULL if any of the 'outAsm' or 'filepath' was NULL.
*/
PURPLE_API PurErrorCode PURPLE_CALL purLoadAssemblyFromFile(PurAssembly** outAsm, const char* asmName, const char* filepath);

/*
	Tries to load a purple assembly from the specified file.

	Parameters:
		a - The assembly to bind the function to.
		fnFullName - The name, as stored in the assembly binary header*, of the function to be bounded.
		fnPtr - The pointer to the function to be bound.

	Returns:
		True if the function has been bounded succesfully to the assembly, false otherwise.
*/
PURPLE_API bool         PURPLE_CALL purBindNativeFnToAssembly(PurAssembly* a, const char* fnFullName, PurNativeFn fnPtr);

EXTERN_C_END

#endif // PURPLE_ASSEMBLY_H