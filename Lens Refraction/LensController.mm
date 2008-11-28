#import "LensController.h"
#include "backend/lens_backend.h"

@implementation LensController

/**
 * Constructor, called after the .nib is loaded.
**/
- (void) awakeFromNib {
	m_backend = new lens_backend();
}

/**
 * Destructor.
**/
- (void) dealloc {
	delete m_backend;
	
    [super dealloc];
}

/// Called when the left radius field changes.
- (void) onLeftRadiusChange: (id) caller {
	m_backend->set_left_radius([m_left_radius floatValue]);
}

/// Called when the right radius field changes.
- (void) onRightRadiusChange: (id) caller {
	m_backend->set_right_radius([m_right_radius floatValue]);
}

/// Called when the inside refraction index changes.
- (void) onInsideNChange: (id) caller {
	m_backend->set_inside_n([m_lens_n floatValue]);
}

/// Called when the outside refraction index changes.
- (void) onOutsideNChange: (id) caller {
	m_backend->set_outside_n([m_outside_n floatValue]);
}

@end
