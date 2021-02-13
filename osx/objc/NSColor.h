/*
 * NSColor.h
 *
 *  Created on: Jan 18, 2021
 *      Author: azeddine
 */

#ifndef OSX_OBJC_NSCOLOR_H_
#define OSX_OBJC_NSCOLOR_H_
#include "NSObject.h"
typedef struct NSColor NSColor;
typedef struct NSColorSpace NSColorSpace;
typedef struct NSImage NSImage;
typedef struct NSGradient NSGradient;
CGFloat NSColor_alphaComponent(NSColor* color);
NSColor* NSColor_alternateSelectedControlColor();
NSColor* NSColor_alternateSelectedControlTextColor();
NSColor* NSColor_blackColor();
CGFloat NSColor_blueComponent(NSColor *color);
NSColor* NSColor_clearColor();
NSColorSpace* NSColor_colorSpace(NSColor *color);
NSColor* NSColor_colorUsingColorSpaceName(NSColor *color,NSString* colorSpace);
NSColor* NSColor_colorWithCalibratedRed(CGFloat red, CGFloat green, CGFloat blue, CGFloat alpha);
NSColor* NSColor_colorWithDeviceRed(CGFloat red, CGFloat green, CGFloat blue, CGFloat alpha);
NSColor* NSColor_colorWithPatternImage(NSImage* image);
NSColor* NSColor_controlBackgroundColor();
NSColor* NSColor_controlDarkShadowColor();
NSColor* NSColor_controlHighlightColor();
NSColor* NSColor_controlLightHighlightColor();
NSColor* NSColor_controlShadowColor();
NSColor* NSColor_controlTextColor();
NSColor* NSColor_disabledControlTextColor();
void NSColor_getComponents(NSColor* color,CGFloat* components);
CGFloat NSColor_greenComponent(NSColor* color);
int NSColor_numberOfComponents(NSColor* color);
CGFloat NSColor_redComponent(NSColor* color);
NSColor* NSColor_secondarySelectedControlColor();
NSColor* NSColor_selectedControlColor();
NSColor* NSColor_selectedControlTextColor();
NSColor* NSColor_selectedTextBackgroundColor();
NSColor* NSColor_selectedTextColor();
void NSColor_set(NSColor* color);
void NSColor_setFill(NSColor* color);
void NSColor_setStroke(NSColor* color);
NSColor* NSColor_textBackgroundColor();
NSColor* NSColor_textColor();
NSColor* NSColor_windowBackgroundColor();
NSColor* NSColor_windowFrameColor();
NSColor* NSColor_windowFrameTextColor();
/*
 * NSGradient
 */
void NSGradient_drawFromPoint(NSGradient* gradient,NSPoint* startingPoint, NSPoint* endingPoint, int options);
void NSGradient_drawInRect(NSGradient* gradient,NSRect* rect, CGFloat angle);
NSGradient* NSGradient_initWithStartingColor(NSColor* startingColor, NSColor* endingColor);
#endif /* OSX_OBJC_NSCOLOR_H_ */
