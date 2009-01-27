/*
 * types.h
 * donner core
 * 
 * Created by Jeff McGlynn on 10/3/08.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * 
 * Copyright (c) 2008 Jeff McGlynn.
 * 
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
**/
 
#ifndef _DONNER_CORE_TYPES_H
#define _DONNER_CORE_TYPES_H

#ifdef __GNUC__
	#include <stdint.h>
	#define HAS_STDINT
#endif

namespace dnr {
/*****************************************************************************/

// Visual C++ Compiler.
#ifdef _MSC_VER
	typedef unsigned __int8 u8;		/// 8 bit unsigned variable.
	typedef __int8 s8;				/// 8 bit signed variable.
	typedef unsigned __int16 u16;	/// 16 bit unsigned variable.
	typedef __int16 s16;			/// 16 bit signed variable.
	typedef unsigned __int32 u32;	/// 32 bit unsigned variable.
	typedef __int32 s32;			/// 32 bit signed variable.
	
// C99-compatible compiler.
#elif defined(HAS_STDINT)
	typedef uint8_t u8;				/// 8 bit unsigned variable.
	typedef int8_t s8;				/// 8 bit signed variable.
	typedef uint16_t u16;			/// 16 bit unsigned variable.
	typedef int16_t s16;			/// 16 bit signed variable.
	typedef uint32_t u32;			/// 32 bit unsigned variable.
	typedef int32_t s32;			/// 32 bit signed variable.
	
// Other compiler.
#else
	typedef unsigned char u8;		/// 8 bit unsigned variable.
	typedef signed char s8;			/// 8 bit signed variable.
	typedef unsigned short u16;		/// 16 bit unsigned variable.
	typedef signed short s16;		/// 16 bit signed variable.
	typedef unsigned int u32;		/// 32 bit unsigned variable.
	typedef signed int s32;			/// 32 bit signed variable.
	
#endif


typedef char c8;					/// 8 bit character variable.
typedef float f32;					/// 32 bit floating point variable.
typedef double f64;					/// 64 bit floating point variable.

/// Insert a string literal and its length into a parameter list.
#define CONST_STR_LEN(str) str, ((sizeof(str) / sizeof(*str)) - 1)

/*****************************************************************************/
} // End of namespace dnr.

#endif // _DONNER_CORE_TYPES_H.
