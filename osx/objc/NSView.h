/*
 * NSView.h
 *
 *  Created on: Dec 30, 2020
 *      Author: azeddine
 */

#ifndef OSX_OBJC_NSVIEW_H_
#define OSX_OBJC_NSVIEW_H_
#include "NSGraphicsContext.h"
typedef struct NSView NSView;
typedef struct NSControl NSControl;
typedef struct NSEvent NSEvent;
typedef struct NSBitmapImageRep NSBitmapImageRep;
typedef struct NSMenu NSMenu;
typedef struct NSPasteboard NSPasteboard;
typedef struct NSTrackingArea NSTrackingArea;
typedef struct NSClipView NSClipView;
typedef struct NSCell NSCell;
typedef struct NSText NSText;
typedef struct NSFormatter NSFormatter;
/*
 * messages
 */
void _ns_super_setNeedsDisplay (w_widget *widget, _w_event_platform *e);
NSView* SWTView_new(w_widget* widget);
w_widget* SWTView_get_widget(NSView* view);
w_widget* NSView_get_widget(NSView* view);
w_widget* NSView_get_widget_0(NSView* view,size_t* index);
int NSView_acceptsFirstMouse(NSView* view,NSEvent* theEvent);
void NSView_addSubview(NSView* view,NSView* aView);
void NSView_addSubview_0(NSView* view,NSView* aView, int place, NSView* otherView);
int NSView_addToolTipRect(NSView* view,NSRect* aRect, NSObject* anObject, void * data);
void NSView_beginDocument(NSView* view);
void NSView_beginPageInRect(NSView* view,NSRect* aRect, NSPoint* location);
NSBitmapImageRep* NSView_bitmapImageRepForCachingDisplayInRect(NSView* view,NSRect* aRect);
void NSView_bounds(NSView* view,NSRect* bounds);
void NSView_cacheDisplayInRect(NSView* view,NSRect* aRect, NSBitmapImageRep* bitmapImageRep);
int NSView_canBecomeKeyView(NSView* view);
void NSView_convertPoint_fromView_(NSView* view,NSPoint* result,NSPoint* aPoint, NSView* aView);
void NSView_convertPoint_toView_(NSView* view,NSPoint* result,NSPoint* aPoint, NSView* aView);
void NSView_convertRect_fromView_(NSView* view,NSRect* result,NSRect* aRect, NSView* aView);
void NSView_convertRect_toView_(NSView* view,NSRect* result,NSRect* aRect, NSView* aView);
void NSView_discardCursorRects(NSView* view);
void NSView_display(NSView* view);
void NSView_displayIfNeeded(NSView* view);
void NSView_displayRectIgnoringOpacity(NSView* view,NSRect* aRect, NSGraphicsContext* context);
void NSView_dragImage(NSView* view,NSImage* anImage, NSPoint* viewLocation, NSSize* initialOffset, NSEvent* event, NSPasteboard* pboard, NSObject* sourceObj, int slideFlag);
void NSView_drawRect(NSView* view,NSRect* dirtyRect);
void NSView_endDocument(NSView* view);
void NSView_endPage(NSView* view);
void NSView_frame(NSView* view,NSRect* result);
NSView* NSView_hitTest(NSView* view,NSPoint* aPoint);
NSView* NSView_initWithFrame(NSView* view,NSRect* frameRect);
int NSView_isDescendantOf(NSView* view,NSView* aView);
int NSView_isFlipped(NSView* view);
int NSView_isHidden(NSView* view);
int NSView_isHiddenOrHasHiddenAncestor(NSView* view);
int NSView_isOpaque(NSView* view);
void NSView_lockFocus(NSView* view);
int NSView_lockFocusIfCanDraw(NSView* view);
NSMenu* NSView_menuForEvent(NSView* view,NSEvent* event);
int NSView_mouse(NSView* view,NSPoint* aPoint, NSRect* aRect);
int NSView_mouseDownCanMoveWindow(NSView* view);
int NSView_needsPanelToBecomeKey(NSView* view);
void NSView_registerForDraggedTypes(NSView* view,NSArray* newTypes);
void NSView_removeFromSuperview(NSView* view);
void NSView_removeToolTip(NSView* view,int tag);
void NSView_removeTrackingArea(NSView* view,NSTrackingArea* trackingArea);
void NSView_resetCursorRects(NSView* view);
void NSView_scrollClipView(NSView* view,NSClipView* aClipView, NSPoint* aPoint);
void NSView_scrollPoint(NSView* view,NSPoint* aPoint);
void NSView_scrollRect(NSView* view,NSRect* aRect, NSSize* delta);
void NSView_setAcceptsTouchEvents(NSView* view,int acceptsTouchEvents);
void NSView_setAutoresizesSubviews(NSView* view,int autoresizesSubviews);
void NSView_setAutoresizingMask(NSView* view,int autoresizingMask);
void NSView_setBoundsRotation(NSView* view,CGFloat boundsRotation);
void NSView_setBoundsSize(NSView* view,NSSize* newSize);
void NSView_setFocusRingType(NSView* view,int focusRingType);
void NSView_setFrame(NSView* view,NSRect* frame);
void NSView_setFrameOrigin(NSView* view,NSPoint* newOrigin);
void NSView_setFrameSize(NSView* view,NSSize* newSize);
void NSView_setHidden(NSView* view,int hidden);
void NSView_setNeedsDisplay(NSView* view,int needsDisplay);
void NSView_setNeedsDisplayInRect(NSView* view,NSRect* invalidRect);
void NSView_setToolTip(NSView* view,const char* toolTip);
void NSView_setWantsRestingTouches(NSView* view,int wantsRestingTouches);
int NSView_shouldDelayWindowOrderingForEvent(NSView* view,NSEvent* theEvent);
NSArray* NSView_subviews(NSView* view);
NSView* NSView_superview(NSView* view);
NSArray* NSView_trackingAreas(NSView* view);
void NSView_unlockFocus(NSView* view);
void NSView_unregisterDraggedTypes(NSView* view);
void NSView_updateTrackingAreas(NSView* view);
void NSView_viewDidMoveToWindow(NSView* view);
void NSView_viewWillMoveToWindow(NSView* view,NSWindow* newWindow);
void NSView_visibleRect(NSView* view,NSRect* result);
NSWindow* NSView_window(NSView* view);
/*
 *  NSControl
 */
NSInteger NSControl_abortEditing(NSControl* control);
SEL NSControl_action(NSControl* control);
NSCell* NSControl_cell(NSControl* control);
Class NSControl_cellClass(NSControl* control);
NSText* NSControl_currentEditor(NSControl* control);
double NSControl_doubleValue(NSControl* control);
NSFont* NSControl_font(NSControl* control);
NSInteger NSControl_isEnabled(NSControl* control);
NSInteger NSControl_sendAction(NSControl* control,SEL theAction, NSObject* theTarget);
void NSControl_setAction(NSControl* control,SEL aSelector);
void NSControl_setAlignment(NSControl* control,NSInteger mode);
void NSControl_setBaseWritingDirection(NSControl* control,NSInteger writingDirection);
void NSControl_setCell(NSControl* control,NSCell* aCell);
void NSControl_setCellClass(Class factoryId);
void NSControl_setDoubleValue(NSControl* control,double aDouble);
void NSControl_setEnabled(NSControl* control,NSInteger flag);
void NSControl_setFont(NSControl* control,NSFont* fontObj);
void NSControl_setFormatter(NSControl* control,NSFormatter* newFormatter);
void NSControl_setStringValue(NSControl* control,NSString* aString);
void NSControl_setTarget(NSControl* control,NSObject* anObject);
void NSControl_sizeToFit(NSControl* control);
NSString* NSControl_stringValue(NSControl* control);
NSObject* NSControl_target(NSControl* control);


#endif /* OSX_OBJC_NSVIEW_H_ */
