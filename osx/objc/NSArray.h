/*
 * NSArray.h
 *
 *  Created on: Jan 15, 2021
 *      Author: azeddine
 */

#ifndef OSX_OBJC_NSARRAY_H_
#define OSX_OBJC_NSARRAY_H_
#include "NSObject.h"
typedef struct NSMutableArray NSMutableArray;
/*
 *  NSArray
 */
int NSArray_containsObject(NSArray *array,NSObject* anObject);
NSUInteger NSArray_count(NSArray *array);
NSUInteger NSArray_indexOfObjectIdenticalTo(NSArray *array,NSObject* anObject);
NSObject* NSArray_objectAtIndex(NSArray *array,NSUInteger index);
/*
 *
 */
void NSMutableArray_(NSMutableArray* array,NSObject* anObject);
void NSMutableArray_addObject(NSMutableArray* array,NSObject* anObject);
void NSMutableArray_addObjectsFromArray(NSMutableArray* array,NSArray* otherArray);
NSMutableArray* NSMutableArray_arrayWithCapacity(int numItems);
NSMutableArray* NSMutableArray_initWithCapacity(int numItems);
void NSMutableArray_removeLastObject(NSMutableArray* array);
void NSMutableArray_removeObject(NSMutableArray* array,NSObject* anObject);
void NSMutableArray_removeObjectAtIndex(NSMutableArray* array,int index);
void NSMutableArray_removeObjectIdenticalTo(NSMutableArray* array,NSObject* anObject);
NSArray* NSMutableArray_array();
NSArray* NSMutableArray_arrayWithObject(NSObject* anObject);


#endif /* OSX_OBJC_NSARRAY_H_ */
