#import "swt.h"
/*
 * NSObject
 */
Class NSObject_class(NSObject* object){
    return object.class;
}
void NSObject_retain(NSObject* object){
    [object retain];
}
void NSObject_release(NSObject* object){
    [object release];
}
void NSObject_autorelease(NSObject* object){
    [object autorelease];
}
NSUInteger NSObject_retainCount(NSObject* object){
    return [object retainCount];
}
NSObject* NSObject_copy(NSObject *object){
    return [object copy];
}
/*
 *  NSNumber
 */
int NSNumber_boolValue(NSNumber* number) {
    return [number boolValue];
}
double NSNumber_doubleValue(NSNumber* number) {
    return [number doubleValue];
}
float NSNumber_floatValue(NSNumber* number) {
    return [number floatValue];
}
int NSNumber_intValue(NSNumber* number) {
    return [number intValue];
}
NSUInteger NSNumber_integerValue(NSNumber* number) {
    return [number integerValue];
}
NSNumber* NSNumber_numberWithBool(int value) {
    return [NSNumber numberWithBool:value];
}
NSNumber* NSNumber_numberWithDouble(double value) {
    return [NSNumber numberWithDouble:value];
}
NSNumber* NSNumber_numberWithInt(int value) {
    return [NSNumber numberWithInt:value];
}
NSNumber* NSNumber_numberWithInteger(NSInteger value) {
    return [NSNumber numberWithInteger:value];
}
NSValue* NSNumber_valueWithPoint(NSPoint* point) {
    return [NSNumber valueWithPoint:*point];
}
NSValue* NSNumber_valueWithRange(NSRange* range) {
    return [NSNumber valueWithRange:*range];
}
NSValue* NSNumber_valueWithRect(NSRect* rect) {
    return [NSNumber valueWithRect:*rect];
}
NSValue* NSNumber_valueWithSize(NSSize* size) {
    return [NSNumber valueWithSize:*size];
}
/*
 *
 */
NSString* NSString_stringWithUTF8String(const char* str) {
    return [NSString stringWithUTF8String:str];
}
NSString* NSString_stringWithCharacters(const wchar_t* str,int length) {
    return [NSString stringWithCharacters:(const unichar*)str length:length];
}
NSString* NSString_stringWithUTF8(const char* str,int length) {
    if(length == -1){
        return [NSString stringWithUTF8String:str];
    }else{
        NSString* _str = [NSString alloc];
        return [_str initWithBytes:str length:length encoding:NSUTF8StringEncoding];
    }
}
NSString* NSString_initWithBytes(const void *bytes,int length, int encoding){
    NSString* str = [NSString alloc];
    return [str initWithBytes:bytes length:length encoding:encoding];
}
NSString* NSString_initWithBytes_utf8(const void *bytes,int length){
    if(length <0){
        return NSString_stringWithUTF8String((const char*)bytes);
    }else{
        return NSString_initWithBytes(bytes,length,NSUTF8StringEncoding);
    }
}
NSString* NSString_new(const void *bytes,int length, int encoding){
    if(bytes == 0) return 0;
    if(encoding == W_ENCODING_UNICODE){
        return NSString_stringWithCharacters((const wchar_t*)bytes,length);
    }else{
        return NSString_initWithBytes_utf8(bytes,length);
    }
}
