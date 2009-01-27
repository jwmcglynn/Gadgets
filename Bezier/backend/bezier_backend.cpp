/*
 * bezier_backend.cpp
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

#include "bezier_backend.h"

namespace dnr {
/*****************************************************************************/

static void draw_circle(cairo_t * context, const vector2dd& pos, f64 radius) {
	cairo_move_to(context, pos.x + radius, pos.y);
	cairo_arc(context, pos.x, pos.y, radius, 0.0, 2.0 * PI64);
}

void bezier_backend::render(cairo_t * context, const vector2dd& size) {
	// Draw black background.
	cairo_set_source_rgb(context, 0.0, 0.0, 0.0);
	cairo_paint(context);
	
	// Draw lines between the control points.
	cairo_set_source_rgb(context, 0.3, 0.3, 0.3);
	cairo_set_line_width(context, 2.0);
	
	cairo_move_to(context, m_curve.origin.x, m_curve.origin.y);
	cairo_line_to(context, m_curve.control_1.x, m_curve.control_1.y);
	cairo_line_to(context, m_curve.control_2.x, m_curve.control_2.y);
	cairo_line_to(context, m_curve.endpoint.x, m_curve.endpoint.y);
	cairo_stroke(context);
	
	
	// Draw bezier curve.
	cairo_set_source_rgb(context, 1.0, 1.0, 1.0);
	cairo_set_line_width(context, 3.0);
	cairo_move_to(context, m_curve.origin.x, m_curve.origin.y);
	cairo_curve_to(context,
		m_curve.control_1.x, m_curve.control_1.y,
		m_curve.control_2.x, m_curve.control_2.y,
		m_curve.endpoint.x, m_curve.endpoint.y
	);
	cairo_stroke(context);
	
	
	// Draw circles at each control point.
	cairo_set_source_rgb(context, 0.0, 0.5, 1.0);
	cairo_set_line_width(context, 2.0);
	
	const f64 radius = 4.0;
	
	draw_circle(context, m_curve.origin, radius);
	cairo_stroke(context);
	
	draw_circle(context, m_curve.control_1, radius);
	cairo_stroke(context);
	
	draw_circle(context, m_curve.control_2, radius);
	cairo_stroke(context);
	
	draw_circle(context, m_curve.endpoint, radius);
	cairo_stroke(context);
	
	
	// Draw tangent vector.
	vector2dd point = m_curve.get_point(m_t);
	
	vector2dd tangent = m_curve.get_tangent(m_t);
	tangent.normalize();
	tangent *= 50.0f; // Scale the vector for easy viewing.
	
	cairo_set_source_rgb(context, 1.0, 0.0, 0.0);
	cairo_move_to(context, point.x, point.y);
	cairo_rel_line_to(context, tangent.x, tangent.y);
	cairo_stroke(context);
	
	// Draw normal vector.
	vector2dd normal = m_curve.get_normal(m_t);
	normal.normalize();
	normal *= 50.0f; // Scale the vector for easy viewing.
	
	cairo_set_source_rgb(context, 0.0, 1.0, 0.0);
	cairo_move_to(context, point.x, point.y);
	cairo_rel_line_to(context, normal.x, normal.y);
	cairo_stroke(context);
}

/*****************************************************************************/
} // End of namespace dnr.
