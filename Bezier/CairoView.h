#import <Cocoa/Cocoa.h>

@class BezierController;

@interface CairoView : NSView {
    IBOutlet BezierController * m_controller;
}


- (void) drawRect: (NSRect) rect;

// Mouse commands.
- (void) mouseDown: (NSEvent *) ev;
- (void) mouseDragged: (NSEvent *) ev;
- (void) mouseUp: (NSEvent *) ev;

@end
