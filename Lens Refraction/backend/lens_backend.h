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

class lens_backend {
public:
	lens_backend() { }
	
	/**
	 * Called to render to the view.
	**/
	void render(cairo_t * context, const vector2dd& size) { }
	
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
	void set_left_radius(f32 r) { m_left_radius = r; }
	
	/// Sets the radius of the right of the lens.
	void set_right_radius(f32 r) { m_right_radius = r; }
	
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
