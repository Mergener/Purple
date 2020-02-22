#ifndef ASSEMBLY_H
#define ASSEMBLY_H

#include <istream>
#include <vector>
#include <string>

#include "bytecodereader.h"
#include "callframe.h"

namespace purple {

class Assembly; // Forward decl for AssemblyDependency

struct AssemblyDependency {
	Assembly* asmDependency;
};

typedef void (*NativeFn)(CallFrame&);

/*
	Tuple containing a pointer to a purple-callable native function
	and it's internal mangled name.
*/
struct NativeFnHandle {
	std::string purpleFullName; // This function's full name from within the assembly
	NativeFn fnPtr;
};

/*
	A compiled Purple executable/library.
*/
class Assembly {
public:
	BytecodeReader getInstructionReader() const;

	/*
		Binds the specified native function to this assembly.
		Returns true if the specified native function name was found,
		false if this assembly does not find it (in this case, this function will have had
		no effect over this assembly).
	*/
	bool bindNativeFunc(NativeFn fnPtr, const char* fnPurpleFullName);

	inline const std::string& getName() const { return m_Name; }
	inline const std::vector<AssemblyDependency>& getDependencies() const { return m_Dependencies; }
	inline const std::vector<NativeFnHandle>& getNativeFuncs() const { return m_NativeFuncs; }

	Assembly(const std::string& asmName, const std::uint8_t* buf, std::size_t bufSize, bool allocCopy = true);
	~Assembly();

private:
	std::vector<AssemblyDependency> m_Dependencies;
	std::vector<NativeFnHandle> m_NativeFuncs;

	std::string m_Name;

	// Bytecode and instructions
	const std::uint8_t* m_BytecodeBuf = nullptr;
	const std::uint8_t* m_InstrStart = nullptr;
	std::size_t m_BytecodeBufSize = 0;

	void initialize();
};

} // Purple

#endif // ASSEMBLY_H