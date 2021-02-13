/*
 * NSAffineTransform.h
 *
 *  Created on: Jan 24, 2021
 *      Author: azeddine
 */

#ifndef OSX_OBJC_NSAFFINETRANSFORM_H_
#define OSX_OBJC_NSAFFINETRANSFORM_H_
#include "NSObject.h"
typedef struct {
    CGFloat m11, m12, m21, m22;
    CGFloat tX, tY;
} NSAffineTransformStruct;
typedef struct NSAffineTransform NSAffineTransform;
NSAffineTransform* NSAffineTransform_new();
void NSAffineTransform_concat(NSAffineTransform* transform);
void NSAffineTransform_set(NSAffineTransform* transform);
NSAffineTransform* NSAffineTransform_initWithTransform(NSAffineTransform* _transform);
void NSAffineTransform_invert(NSAffineTransform* transform);
void NSAffineTransform_prependTransform(NSAffineTransform* transform,NSAffineTransform* _transform);
void NSAffineTransform_rotateByDegrees(NSAffineTransform* transform,CGFloat angle);
void NSAffineTransform_scaleXBy(NSAffineTransform* transform,CGFloat scaleX, CGFloat scaleY);
void NSAffineTransform_setTransformStruct(NSAffineTransform* transform,NSAffineTransformStruct* transformStruct);
NSAffineTransform* NSAffineTransform_transform();
void NSAffineTransform_transformPoint(NSAffineTransform* transform,NSPoint* aPoint,NSPoint* result);
void NSAffineTransform_transformSize(NSAffineTransform* transform,NSSize* aSize,NSSize* result);
void NSAffineTransform_transformStruct(NSAffineTransform* transform,NSAffineTransformStruct* st);
void NSAffineTransform_translateXBy(NSAffineTransform* transform,CGFloat deltaX, CGFloat deltaY);
#endif /* OSX_OBJC_NSAFFINETRANSFORM_H_ */
