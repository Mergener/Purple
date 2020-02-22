#ifndef PURLIBS_IO_H
#define PURLIBS_IO_H

#include "purple_natfns.h"

EXTERN_C_BEGIN

PURPLE_API void PURPLE_CALL purple_io_print(PurCallFrame* c);
PURPLE_API void PURPLE_CALL purple_io_read(PurCallFrame* c);

EXTERN_C_END

#endif // PURLIBS_IO_H