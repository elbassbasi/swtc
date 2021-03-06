#import "swt.h"
@interface SWTButton : NSButton {
 @public w_widget* widget;
}
@end
@implementation SWTButton
-(void)btnClicked {
    _w_event_platform e;
    _w_event_platform_init(&e, widget, self, _NS_selection);
    _w_widget_send_event(widget,W_EVENT(&e));
}

@end
/*
 * NSButton
 */
w_widget* SWTButton_get_widget(NSView* view){
    if([view isKindOfClass:SWTButton.class]){
        return ((SWTButton*)view)->widget;
    }
    return 0;
}
NSButton* SWTButton_new(w_widget* widget){
    SWTButton* button =  [[SWTButton alloc] init];
    _w_toolkit_registre_class(_NS_CLASS_BUTTON,button.class,SWTButton_get_widget);
    button->widget = widget;
    [button setTarget:button];
    [button setAction:@selector(btnClicked)];
    return button;
}
NSAttributedString* NSButton_attributedTitle(NSButton* button) {
    return [button attributedTitle];
}

long NSButton_bezelStyle(NSButton* button) {
    return [button bezelStyle];
}

void NSButton_setAllowsMixedState(NSButton* button,int allowsMixedState) {
    [button setAllowsMixedState:allowsMixedState];
}

void NSButton_setAttributedTitle(NSButton* button,NSAttributedString* attributedTitle) {
    [button setAttributedTitle:attributedTitle];
}

void NSButton_setBezelStyle(NSButton* button,long bezelStyle) {
    [button setBezelStyle:bezelStyle];
}

void NSButton_setBordered(NSButton* button,int bordered) {
    [button setBordered:bordered];
}

void NSButton_setButtonType(NSButton* button,long aType) {
    [button setButtonType:aType];
}

void NSButton_setImage(NSButton* button,NSImage* image) {
    [button setImage:image];
}

void NSButton_setImagePosition(NSButton* button,long imagePosition) {
    [button setImagePosition:imagePosition];
}

void NSButton_setKeyEquivalent(NSButton* button,NSString* keyEquivalent) {
    [button setKeyEquivalent:keyEquivalent];
}

void NSButton_setState(NSButton* button,long state) {
    [button setState:state];
}

void NSButton_setTitle(NSButton* button,NSString* title) {
    [button setTitle:title];
}

long NSButton_state(NSButton* button) {
    return [button state];
}

Class NSButton_cellClass() {
    return [NSButton cellClass];
}

void NSButton_setCellClass(Class factoryId) {
    [NSButton setCellClass:factoryId];
}
/*
 * NSButtonCell
 */
@interface SWTButtonCell : NSButtonCell {
    w_widget* widget;
}
@end
@implementation SWTButtonCell

@end
SWTButtonCell* SWTButtonCell_new() {
    return [[SWTButtonCell alloc] init];
}
void NSButtonCell_drawBezelWithFrame(NSButtonCell* cell,NSRect* frame, NSView* controlView) {
    [cell drawBezelWithFrame:*frame inView:controlView];
}

void NSButtonCell_drawImage(NSButtonCell* cell,NSImage* image, NSRect* frame, NSView *controlView) {
    [cell drawImage:image withFrame:*frame inView:controlView];
}

void NSButtonCell_drawTitle(NSButtonCell* cell,NSAttributedString* title,NSRect* result, NSRect* frame, NSView* controlView) {
    *result = [cell drawTitle:title withFrame:*frame inView:controlView];
}

void NSButtonCell_setBackgroundColor(NSButtonCell* cell,NSColor* backgroundColor) {
    [cell setBackgroundColor:backgroundColor];
}

void NSButtonCell_setButtonType(NSButtonCell* cell,NSInteger aType) {
    [cell setButtonType:aType];
}

void NSButtonCell_setHighlightsBy(NSButtonCell* cell,NSInteger highlightsBy) {
    [cell setHighlightsBy:highlightsBy];
}

void NSButtonCell_setImagePosition(NSButtonCell* cell,NSInteger imagePosition) {
    [cell setImagePosition:imagePosition];
}

NSString* NSButtonCell_title(NSButtonCell* cell) {
    return [cell title];
}

