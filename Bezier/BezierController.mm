//
//  BezierController.mm
//  Bezier
//
//  Created by Jeff McGlynn on 1/26/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "BezierController.h"
#include "backend/bezier_backend.h"


@implementation BezierController

/**
 * Constructor, called after the .nib is loaded.
**/
- (void) awakeFromNib {
	m_backend = new bezier_backend();
	
	// Initialize the form.
	f64 t = 0.0;
	[m_t_field setDoubleValue: t];
	[self onChangeT: m_t_field];
}

- (BOOL) applicationShouldTerminateAfterLastWindowClosed: (NSApplication *) app {
	return YES;
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

/**
 * Called when the position text field is updated.
**/
- (IBAction) onChangeT: (id) sender {
	f64 val = clamp([sender doubleValue], 0.0, 1.0);
	
	[m_t_field setDoubleValue: val];
	if (sender != m_t_slider) [m_t_slider setDoubleValue: val];
	if (sender != m_t_stepper) [m_t_stepper setDoubleValue: val];
	
	m_backend->t(val);
	[m_view setNeedsDisplay: YES];
}

@end
