/*
 *  mathutil.h
 *  donner
 *
 *  Created by Jeff McGlynn on 10/5/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _DONNER_CORE_MATHUTIL_H
#define _DONNER_CORE_MATHUTIL_H

#include "types.h"
#include <cmath>

namespace donner {
/*****************************************************************************/

//! Rounding error constant often used when comparing f32 values.
const f32 ROUNDING_ERROR_32 = 0.000001f;
const f64 ROUNDING_ERROR_64 = 0.00000001f;

#ifdef PI // Make sure we don't collide with a define.
#undef PI
#endif
/// Constant for PI.
const f32 PI = 3.14159265359f;

/// Constant for reciprocal of PI.
const f32 RECIPROCAL_PI = 1.0f / PI;

/// Constant for half of PI.
const f32 HALF_PI = PI / 2.0f;

#ifdef PI64 // Make sure we don't collide with a define.
#undef PI64
#endif
/// Constant for 64-bit PI.
const f64 PI64 = 3.1415926535897932384626433832795028841971693993751;

/// Constant for 64-bit reciprocal of PI.
const f64 RECIPROCAL_PI64 = 1.0 / PI64;

/// 32-bit Constant for converting from degrees to radians.
const f32 DEGTORAD = PI / 180.0f;

/// 32-bit constant for converting from radians to degrees (formally known as GRAD_PI).
const f32 RADTODEG = 180.0f / PI;

/// 64-bit constant for converting from degrees to radians (formally known as GRAD_PI2).
const f64 DEGTORAD64 = PI64 / 180.0;

/// 64-bit constant for converting from radians to degrees.
const f64 RADTODEG64 = 180.0 / PI64;

/// Returns minimum of two values.
template<typename T>
inline const T& min_(const T& a, const T& b) {
	return a < b ? a : b;
}

/// Returns minimum of three values.
template<typename T>
inline const T& min_(const T& a, const T& b, const T& c) {
	return a < b ? min_(a, c) : min_(b, c);
}

/// Returns maximum of two values.
template<typename T>
inline const T& max_(const T& a, const T& b) {
	return a < b ? b : a;
}

/// Returns maximum of three values.
template<typename T>
inline const T& max_(const T& a, const T& b, const T& c) {
	return a < b ? max_(b, c) : max_(a, c);
}

/// Returns abs of two values.
template<typename T>
inline T abs_(const T& a) {
	return a < (T) 0 ? -a : a;
}

/// Rounds a double to an integer.
template<typename T>
inline T round_(T orig) {
	return (T) floor(f64(orig + 0.5));
}

/// Gets the sign of a number.
template<typename T>
inline T sign_(T orig) {
	if (orig < T(0)) return T(-1);
	else return T(1);
}

/**
 * Returns linear interpolation of a and b with ratio t.
 * 
 * @return a if t == 0, b if t == 1, and the linear interpolation else.
**/
template<typename T>
inline T lerp(const T& a, const T& b, const f32 t) {
	return T(a * (1.0f - t)) + (b * t);
}

/// Clamps a value between low and high.
template <typename T>
inline const T clamp(const T& value, const T& low, const T& high) {
	return min_(max_(value, low), high);
}

/// Returns if a equals b, taking possible rounding errors into account.
inline bool equals(const f32 a, const f32 b, const f32 tolerance = ROUNDING_ERROR_32) {
	return (a + tolerance >= b) && (a - tolerance <= b);
}

/// Returns if a equals b, taking possible rounding errors into account.
inline bool equals(const f64 a, const f64 b, const f64 tolerance = ROUNDING_ERROR_64) {
	return (a + tolerance >= b) && (a - tolerance <= b);
}

/// Returns if a equals b, taking possible rounding errors into account/
inline bool equals(const s32 a, const s32 b, const s32 tolerance = 0) {
	return (a + tolerance >= b) && (a - tolerance <= b);
}

/// Returns if a equals b, taking possible rounding errors into account/
inline bool equals(const u32 a, const u32 b, const u32 tolerance = 0) {
	return (a + tolerance >= b) && (a - tolerance <= b);
}

/// Returns if a equals zero, taking rounding errors into account.
inline bool iszero(const f32 a, const f32 tolerance = ROUNDING_ERROR_32) {
	return fabsf(a) <= tolerance;
}

/// Returns if a equals zero, taking rounding errors into account.
inline bool iszero(const f64 a, const f64 tolerance = ROUNDING_ERROR_64) {
	return fabs(a) <= tolerance;
}

/// Returns if a equals zero, taking rounding errors into account.
inline bool iszero(const s32 a, const s32 tolerance = 0) {
	return (a & 0x7FFFFFF) <= tolerance;
}

/// Returns if a equals zero, taking rounding errors into account.
inline bool iszero(const u32 a, const u32 tolerance = 0) {
	return a <= tolerance;
}

template<typename T>
inline T next_power_of_two(T n) {
	n--;
	
	/* Find the next power of two by ORing the highest bit into
	 * every lower spot, then adding one. */
	const u16 numBits = sizeof(T) * 8;
	for (int i = 1; i < numBits; i = i * 2) {
		n = n | (n >> i);
	}
	
	return n + 1;
}

/*****************************************************************************/
} // End of namespace donner.

#endif // _DONNER_CORE_MATH_H.
