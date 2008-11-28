#import "CairoView.h"
#import "LensController.h"

#include <cairo/cairo.h>
#include <cairo/cairo-quartz.h>

#include "backend/vector2d.h"
using namespace donner;

@implementation CairoView

- (void) drawRect: (NSRect) rect {
	CGContextRef window_ctx = (CGContextRef) [[NSGraphicsContext currentContext] graphicsPort];
	
	// Draw the rectangle using cairo.
	cairo_surface_t * surf = cairo_quartz_surface_create(CAIRO_FORMAT_RGB24, rect.size.width, rect.size.height);
	cairo_surface_set_device_offset(surf, rect.origin.x, rect.origin.y);
	
	cairo_t * ctx = cairo_create(surf);
	[m_controller onRender: ctx: rect.size.width: rect.size.height];
	cairo_destroy(ctx);
	
	
	CGContextRef img_ctx = (CGContextRef) cairo_quartz_surface_get_cg_context(surf);
	
	// Draw the image.
	if (img_ctx) {
		CGImageRef img = CGBitmapContextCreateImage(img_ctx);
		CGContextDrawImage(window_ctx, NSRectToCGRect(rect), img);
		CGImageRelease(img);
		CGContextFlush(window_ctx);
	}
	
	cairo_surface_destroy(surf);
}

@end
