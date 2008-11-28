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
	
	bool ray_intersect(vector2dd start, vector2dd dir, vector2dd& intersection, vector2dd& normal);
		
private:
	bool angle_in_range(f64 angle) {
		angle -= theta_start;
		f64 end = theta_end - theta_start;
		
		if (end < 0.0) return (angle >= end && angle <= 0.0);
		else return (angle >= 0.0 && angle <= end);
	}
	
	bool ray_intersect_validate(vector2dd start, vector2dd dir, f64 t, vector2dd& intersection, vector2dd& normal) {
		intersection = start + t * dir;
		
		f64 angle = atan2(intersection.y, intersection.x);
		if (!angle_in_range(angle)) return false;
		
		normal.set(cos(angle), sin(angle));
		intersection += center;
		return true;
	}
};

class lens_backend {
public:
	lens_backend()
		: m_left_radius(1.5f),
		  m_right_radius(1.5f),
		  m_inside_n(1.01f),
		  m_outside_n(1.0f) { }
	
	/**
	 * Called to render to the view.
	**/
	void render(cairo_t * context, const vector2dd& size) {
		vector2dd center(size * 0.5);
		
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
				
			// Draw a convex curve.
			} else {
				left_circle.center = center;
				left_circle.center.x += (m_left_radius - arc_w) * k_meters_to_pixels;
				
				cairo_arc(context, left_circle.center.x, left_circle.center.y, m_left_radius * k_meters_to_pixels, PI64 - theta, PI64 + theta);
				left_circle.theta_start = PI64 - theta;
				left_circle.theta_end = PI64 + theta;
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
				
			// Draw a convex curve.
			} else {
				right_circle.center = center;
				right_circle.center.x -= (m_right_radius - arc_w) * k_meters_to_pixels;
				cairo_arc(context, right_circle.center.x, right_circle.center.y, m_right_radius * k_meters_to_pixels, -theta, theta);
				
				right_circle.theta_start = -theta;
				right_circle.theta_end = theta;
			}
			
			right_circle.center /= k_meters_to_pixels;
		}
		
		cairo_close_path(context);
		cairo_stroke(context);
		
		
		cairo_set_line_width(context, 0.5);
		
		// Now do raycasts to determine the intersections.
		for (f64 y_offset = -1.5; y_offset <= 1.5; y_offset += 0.1) {
			vector2dd ray_start(0.0, center.y / k_meters_to_pixels + y_offset);
			vector2dd ray_dir(1.0, 0.0);
			
			vector2dd intersection;
			vector2dd normal;
			
			u32 i = 0;
			while (++i < 5) {
				if (((i & 1) && left_circle.ray_intersect(ray_start, ray_dir, intersection, normal)) || ((i & 1) == 0 && right_circle.ray_intersect(ray_start, ray_dir, intersection, normal))) {
					// Set the line color to red.
					cairo_set_source_rgb(context, 1.0, 0.0, 0.0);
					
					cairo_move_to(context, ray_start.x * k_meters_to_pixels, ray_start.y * k_meters_to_pixels);
					cairo_line_to(context, intersection.x * k_meters_to_pixels, intersection.y * k_meters_to_pixels);
					cairo_stroke(context);
					
					ray_start = intersection;
					
					// n_1 sin(theta_1) = n_2 sin(theta_2)
					// Which becomes: theta_2 = asin(n_1 / n_2 * sin(theta_1))
					
					if (normal.dot_product(ray_dir) > 0.0) {
						normal = -normal;
					}
					
					f64 normal_angle = atan2(normal.y, normal.x);
					f64 theta = atan2(ray_dir.y, ray_dir.x);
					
					theta -= normal_angle;
					theta = asin((i & 1) ? (m_inside_n / m_outside_n) : (m_outside_n / m_inside_n) * sin(theta));
					theta += normal_angle;
					
					ray_dir.set(cos(theta), sin(theta));
				}
			}
			
			// Draw final line.
			f64 max_dim = max_(size.x, size.y);
			
			// Set the line color to blue.
			cairo_set_source_rgb(context, 0.5, 0.5, 1.0);
			
			cairo_move_to(context, ray_start.x * k_meters_to_pixels, ray_start.y * k_meters_to_pixels);
			cairo_rel_line_to(context, ray_dir.x * max_dim, ray_dir.y * max_dim);
			cairo_stroke(context);
		}
	}
	
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
