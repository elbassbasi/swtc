/*
 * NSButton.h
 *
 *  Created on: Mar 6, 2021
 *      Author: azeddine
 */

#ifndef OSX_OBJC_NSBUTTON_H_
#define OSX_OBJC_NSBUTTON_H_
#include "NSView.h"
typedef struct NSButton NSButton;
typedef struct NSAttributedString NSAttributedString;
NSButton* SWTButton_new(w_widget* widget);
w_widget* SWTButton_get_widget(NSView* view);
NSAttributedString* NSButton_attributedTitle(NSButton* button);
long NSButton_bezelStyle(NSButton* button);
void NSButton_setAllowsMixedState(NSButton* button,int allowsMixedState);
void NSButton_setAttributedTitle(NSButton* button,NSAttributedString* attributedTitle);
void NSButton_setBezelStyle(NSButton* button,long bezelStyle);
void NSButton_setBordered(NSButton* button,int bordered);
void NSButton_setButtonType(NSButton* button,long aType);
void NSButton_setImage(NSButton* button,NSImage* image);
void NSButton_setImagePosition(NSButton* button,long imagePosition);
void NSButton_setKeyEquivalent(NSButton* button,NSString* keyEquivalent);
void NSButton_setState(NSButton* button,long state);
void NSButton_setTitle(NSButton* button,NSString* title);
long NSButton_state(NSButton* button);
Class NSButton_cellClass();
void NSButton_setCellClass(Class factoryId);
/*
 * NSButtonCell
 */
NSButtonCell* SWTButtonCell_new();
void NSButtonCell_drawBezelWithFrame(NSButtonCell* cell,NSRect* frame, NSView* controlView);
void NSButtonCell_drawImage(NSButtonCell* cell,NSImage* image, NSRect* frame, NSView *controlView);
void NSButtonCell_drawTitle(NSButtonCell* cell,NSAttributedString* title,NSRect* result, NSRect* frame, NSView* controlView);
void NSButtonCell_setBackgroundColor(NSButtonCell* cell,NSColor* backgroundColor);
void NSButtonCell_setButtonType(NSButtonCell* cell,NSInteger aType);
void NSButtonCell_setHighlightsBy(NSButtonCell* cell,NSInteger highlightsBy);
void NSButtonCell_setImagePosition(NSButtonCell* cell,NSInteger imagePosition);
NSString* NSButtonCell_title(NSButtonCell* cell);

#endif /* OSX_OBJC_NSBUTTON_H_ */
