#import <Cocoa/Cocoa.h>

@class LensController;

@interface CairoView : NSView {
    IBOutlet LensController * m_controller;
}


- (void) drawRect: (NSRect) rect;

@end
