/*
 *  lens_backend.cpp
 *  Lens Refraction
 *
 *  Created by Jeff McGlynn on 11/27/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "lens_backend.h"

bool circle::ray_intersect(vector2dd start, vector2dd dir, vector2dd& intersection, vector2dd& normal) {
	start -= center; // Change the coordinate to be in circle-space.
	dir.normalize();
	
	f64 a = 1.0; // Length of dir.
	f64 b = 2.0 * start.dot_product(dir);
	f64 c = start.length_sq() - radius * radius;
	
	if (b * b < 4.0 * a * c) return false; // sqrt is negative.
	
	// Apply the quadratic formula to find t.
	f64 t_1 = (-b + sqrt(b * b - 4.0 * a * c)) / (2.0 * a);
	f64 t_2 = (-b - sqrt(b * b - 4.0 * a * c)) / (2.0 * a);
	
	if (t_2 < t_1) {
		// Swap t_1 and t_2 ...
		f64 tmp = t_1;
		t_1 = t_2;
		t_2 = tmp;
	}
	
	if (ray_intersect_validate(start, dir, t_1, intersection, normal)) return true;
	if (ray_intersect_validate(start, dir, t_2, intersection, normal)) return true;
	
	return false;
}

/**
 * Called to render to the view.
**/
void lens_backend::render(cairo_t * context, const vector2dd& size) {
	vector2dd center(size * 0.5);
	/*
	cairo_set_source_rgb(context, 1.0, 1.0, 1.0);
	cairo_move_to(context, center.x, center.y + k_lens_half_height * k_meters_to_pixels);
	
	// Left half of the lens.
	circle left_circle;
	
	{
		left_circle.radius = abs_(m_left_radius);
		
		f32 theta = asin(k_lens_half_height / left_circle.radius);
		f32 arc_w = left_circle.radius * (1.0 - abs_(cos(theta)));
		
		// Draw a concave curve with lines connecting it to the horizontal center line.
		if (m_left_radius < 0.0f) {
			left_circle.center = center;
			left_circle.center.x += m_left_radius * k_meters_to_pixels;
			
			cairo_rel_line_to(context, -arc_w * k_meters_to_pixels, 0.0f);
			cairo_arc_negative(context, left_circle.center.x, left_circle.center.y, left_circle.radius * k_meters_to_pixels, theta, -theta);
			cairo_line_to(context, center.x, center.y - k_lens_half_height * k_meters_to_pixels);
			
			left_circle.theta_start = -theta;
			left_circle.theta_end = theta;
			left_circle.convex = false;
			
		// Draw a convex curve.
		} else {
			left_circle.center = center;
			left_circle.center.x += (m_left_radius - arc_w) * k_meters_to_pixels;
			
			cairo_arc(context, left_circle.center.x, left_circle.center.y, m_left_radius * k_meters_to_pixels, PI64 - theta, PI64 + theta);
			left_circle.theta_start = PI64 - theta;
			left_circle.theta_end = PI64 + theta;
			
			left_circle.convex = true;
		}
		
		left_circle.center /= k_meters_to_pixels;
	}
	
	
	// Right half of the lens.
	circle right_circle;
	
	{
		right_circle.radius = abs_(m_right_radius);
		
		f32 theta = asin(k_lens_half_height / right_circle.radius);
		f32 arc_w = right_circle.radius * (1.0 - abs_(cos(theta)));
		
		// Draw a concave curve with lines connecting it to the horizontal center line.
		if (m_right_radius < 0.0f) {
			right_circle.center = center;
			right_circle.center.x -= m_right_radius * k_meters_to_pixels;
			
			cairo_rel_line_to(context, arc_w * k_meters_to_pixels, 0.0f);
			cairo_arc_negative(context, right_circle.center.x, right_circle.center.y, right_circle.radius * k_meters_to_pixels, PI64 + theta, PI64 - theta);
			
			right_circle.theta_start = PI64 - theta;
			right_circle.theta_end = PI64 + theta;
			right_circle.convex = false;
			
		// Draw a convex curve.
		} else {
			right_circle.center = center;
			right_circle.center.x -= (m_right_radius - arc_w) * k_meters_to_pixels;
			cairo_arc(context, right_circle.center.x, right_circle.center.y, m_right_radius * k_meters_to_pixels, -theta, theta);
			
			right_circle.theta_start = -theta;
			right_circle.theta_end = theta;
			right_circle.convex = true;
		}
		
		right_circle.center /= k_meters_to_pixels;
	}
	
	cairo_close_path(context);
	cairo_stroke(context);
	
	// Now do raycasts to determine the intersections.
	cairo_set_line_width(context, 1.0);
	
	for (f64 y_offset = -0.5; y_offset <= 0.5; y_offset += 0.1) {
		vector2dd ray_start(0.0, center.y / k_meters_to_pixels + y_offset);
		vector2dd ray_dir(1.0, 0.0);
		
		vector2dd intersection;
		vector2dd normal;
		bool inside = false;
			
		u32 i = 0;
		const u32 k_cast_limit = 10;
		
		// Start raycasting, up to a maximum of ten hits.
		while (++i <= k_cast_limit) {
			bool intersect = false;
			
			if (((!inside && ray_dir.x >= 0.0) // Intersect with left half if the direction is in the +x and start point is outside lens.
					|| (inside && ray_dir.x <= 0.0)) // Intersect with left half if the direction is in the -x and start point is inside lens.
					&& left_circle.ray_intersect(ray_start, ray_dir, intersection, normal)) {
				intersect = true;
			} else if (((!inside && ray_dir.x <= 0.0) // Intersect with right half if the direction is in the -x and start point is outside lens.
						|| (inside && ray_dir.x >= 0.0)) // Intersect with right half if the direction is in the +x and start point is inside lens.
						&& right_circle.ray_intersect(ray_start, ray_dir, intersection, normal)) {
				intersect = true;
			}
			
			if (!intersect) break;
			
			// Set the line color to red.
			cairo_set_source_rgb(context, 1.0, 0.0, 0.0);
			
			// Draw the ray.
			cairo_move_to(context, ray_start.x * k_meters_to_pixels, ray_start.y * k_meters_to_pixels);
			cairo_line_to(context, intersection.x * k_meters_to_pixels, intersection.y * k_meters_to_pixels);
			cairo_stroke(context);
			
			// Calculate the angle from the normal.
			f64 theta = atan2(-ray_dir.y, -ray_dir.x) - atan2(normal.y, normal.x);
			if (theta > PI64) theta -= 2.0 * PI64; // Normalize the angle so that it is between (-pi, pi].
			if (theta <= -PI64) theta += 2.0 * PI64;
			
			// Flip the normal if the separation is more than 90 degrees.
			if (abs_(theta) > 0.5 * PI64) {
				normal = -normal;
				theta = atan2(-ray_dir.y, -ray_dir.x) - atan2(normal.y, normal.x);
			}
			
			// Draw the normal.
			cairo_set_source_rgb(context, 0.5, 1.0, 0.5);
			cairo_move_to(context, intersection.x * k_meters_to_pixels, intersection.y * k_meters_to_pixels);
			cairo_rel_line_to(context, normal.x * k_meters_to_pixels * 0.1, normal.y * k_meters_to_pixels * 0.1);
			cairo_stroke(context);
			
			// Apply snell's law:
			// n_1 sin(theta_1) = n_2 sin(theta_2)
			// Which becomes: theta_2 = asin(n_1 / n_2 * sin(theta_1))
			f64 amt = (inside ? (m_inside_n / m_outside_n) : (m_outside_n / m_inside_n)) * sin(theta);
			if (abs_(amt) > 1.0) {
				// Reflect along normal.
				theta = -theta;
			} else {
				// Normal, refract through surface.
				theta = asin(amt) + PI64;
				inside = !inside;
			}
			
			// Convert the angle back to absolute coordinates.
			theta += atan2(normal.y, normal.x);
			
			// Set up the next raycast.
			ray_start = intersection;
			ray_dir.set(cos(theta), sin(theta));
		}
		
		if (i <= k_cast_limit) {
			// Draw final line.
			f64 max_dim = max_(size.x, size.y);
			
			// Set the line color to blue.
			cairo_set_source_rgb(context, 0.5, 0.5, 1.0);
			
			cairo_move_to(context, ray_start.x * k_meters_to_pixels, ray_start.y * k_meters_to_pixels);
			cairo_rel_line_to(context, ray_dir.x * max_dim, ray_dir.y * max_dim);
			cairo_stroke(context);
		}
	}*/
}
