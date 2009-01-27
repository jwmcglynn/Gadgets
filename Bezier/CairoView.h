#import <Cocoa/Cocoa.h>

@class BezierController;

@interface CairoView : NSView {
    IBOutlet BezierController * m_controller;
}


- (void) drawRect: (NSRect) rect;

@end
