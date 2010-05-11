/*
 * CairoView.mm
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

#import "CairoView.h"
#import "LensController.h"

#ifdef WINDOWS
#include <cairo.h>
#else
#include <cairo/cairo.h>
#endif

#if CAIRO_HAS_QUARTZ_SURFACE
#include <cairo/cairo-quartz.h>
#endif

#include "backend/vector2d.h"
using namespace donner;

@implementation CairoView

- (void) drawRect: (NSRect) rect {
	CGContextRef window_ctx = (CGContextRef) [[NSGraphicsContext currentContext] graphicsPort];
	
	// Draw the rectangle using cairo.
	#if CAIRO_HAS_QUARTZ_SURFACE
	cairo_surface_t * surf = cairo_quartz_surface_create(CAIRO_FORMAT_RGB24, rect.size.width, rect.size.height);
	#else
	cairo_surface_t * surf = cairo_image_surface_create(CAIRO_FORMAT_RGB24, rect.size.width, rect.size.height);
	#endif
	cairo_surface_set_device_offset(surf, rect.origin.x, rect.origin.y);
	
	cairo_t * ctx = cairo_create(surf);
	[m_controller onRender: ctx: rect.size.width: rect.size.height];
	cairo_destroy(ctx);
	
	#if CAIRO_HAS_QUARTZ_SURFACE
	CGContextRef img_ctx = (CGContextRef) cairo_quartz_surface_get_cg_context(surf);
	
	// Draw the image.
	if (img_ctx) {
		CGImageRef img = CGBitmapContextCreateImage(img_ctx);
		CGContextDrawImage(window_ctx, NSRectToCGRect(rect), img);
		CGImageRelease(img);
		CGContextFlush(window_ctx);
	}
	#else
	// TODO.
	#error "Not implemented."
	#endif
	
	cairo_surface_destroy(surf);
}

@end
