/*
 *  lens_backend.h
 *  Lens Refraction
 *
 *  Created by Jeff McGlynn on 11/27/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef _LENS_BACKEND_H
#define _LENS_BACKEND_H

#include <cairo/cairo.h>
#include "vector2d.h"
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
		  m_outside_n(1.0f) { }
	
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
	
	/// Sets the radius of the left of the lens.
	void set_left_radius(f32 r) { m_left_radius = r; std::cout << "Left radius = " << m_left_radius << std::endl; }
	
	/// Sets the radius of the right of the lens.
	void set_right_radius(f32 r) { m_right_radius = r; std::cout << "Right radius = " << m_right_radius << std::endl; }
	
	/// Sets the refractive index of the inside of the lens.
	void set_inside_n(f32 n) { m_inside_n = n; }
	
	/// Sets the refractive index of the outside of the lens.
	void set_outside_n(f32 n) { m_inside_n = n; }
	
private:
	f32 m_left_radius;
	f32 m_right_radius;
	
	f32 m_inside_n;
	f32 m_outside_n;
};

#endif _LENS_BACKEND_H
