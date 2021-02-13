#import "swt.h"
int NSArray_containsObject(NSArray *array,NSObject* anObject) {
    return [array containsObject:anObject];
}

NSUInteger NSArray_count(NSArray *array) {
    return [array count];
}

NSUInteger NSArray_indexOfObjectIdenticalTo(NSArray *array,NSObject* anObject) {
    return [array indexOfObjectIdenticalTo:anObject];
}

NSObject* NSArray_objectAtIndex(NSArray *array,NSUInteger index) {
    return [array objectAtIndex:index];
}
/*
 * NSMutableArray
 */
void NSMutableArray_addObject(NSMutableArray* array,NSObject* anObject) {
    [array addObject:anObject];
}

void NSMutableArray_addObjectsFromArray(NSMutableArray* array,NSArray* otherArray) {
    [array addObjectsFromArray:otherArray];
}

NSMutableArray* NSMutableArray_arrayWithCapacity(int numItems) {
    return [NSMutableArray arrayWithCapacity:numItems];
}

NSMutableArray* NSMutableArray_initWithCapacity(int numItems) {
    return [[NSMutableArray alloc] initWithCapacity:numItems];
}

void NSMutableArray_removeLastObject(NSMutableArray* array) {
    [array removeLastObject];
}

void NSMutableArray_removeObject(NSMutableArray* array,NSObject* anObject) {
    [array removeObject:anObject];
}

void NSMutableArray_removeObjectAtIndex(NSMutableArray* array,int index) {
     [array removeObjectAtIndex:index];
}

void NSMutableArray_removeObjectIdenticalTo(NSMutableArray* array,NSObject* anObject) {
    [array removeObjectIdenticalTo:anObject];
}

NSArray* NSMutableArray_array() {
    return [NSMutableArray array];
}

NSArray* NSMutableArray_arrayWithObject(NSObject* anObject) {
    return [NSMutableArray arrayWithObject:anObject];
}
