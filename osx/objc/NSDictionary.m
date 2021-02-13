#import "swt.h"
/*
 *  NSDictionary
 */
NSArray* NSDictionary_allKeys(NSDictionary* dictionary) {
    return [dictionary allKeys];
}
int NSDictionary_count(NSDictionary* dictionary) {
    return [dictionary count];
}
NSDictionary* NSDictionary_dictionaryWithObject(NSObject* object, NSObject* key) {
    return [NSDictionary dictionaryWithObject:object forKey:(NSObject<NSCopying>*)key];
}
NSEnumerator* NSDictionary_objectEnumerator(NSDictionary* dictionary) {
    return [dictionary objectEnumerator];
}
NSObject* NSDictionary_objectForKey(NSDictionary* dictionary,NSObject* aKey) {
    return [dictionary objectForKey:aKey];
}
NSObject* NSDictionary_valueForKey(NSDictionary* dictionary,NSString* key) {
    return [dictionary valueForKey:key];
}
/*
 *  NSMutableDictionary
 */
NSMutableDictionary* NSMutableDictionary_dictionaryWithCapacity(int numItems) {
    return [NSMutableDictionary dictionaryWithCapacity:numItems];
}
NSMutableDictionary* NSMutableDictionary_initWithCapacity(int numItems) {
    return [[NSMutableDictionary alloc] initWithCapacity:numItems];
}
void NSMutableDictionary_removeObjectForKey(NSMutableDictionary* dictionary,NSObject* aKey) {
    return [dictionary removeObjectForKey:aKey];
}
void NSMutableDictionary_setDictionary(NSMutableDictionary* dictionary,NSDictionary* otherDictionary) {
    return [dictionary setDictionary:otherDictionary];
}
void NSMutableDictionary_setObject(NSMutableDictionary* dictionary,NSObject* anObject, NSObject* aKey) {
    return [dictionary setObject:anObject forKey:(NSObject<NSCopying>*)aKey];
}
 void NSMutableDictionary_setValue(NSMutableDictionary* dictionary,NSObject* value, NSString *key) {
    return [dictionary setValue:value forKey:key];
}
NSDictionary* NSMutableDictionary_dictionaryWithObject(NSObject* object, NSObject* key) {
    return [NSMutableDictionary dictionaryWithObject:object forKey:(NSObject<NSCopying>*)key];
}
