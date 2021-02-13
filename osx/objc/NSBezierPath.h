/*
 * NSBezierPath.h
 *
 *  Created on: Jan 17, 2021
 *      Author: azeddine
 */

#ifndef OSX_OBJC_NSBEZIERPATH_H_
#define OSX_OBJC_NSBEZIERPATH_H_
#include "NSArray.h"
typedef struct NSBezierPath NSBezierPath;
typedef struct NSGlyph NSGlyph;
typedef struct NSFont NSFont;
typedef struct NSAffineTransform NSAffineTransform;
NSBezierPath* NSBezierPath_new();
void NSBezierPath_addClip(NSBezierPath* path);
void NSBezierPath_appendBezierPath(NSBezierPath* path,NSBezierPath* _path);
void NSBezierPath_appendBezierPathWithArcWithCenter(NSBezierPath* path,NSPoint* center, double radius, double startAngle, double endAngle);
void NSBezierPath_appendBezierPathWithArcWithCenter_0(NSBezierPath* path,NSPoint* center, double radius, double startAngle, double endAngle, int clockwise);
void NSBezierPath_appendBezierPathWithGlyphs(NSBezierPath* path,NSGlyph* glyphs, int count, NSFont* font);
void NSBezierPath_appendBezierPathWithOvalInRect(NSBezierPath* path,NSRect* rect);
void NSBezierPath_appendBezierPathWithRect(NSBezierPath* path,NSRect* rect);
void NSBezierPath_appendBezierPathWithRoundedRect(NSBezierPath* path,NSRect* rect, double xRadius, double yRadius);
NSBezierPath* NSBezierPath_bezierPath();
NSBezierPath* NSBezierPath_bezierPathByFlatteningPath(NSBezierPath* path);
NSBezierPath* NSBezierPath_bezierPathWithRect(NSRect* rect);
void NSBezierPath_bounds(NSBezierPath* path,NSRect* rect);
void NSBezierPath_closePath(NSBezierPath* path);
int NSBezierPath_containsPoint(NSBezierPath* path,NSPoint* point);
void NSBezierPath_controlPointBounds(NSBezierPath* path,NSRect* rect);
void NSBezierPath_currentPoint(NSBezierPath* path,NSPoint* point);
void NSBezierPath_curveToPoint(NSBezierPath* path,NSPoint* endPoint, NSPoint* controlPoint1, NSPoint* controlPoint2);
double NSBezierPath_defaultFlatness();
int NSBezierPath_elementAtIndex(NSBezierPath* path,int index, NSPoint* points);
int NSBezierPath_elementCount(NSBezierPath* path);
void NSBezierPath_fill(NSBezierPath* path);
void NSBezierPath_fillRect(NSRect* rect);
int NSBezierPath_isEmpty(NSBezierPath* path);
void NSBezierPath_lineToPoint(NSBezierPath* path,NSPoint* point);
void NSBezierPath_moveToPoint(NSBezierPath* path,NSPoint* point);
void NSBezierPath_removeAllPoints(NSBezierPath* path);
void NSBezierPath_setClip(NSBezierPath* path);
void NSBezierPath_setDefaultFlatness(double flatness);
void NSBezierPath_setLineCapStyle(NSBezierPath* path,int lineCapStyle);
void NSBezierPath_setLineDash(NSBezierPath* path,double* pattern, int count, double phase);
void NSBezierPath_setLineJoinStyle(NSBezierPath* path,int lineJoinStyle);
void NSBezierPath_setLineWidth(NSBezierPath* path,double lineWidth);
void NSBezierPath_setMiterLimit(NSBezierPath* path,double miterLimit);
void NSBezierPath_setWindingRule(NSBezierPath* path,int windingRule);
void NSBezierPath_stroke(NSBezierPath* path);
void NSBezierPath_strokeRect(NSBezierPath* path,NSRect* rect);
void NSBezierPath_transformUsingAffineTransform(NSBezierPath* path,NSAffineTransform* transform);
#endif /* OSX_OBJC_NSBEZIERPATH_H_ */
