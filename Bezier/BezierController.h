//
//  BezierController.h
//  Bezier
//
//  Created by Jeff McGlynn on 1/26/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#include <cairo/cairo.h>

@class CairoView;

// C++ backend.
class bezier_backend;

@interface BezierController : NSObject {
	IBOutlet CairoView * m_view;
@public	
	bezier_backend * m_backend;
	
@protected
	// Controls.
	IBOutlet NSTextField * m_t_field;
	IBOutlet NSSlider * m_t_slider;
	IBOutlet NSStepper * m_t_stepper;
}

/**
 * Constructor, called after the .nib is loaded.
**/
- (void) awakeFromNib;

/**
 * Render the cairo surface.
**/
- (void) onRender: (cairo_t *) context: (float) width: (float) height;

/**
 * Called when the position text field is updated.
**/
- (IBAction) onChangeT: (id) sender;

@end
