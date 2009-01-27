#import "CairoView.h"
#import "BezierController.h"

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

@end
