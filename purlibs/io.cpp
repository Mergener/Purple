#include "include/io.h"

#include <iostream>
#include <string>
#include <array>

C_LINKAGE void purple_io_print(PurCallFrame* c) {
	PurString* str = purCallFrameGetStr(c);

	purStrRelease(str);
}

/*****************************/

/*
	Our implementation of purple_io_read relies on putting strings that are read from stdin into buffers.
	To reduce heap allocation overhead, we'll be pooling some strings to be used by most calls to purple_io_read.
	To prevent race conditions, each string in the pool will contain a lock that indicates whether its being used or not.
	In case a string from the pool is being used, we fall back to creating a new std::string temporarily to receive
	input.
*/

static constexpr int STR_POOL_SIZE = 8;

struct StrPoolElement {
	std::string str;
	bool locked = false;
	int idx = -1;
};

static std::array<StrPoolElement, STR_POOL_SIZE> s_StrPool;

/*
	Returns a valid StrPoolElement, either from the pool or dynamically allocated.
	Release it with unlockCxxStr.
*/
static StrPoolElement& getCxxStrFromPool() {
	for (int i = 0; i < s_StrPool.size(); ++i) {
		if (!s_StrPool[i].locked) {
			s_StrPool[i].locked = true;
			return s_StrPool[i];
		}
	}
	return *(new StrPoolElement);
}

/*
	Unlocks a string from the pool or disposes resources for a string
	outside the pool.
*/
static void unlockCxxStr(StrPoolElement& s) {
	if (s.idx == -1) {
		delete &s;
	} else {
		s.locked = false;
	}
}

C_LINKAGE void purple_io_read(PurCallFrame* c) {
	auto& s = getCxxStrFromPool();
	s.str.clear();

	std::cin >> s.str;

	PurString* ret = purGetPurStrFromSz(s.str.c_str());
	purCallFrameWriteStr(c, ret);

	// After referencing a Purple String, we should always release references to it.
	purStrRelease(ret);

	unlockCxxStr(s);
}