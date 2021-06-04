#import "swt.h"
@interface SWTTextField : NSTextField<NSTextFieldDelegate> {
 @public w_widget* widget;
}
@end
@implementation SWTTextField

@end
/*
 * NSTextField
 */
w_widget* SWTTextField_get_widget(NSView* view){
    if([view isKindOfClass:SWTTextField.class]){
        return ((SWTTextField*)view)->widget;
    }
    return 0;
}
NSTextField* SWTTextField_new(w_widget* widget){
    SWTTextField* text =  [[SWTTextField alloc] init];
    _w_toolkit_registre_class(_NS_CLASS_TEXTFIELD,text.class,SWTTextField_get_widget);
    text->widget = widget;
    return text;
}
void NSTextField_selectText(NSTextField *text,NSObject* sender) {
    [text selectText:sender];
}

void NSTextField_setBackgroundColor(NSTextField *text,NSColor* color) {
    [text setBackgroundColor:color];
}

void NSTextField_setBordered(NSTextField *text,int flag) {
    [text setBordered:flag];
}

void NSTextField_setDelegate(NSTextField *text,NSObject* anObject) {
    [text setDelegate:(id<NSTextFieldDelegate>)anObject];
}

void NSTextField_setDrawsBackground(NSTextField *text,int flag) {
    [text setDrawsBackground:flag];
}

void NSTextField_setEditable(NSTextField *text,int flag) {
    [text setEditable:flag];
}

void NSTextField_setSelectable(NSTextField *text,int flag) {
    [text setSelectable:flag];
}

void NSTextField_setTextColor(NSTextField *text,NSColor* color) {
    [text setTextColor:color];
}

Class NSTextField_cellClass() {
    return [NSTextField cellClass];
}

void NSTextField_setCellClass(Class factoryId) {
    [NSTextField setCellClass:factoryId];
}
/*
 *  NSTextFieldCell
 */
void NSTextFieldCell_setPlaceholderString(NSTextFieldCell *cell,NSString* string) {
    [cell setPlaceholderString:string];
}

void NSTextFieldCell_setTextColor(NSTextFieldCell *cell,NSColor* color) {
    [cell setTextColor:color];
}

NSColor* NSTextFieldCell_textColor(NSTextFieldCell *cell) {
    return [cell textColor];
}
