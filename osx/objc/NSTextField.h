/*
 * NSTextField.h
 *
 *  Created on: May 1, 2021
 *      Author: azeddine
 */

#ifndef OSX_OBJC_NSTEXTFIELD_H_
#define OSX_OBJC_NSTEXTFIELD_H_
#include "NSView.h"
typedef struct NSTextField NSTextField;
typedef struct NSTextFieldCell NSTextFieldCell;
/*
 * NSTextField
 */
w_widget* SWTTextField_get_widget(NSView *view);
NSTextField* SWTTextField_new(w_widget *widget);
void NSTextField_selectText(NSTextField *text, NSObject *sender);
void NSTextField_setBackgroundColor(NSTextField *text, NSColor *color);
void NSTextField_setBordered(NSTextField *text, int flag);
void NSTextField_setDelegate(NSTextField *text, NSObject *anObject);
void NSTextField_setDrawsBackground(NSTextField *text, int flag);
void NSTextField_setEditable(NSTextField *text, int flag);
void NSTextField_setSelectable(NSTextField *text, int flag);
void NSTextField_setTextColor(NSTextField *text, NSColor *color);
Class NSTextField_cellClass();
void NSTextField_setCellClass(Class factoryId);
/*
 *  NSTextFieldCell
 */
void NSTextFieldCell_setPlaceholderString(NSTextFieldCell *cell,NSString* string);
void NSTextFieldCell_setTextColor(NSTextFieldCell *cell,NSColor* color);
NSColor* NSTextFieldCell_textColor(NSTextFieldCell *cell);

#endif /* OSX_OBJC_NSTEXTFIELD_H_ */
