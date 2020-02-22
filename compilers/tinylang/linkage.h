#ifndef LINKAGE_H
#define LINKAGE_H

namespace tinylang {

enum class LinkageType {
	Normal,		// Implemented at Purple level in local Purple assembly.
	Extern,		// Implemented at Purple level in external Purple assembly
	Native		// Implemented at native level, linked by local assembly
};

} // tinylang

#endif // LINKAGE_H