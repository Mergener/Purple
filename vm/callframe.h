#ifndef CALLFRAME_H
#define CALLFRAME_H

#include "nativetypes.h"

namespace purple {

struct CallFrame {
	PurpleNative* stackView;
};

} // Purple

#endif // CALLFRAME_H