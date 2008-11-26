#import <Cocoa/Cocoa.h>

@class CairoView;

@interface LensController : NSObject {
	CairoView * m_view;
	NSTextField * m_left_radius;
	NSTextField * m_right_radius;
	
	NSTextField * m_lens_n;
	NSTextField * m_outside_n;
}

@end
