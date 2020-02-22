#ifndef PURPLE_LIBCONF_H
#define PURPLE_LIBCONF_H

#define PURPLE_CALL

#define PURPLE_API

/*
	Cross compatibility with useful C++ features:
*/
#ifdef __cplusplus
#define C_LINKAGE extern "C"
#define EXTERN_C_BEGIN extern "C" {
#define EXTERN_C_END }

#define REINTERPRET(t, val) reinterpret_cast<t>(val);
#define CONVERT(t, val) static_cast<t>(val);
#define CONSTCAST(t, val) const_cast<t>(val);
#else
#define C_LINKAGE
#define EXTERN_C_BEGIN 
#define EXTERN_C_END

#define REINTERPRET(t, val) ((t)(val));
#define CONVERT(t, val) ((t)(val));
#define CONSTCAST(t, val) ((t)(val));
#endif 

#endif // PURPLE_LIBCONF_H