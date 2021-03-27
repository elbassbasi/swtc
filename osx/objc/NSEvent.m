#import "swt.h"
long NSEvent_CGEvent(NSEvent *nsEvent) {
    return [nsEvent CGEvent];
}
long NSEvent_buttonNumber(NSEvent *nsEvent) {
    return [nsEvent buttonNumber];
}
NSString* NSEvent_characters(NSEvent *nsEvent) {
    return [nsEvent characters];
}
NSString* NSEvent_charactersIgnoringModifiers(NSEvent *nsEvent) {
    return [nsEvent charactersIgnoringModifiers];
}
long NSEvent_clickCount(NSEvent *nsEvent) {
    return [nsEvent clickCount];
}
double NSEvent_deltaX(NSEvent *nsEvent) {
    return [nsEvent deltaX];
}
double NSEvent_deltaY(NSEvent *nsEvent) {
    return [nsEvent deltaY];
}
double NSEvent_doubleClickInterval() {
    return [NSEvent doubleClickInterval];
}
NSEvent* NSEvent_enterExitEventWithType(long type, NSPoint* location, long flags, double time, NSUInteger wNum, NSGraphicsContext* context, NSInteger eNum, NSInteger tNum, void* data) {
    return [NSEvent enterExitEventWithType:type location:*location modifierFlags:flags timestamp:time windowNumber:wNum context:context eventNumber:eNum trackingNumber:tNum userData:data];
}
short NSEvent_keyCode(NSEvent *nsEvent) {
    return [nsEvent keyCode];
}
void NSEvent_locationInWindow(NSEvent *nsEvent,NSPoint *result) {
    *result = [nsEvent locationInWindow];
}
double NSEvent_magnification(NSEvent *nsEvent) {
    return [nsEvent magnification];
}
long NSEvent_modifierFlags(NSEvent *nsEvent) {
    return [nsEvent modifierFlags];
}
void NSEvent_mouseLocation(NSPoint *result) {
    *result = [NSEvent mouseLocation];
}
NSEvent* NSEvent_otherEventWithType(long type, NSPoint* location, long flags, double time, NSInteger wNum, NSGraphicsContext *context, short subtype, NSInteger d1, NSInteger d2) {
    return [NSEvent otherEventWithType:type location:*location modifierFlags:flags timestamp:time windowNumber:wNum context:context subtype:subtype data1:d1 data2:d2];
}
long NSEvent_phase(NSEvent *nsEvent) {
    return [nsEvent phase];
}
NSUInteger NSEvent_pressedMouseButtons() {
    return [NSEvent pressedMouseButtons];
}
float NSEvent_rotation(NSEvent *nsEvent) {
    return [nsEvent rotation];
}
double NSEvent_timestamp(NSEvent *nsEvent) {
    return [nsEvent timestamp];
}
NSSet* NSEvent_touchesMatchingPhase(NSEvent *nsEvent,long phase, NSView* view) {
    return [nsEvent touchesMatchingPhase:phase inView:view];
}
long NSEvent_type(NSEvent *nsEvent) {
    return [nsEvent type];
}
NSWindow* NSEvent_window(NSEvent *nsEvent) {
    return [nsEvent window];
}

