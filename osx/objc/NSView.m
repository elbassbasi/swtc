#import "swt.h"
#import <objc/objc.h>
#import <objc/runtime.h>
@interface SWTView : NSView{
@public w_widget* widget;
}
@end
@implementation SWTView
//_NSMETHOD_EVENT_3(NSView)
//_NSMETHOD_EVENT_2(NSView)
//_NSMETHOD_setNeedsDisplayInRect(NSView)
_NSMETHOD_drawRect(NSView)
//_NSMETHOD_hitTest(NSView)
@end
w_widget* SWTView_get_widget(NSView* view){
    if( [view isKindOfClass:[SWTView class]])
    return ((SWTView*)view)->widget;
    else return 0;
}
NSView* SWTView_new(w_widget* widget){
    NSRect frame=NSMakeRect(0, 0, 0, 0);
    SWTView* canvasView = [[SWTView alloc] initWithFrame:frame];
    _w_toolkit_registre_class(_NS_CLASS_VIEW,canvasView.class,SWTView_get_widget);
    canvasView->widget = widget;
    return canvasView;
}

int NSView_acceptsFirstMouse(NSView* view,NSEvent* theEvent) {
    return  [view acceptsFirstMouse:theEvent];
}

void NSView_addSubview(NSView* view,NSView* aView) {
     [view addSubview:aView];
}

void NSView_addSubview_0(NSView* view,NSView* aView, int place, NSView* otherView) {
     [view addSubview:aView positioned:place relativeTo:otherView];
}

int NSView_addToolTipRect(NSView* view,NSRect* aRect, NSObject* anObject, void * data) {
    return [view addToolTipRect:*aRect owner:anObject userData:data];
}

void NSView_beginDocument(NSView* view) {
    [view beginDocument];
}

void NSView_beginPageInRect(NSView* view,NSRect* aRect, NSPoint* location) {
    [view beginPageInRect:*aRect atPlacement:*location];
}

NSBitmapImageRep* NSView_bitmapImageRepForCachingDisplayInRect(NSView* view,NSRect* aRect) {
    return [view bitmapImageRepForCachingDisplayInRect:*aRect];
}

void NSView_bounds(NSView* view,NSRect* bounds) {
    *bounds = view.bounds;
}

void NSView_cacheDisplayInRect(NSView* view,NSRect* aRect, NSBitmapImageRep* bitmapImageRep) {
    [view cacheDisplayInRect:*aRect toBitmapImageRep:bitmapImageRep];
}

int NSView_canBecomeKeyView(NSView* view) {
    return [view canBecomeKeyView];
}

void NSView_convertPoint_fromView_(NSView* view,NSPoint* result,NSPoint* aPoint, NSView* aView) {
    *result = [view convertPoint:*aPoint fromView:aView];
}

void NSView_convertPoint_toView_(NSView* view,NSPoint* result,NSPoint* aPoint, NSView* aView) {
    *result = [view convertPoint:*aPoint toView:aView];
}

void NSView_convertRect_fromView_(NSView* view,NSRect* result,NSRect* aRect, NSView* aView) {
    *result = [view convertRect:*aRect fromView:aView];
}

void NSView_convertRect_toView_(NSView* view,NSRect* result,NSRect* aRect, NSView* aView) {
    *result = [view convertRect:*aRect toView:aView];
}

void NSView_discardCursorRects(NSView* view) {
    [view discardCursorRects];
}

void NSView_display(NSView* view) {
    [view display];
}

void NSView_displayIfNeeded(NSView* view) {
    [view displayIfNeeded];
}

void NSView_displayRectIgnoringOpacity(NSView* view,NSRect* aRect, NSGraphicsContext* context) {
    [view displayRectIgnoringOpacity:*aRect inContext:context];
}

void NSView_dragImage(NSView* view,NSImage* anImage, NSPoint* viewLocation, NSSize* initialOffset, NSEvent* event, NSPasteboard* pboard, NSObject* sourceObj, int slideFlag) {
    [view dragImage:anImage at:*viewLocation offset:*initialOffset event:event pasteboard:pboard source:sourceObj slideBack:slideFlag];
}

void NSView_drawRect(NSView* view,NSRect* dirtyRect) {
     [view drawRect:*dirtyRect];
}

void NSView_endDocument(NSView* view) {
    [view endDocument];
}

void NSView_endPage(NSView* view) {
    [view endPage];
}

void NSView_frame(NSView* view,NSRect* result) {
    *result = view.frame;
}

NSView* NSView_hitTest(NSView* view,NSPoint* aPoint) {
    return [view hitTest:*aPoint];
}

NSView* NSView_initWithFrame(NSView* view,NSRect* frameRect) {
    return [view initWithFrame:*frameRect];
}

int NSView_isDescendantOf(NSView* view,NSView* aView) {
    return [view isDescendantOf:aView];
}

int NSView_isFlipped(NSView* view) {
    return [view isFlipped];
}

int NSView_isHidden(NSView* view) {
    return [view isHidden];
}

int NSView_isHiddenOrHasHiddenAncestor(NSView* view) {
    return [view isHiddenOrHasHiddenAncestor];
}

int NSView_isOpaque(NSView* view) {
    return [view isOpaque];
}

void NSView_lockFocus(NSView* view) {
    [view lockFocus];
}

int NSView_lockFocusIfCanDraw(NSView* view) {
    return [view lockFocusIfCanDraw];
}

NSMenu* NSView_menuForEvent(NSView* view,NSEvent* event) {
    return [view menuForEvent:event];
}

int NSView_mouse(NSView* view,NSPoint* aPoint, NSRect* aRect) {
    return [view mouse:*aPoint inRect:*aRect];
}

int NSView_mouseDownCanMoveWindow(NSView* view) {
    return [view mouseDownCanMoveWindow];
}

int NSView_needsPanelToBecomeKey(NSView* view) {
    return [view needsPanelToBecomeKey];
}

void NSView_registerForDraggedTypes(NSView* view,NSArray* newTypes) {
    [view registerForDraggedTypes:newTypes];
}

void NSView_removeFromSuperview(NSView* view) {
    [view removeFromSuperview];
}

void NSView_removeToolTip(NSView* view,int tag) {
    [view removeToolTip:tag];
}

void NSView_removeTrackingArea(NSView* view,NSTrackingArea* trackingArea) {
    [view removeTrackingArea:trackingArea];
}

void NSView_resetCursorRects(NSView* view) {
    [view resetCursorRects];
}

void NSView_scrollClipView(NSView* view,NSClipView* aClipView, NSPoint* aPoint) {
    [view scrollClipView:aClipView toPoint:*aPoint];
}

void NSView_scrollPoint(NSView* view,NSPoint* aPoint) {
    [view scrollPoint:*aPoint];
}

void NSView_scrollRect(NSView* view,NSRect* aRect, NSSize* delta) {
    [view scrollRect:*aRect by:*delta];
}

void NSView_setAcceptsTouchEvents(NSView* view,int acceptsTouchEvents) {
    [view setAcceptsTouchEvents:acceptsTouchEvents];
}

void NSView_setAutoresizesSubviews(NSView* view,int autoresizesSubviews) {
    [view setAutoresizesSubviews:autoresizesSubviews];
}

void NSView_setAutoresizingMask(NSView* view,int autoresizingMask) {
    [view setAutoresizingMask:autoresizingMask];
}

void NSView_setBoundsRotation(NSView* view,CGFloat boundsRotation) {
    [view setBoundsRotation:boundsRotation];
}

void NSView_setBoundsSize(NSView* view,NSSize* newSize) {
    [view setBoundsSize:*newSize];
}

void NSView_setFocusRingType(NSView* view,int focusRingType) {
    [view setFocusRingType:focusRingType];
}

void NSView_setFrame(NSView* view,NSRect* frame) {
    [view setFrame:*frame];
}

void NSView_setFrameOrigin(NSView* view,NSPoint* newOrigin) {
    [view setFrameOrigin:*newOrigin];
}

void NSView_setFrameSize(NSView* view,NSSize* newSize) {
     [view setFrameSize:*newSize];
}

void NSView_setHidden(NSView* view,int hidden) {
    [view setHidden:hidden];
}

void NSView_setNeedsDisplay(NSView* view,int needsDisplay) {
   [view setNeedsDisplay:needsDisplay];
}

void NSView_setNeedsDisplayInRect(NSView* view,NSRect* invalidRect) {
    [view setNeedsDisplayInRect:*invalidRect];
}

void NSView_setToolTip(NSView* view,NSString* toolTip) {
    [view setToolTip:toolTip];
}

void NSView_setWantsRestingTouches(NSView* view,int wantsRestingTouches) {
    [view setWantsRestingTouches:wantsRestingTouches];
}

int NSView_shouldDelayWindowOrderingForEvent(NSView* view,NSEvent* theEvent) {
    return [view shouldDelayWindowOrderingForEvent:theEvent];
}

NSArray* NSView_subviews(NSView* view) {
    return [view subviews];
}

NSView* NSView_superview(NSView* view) {
    return [view superview];
}

NSArray* NSView_trackingAreas(NSView* view) {
    return [view trackingAreas];;
}

void NSView_unlockFocus(NSView* view) {
    [view unlockFocus];
}

void NSView_unregisterDraggedTypes(NSView* view) {
    [view unregisterDraggedTypes];
}

void NSView_updateTrackingAreas(NSView* view) {
    [view updateTrackingAreas];
}

void NSView_viewDidMoveToWindow(NSView* view) {
    [view viewDidMoveToWindow];
}

void NSView_viewWillMoveToWindow(NSView* view,NSWindow* newWindow) {
    [view viewWillMoveToWindow:newWindow];
}

void NSView_visibleRect(NSView* view,NSRect* result) {
    *result = view.visibleRect;
}

NSWindow* NSView_window(NSView* view) {
    return [view window];;
}

