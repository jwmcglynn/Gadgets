/*
 * vector2d.h
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

#ifndef _DONNER_CORE_VECTOR2D_H
#define _DONNER_CORE_VECTOR2D_H

#include "mathutil.h"
#include <ostream>

namespace dnr {
/*****************************************************************************/

template <typename T>
struct vector2d {
	T x;
	T y;
	
	struct defaults {
		static const vector2d<T> zero;
		static const vector2d<T> xaxis;
		static const vector2d<T> yaxis;
		static const vector2d<T> neg_xaxis;
		static const vector2d<T> neg_yaxis;
	};
	
	/*************************************************************************/
	// Constructors.
	
	vector2d() : x(0), y(0) { }
	
	// Standard constructors.
	vector2d(const vector2d<T>& other) : x(other.x), y(other.y) { }
	vector2d(T _x, T _y) : x(_x), y(_y) { }
	
	// Cast constructors.
	template <typename S>
	vector2d(const vector2d<S>& other) : x(other.x), y(other.y) { }
	
	template <typename S>
	vector2d(S _x, S _y) : x(_x), y(_y) { }
	
	/*************************************************************************/
	// Manipulators.
	
	/**
	 * Standard set.
	**/
	void set(const vector2d<T>& rhs) {
		x = rhs.x;
		y = rhs.y;
	}
	
	/**
	 * Individual set.
	**/
	void set(T _x, T _y) {
		x = _x;
		y = _y;
	}
	
	/**
	 * Set with cast.
	**/
	template <typename S>
	void set(const vector2d<S>& rhs) {
		x = rhs.x;
		y = rhs.y;
	}
	
	/**
	 * Individual set with cast.
	**/
	template <typename S>
	void set(S _x, S _y) {
		x = _x;
		y = _y;
	}
	
	bool equals(const vector2d<T>& rhs) const {
		return ::dnr::equals(x, rhs.x) && ::dnr::equals(y, rhs.y);
	}
	
	/**
	 * Get the length or square length of the vector.
	**/
	T length() const { return (T) sqrt(f64(x * x + y * y)); }
	T length_sq() const { return x * x + y * y; }
	
	/**
	 * Gets the distance between two vectors, assuming that each represents
	 * a point in space.
	**/
	T distance(const vector2d<T>& rhs) const { return (rhs - *this).length(); }
	T distance_sq(const vector2d<T>& rhs) const { return (rhs - *this).length_sq(); }
	
	/**
	 * Finds the dot product.
	**/
	T dot_product(const vector2d<T>& rhs) const { return x * rhs.x + y * rhs.y; }
	
	/**
	 * Rotate this vector.
	**/
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
	
	/**
	 * Get the angle that this vector makes with the +x axis, in radians.
	**/
	T angle() const {
		return (T) atan2(f64(y), f64(x));
	}
	
	/**
	 * Normalize this vector.
	**/
	vector2d<T>& normalize() {
		T len = length();
		
		if (iszero(len)) {
			x = T(0);
			y = T(0);
		} else {
			T mag = T(1.0) / len;
			x *= mag;
			y *= mag;
		}
		
		return *this;
	}
	
	/*************************************************************************/
	// Operators.
	
	vector2d<T> operator-() const { return vector2d<T>(-x, -y); }
	vector2d<T>& operator=(const vector2d<T>& rhs) { set(rhs); return *this; }
	
	template <typename S>
	vector2d<T>& operator=(const vector2d<S>& rhs) { set(rhs); return *this; }
	
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
// Defaults struct.

template <typename T> const vector2d<T> vector2d<T>::defaults::zero = vector2d<T>(T(0), T(0));

template <typename T> const vector2d<T> vector2d<T>::defaults::xaxis = vector2d<T>(T(1), T(0));
template <typename T> const vector2d<T> vector2d<T>::defaults::yaxis = vector2d<T>(T(0), T(1));

template <typename T> const vector2d<T> vector2d<T>::defaults::neg_xaxis = vector2d<T>(T(-1), T(0));
template <typename T> const vector2d<T> vector2d<T>::defaults::neg_yaxis = vector2d<T>(T(0), T(-1));

/*****************************************************************************/
// Helper typedefs.

typedef vector2d<f32> vector2df;
typedef vector2d<f64> vector2dd;

typedef vector2d<s32> vector2di;

/*****************************************************************************/
} // End of namespace dnr.

#endif // _DONNER_CORE_VECTOR2D_H.
