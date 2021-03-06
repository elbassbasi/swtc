/*
 * NSMenu.h
 *
 *  Created on: Feb 26, 2021
 *      Author: azeddine
 */

#ifndef OSX_OBJC_NSMENU_H_
#define OSX_OBJC_NSMENU_H_
#include "NSView.h"
typedef struct NSMenu NSMenu;
typedef struct NSMenuItem NSMenuItem;
/*
 *  NSMenu
 */
void NSMenu_addItem(NSMenu *menu, NSMenuItem *newItem);
NSMenuItem* NSMenu_addItemWithTitle(NSMenu *menu, NSString *aString,
		SEL aSelector, NSString *charCode);
void NSMenu_cancelTracking(NSMenu *menu);
NSInteger NSMenu_indexOfItemWithTarget(NSMenu *menu, NSObject *target,
		SEL actionSelector);
NSMenu* NSMenu_initWithTitle(NSString *aTitle);
void NSMenu_insertItem(NSMenu *menu, NSMenuItem *newItem, NSInteger index);
NSArray* NSMenu_itemArray(NSMenu *menu);
NSMenuItem* NSMenu_itemAtIndex(NSMenu *menu, NSInteger index);
NSMenuItem* NSMenu_itemWithTag(NSMenu *menu, NSInteger tag);
NSInteger NSMenu_numberOfItems(NSMenu *menu);
int NSMenu_performKeyEquivalent(NSMenu *menu, NSEvent *theEvent);
void NSMenu_popUpContextMenu(NSMenu *menu, NSEvent *event, NSView *view);
void NSMenu_removeItem(NSMenu *menu, NSMenuItem *item);
void NSMenu_removeItemAtIndex(NSMenu *menu, NSInteger index);
void NSMenu_setAutoenablesItems(NSMenu *menu, int flag);
void NSMenu_setDelegate(NSMenu *menu, NSObject *anObject);
void NSMenu_setSubmenu(NSMenu *menu, NSMenu *aMenu, NSMenuItem *anItem);
void NSMenu_setTitle(NSMenu *menu, NSString *aString);
NSString* NSMenu_title(NSMenu *menu);
/*
 *  NSMenuItem
 */
SEL NSMenuItem_action(NSMenuItem *item);
NSAttributedString* NSMenuItem_attributedTitle(NSMenuItem *item);
NSImage* NSMenuItem_image(NSMenuItem *item);
NSMenuItem* NSMenuItem_initWithTitle(NSString *aString, SEL aSelector,
		NSString *charCode);
int NSMenuItem_isHidden(NSMenuItem *item);
int NSMenuItem_isSeparatorItem(NSMenuItem *item);
NSString* NSMenuItem_keyEquivalent(NSMenuItem *item);
int NSMenuItem_keyEquivalentModifierMask(NSMenuItem *item);
NSMenuItem* NSMenuItem_separatorItem();
void NSMenuItem_setAction(NSMenuItem *item, SEL aSelector);
void NSMenuItem_setAttributedTitle(NSMenuItem *item,
		NSAttributedString *string);
void NSMenuItem_setEnabled(NSMenuItem *item, int flag);
void NSMenuItem_setHidden(NSMenuItem *item, int hidden);
void NSMenuItem_setImage(NSMenuItem *item, NSImage *menuImage);
void NSMenuItem_setKeyEquivalent(NSMenuItem *item, NSString *aKeyEquivalent);
void NSMenuItem_setKeyEquivalentModifierMask(NSMenuItem *item, int mask);
void NSMenuItem_setMenu(NSMenuItem *item, NSMenu *menu);
void NSMenuItem_setState(NSMenuItem *item, int state);
void NSMenuItem_setSubmenu(NSMenuItem *item, NSMenu *submenu);
void NSMenuItem_setTag(NSMenuItem *item, NSInteger anInt);
void NSMenuItem_setTarget(NSMenuItem *item, NSObject *anObject);
void NSMenuItem_setTitle(NSMenuItem *item, NSString *aString);
void NSMenuItem_setToolTip(NSMenuItem *item, NSString *toolTip);
int NSMenuItem_state(NSMenuItem *item);
NSMenu* NSMenuItem_submenu(NSMenuItem *item);
NSInteger NSMenuItem_tag(NSMenuItem *item);
NSObject* NSMenuItem_target(NSMenuItem *item);
NSString* NSMenuItem_title(NSMenuItem *item);
#endif /* OSX_OBJC_NSMENU_H_ */
