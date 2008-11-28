/*
 *  vector2d.h
 *  donner
 *
 *  Created by Jeff McGlynn on 10/5/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _DONNER_CORE_VECTOR2D_H
#define _DONNER_CORE_VECTOR2D_H

#include "mathutil.h"
#include <iostream>

namespace donner {
/*****************************************************************************/

template <typename T>
struct vector2d {
	T x;
	T y;
	
	/*************************************************************************/
	// Constructors.
	
	vector2d() : x(0), y(0) { }
	vector2d(const vector2d<T>& other) : x(other.x), y(other.y) { }
	vector2d(T _x, T _y) : x(_x), y(_y) { }
	
	
	/*************************************************************************/
	// Manipulators.
	
	void set(const vector2d<T>& rhs) {
		x = rhs.x;
		y = rhs.y;
	}
	
	void set(T _x, T _y) {
		x = _x;
		y = _y;
	}
	
	bool equals(const vector2d<T>& rhs) const {
		return donner::equals(x, rhs.x) && donner::equals(y, rhs.y);
	}
	
	T length() const { return (T) sqrt(f64(x * x + y * y)); }
	T length_sq() const { return x * x + y * y; }
	
	T dot_product(const vector2d<T>& rhs) const { return x * rhs.x + y * rhs.y; }
	
	void rotate(f64 degrees) {
		degrees *= DEGTORAD64;
		rotate(vector2d<T>((T) cos(degrees), (T) sin(degrees)));
	}
	
	void rotate_rad(f64 rad) {
		rotate(vector2d<T>((T) cos(rad), (T) sin(rad)));
	}
	
	void rotate(const vector2d<T>& cos_sin) {
		set(x * cos_sin.x - y * cos_sin.y, x * cos_sin.y + y * cos_sin.x);
	}
	
	const vector2d<T>& normalize() {
		T inv_len = T(1.0) / length();
		x *= inv_len;
		y *= inv_len;
		
		return *this;
	}
	
	/*************************************************************************/
	// Operators.
	
	vector2d<T> operator-() const { return vector2d<T>(-x, -y); }
	vector2d<T>& operator=(const vector2d<T>& rhs) { set(rhs); return *this; }
	
	// Addition.
	vector2d<T> operator+(const vector2d<T>& rhs) const { return vector2d<T>(x + rhs.x, y + rhs.y); }
	vector2d<T>& operator+=(const vector2d<T>& rhs) { x += rhs.x; y += rhs.y; return *this; }
	vector2d<T> operator+(const T a) const { return vector2d<T>(x + a, y + a); }
	vector2d<T>& operator+=(const T a) { x += a; y += a; return *this; }
	friend vector2d<T> operator+(const T a, const vector2d<T>& other) { return vector2d<T>(a + other.x, a + other.y); }
	
	// Subtraction.
	vector2d<T> operator-(const vector2d<T>& rhs) const { return vector2d<T>(x - rhs.x, y - rhs.y); }
	vector2d<T>& operator-=(const vector2d<T>& rhs) { x -= rhs.x; y -= rhs.y; return *this; }
	vector2d<T> operator-(const T a) const { return vector2d<T>(x - a, y - a); }
	vector2d<T>& operator-=(const T a) { x -= a; y -= a; return *this; }
	friend vector2d<T> operator-(const T a, const vector2d<T>& other) { return vector2d<T>(a - other.x, a - other.y); }
	
	// Multiplication.
	vector2d<T> operator*(const vector2d<T>& rhs) const { return vector2d<T>(x * rhs.x, y * rhs.y); }
	vector2d<T>& operator*=(const vector2d<T>& rhs) { x *= rhs.x; y *= rhs.y; return *this; }
	vector2d<T> operator*(const T a) const { return vector2d<T>(x * a, y * a); }
	vector2d<T>& operator*=(const T a) { x *= a; y *= a; return *this; }
	friend vector2d<T> operator*(const T a, const vector2d<T>& other) { return vector2d<T>(a * other.x, a * other.y); }
	
	// Division.
	vector2d<T> operator/(const vector2d<T>& rhs) const { return vector2d<T>(x / rhs.x, y / rhs.y); }
	vector2d<T>& operator/=(const vector2d<T>& rhs) { x /= rhs.x; y /= rhs.y; return *this; }
	vector2d<T> operator/(const T a) const { return vector2d<T>(x / a, y / a); }
	vector2d<T>& operator/=(const T a) { x /= a; y /= a; return *this; }
	friend vector2d<T> operator/(const T a, const vector2d<T>& other) { return vector2d<T>(a / other.x, a / other.y); }
	
	// Comparison.
	bool operator<=(const vector2d<T>& rhs) const { return x <= rhs.x && y <= rhs.y; }
	bool operator>=(const vector2d<T>& rhs) const { return x >= rhs.x && y >= rhs.y; }
	
	bool operator<(const vector2d<T>& rhs) const { return x < rhs.x && y < rhs.y; }
	bool operator>(const vector2d<T>& rhs) const { return x > rhs.x && y > rhs.y; }
	
	bool operator==(const vector2d<T>& rhs) const { return equals(rhs); }
	bool operator!=(const vector2d<T>& rhs) const { return !equals(rhs); }
	
	// Ostream.
	friend std::ostream& operator<<(std::ostream& os, const vector2d<T>& vec) {
		os << "(" << vec.x << ", " << vec.y << ")";
		return os;
	}
};


/*****************************************************************************/
// Helper typedefs.

typedef vector2d<f32> vector2df;
typedef vector2d<f64> vector2dd;

typedef vector2d<s32> vector2di;
typedef vector2d<u32> vector2du;

/*****************************************************************************/
} // End of namespace donner.

#endif // _DONNER_CORE_VECTOR2D_H.
