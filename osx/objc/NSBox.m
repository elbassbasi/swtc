#import "swt.h"
#import <objc/objc.h>
#import <objc/runtime.h>
@interface SWTBox : NSBox{
@public w_widget* widget;
}
@end
@implementation SWTBox
//_NSMETHOD_EVENT_3(NSBox)
//_NSMETHOD_EVENT_2(NSBox)
//_NSMETHOD_setNeedsDisplayInRect(NSBox)
//_NSMETHOD_drawRect(NSBox)
//_NSMETHOD_hitTest(NSBox)
@end
w_widget* SWTBox_get_widget(NSView* view){
    if( [view isKindOfClass:[SWTBox class]])
    return ((SWTBox*)view)->widget;
    else return 0;
}
NSView* SWTBox_new(w_widget* widget){
    NSRect frame=NSMakeRect(0, 0, 0, 0);
    SWTBox* box = [[SWTBox alloc] initWithFrame:frame];
    _w_toolkit_registre_class(_NS_CLASS_BOX,box.class,SWTBox_get_widget);
    box->widget = widget;
    return box;
}
CGFloat NSBox_borderWidth(NSBox* box) {
     return [box borderWidth];
}

NSView* NSBox_contentView(NSBox* box) {
     return [box contentView];
}

void NSBox_contentViewMargins(NSBox* box,NSSize* size) {
     *size = [box contentViewMargins];
}

void NSBox_setBorderType(NSBox* box,int aType) {
     [box setBorderType:aType];
}

void NSBox_setBorderWidth(NSBox* box,CGFloat borderWidth) {
     [box setBorderWidth:borderWidth];
}

void NSBox_setBoxType(NSBox* box,int boxType) {
     [box setBoxType:boxType];
}

void NSBox_setContentView(NSBox* box,NSView* aView) {
     [box setContentView:aView];
}

void NSBox_setContentViewMargins(NSBox* box,NSSize* offsetSize) {
     [box setContentViewMargins:*offsetSize];
}

void NSBox_setFillColor(NSBox* box,NSColor* fillColor) {
     [box setFillColor:fillColor];
}

void NSBox_setFrameFromContentFrame(NSBox* box,NSRect* contentFrame) {
     [box setFrameFromContentFrame:*contentFrame];
}

void NSBox_setTitle(NSBox* box,NSString* aString) {
     [box setTitle:aString];
}

void NSBox_setTitleFont(NSBox* box,NSFont* fontObj) {
     [box setTitleFont:fontObj];
}

void NSBox_setTitlePosition(NSBox* box,int aPosition) {
     [box setTitlePosition:aPosition];
}

void NSBox_sizeToFit(NSBox* box) {
     [box sizeToFit];
}

NSCell* NSBox_titleCell(NSBox* box) {
     return [box titleCell];
}

NSFont* NSBox_titleFont(NSBox* box) {
    return [box titleFont];
}
