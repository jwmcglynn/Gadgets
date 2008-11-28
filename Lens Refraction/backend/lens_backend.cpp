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
	
	if (t_1 < ROUNDING_ERROR_32 && t_2 < ROUNDING_ERROR_32) {
		return false; // Intersection behind ray.
	}
	
	// 
	if (t_2 < t_1) {
		// Swap t_1 and t_2 ...
		f64 tmp = t_1;
		t_1 = t_2;
		t_2 = tmp;
	}
	
	if (ray_intersect_validate(start, dir, t_1, intersection, normal)) return true;
	if (ray_intersect_validate(start, dir, t_2, intersection, normal)) return true;
	
	return false;
	
	/*
	f64 d = (start.x * (start.y + dir.y)) - (start.y * (start.x + dir.x));
	f64 discriminant = radius * radius - d * d;
	if (discriminant <= 0.0) return false;
	
	
	f64 sqrt_disc = sqrt(discriminant);
	
	// The value that is either subtracted or added to the first half of the quadratic formula.
	vector2dd operand(
		sign_(dir.y) * dir.x * sqrt_disc,
		abs_(dir.y) * sqrt_disc
	);
	
	vector2dd intersection_pt(d * dir.y, -d * dir.x);
	vector2dd other_int = intersection_pt;
	
	intersection_pt -= operand;
	other_int += operand;
	
	f64 angle_1 = atan2(intersection_pt.y, intersection_pt.x);
	f64 angle_2 = atan2(other_int.y, other_int.x);
	
	bool angle_1_good = on_ray(start, dir, intersection_pt) && angle_in_range(angle_1);
	bool angle_2_good = on_ray(start, dir, intersection_pt) && angle_in_range(angle_2);
	
	// Pick which intersection is good.
	if (!angle_1_good) {
		if (!angle_2_good) return false;
		intersection_pt = other_int;
	}
	
	// Determine the angle on the circle that the intersection occurs at.
	f64 angle = atan2(intersection_pt.y, intersection_pt.x);
	
	// There is an intersection, set the normal.
	intersection.set(intersection_pt + center);
	normal.set(cos(angle), sin(angle));
	return true;*/
}
