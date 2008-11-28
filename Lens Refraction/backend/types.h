/*
 *  types.h
 *  donner
 *
 *  Created by Jeff McGlynn on 10/3/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */
 
#ifndef _DONNER_CORE_TYPES_H
#define _DONNER_CORE_TYPES_H

namespace donner {
/*****************************************************************************/

/// 8 bit unsigned variable.
#ifdef _MSC_VER
typedef unsigned __int8		u8;
#else
typedef unsigned char		u8;
#endif

/// 8 bit signed variable.
#ifdef _MSC_VER
typedef __int8			s8;
#else
typedef signed char		s8;
#endif

/// 8 bit character variable.
typedef char			c8;



/// 16 bit unsigned variable.
#ifdef _MSC_VER
typedef unsigned __int16	u16;
#else
typedef unsigned short		u16;
#endif

/// 16 bit signed variable.
#ifdef _MSC_VER
typedef __int16			s16;
#else
typedef signed short		s16;
#endif



/// 32 bit unsigned variable.
#ifdef _MSC_VER
typedef unsigned __int32	u32;
#else
typedef unsigned int		u32;
#endif

/// 32 bit signed variable.
#ifdef _MSC_VER
typedef __int32			s32;
#else
typedef signed int		s32;
#endif



/// 32 bit floating point variable.
typedef float				f32;

/// 64 bit floating point variable.
typedef double				f64;


/// Insert a string literal and its length into a parameter list.
#define CONST_STR_LEN(str) str, ((sizeof(str) / sizeof(*str)) - 1)

/*****************************************************************************/
} // End of namespace donner.

#endif // _DONNER_CORE_TYPES_H.
