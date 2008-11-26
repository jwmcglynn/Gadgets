/* Replacement for auto-generated src/cairo-features.h */
#ifndef CAIRO_FEATURES_H
#define CAIRO_FEATURES_H

#define CAIRO_HAS_IMAGE_SURFACE 1
#define CAIRO_HAS_FONT_SUBSET 1

#if defined(__MacOSX__) || defined(__APPLE__) || defined(MACOSX)
	#define CAIRO_HAS_QUARTZ_FONT 1
	#define CAIRO_HAS_QUARTZ_SURFACE 1
#elif defined(_WIN32) || defined(_WIN64)
	#define CAIRO_HAS_WIN32_FONT 1
	#define CAIRO_HAS_WIN32_SURFACE 1
#endif

#endif
