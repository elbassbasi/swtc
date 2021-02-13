#import "swt.h"
NSAffineTransform* NSAffineTransform_new() {
    return [[NSAffineTransform alloc] init];
}
void NSAffineTransform_concat(NSAffineTransform* transform) {
    [transform concat];
}
void NSAffineTransform_set(NSAffineTransform* transform) {
    [transform set];
}

NSAffineTransform* NSAffineTransform_initWithTransform(NSAffineTransform* _transform) {
    return [[NSAffineTransform alloc] initWithTransform:_transform];
}

void NSAffineTransform_invert(NSAffineTransform* transform) {
    [transform invert];
}

void NSAffineTransform_prependTransform(NSAffineTransform* transform,NSAffineTransform* _transform) {
    [transform prependTransform:_transform];
}

void NSAffineTransform_rotateByDegrees(NSAffineTransform* transform,CGFloat angle) {
    [transform rotateByDegrees:angle];
}

void NSAffineTransform_scaleXBy(NSAffineTransform* transform,CGFloat scaleX, CGFloat scaleY) {
    [transform scaleXBy:scaleX yBy:scaleY];
}

void NSAffineTransform_setTransformStruct(NSAffineTransform* transform,NSAffineTransformStruct* transformStruct) {
    [transform setTransformStruct:*transformStruct];
}

NSAffineTransform* NSAffineTransform_transform() {
    return [NSAffineTransform transform];
}

void NSAffineTransform_transformPoint(NSAffineTransform* transform,NSPoint* aPoint,NSPoint* result) {
    *result = [transform transformPoint:*aPoint];
}

void NSAffineTransform_transformSize(NSAffineTransform* transform,NSSize* aSize,NSSize* result) {
    *result = [transform transformSize:*aSize];
}

void NSAffineTransform_transformStruct(NSAffineTransform* transform,NSAffineTransformStruct* st) {
    *st = [transform transformStruct];
}

void NSAffineTransform_translateXBy(NSAffineTransform* transform,CGFloat deltaX, CGFloat deltaY) {
    [transform translateXBy:deltaX yBy:deltaY];
}
