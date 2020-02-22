#ifndef DEFINES_H
#define DEFINES_H

#if defined(_WIN32) || defined(__CYGWIN__) || defined(__MINGW32__)
	#ifdef PURPLE_COMPILING_LIBRARY
		#ifdef __GNUC__
			#define PURPLE_API __attribute__ ((dllexport))
		#else
			#define PURPLE_API __declspec(dllexport) 
		#endif
	#else
		#ifdef __GNUC__
			#define PURPLE_API __attribute__ ((dllimport))
		#else
			#define PURPLE_API __declspec(dllimport) 
		#endif
	#endif
	#define PURPLE_LOCAL
#else
	#if __GNUC__ >= 4
		#define PURPLE_API __attribute__ ((visibility ("default")))
		#define PURPLE_LOCAL  __attribute__ ((visibility ("hidden")))
	#else
		#define PURPLE_API
		#define PURPLE_LOCAL
	#endif
#endif

#define PURPLE_LITTLE_ENDIAN 1234
#define PURPLE_BIG_ENDIAN 4321

#ifndef PURPLE_ENDIANNESS
	#ifdef __linux__
		#include <endian.h>
		#define PURPLE_ENDIANNESS __BYTE_ORDER
	#else
		#if defined(__hppa__) || \
			defined(__m68k__) || defined(mc68000) || defined(_M_M68K) || \
			(defined(__MIPS__) && defined(__MISPEB__)) || \
			defined(__ppc__) || defined(__POWERPC__) || defined(_M_PPC) || \
			defined(__sparc__)
			#define PURPLE_ENDIANNESS PURPLE_BIG_ENDIAN
		#elif defined(__BYTE_ORDER) && __BYTE_ORDER == __LITTLE_ENDIAN || \
			defined(__ARMEL__) || \
			defined(__THUMBEL__) || \
			defined(__AARCH64EL__) || \
			defined(_MIPSEL) || defined(__MIPSEL) || defined(__MIPSEL__) || \
			defined(_M_IX86) || defined(_M_X64) || defined(_M_IA64) || defined(_M_ARM)
			#define PURPLE_ENDIANNESS PURPLE_LITTLE_ENDIAN
		#else
			#error "Cannot resolve target architecture endianness."
		#endif
	#endif 
#endif

#define PURPLE_SWAP_INT16(x) (((x) << 8) | ((x) >> 8))
#define PURPLE_SWAP_INT32(x) (((x) << 24) | (((x) & 0x00FF0000) >> 8) | (((x) & 0x0000FF00) << 8) | ((x) >> 24))
#define PURPLE_SWAP_INT64(x) (((x) << 56) |\
	(((x) & 0x000000000000FF00) << 40) | \
	(((x) & 0x0000000000FF0000) << 24) | \
	(((x) & 0x00000000FF000000) << 8) | \
	(((x) & 0x000000FF00000000) >> 8) | \
	(((x) & 0x0000FF0000000000) >> 24) | \
	(((x) & 0x00FF000000000000) >> 40) | \
	((x) >> 56))
#define PURPLE_SWAP_FLOAT(x) *(float*)PURPLE_SWAP_INT32(*(uint32_t*)&x)
#define PURPLE_SWAP_DOUBLE(x) *(double*)PURPLE_SWAP_INT64(*(uint64_t*)&x)

#if PURPLE_ENDIANNESS != PURPLE_LITTLE_ENDIAN
	#define PURPLE_LOCAL_TO_LITTLE_ENDIAN_INT16(x) PURPLE_SWAP_INT16(x)
	#define PURPLE_LOCAL_TO_LITTLE_ENDIAN_INT32(x) PURPLE_SWAP_INT32(x)
	#define PURPLE_LOCAL_TO_LITTLE_ENDIAN_INT64(x) PURPLE_SWAP_INT64(x)
#else
	#define PURPLE_LOCAL_TO_LITTLE_ENDIAN_INT16(x) x
	#define PURPLE_LOCAL_TO_LITTLE_ENDIAN_INT32(x) x
	#define PURPLE_LOCAL_TO_LITTLE_ENDIAN_INT64(x) x
#endif

#if PURPLE_ENDIANNESS != PURPLE_BIG_ENDIAN
	#define PURPLE_FORCE_BIG_ENDIAN_INT16(x) PURPLE_SWAP_INT16(x)
	#define PURPLE_FORCE_BIG_ENDIAN_INT32(x) PURPLE_SWAP_INT32(x)
	#define PURPLE_FORCE_BIG_ENDIAN_INT64(x) PURPLE_SWAP_INT64(x)
#else
	#define PURPLE_FORCE_BIG_ENDIAN_INT16(x) x
	#define PURPLE_FORCE_BIG_ENDIAN_INT32(x) x
	#define PURPLE_FORCE_BIG_ENDIAN_INT64(x) x
#endif

#if PURPLE_ENDIANNESS != PURPLE_BIG_ENDIAN
	#define PURPLE_FORCE_LITTLE_ENDIAN_INT16(x) x
	#define PURPLE_FORCE_LITTLE_ENDIAN_INT32(x) x
	#define PURPLE_FORCE_LITTLE_ENDIAN_INT64(x) x
#else
	#define PURPLE_FORCE_LTTLE_ENDIAN_INT16(x) PURPLE_SWAP_INT16(x)
	#define PURPLE_FORCE_LTTLE_ENDIAN_INT32(x) PURPLE_SWAP_INT32(x)
	#define PURPLE_FORCE_LTTLE_ENDIAN_INT64(x) PURPLE_SWAP_INT64(x)
#endif

#if PURPLE_ENDIANNESS == PURPLE_LITTLE_ENDIAN
	#define PURPLE_LITTLE_TO_LOCAL_ENDIAN16(x) x
	#define PURPLE_LITTLE_TO_LOCAL_ENDIAN32(x) x
	#define PURPLE_LITTLE_TO_LOCAL_ENDIAN64(x) x
	#define PURPLE_BIG_TO_LOCAL_ENDIAN16(x) PURPLE_SWAP_INT16(x)
	#define PURPLE_BIG_TO_LOCAL_ENDIAN32(x) PURPLE_SWAP_INT32(x)
	#define PURPLE_BIG_TO_LOCAL_ENDIAN64(x) PURPLE_SWAP_INT64(x)
#else
	#define PURPLE_LITTLE_TO_LOCAL_ENDIAN16(x) PURPLE_SWAP_INT16(x)
	#define PURPLE_LITTLE_TO_LOCAL_ENDIAN32(x) PURPLE_SWAP_INT32(x)
	#define PURPLE_LITTLE_TO_LOCAL_ENDIAN64(x) PURPLE_SWAP_INT64(x)
	#define PURPLE_BIG_TO_LOCAL_ENDIAN16(x) x
	#define PURPLE_BIG_TO_LOCAL_ENDIAN32(x) x
	#define PURPLE_BIG_TO_LOCAL_ENDIAN64(x) x
#endif

#ifdef __GNUC__
#define PURPLE_CDECL __attribute__((__cdecl__))
#define PURPLE_STDCALL __attribute__((__stdcall__))
#else
#define PURPLE_CDECL __cdecl
#define PURPLE_STDCALL __cdecl
#endif

#define PURPLE_CALL PURPLE_CDECL

#endif // DEFINES_H