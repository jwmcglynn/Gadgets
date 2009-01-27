/*
 * cubic_bezier.h
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

#ifndef _CUBIC_BEZIER_H
#define _CUBIC_BEZIER_H

#include <stdexcept>
#include "vector2d.h"
#include "aabbox.h"
using namespace dnr;

namespace dnr {
/*****************************************************************************/

struct cubic_bezier {
public:
	vector2dd origin;
	vector2dd control_1;
	vector2dd control_2;
	vector2dd endpoint;
	
	/*************************************************************************/
	// Constructor.
	cubic_bezier()
			: origin(-140.0, -140.0),
			  control_1(-100.0, 150.0),
			  control_2(0.0, -140.0),
			  endpoint(150.0, 50.0) {
	}
	
	/*************************************************************************/
	// Helper operators.
	
	vector2dd& operator[](size_t idx) {
		switch (idx) {
			case 0:		return origin;
			case 1:		return control_1;
			case 2:		return control_2;
			case 3:		return endpoint;
			default:
				throw std::out_of_range("invalid index");
		}
	}
	
	/*************************************************************************/
	// Info querying.
	
	/**
	 * Calculate the bounding box for this spline.
	**/
	aabboxd bounds() const {
		aabboxd box;
		box.reset(origin); // Absolute start point.
		box.add_internal_point(endpoint); // Absolute end point.
		
		// Find coefficients.
		/* In the form of at^2 + bt + c, reduced from the derivative of:
		 * (1 - t)^3 P_0 + 3(1 - t)^2 t P_1 + 3(1 - t) t^2 P_2 + t^3 P_3,
		 * 
		 * Which is:
		 * 3(P_1 - P_0)(1 - t)^2 + 6(P_2 - P_1) * t * (1 - t) + 3(P_3 - P_2)t^2
		 * 
		 * Reference: http://www.cs.sunysb.edu/~qin/courses/geometry/4.pdf
		 * http://en.wikipedia.org/wiki/Bezier_curve#Examination_of_cases
		 * http://www.cs.mtu.edu/~shene/COURSES/cs3621/NOTES/spline/Bezier/bezier-der.html
		**/
		vector2dd a_coeff = 3.0 * (-origin + 3.0 * control_1 - 3.0 * control_2 + endpoint);
		vector2dd b_coeff = 6.0 * ( origin + control_2 - 2.0 * control_1);
		vector2dd c_coeff = 3.0 * (-origin + control_1);
		
		// Add the x extrema.
		if (iszero(a_coeff.x)) {
			if (!iszero(b_coeff.x)) {
				f64 t = -c_coeff.x / b_coeff.x;
				if (t >= 0.0 && t <= 1.0) box.add_internal_point(get_point(t));
			}
		} else {
			// Solve using the quadratic formula.
			quadratic_solution<f64> res = solve_quadratic(a_coeff.x, b_coeff.x, c_coeff.x);
			
			if (res.has_solution) {
				if (res.solution[0] >= 0.0 && res.solution[0] <= 1.0) {
					box.add_internal_point(get_point(res.solution[0]));
				}
				
				if (res.solution[1] >= 0.0 && res.solution[1] <= 1.0) {
					box.add_internal_point(get_point(res.solution[1]));
				}
			}
		}
		
		// Add the y extrema.
		if (iszero(a_coeff.y)) {
			if (!iszero(b_coeff.y)) {
				f64 t = -c_coeff.y / b_coeff.y;
				if (t >= 0.0 && t <= 1.0) box.add_internal_point(get_point(t));
			}
		} else {
			// Solve using the quadratic formula.
			quadratic_solution<f64> res = solve_quadratic(a_coeff.y, b_coeff.y, c_coeff.y);
			
			if (res.has_solution) {
				if (res.solution[0] >= 0.0 && res.solution[0] <= 1.0) {
					box.add_internal_point(get_point(res.solution[0]));
				}
				
				if (res.solution[1] >= 0.0 && res.solution[1] <= 1.0) {
					box.add_internal_point(get_point(res.solution[1]));
				}
			}
		}
		
		return box;
	}
	
	/**
	 * Get the point at position t.
	 * 
	 * @param	t		Position on curve, between 0.0 and 1.0.
	**/
	vector2dd get_point(f64 t) const {
		const f64 rev_t = 1.0 - t;
		
		// Evaluate the Bernstein polynomial.
		return (
			rev_t * rev_t * rev_t * origin			// (1 - t)^3 * p_0
			+ 3.0 * t * rev_t * rev_t * control_1	// + 3t(1 - t)^2 * p_1
			+ 3.0 * t * t * rev_t * control_2		// + 3 t^2 (1 - t) * p_2
			+ t * t * t * endpoint					// + t^3 * p_3
		);
	}
	
	/**
	 * Get the tangent vector at position t on the curve.
	 * 
	 * @param	t		Position on spline, between 0.0 and 1.0.
	**/
	vector2dd get_tangent(f64 t) const {
		const f64 rev_t = 1.0 - t;
		
		/* The tangent of a bezier curve is proportional to its first
		 * derivative.  The derivative is:
		 * 
		 * 3(P_1 - P_0)(1 - t)^2 + 6(P_2 - P_1) * t * (1 - t) + 3(P_3 - P_2)t^2
		 * 
		 * Basically, the derivative of a cubic bezier curve is three times the
		 * difference between two quadratic bezier curves.
		 * 
		 * See bounds() for more details.
		**/
		const vector2dd p_1_0 = (control_1 - origin);
		const vector2dd p_2_1 = (control_2 - control_1);
		const vector2dd p_3_2 = (endpoint - control_2);
		
		return 3.0 * (
			rev_t * rev_t * p_1_0		// (1 - t)^2 * (P_1 - P_0)
			+ 2.0 * t * rev_t * p_2_1	// (1 - t) * t * (P_2 - P_1)
			+ t * t * p_3_2				// t^2 * (P_3 - P_2)
		);
	}
	
	/**
	 * Get the normal vector at position t on the curve.
	 * 
	 * @param	t		Position on spline, between 0.0 and 1.0.
	**/
	vector2dd get_normal(f64 t) const {
		const f64 rev_t = 1.0 - t;
		
		/* The normal of a bezier curve is proportional to its second
		 * derivative.  The second derivative is:
		 * 
		 * 6 [ (1 - t) * (P_2 - 2 P_1 + P_0) + t * (P_3 - 2 P_2 + P_1) ]
		**/
		const vector2dd p_2_1_0 = (control_2 - 2.0 * control_1 + origin);
		const vector2dd p_3_2_1 = (endpoint - 2.0 * control_2 + control_1);
		
		return 6.0 * (rev_t * p_2_1_0 + t * p_3_2_1);
	}
};

/*****************************************************************************/
} // End of namespace dnr.

#endif // _CUBIC_BEZIER_H.
