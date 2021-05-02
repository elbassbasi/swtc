#import "swt.h"
@interface SWTProgressIndicator : NSProgressIndicator {
 @public w_widget* widget;
}
@end
@implementation SWTProgressIndicator

@end
w_widget* SWTProgressIndicator_get_widget(NSView* view){
    if([view isKindOfClass:SWTProgressIndicator.class]){
        return ((SWTProgressIndicator*)view)->widget;
    }
    return 0;
}
NSProgressIndicator* SWTProgressIndicator_new(w_widget* widget){
    SWTProgressIndicator* progress =  [[SWTProgressIndicator alloc] init];
    _w_toolkit_registre_class(_NS_CLASS_PROGRESSINDICATOR,progress.class,SWTProgressIndicator_get_widget);
    progress->widget = widget;
    return progress;
}
NSProgressIndicator* NSProgressIndicator_new(){
    return [[SWTProgressIndicator alloc] init];
}
NSUInteger NSProgressIndicator_controlSize(NSProgressIndicator* progress) {
    return [progress controlSize];
}

CGFloat NSProgressIndicator_doubleValue(NSProgressIndicator* progress) {
    return [progress doubleValue];
}

CGFloat NSProgressIndicator_maxValue(NSProgressIndicator* progress) {
    return [progress maxValue];
}

CGFloat NSProgressIndicator_minValue(NSProgressIndicator* progress) {
    return [progress minValue];
}

void NSProgressIndicator_setControlSize(NSProgressIndicator* progress,NSUInteger size) {
    [progress setControlSize:size];
}

void NSProgressIndicator_setDoubleValue(NSProgressIndicator* progress,CGFloat doubleValue) {
    [progress setDoubleValue:doubleValue];
}

void NSProgressIndicator_setIndeterminate(NSProgressIndicator* progress,NSInteger flag) {
    [progress setIndeterminate:flag];
}

void NSProgressIndicator_setMaxValue(NSProgressIndicator* progress,CGFloat newMaximum) {
    [progress setMaxValue:newMaximum];
}

void NSProgressIndicator_setMinValue(NSProgressIndicator* progress,CGFloat newMinimum) {
    [progress setMinValue:newMinimum];
}

void NSProgressIndicator_setUsesThreadedAnimation(NSProgressIndicator* progress,NSInteger threadedAnimation) {
    [progress setUsesThreadedAnimation:threadedAnimation];
}

void NSProgressIndicator_sizeToFit(NSProgressIndicator* progress) {
    [progress sizeToFit];
}

void NSProgressIndicator_startAnimation(NSProgressIndicator* progress,NSObject*  sender) {
    [progress startAnimation:sender];
}

void NSProgressIndicator_stopAnimation(NSProgressIndicator* progress,NSObject* sender) {
    [progress stopAnimation:sender];
}
