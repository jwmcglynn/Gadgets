/*
 * LensController.mm
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

#import "LensController.h"
#include "backend/lens_backend.h"

@implementation LensController

/**
 * Constructor, called after the .nib is loaded.
**/
- (void) awakeFromNib {
	m_backend = new lens_backend();
	
	// Fill in the fields with the default values.
	[m_left_radius setFloatValue: m_backend->left_radius()];
	[m_left_radius_slider setFloatValue: m_backend->left_radius()];
	
	[m_right_radius setFloatValue: m_backend->right_radius()];
	[m_right_radius_slider setFloatValue: m_backend->right_radius()];
	
	[m_inside_n setFloatValue: m_backend->inside_n()];
	[m_outside_n setFloatValue: m_backend->outside_n()];
	
	// Set up the index of refraction presets.
	NSMenu * menu = [[[NSMenu alloc] init] autorelease];
	const lens_backend::index_array& arr = m_backend->n_presets();
	
	/* Set up start of menu:
	 * Presets ...
	 * -----
	**/
	[[menu addItemWithTitle: @"Presets…"  action: nil  keyEquivalent: @""] setTag: -1];
	[menu addItem: [NSMenuItem separatorItem]];
	
	size_t sz = arr.size();
	for (size_t i = 0; i < sz; ++i) {
		NSString * name = [NSString stringWithUTF8String: arr[i].name.c_str()];
		name = [NSString stringWithFormat: @"%@ – %f", name, arr[i].n];
		
		NSMenuItem * item = [menu
			addItemWithTitle: name
			action: @selector(onSelectPreset:)
			keyEquivalent: @""
		];
		
		[item setTag: (int) i];
	}
	
	/* Set up end of menu:
	 * -----
	 * Other
	**/
	[menu addItem: [NSMenuItem separatorItem]];
	[[menu addItemWithTitle: @"Other"  action: nil  keyEquivalent: @""] setTag: -2];
	
	
	[m_inside_presets setMenu: [[menu copy] autorelease]];
	[m_outside_presets setMenu: [[menu copy] autorelease]];
}

/**
 * Destructor.
**/
- (void) dealloc {
	delete m_backend;
	
    [super dealloc];
}

/**
 * Render the cairo surface.
**/
- (void) onRender: (cairo_t *) context: (float) width: (float) height {
	vector2dd size(width, height);
	m_backend->render(context, size);
}

static float clamp_radius(float rad) {
	if (rad < 0.0f && rad > -1.0f) return -1.0f;
	if (rad >= 0.0f && rad < 1.0f) return 1.0f;
	return rad;
}

/// Called when the left radius field changes.
- (IBAction) onLeftRadiusChange: (id) caller {
	float radius = clamp_radius([m_left_radius floatValue]);
	
	m_backend->left_radius(radius);
	[m_left_radius setFloatValue: radius];
	[m_left_radius_slider setFloatValue: radius];
	[m_view setNeedsDisplay: YES];
}

- (IBAction) onLeftRadiusSliderChange: (id) caller {
	float radius = clamp_radius([m_left_radius_slider floatValue]);
	
	m_backend->left_radius(radius);
	[m_left_radius setFloatValue: radius];
	[m_left_radius_slider setFloatValue: radius];
	[m_view setNeedsDisplay: YES];
}

/// Called when the right radius field changes.
- (IBAction) onRightRadiusChange: (id) caller {
	float radius = clamp_radius([m_right_radius floatValue]);
	
	m_backend->right_radius(radius);
	[m_right_radius setFloatValue: radius];
	[m_right_radius_slider setFloatValue: radius];
	[m_view setNeedsDisplay: YES];
}

- (IBAction) onRightRadiusSliderChange: (id) caller {
	float radius = clamp_radius([m_right_radius_slider floatValue]);
	
	m_backend->right_radius(radius);
	[m_right_radius setFloatValue: radius];
	[m_right_radius_slider setFloatValue: radius];
	[m_view setNeedsDisplay: YES];
}


/// Called when the refraction presets are changed.
- (void) onSelectPreset: (NSMenuItem *) caller {
	float val = m_backend->n_presets().at([caller tag]).n;
	
	if ([caller menu] == [m_inside_presets menu]) {
		m_backend->inside_n(val);
		[m_inside_n setFloatValue: val];
		
	} else if ([caller menu] == [m_outside_presets menu]) {
		m_backend->outside_n(val);
		[m_outside_n setFloatValue: val];
	}
	
	[m_view setNeedsDisplay: YES];
}

/// Called when the inside refraction index changes.
- (IBAction) onInsideNChange: (id) caller {
	float val = [m_inside_n floatValue];
	if (val < 1.0f) val = 1.0f;
	m_backend->inside_n(val);
	
	[m_inside_n setFloatValue: val];
	[m_inside_presets selectItem: [[m_inside_presets menu] itemWithTag: -2]];
	[m_view setNeedsDisplay: YES];
}

/// Called when the outside refraction index changes.
- (IBAction) onOutsideNChange: (id) caller {
	float val = [m_outside_n floatValue];
	if (val < 1.0f) val = 1.0f;
	m_backend->outside_n(val);
	
	[m_outside_n setFloatValue: val];
	[m_outside_presets selectItem: [[m_inside_presets menu] itemWithTag: -2]];
	[m_view setNeedsDisplay: YES];
}

@end
