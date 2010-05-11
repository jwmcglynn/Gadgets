/*
 * lens_backend.h
 * Lens Refraction
 * 
 * Created by Jeff McGlynn on 11/27/08.
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

#ifndef _LENS_BACKEND_H
#define _LENS_BACKEND_H

typedef struct _cairo cairo_t;

#include "vector2d.h"
#include <vector>
using namespace donner;

static const f32 k_meters_to_pixels = 100.0f;
static const f32 k_lens_half_height = 1.0f; // Meters.

struct circle {
	vector2dd center;
	f64 radius;
	
	f64 theta_start;
	f64 theta_end;
	
	bool convex; // Is this a convex or concave segment?
	
	bool ray_intersect(vector2dd start, vector2dd dir, vector2dd& intersection, vector2dd& normal);
		
private:
	bool angle_in_range(f64 angle) {
		angle -= theta_start;
		f64 end = theta_end - theta_start;
		if (angle < -PI64) angle += 2.0 * PI64;
		
		if (end < 0.0) return (angle >= end && angle <= 0.0);
		else return (angle >= 0.0 && angle <= end);
	}
	
	bool ray_intersect_validate(vector2dd start, vector2dd dir, f64 t, vector2dd& intersection, vector2dd& normal) {
		if (t < 0.0) return false;
		
		intersection = start + t * dir;
		
		f64 angle = atan2(intersection.y, intersection.x);
		if (!angle_in_range(angle)) return false;
		
		normal.set(cos(angle), sin(angle));
		if (!convex) normal = -normal;
		
		intersection += center;
		return true;
	}
};

class lens_backend {
public:
	lens_backend()
		: m_left_radius(1.5f),
		  m_right_radius(1.5f),
		  m_inside_n(1.5f),
		  m_outside_n(1.0f) {
		
		// Build the presets array.
		m_presets.push_back(index("Vacuum", 1.0f));
		m_presets.push_back(index("Air @ STP", 1.0002926f));
		m_presets.push_back(index("Helium (0°C, 1 atm)", 1.000036f));
		m_presets.push_back(index("Hydrogen (0°C, 1 atm)", 1.000132f));
		m_presets.push_back(index("Benzene (20°C)", 1.501f));
		m_presets.push_back(index("Water (20°C)", 1.333f));
		m_presets.push_back(index("Ethyl Alcohol (20°C)", 1.361f));
		m_presets.push_back(index("Diamond", 2.419f));
		m_presets.push_back(index("Water Ice", 1.31f));
		m_presets.push_back(index("Silicon", 4.01f));
	}
	
	/**
	 * Called to render to the view.
	**/
	void render(cairo_t * context, const vector2dd& size);
	
	/**
	 * Called when the mouse is pressed.
	**/
	void click(const vector2dd& pos) { }
	
	/**
	 * Called when the mouse is moved when being held.
	**/
	void drag(const vector2dd& pos) { }
	
	/**
	 * Called when the mouse is released.
	**/
	void release(const vector2dd& pos) { }
	
	/*************************************************************************/
	
	/// Gets/sets the radius of the left of the lens.
	f32 left_radius() const { return m_left_radius; }
	void left_radius(f32 r) { m_left_radius = r; }
	
	/// Gets/sets the radius of the right of the lens.
	f32 right_radius() const { return m_right_radius; }
	void right_radius(f32 r) { m_right_radius = r; }
	
	/// Gets/sets the refractive index of the inside of the lens.
	f32 inside_n() const { return m_inside_n; }
	void inside_n(f32 n) { m_inside_n = n; }
	
	/// Gets/sets the refractive index of the outside of the lens.
	f32 outside_n() const { return m_outside_n; }
	void outside_n(f32 n) { m_outside_n = n; }
	
	/// Gets the list of presets for indices of refraction.
	struct index {
		index(std::string _name, f32 _n) : name(_name), n(_n) { }
		
		std::string name;
		f32 n;
	};
	
	typedef std::vector<index> index_array;
	const index_array& n_presets() { return m_presets; }
	
private:
	index_array m_presets;
	
	f32 m_left_radius;
	f32 m_right_radius;
	
	f32 m_inside_n;
	f32 m_outside_n;
};

#endif // _LENS_BACKEND_H
