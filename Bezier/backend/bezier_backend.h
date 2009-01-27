/*
 * bezier_backend.h
 * Bezier
 * 
 * Created by Jeff McGlynn on 1/26/09.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * 
 * Copyright (c) 2009 Jeff McGlynn.
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

#ifndef _BEZIER_BACKEND_H
#define _BEZIER_BACKEND_H

#include <cairo/cairo.h>
#include "vector2d.h"
#include "cubic_bezier.h"
using namespace dnr;

/*****************************************************************************/

class bezier_backend {
private:
	bool m_needs_redraw;
	vector2dd * m_anchor;
	
	cubic_bezier m_curve;
	f64 m_t;
	
public:
	bezier_backend()
			: m_needs_redraw(true)
			, m_t(0.0)
			, m_anchor(0) { }
	
	/**
	 * Called to render to the view.
	**/
	void render(cairo_t * context, const vector2dd& size);
	
	/**
	 * Called when the mouse is pressed.
	**/
	void click(const vector2dd& pos, bool inside);
	
	/**
	 * Called when the mouse is moved when being held.
	**/
	void drag(const vector2dd& pos, bool inside);
	
	/**
	 * Called when the mouse is released.
	**/
	void release(const vector2dd& pos, bool inside);
	
	/*************************************************************************/
	// Accessors.
	
	/**
	 * Get the current position on the spline.
	**/
	f64 t() const { return m_t; }
	
	/**
	 * Set the position on the spline.
	**/
	void t(f64 _t) { m_t = _t; }
	
	/**
	 * Does the view need to be redrawn?
	**/
	bool needs_redraw() const { return m_needs_redraw; }
};

/*****************************************************************************/

#endif // _BEZIER_BACKEND_H.
