/*
 * NSBox.h
 *
 *  Created on: Mar 27, 2021
 *      Author: azeddine
 */

#ifndef OSX_OBJC_NSBOX_H_
#define OSX_OBJC_NSBOX_H_
#include "NSView.h"
typedef struct NSBox NSBox;
w_widget* SWTBox_get_widget(NSView* view);
NSView* SWTBox_new(w_widget* widget);
CGFloat NSBox_borderWidth(NSBox *box);
NSView* NSBox_contentView(NSBox *box);
void NSBox_contentViewMargins(NSBox *box, NSSize *size);
void NSBox_setBorderType(NSBox *box, int aType);
void NSBox_setBorderWidth(NSBox *box, CGFloat borderWidth);
void NSBox_setBoxType(NSBox *box, int boxType);
void NSBox_setContentView(NSBox *box, NSView *aView);
void NSBox_setContentViewMargins(NSBox *box, NSSize *offsetSize);
void NSBox_setFillColor(NSBox *box, NSColor *fillColor);
void NSBox_setFrameFromContentFrame(NSBox *box, NSRect *contentFrame);
void NSBox_setTitle(NSBox *box, NSString *aString);
void NSBox_setTitleFont(NSBox *box, NSFont *fontObj);
void NSBox_setTitlePosition(NSBox *box, int aPosition);
void NSBox_sizeToFit(NSBox *box);
NSCell* NSBox_titleCell(NSBox *box);
NSFont* NSBox_titleFont(NSBox *box);

#endif /* OSX_OBJC_NSBOX_H_ */
