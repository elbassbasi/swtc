/*
 * NSEvent.h
 *
 *  Created on: Mar 27, 2021
 *      Author: azeddine
 */

#ifndef OSX_OBJC_NSEVENT_H_
#define OSX_OBJC_NSEVENT_H_
#include "NSView.h"
typedef struct NSSet NSSet;
long NSEvent_CGEvent(NSEvent *nsEvent);
long NSEvent_buttonNumber(NSEvent *nsEvent);
NSString* NSEvent_characters(NSEvent *nsEvent);
NSString* NSEvent_charactersIgnoringModifiers(NSEvent *nsEvent);
long NSEvent_clickCount(NSEvent *nsEvent);
double NSEvent_deltaX(NSEvent *nsEvent);
double NSEvent_deltaY(NSEvent *nsEvent);
double NSEvent_doubleClickInterval();
NSEvent* NSEvent_enterExitEventWithType(long type, NSPoint *location,
		long flags, double time, NSUInteger wNum, NSGraphicsContext *context,
		NSInteger eNum, NSInteger tNum, void *data);
short NSEvent_keyCode(NSEvent *nsEvent);
void NSEvent_locationInWindow(NSEvent *nsEvent, NSPoint *result);
double NSEvent_magnification(NSEvent *nsEvent);
long NSEvent_modifierFlags(NSEvent *nsEvent);
void NSEvent_mouseLocation(w_point *result);
NSEvent* NSEvent_otherEventWithType(long type, NSPoint *location, long flags,
		double time, NSInteger wNum, NSGraphicsContext *context, short subtype,
		NSInteger d1, NSInteger d2);
long NSEvent_phase(NSEvent *nsEvent);
NSUInteger NSEvent_pressedMouseButtons();
float NSEvent_rotation(NSEvent *nsEvent);
double NSEvent_timestamp(NSEvent *nsEvent);
NSSet* NSEvent_touchesMatchingPhase(NSEvent *nsEvent, long phase, NSView *view);
long NSEvent_type(NSEvent *nsEvent);
NSWindow* NSEvent_window(NSEvent *nsEvent);

#endif /* OSX_OBJC_NSEVENT_H_ */
