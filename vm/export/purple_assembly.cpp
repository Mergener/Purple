#include "include/purple_assembly.h"

#include "../assembly.h"

#include <string>
#include <fstream>
#include <iterator>

using namespace purple;

/******************************/

C_LINKAGE PurErrorCode purLoadAssemblyFromFile(PurAssembly** outAsm, const char* asmName, const char* filepath) {
	*outAsm = nullptr;
	Assembly* ret;

	try {
		std::ifstream stream(filepath, std::ios::binary);
		std::vector<std::uint8_t> tempBuf;

		tempBuf.reserve(4096);
		tempBuf.assign(std::istreambuf_iterator<char>(stream),
			std::istreambuf_iterator<char>());
		
		try {
			ret = new(std::nothrow) Assembly(asmName, reinterpret_cast<std::uint8_t*>(tempBuf.data()), tempBuf.size(), true);

			if (ret == nullptr) {
				return PUR_ERR_NOMEMORY;
			}

		} catch (const std::runtime_error&) {
			delete ret;
			return PUR_ERR_BAD_ASSEMBLY;
		}

		*outAsm = reinterpret_cast<PurAssembly*>(ret);

	} catch (const std::bad_alloc&) {
		return PUR_ERR_NOMEMORY;
	} catch (const std::ifstream::failure& f) {		
		return PUR_ERR_FILE_NOT_FOUND;
	}
	return PUR_ERR_SUCCESS;
}

/******************************/

C_LINKAGE bool purBindNativeFnToAssembly(PurAssembly* a, const char* fnFullName, PurNativeFn fnPtr) {
	if (a == nullptr) {
		return;
	}

	Assembly& as = *reinterpret_cast<Assembly*>(a);
	as.bindNativeFunc(reinterpret_cast<NativeFn>(fnPtr), fnFullName);
}

/******************************/