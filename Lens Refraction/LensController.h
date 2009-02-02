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
