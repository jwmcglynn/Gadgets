/*
 * aabbox.h
 * donner core
 * 
 * Created by Jeff McGlynn on 11/5/08.
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

#ifndef _DONNER_CORE_AABBOX_H
#define _DONNER_CORE_AABBOX_H

#include "types.h"
#include "mathutil.h"
#include "vector2d.h"

#include <ostream>

namespace dnr {
/*****************************************************************************/

template<typename T>
struct aabbox {
	vector2d<T> origin; /// Top left.
	vector2d<T> extent; /// Dimensions.
	bool active; /// Is this bbox in use?
	
	/*************************************************************************/
	
	aabbox() : active(false) { }
	aabbox(T _x, T _y, T _w, T _h) : origin(_x, _y), extent(_w, _h), active(true) { }
	aabbox(const vector2d<T>& _origin, const vector2d<T>& _extent) : origin(_origin), extent(_extent), active(true) { }
	aabbox(const aabbox& other) { *this = other; }
	
	aabbox<T>& operator=(const aabbox<T>& other) {
		origin = other.origin;
		extent = other.extent;
		active = other.active;
		
		return *this;
	}
	
	aabbox<T> operator-(const vector2d<T>& amt) const {
		aabbox<T> ret = *this;
		ret -= amt;
		return ret;
	}
	
	aabbox<T>& operator-=(const vector2d<T> amt) {
		origin -= amt;
		extent -= amt;
		
		return *this;
	}
	
	friend std::ostream& operator<<(std::ostream& os, const aabbox<T>& box) {
		if (box.active) {
			os << "[" << box.origin.x << ", " << box.origin.y << "] => [" << box.extent.x << ", " << box.extent.y << "]";
		} else {
			os << "[not active]";
		}
		
		return os;
	}
	
	/*************************************************************************/
	
	inline void deactivate() {
		active = false;
	}
	
	inline void reset(const vector2d<T>& start_pt) {
		active = true;
		origin.set(start_pt);
		extent.set(T(0), T(0));
	}
	
	/*************************************************************************/
	
	/**
	 * Add a point to the bounding box.
	 * 
	 * @param	x			x-coordinate.
	 * @param	y			y-coordinate.
	**/
	void add_internal_point(T x, T y) {
		if (!active) reset(vector2d<T>(x, y));
		else {
			add_internal_point_x(x);
			add_internal_point_y(y);
		}
	}
	
	/**
	 * Add a point to the bounding box.
	 * 
	 * @param	pt	Point to add.
	**/
	void add_internal_point(const vector2d<T>& pt) {
		if (!active) reset(pt);
		else {
			add_internal_point_x(pt.x);
			add_internal_point_y(pt.y);
		}
	}
	
	/**
	 * Add a x-coordinate point to the bounding box.
	 * 
	 * @param	x	x-coordinate.
	**/
	void add_internal_point_x(T x) {
		active = true;
		x -= origin.x; // Convert to bbox space.
		
		if (x > extent.x) extent.x = x; // Expand the bbox if it is too small.
		else if (x < T(0)) { // Move the origin if necessary.
			origin.x += x;
			extent.x -= x;
		}
	}
	
	/**
	 * Add a y-coordinate point to the bounding box.
	 * 
	 * @param	x	x-coordinate.
	**/
	void add_internal_point_y(T y) {
		active = true;
		y -= origin.y; // Convert to bbox space.
		
		if (y > extent.y) extent.y = y; // Expand the bbox if it is too small.
		else if (y < T(0)) { // Move the origin if necessary.
			origin.y += y;
			extent.y -= y;
		}
	}
	
	/**
	 * Adds a bounding box inside this bounding box.
	**/
	void add_internal_box(const aabbox<T>& box) {
		if (!box.active) return;
		if (!active) {
			*this = box;
			return;
		}
		
		add_internal_point(box.origin);
		add_internal_point(box.origin + box.extent);
	}
};

typedef aabbox<f64> aabboxd;

/*****************************************************************************/
} // End of namespace dnr.

#endif // _DONNER_CORE_AABBOX_H.
