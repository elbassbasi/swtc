#import "swt.h"
NSGraphicsContext* NSGraphicsContext_currentContext() {
    return [NSGraphicsContext currentContext];
}
void NSGraphicsContext_flushGraphics(NSGraphicsContext* graphicsContext) {
    [graphicsContext flushGraphics];
}
NSGraphicsContext* NSGraphicsContext_graphicsContextWithBitmapImageRep(NSBitmapImageRep* bitmapRep) {
    return [NSGraphicsContext graphicsContextWithBitmapImageRep:bitmapRep];
}

NSGraphicsContext* NSGraphicsContext_graphicsContextWithGraphicsPort(void* graphicsPort, int initialFlippedState) {
    return [NSGraphicsContext graphicsContextWithGraphicsPort:graphicsPort flipped:initialFlippedState];
}

NSGraphicsContext* NSGraphicsContext_graphicsContextWithWindow(NSWindow* window) {
    return [NSGraphicsContext graphicsContextWithWindow:window];
}
void* NSGraphicsContext_graphicsPort(NSGraphicsContext* graphicsContext) {
    return [graphicsContext graphicsPort];
}
NSUInteger NSGraphicsContext_imageInterpolation(NSGraphicsContext* graphicsContext) {
     return [graphicsContext imageInterpolation];
}
int NSGraphicsContext_isDrawingToScreen(NSGraphicsContext* graphicsContext) {
    return [graphicsContext isDrawingToScreen];
}

int NSGraphicsContext_isFlipped(NSGraphicsContext* graphicsContext) {
    [graphicsContext isFlipped];
}

void NSGraphicsContext_static_restoreGraphicsState() {
    [NSGraphicsContext restoreGraphicsState];
}

void NSGraphicsContext_restoreGraphicsState(NSGraphicsContext* graphicsContext) {
    [graphicsContext restoreGraphicsState];
}

void NSGraphicsContext_static_saveGraphicsState() {
    [NSGraphicsContext saveGraphicsState];
}

void NSGraphicsContext_saveGraphicsState(NSGraphicsContext* graphicsContext) {
    [graphicsContext saveGraphicsState];
}

void NSGraphicsContext_setCompositingOperation(NSGraphicsContext* graphicsContext,NSCompositingOperation* operation) {
    [graphicsContext setCompositingOperation:operation];
}

void NSGraphicsContext_setCurrentContext(NSGraphicsContext *context) {
    [NSGraphicsContext setCurrentContext:context];
}

void NSGraphicsContext_setImageInterpolation(NSGraphicsContext* graphicsContext,NSUInteger interpolation) {
    [graphicsContext setImageInterpolation:interpolation];
}

void NSGraphicsContext_setPatternPhase(NSGraphicsContext* graphicsContext,w_point* phase) {
    NSPoint _phase = NSMakePoint(phase->x, phase->y);
    [graphicsContext setPatternPhase:_phase];
}

void NSGraphicsContext_setShouldAntialias(NSGraphicsContext* graphicsContext,int antialias) {
    [graphicsContext setShouldAntialias:antialias];
}

int NSGraphicsContext_shouldAntialias(NSGraphicsContext* graphicsContext) {
    return [graphicsContext shouldAntialias];
}
