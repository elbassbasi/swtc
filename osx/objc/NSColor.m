#import "swt.h"
CGFloat NSColor_alphaComponent(NSColor* color) {
    return [color alphaComponent];
}

NSColor* NSColor_alternateSelectedControlColor() {
    return [NSColor alternateSelectedControlColor];
}

NSColor* NSColor_alternateSelectedControlTextColor() {
    return [NSColor alternateSelectedControlTextColor];
}
NSColor* NSColor_blackColor() {
    return [NSColor blackColor];
}

CGFloat NSColor_blueComponent(NSColor *color) {
    return [color blueComponent];
}

NSColor* NSColor_clearColor() {
    return [NSColor clearColor];
}

NSColorSpace* NSColor_colorSpace(NSColor *color) {
    return [color colorSpace];
}

NSColor* NSColor_colorUsingColorSpaceName(NSColor *color,NSString* colorSpace) {
    return [color colorUsingColorSpaceName:colorSpace];
}

NSColor* NSColor_colorWithCalibratedRed(CGFloat red, CGFloat green, CGFloat blue, CGFloat alpha) {
    return [NSColor colorWithCalibratedRed:red green:green blue:blue alpha:alpha];
}

NSColor* NSColor_colorWithDeviceRed(CGFloat red, CGFloat green, CGFloat blue, CGFloat alpha) {
        return [NSColor colorWithDeviceRed:red green:green blue:blue alpha:alpha];
}

NSColor* NSColor_colorWithPatternImage(NSImage* image) {
    return [NSColor colorWithPatternImage:image];
}

NSColor* NSColor_controlBackgroundColor() {
    return [NSColor controlBackgroundColor];
}

NSColor* NSColor_controlDarkShadowColor() {
    return [NSColor controlDarkShadowColor];
}

NSColor* NSColor_controlHighlightColor() {
    return [NSColor controlHighlightColor];
}

NSColor* NSColor_controlLightHighlightColor() {
    return [NSColor controlLightHighlightColor];
}

NSColor* NSColor_controlShadowColor() {
    return [NSColor controlShadowColor];
}

NSColor* NSColor_controlTextColor() {
    return [NSColor controlTextColor];
}

NSColor* NSColor_disabledControlTextColor() {
    return [NSColor disabledControlTextColor];
}

void NSColor_getComponents(NSColor* color,CGFloat* components) {
    [color getComponents:components];
}

CGFloat NSColor_greenComponent(NSColor* color) {
    return [color greenComponent];
}

int NSColor_numberOfComponents(NSColor* color) {
    return [color numberOfComponents];
}

CGFloat NSColor_redComponent(NSColor* color) {
    return [color redComponent];
}

NSColor* NSColor_secondarySelectedControlColor() {
    return [NSColor secondarySelectedControlColor];
}

NSColor* NSColor_selectedControlColor() {
    return [NSColor selectedControlColor];
}

NSColor* NSColor_selectedControlTextColor() {
    return [NSColor selectedControlTextColor];
}

NSColor* NSColor_selectedTextBackgroundColor() {
    return [NSColor selectedTextBackgroundColor];
}

NSColor* NSColor_selectedTextColor() {
    return [NSColor selectedTextColor];
}

void NSColor_set(NSColor* color) {
    [color set];
}

void NSColor_setFill(NSColor* color) {
    [color setFill];
}

void NSColor_setStroke(NSColor* color) {
    [color setStroke];
}

NSColor* NSColor_textBackgroundColor() {
    return [NSColor textBackgroundColor];
}

NSColor* NSColor_textColor() {
    return [NSColor textColor];
}

NSColor* NSColor_windowBackgroundColor() {
    return [NSColor windowBackgroundColor];
}

NSColor* NSColor_windowFrameColor() {
    return [NSColor windowFrameColor];
}

NSColor* NSColor_windowFrameTextColor() {
    return [NSColor windowFrameTextColor];
}
/*
 * NSGradient
 */
void NSGradient_drawFromPoint(NSGradient* gradient,NSPoint* startingPoint, NSPoint* endingPoint, int options) {
    [gradient drawFromPoint:*startingPoint toPoint:*endingPoint options:options];
}

void NSGradient_drawInRect(NSGradient* gradient,NSRect* rect, CGFloat angle) {
    [gradient drawInRect:*rect angle:angle];
}

NSGradient* NSGradient_initWithStartingColor(NSColor* startingColor, NSColor* endingColor) {
    return [[NSGradient alloc] initWithStartingColor:startingColor endingColor:endingColor];
}
