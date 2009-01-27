/*
 * mathutil.h
 * donner core
 * 
 * Created by Jeff McGlynn on 10/5/08.
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

#ifndef _DONNER_CORE_MATHUTIL_H
#define _DONNER_CORE_MATHUTIL_H

#include "types.h"
#include <cmath>

namespace dnr {
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

// Rounds a double to an integer.
template<typename T>
inline T round_(T orig) {
	return (T) floor(f64(orig + 0.5));
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

template <typename T>
struct quadratic_solution {
	T solution[2];
	bool has_solution;
};

/**
 * Solve a quadratic equation.
 * 
 * @param a	First coefficient.
 * @param b	Second coefficient.
 * @param c	Third coefficient.
 * @return quadratic_solution, containing 0-2 solutions.
**/
template <typename T>
quadratic_solution<T> solve_quadratic(T a, T b, T c) {
	quadratic_solution<T> res;
	
	// b^2 - 4ac.
	T sqrt_content = b * b - 4.0 * a * c;
	
	// Check to see if any solutions exist.
	if (sqrt_content < T(0.0)) {
		res.has_solution = false;
		return res;
	}
	
	// Build up the equation.
	sqrt_content = sqrt(sqrt_content);
	
	// Solve the two conditions.
	res.solution[0] = (-b + sqrt_content) / (2.0 * a);
	res.solution[1] = (-b - sqrt_content) / (2.0 * a);
	res.has_solution = true;
	return res;
}

/**
 * Test if a variable is in a specific range, using an optimized technique
 * that requires only one branch.  Some compilers do this automatically.
 * 
 * It converts:
 * From: var >= 'a' && var <= 'z'
 *   To: unsigned(var - 'a') <= ('z' - 'a').
 * 
 * Use as a function:
 * if (in_range(var, 'a', 'z')) // ...
**/
#define in_range(var, start, end) ( \
	unsigned((var) - (start)) <= ((end) - (start)) \
)

/*****************************************************************************/
} // End of namespace dnr.

#endif // _DONNER_CORE_MATH_H.
