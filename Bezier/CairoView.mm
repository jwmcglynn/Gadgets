#import "CairoView.h"
#import "BezierController.h"
#include "backend/bezier_backend.h"

#include <cairo/cairo.h>
#include <cairo/cairo-quartz.h>

#include "backend/vector2d.h"
using namespace dnr;

@implementation CairoView

- (void) drawRect: (NSRect) rect {
	CGContextRef window_ctx = (CGContextRef) [[NSGraphicsContext currentContext] graphicsPort];
	
	// Draw the rectangle using cairo.
	cairo_surface_t * surf = cairo_quartz_surface_create_for_cg_context(window_ctx, rect.size.width, rect.size.height);
	cairo_surface_set_device_offset(surf, rect.size.width * 0.5, rect.size.height * 0.5);
	
	cairo_t * ctx = cairo_create(surf);
	[m_controller onRender: ctx: rect.size.width: rect.size.height];
	cairo_destroy(ctx);
	
	cairo_surface_flush(surf);
	cairo_surface_destroy(surf);
}

/**
 * All of the mouse commands are the same, but call a different method in the
 * backend.  This macro implements them but allows the method name and backend
 * method name to be changed.
**/
#define IMPLEMENT_MOUSE_COMMAND(fcn_name, callback) \
- (void) fcn_name: (NSEvent *) ev { \
	NSPoint mouse_loc = [self convertPoint: [ev locationInWindow]  fromView: nil]; \
	BOOL is_inside = [self mouse: mouse_loc  inRect: [self bounds]]; \
	 \
	mouse_loc = NSMakePoint(mouse_loc.x - 0.5 * [self bounds].size.width, mouse_loc.y - 0.5 * [self bounds].size.height); \
	 \
	vector2dd pos(mouse_loc.x, mouse_loc.y); \
	m_controller->m_backend->callback(pos, is_inside); \
	 \
	if (m_controller->m_backend->needs_redraw()) [self setNeedsDisplay:YES]; \
}

IMPLEMENT_MOUSE_COMMAND(mouseDown, click)
IMPLEMENT_MOUSE_COMMAND(mouseDragged, drag)
IMPLEMENT_MOUSE_COMMAND(mouseUp, release)

#undef IMPLEMENT_MOUSE_COMMAND

@end
