#import "swt.h"
NSBezierPath* NSBezierPath_new() {
    return [[NSBezierPath alloc] init];
}
void NSBezierPath_addClip(NSBezierPath* path) {
    [path addClip];
}

void NSBezierPath_appendBezierPath(NSBezierPath* path,NSBezierPath* _path) {
    [path appendBezierPath:_path];
}

void NSBezierPath_appendBezierPathWithArcWithCenter(NSBezierPath* path,NSPoint* center, double radius, double startAngle, double endAngle) {
    [path appendBezierPathWithArcWithCenter:*center radius:radius startAngle:startAngle endAngle:endAngle];
}

void NSBezierPath_appendBezierPathWithArcWithCenter_0(NSBezierPath* path,NSPoint* center, double radius, double startAngle, double endAngle, int clockwise) {
    [path appendBezierPathWithArcWithCenter:*center radius:radius startAngle:startAngle endAngle:endAngle clockwise:clockwise];
}

void NSBezierPath_appendBezierPathWithGlyphs(NSBezierPath* path,NSGlyph* glyphs, int count, NSFont* font) {
    [path appendBezierPathWithGlyphs:glyphs count:count inFont:font];
}

void NSBezierPath_appendBezierPathWithOvalInRect(NSBezierPath* path,NSRect* rect) {
    [path appendBezierPathWithOvalInRect:*rect];
}

void NSBezierPath_appendBezierPathWithRect(NSBezierPath* path,NSRect* rect) {
    [path appendBezierPathWithRect:*rect];
}

void NSBezierPath_appendBezierPathWithRoundedRect(NSBezierPath* path,NSRect* rect, double xRadius, double yRadius) {
    [path appendBezierPathWithRoundedRect:*rect xRadius:xRadius yRadius:yRadius];
}

NSBezierPath* NSBezierPath_bezierPath() {
    return [NSBezierPath bezierPath];
}

NSBezierPath* NSBezierPath_bezierPathByFlatteningPath(NSBezierPath* path) {
    return [path bezierPathByFlatteningPath];
}

NSBezierPath* NSBezierPath_bezierPathWithRect(NSRect* rect) {
    return [NSBezierPath bezierPathWithRect:*rect];
}

void NSBezierPath_bounds(NSBezierPath* path,NSRect* rect) {
    *rect = [path bounds];
}

void NSBezierPath_closePath(NSBezierPath* path) {
    [path closePath];
}

int NSBezierPath_containsPoint(NSBezierPath* path,NSPoint* point) {
    return [path containsPoint:*point];
}

void NSBezierPath_controlPointBounds(NSBezierPath* path,NSRect* rect) {
    *rect = [path controlPointBounds];
}

void NSBezierPath_currentPoint(NSBezierPath* path,NSPoint* point) {
   *point =  [path currentPoint];
}

void NSBezierPath_curveToPoint(NSBezierPath* path,NSPoint* endPoint, NSPoint* controlPoint1, NSPoint* controlPoint2) {
    [path curveToPoint:*endPoint controlPoint1:*controlPoint1 controlPoint2:*controlPoint2];
}

double NSBezierPath_defaultFlatness() {
    return [NSBezierPath defaultFlatness];
}

int NSBezierPath_elementAtIndex(NSBezierPath* path,int index, NSPoint* points) {
    return [path elementAtIndex:index associatedPoints:points];
}

int NSBezierPath_elementCount(NSBezierPath* path) {
    return [path elementCount];
}

void NSBezierPath_fill(NSBezierPath* path) {
    [path fill];
}

void NSBezierPath_fillRect(NSRect* rect) {
    [NSBezierPath fillRect:*rect];
}

int NSBezierPath_isEmpty(NSBezierPath* path) {
    return [path isEmpty];
}

void NSBezierPath_lineToPoint(NSBezierPath* path,NSPoint* point) {
    [path lineToPoint:*point];
}

void NSBezierPath_moveToPoint(NSBezierPath* path,NSPoint* point) {
   [path moveToPoint:*point];
}

void NSBezierPath_removeAllPoints(NSBezierPath* path) {
    [path removeAllPoints];
}

void NSBezierPath_setClip(NSBezierPath* path) {
    [path setClip];
}

void NSBezierPath_setDefaultFlatness(double flatness) {
    [NSBezierPath setDefaultFlatness:flatness];
}

void NSBezierPath_setLineCapStyle(NSBezierPath* path,int lineCapStyle) {
    [path setLineCapStyle:lineCapStyle];
}

void NSBezierPath_setLineDash(NSBezierPath* path,double* pattern, int count, double phase) {
    [path setLineDash:pattern count:count phase:phase];
}

void NSBezierPath_setLineJoinStyle(NSBezierPath* path,int lineJoinStyle) {
    [path setLineJoinStyle:lineJoinStyle];
}

void NSBezierPath_setLineWidth(NSBezierPath* path,double lineWidth) {
    [path setLineWidth:lineWidth];
}

void NSBezierPath_setMiterLimit(NSBezierPath* path,double miterLimit) {
    [path setMiterLimit:miterLimit];
}

void NSBezierPath_setWindingRule(NSBezierPath* path,int windingRule) {
    [path setWindingRule:windingRule];
}

void NSBezierPath_stroke(NSBezierPath* path) {
    [path stroke];
}

void NSBezierPath_strokeRect(NSBezierPath* path,NSRect* rect) {
    [NSBezierPath strokeRect:*rect];
}

void NSBezierPath_transformUsingAffineTransform(NSBezierPath* path,NSAffineTransform* transform) {
    [path transformUsingAffineTransform:transform];
}
