/*
 * NSObject.h
 *
 *  Created on: Dec 31, 2020
 *      Author: azeddine
 */

#ifndef OSX_OBJC_NSOBJECT_H_
#define OSX_OBJC_NSOBJECT_H_
#include "swt.h"
typedef struct NSString NSString;
typedef struct NSArray NSArray;
typedef struct NSWindow NSWindow;
#if defined(__LP64__) && __LP64__
typedef long NSInteger;
typedef unsigned long NSUInteger;
# define CGFLOAT_TYPE double
# define CGFLOAT_IS_DOUBLE 1
#else
typedef int NSInteger;
typedef unsigned int NSUInteger;
# define CGFLOAT_TYPE float
# define CGFLOAT_IS_DOUBLE 0
#endif
typedef CGFLOAT_TYPE CGFloat;
typedef struct _NSPoint {
	CGFloat x;
	CGFloat y;
} NSPoint;

typedef NSPoint *NSPointPointer;
typedef NSPoint *NSPointArray;

typedef struct _NSSize {
	CGFloat width; /* should never be negative */
	CGFloat height; /* should never be negative */
} NSSize;

typedef NSSize *NSSizePointer;
typedef NSSize *NSSizeArray;

typedef union _NSRect {
	struct {
		NSPoint origin;
		NSSize size;
	};
	struct {
		CGFloat x;
		CGFloat y;
		CGFloat width; /* should never be negative */
		CGFloat height; /* should never be negative */
	};
} NSRect;

typedef NSRect *NSRectPointer;
typedef NSRect *NSRectArray;

typedef struct NSRange {
    NSUInteger location;
    NSUInteger length;
} NSRange;
typedef struct NSNumber NSNumber;
typedef struct NSValue NSValue;
/*
 * NSObject
 */
#define NSOBJECT(x) ((NSObject*)x)
Class NSObject_class(NSObject *object);
void NSObject_retain(NSObject *object);
void NSObject_release(NSObject *object);
void NSObject_autorelease(NSObject *object);
NSUInteger NSObject_retainCount(NSObject *object);
/*
 *  NSNumber
 */
int NSNumber_boolValue(NSNumber* number);
double NSNumber_doubleValue(NSNumber* number);
float NSNumber_floatValue(NSNumber* number);
int NSNumber_intValue(NSNumber* number);
NSUInteger NSNumber_integerValue(NSNumber* number);
NSNumber* NSNumber_numberWithBool(int value);
NSNumber* NSNumber_numberWithDouble(double value);
NSNumber* NSNumber_numberWithInt(int value);
NSNumber* NSNumber_numberWithInteger(NSInteger value);
NSValue* NSNumber_valueWithPoint(NSPoint* point);
NSValue* NSNumber_valueWithRange(NSRange* range);
NSValue* NSNumber_valueWithRect(NSRect* rect);
NSValue* NSNumber_valueWithSize(NSSize* size);
/*
 *  NSString
 */
NSString* NSString_new(const void *bytes, int length, int encoding);
NSString* NSString_stringWithUTF8String(const char *str);
NSString* NSString_stringWithUTF8(const char *str, int length);
NSString* NSString_initWithBytes(const void *bytes, int length, int encoding);
NSString* NSString_initWithBytes_utf8(const void *bytes, int length);

#endif /* OSX_OBJC_NSOBJECT_H_ */
