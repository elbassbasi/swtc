/*
 * NSProgressIndicator.h
 *
 *  Created on: May 1, 2021
 *      Author: azeddine
 */

#ifndef OSX_OBJC_NSPROGRESSINDICATOR_H_
#define OSX_OBJC_NSPROGRESSINDICATOR_H_
#include "NSView.h"

typedef struct NSProgressIndicator NSProgressIndicator;
w_widget* SWTProgressIndicator_get_widget(NSView* view);
NSProgressIndicator* SWTProgressIndicator_new(w_widget* widget);
NSProgressIndicator* NSProgressIndicator_new();

NSUInteger NSProgressIndicator_controlSize(NSProgressIndicator *progress);
CGFloat NSProgressIndicator_doubleValue(NSProgressIndicator *progress);
CGFloat NSProgressIndicator_maxValue(NSProgressIndicator *progress);
CGFloat NSProgressIndicator_minValue(NSProgressIndicator *progress);
void NSProgressIndicator_setControlSize(NSProgressIndicator *progress,
		NSUInteger size);
void NSProgressIndicator_setDoubleValue(NSProgressIndicator *progress,
		CGFloat doubleValue);
void NSProgressIndicator_setIndeterminate(NSProgressIndicator *progress,
		NSInteger flag);
void NSProgressIndicator_setMaxValue(NSProgressIndicator *progress,
		CGFloat newMaximum);
void NSProgressIndicator_setMinValue(NSProgressIndicator *progress,
		CGFloat newMinimum);
void NSProgressIndicator_setUsesThreadedAnimation(NSProgressIndicator *progress,
		NSInteger threadedAnimation);
void NSProgressIndicator_sizeToFit(NSProgressIndicator *progress);
void NSProgressIndicator_startAnimation(NSProgressIndicator *progress,
		NSObject *sender);
void NSProgressIndicator_stopAnimation(NSProgressIndicator *progress,
		NSObject *sender);

#endif /* OSX_OBJC_NSPROGRESSINDICATOR_H_ */
