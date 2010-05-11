/*
 * LensController.h
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

#import <Cocoa/Cocoa.h>

typedef struct _cairo cairo_t;
@class CairoView;

// C++ backend.
class lens_backend;

@interface LensController : NSObject {
	IBOutlet CairoView * m_view;
	lens_backend * m_backend;
	
	/*************************************************************************/
	// Radius.
	
	// Text boxes.
	IBOutlet NSTextField * m_left_radius;
	IBOutlet NSTextField * m_right_radius;
	
	// Sliders.
	IBOutlet NSSlider * m_left_radius_slider;
	IBOutlet NSSlider * m_right_radius_slider;
	
	/*************************************************************************/
	// Index of refraction.
	
	IBOutlet NSFormCell * m_inside_n;
	IBOutlet NSFormCell * m_outside_n;
	
	IBOutlet NSPopUpButton * m_inside_presets;
	IBOutlet NSPopUpButton * m_outside_presets;
}

/**
 * Constructor, called after the .nib is loaded.
**/
- (void) awakeFromNib;

/**
 * Render the cairo surface.
**/
- (void) onRender: (cairo_t *) context: (float) width: (float) height;

/// Called when the left radius field changes.
- (IBAction) onLeftRadiusChange: (id) caller;
- (IBAction) onLeftRadiusSliderChange: (id) caller;

/// Called when the right radius field changes.
- (IBAction) onRightRadiusChange: (id) caller;
- (IBAction) onRightRadiusSliderChange: (id) caller;

/// Called when the refraction presets are changed.
- (void) onSelectPreset: (NSMenuItem *) caller;

/// Called when the inside refraction index changes.
- (IBAction) onInsideNChange: (id) caller;

/// Called when the outside refraction index changes.
- (IBAction) onOutsideNChange: (id) caller;

@end
