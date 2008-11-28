#import <Cocoa/Cocoa.h>

@class CairoView;

// C++ backend.
class lens_backend;

@interface LensController : NSObject {
	CairoView * m_view;
	NSTextField * m_left_radius;
	NSTextField * m_right_radius;
	
	NSTextField * m_lens_n;
	NSTextField * m_outside_n;
	
	lens_backend * m_backend;
}

/**
 * Constructor, called after the .nib is loaded.
**/
- (void) awakeFromNib;

/// Called when the left radius field changes.
- (void) onLeftRadiusChange: (id) caller;

/// Called when the right radius field changes.
- (void) onRightRadiusChange: (id) caller;

/// Called when the inside refraction index changes.
- (void) onInsideNChange: (id) caller;

/// Called when the outside refraction index changes.
- (void) onOutsideNChange: (id) caller;

@end
