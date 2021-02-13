/*
 * NSDictionary.h
 *
 *  Created on: Jan 24, 2021
 *      Author: azeddine
 */

#ifndef OSX_OBJC_NSDICTIONARY_H_
#define OSX_OBJC_NSDICTIONARY_H_
#include "NSArray.h"
typedef struct NSDictionary NSDictionary;
typedef struct NSMutableDictionary NSMutableDictionary;
typedef struct NSEnumerator NSEnumerator;
/*
 *  NSDictionary
 */
NSArray* NSDictionary_allKeys(NSDictionary* dictionary);
int NSDictionary_count(NSDictionary* dictionary);
NSDictionary* NSDictionary_dictionaryWithObject(NSObject* object, NSObject* key);
NSEnumerator* NSDictionary_objectEnumerator(NSDictionary* dictionary);
NSObject* NSDictionary_objectForKey(NSDictionary* dictionary,NSObject* aKey);
NSObject* NSDictionary_valueForKey(NSDictionary* dictionary,NSString* key);
/*
 *  NSMutableDictionary
 */
NSMutableDictionary* NSMutableDictionary_dictionaryWithCapacity(int numItems);
NSMutableDictionary* NSMutableDictionary_initWithCapacity(int numItems);
void NSMutableDictionary_removeObjectForKey(NSMutableDictionary* dictionary,NSObject* aKey);
void NSMutableDictionary_setDictionary(NSMutableDictionary* dictionary,NSDictionary* otherDictionary);
void NSMutableDictionary_setObject(NSMutableDictionary* dictionary,NSObject* anObject, NSObject* aKey);
void NSMutableDictionary_setValue(NSMutableDictionary* dictionary,NSObject* value, NSString *key);
NSDictionary* NSMutableDictionary_dictionaryWithObject(NSObject* object, NSObject* key);



#endif /* OSX_OBJC_NSDICTIONARY_H_ */
