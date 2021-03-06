#import "swt.h"
/*
 *  NSMenu
 */
void NSMenu_addItem(NSMenu* menu,NSMenuItem *newItem) {
    [menu addItem:newItem];
}

NSMenuItem* NSMenu_addItemWithTitle(NSMenu* menu,NSString* aString, SEL aSelector, NSString* charCode) {
    return [menu addItemWithTitle:aString action:aSelector keyEquivalent:charCode];
}

void NSMenu_cancelTracking(NSMenu* menu) {
    [menu cancelTracking];
}

NSInteger NSMenu_indexOfItemWithTarget(NSMenu* menu,NSObject* target, SEL actionSelector) {
    return [menu indexOfItemWithTarget:target andAction:actionSelector];
}

NSMenu* NSMenu_initWithTitle(NSString* aTitle) {
    return [[NSMenu alloc] initWithTitle:aTitle];
}

void NSMenu_insertItem(NSMenu* menu,NSMenuItem* newItem, NSInteger index) {
    return [menu insertItem:newItem atIndex:index];
}

NSArray* NSMenu_itemArray(NSMenu* menu) {
    return [menu itemArray];
}

NSMenuItem* NSMenu_itemAtIndex(NSMenu* menu,NSInteger index) {
    return [menu itemAtIndex:index];
}

NSMenuItem* NSMenu_itemWithTag(NSMenu* menu,NSInteger tag) {
    return [menu itemWithTag:tag];
}

NSInteger NSMenu_numberOfItems(NSMenu* menu) {
    return [menu numberOfItems];
}

int NSMenu_performKeyEquivalent(NSMenu* menu,NSEvent* theEvent) {
    return [menu performKeyEquivalent:theEvent];
}

void NSMenu_popUpContextMenu(NSMenu* menu, NSEvent* event, NSView* view) {
    return [NSMenu popUpContextMenu:menu withEvent:event forView:view];
}

void NSMenu_removeItem(NSMenu* menu,NSMenuItem* item) {
    [menu removeItem:item];
}

void NSMenu_removeItemAtIndex(NSMenu* menu,NSInteger index) {
    [menu removeItemAtIndex:index];
}

void NSMenu_setAutoenablesItems(NSMenu* menu,int flag) {
    [menu setAutoenablesItems:flag];
}

void NSMenu_setDelegate(NSMenu* menu,NSObject* anObject) {
    [menu setDelegate:(id<NSMenuDelegate>)anObject];
}

void NSMenu_setSubmenu(NSMenu* menu,NSMenu* aMenu, NSMenuItem* anItem) {
    [menu setSubmenu:aMenu forItem:anItem];
}

void NSMenu_setTitle(NSMenu* menu,NSString* aString) {
    [menu setTitle:aString];
}

NSString* NSMenu_title(NSMenu* menu) {
    return [menu title];
}
/*
 *  NSMenuItem
 */
SEL NSMenuItem_action(NSMenuItem* item) {
    return [item action];
}

NSAttributedString* NSMenuItem_attributedTitle(NSMenuItem* item) {
    return [item attributedTitle];
}

NSImage* NSMenuItem_image(NSMenuItem* item) {
    return [item image];
}

NSMenuItem* NSMenuItem_initWithTitle(NSString* aString,SEL aSelector, NSString* charCode) {
    return [[NSMenuItem alloc] initWithTitle:aString action:aSelector keyEquivalent:charCode];
}

int NSMenuItem_isHidden(NSMenuItem* item) {
    return [item isHidden];
}

int NSMenuItem_isSeparatorItem(NSMenuItem* item) {
    return [item isSeparatorItem];
}

NSString* NSMenuItem_keyEquivalent(NSMenuItem* item) {
    return [item keyEquivalent];
}

int NSMenuItem_keyEquivalentModifierMask(NSMenuItem* item) {
    return [item keyEquivalentModifierMask];
}

NSMenuItem* NSMenuItem_separatorItem() {
    return [NSMenuItem separatorItem];
}

void NSMenuItem_setAction(NSMenuItem* item,SEL aSelector) {
    [item setAction:aSelector];
}

void NSMenuItem_setAttributedTitle(NSMenuItem* item,NSAttributedString* string) {
    [item setAttributedTitle:string];
}

void NSMenuItem_setEnabled(NSMenuItem* item,int flag) {
    [item setEnabled:flag];
}

void NSMenuItem_setHidden(NSMenuItem* item,int hidden) {
    [item setHidden:hidden];
}

void NSMenuItem_setImage(NSMenuItem* item,NSImage* menuImage) {
    [item setImage:menuImage];
}

void NSMenuItem_setKeyEquivalent(NSMenuItem* item,NSString *aKeyEquivalent) {
    [item setKeyEquivalent:aKeyEquivalent];
}

void NSMenuItem_setKeyEquivalentModifierMask(NSMenuItem* item,int mask) {
    [item setKeyEquivalentModifierMask:mask];
}

void NSMenuItem_setMenu(NSMenuItem* item,NSMenu* menu) {
    [item setMenu:menu];
}

void NSMenuItem_setState(NSMenuItem* item,int state) {
    [item setState:state];
}

void NSMenuItem_setSubmenu(NSMenuItem* item,NSMenu* submenu) {
    [item setSubmenu:submenu];
}

void NSMenuItem_setTag(NSMenuItem* item,NSInteger anInt) {
    [item setTag:anInt];
}

void NSMenuItem_setTarget(NSMenuItem* item,NSObject* anObject) {
    [item setTarget:anObject];
}

void NSMenuItem_setTitle(NSMenuItem* item,NSString* aString) {
    [item setTitle:aString];
}

void NSMenuItem_setToolTip(NSMenuItem* item,NSString* toolTip) {
    [item setToolTip:toolTip];
}

int NSMenuItem_state(NSMenuItem* item) {
    return [item state];
}

NSMenu* NSMenuItem_submenu(NSMenuItem* item) {
    return [item submenu];
}

NSInteger NSMenuItem_tag(NSMenuItem* item) {
    return [item tag];
}

NSObject* NSMenuItem_target(NSMenuItem* item) {
    return [item target];
}

NSString* NSMenuItem_title(NSMenuItem* item) {
     return [item title];
}
