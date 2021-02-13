#import "swt.h"
@interface SWTWindowDelegate : NSObject <NSWindowDelegate> {
@public w_widget* widget;
}
- (BOOL)windowShouldClose:(NSWindow *)sender;
- (void)windowWillClose:(NSNotification *)notification;
- (void)windowDidResize:(NSNotification *)notification;
@end
@implementation SWTWindowDelegate
- (BOOL)windowShouldClose:(NSWindow *)sender{
    _w_event_platform e;
    _w_event_platform_init(&e, widget, self, _NS_windowShouldClose);
    _w_widget_send_event(widget,W_EVENT(&e));
    return e.result._int;
}
- (void)windowWillClose:(NSNotification *)notification{
    _w_event_platform e;
    _w_event_platform_init(&e, widget, self, _NS_windowWillClose);
    _w_widget_send_event(widget,W_EVENT(&e));
}
- (void)windowDidResize:(NSNotification *)notification{
    _w_event_platform e;
    _w_event_platform_init(&e, widget, self, _NS_windowDidResize);
    _w_widget_send_event(widget,W_EVENT(&e));
}
@end
@interface SWTWindow : NSWindow {
@public w_widget* widget;
}
@end
@implementation SWTWindow
@end
@interface SWTPanel : NSPanel {
@public w_widget* widget;
}
@end
@implementation SWTPanel
@end
NSObject* SWTWindowDelegate_alloc(w_widget* widget){
    SWTWindowDelegate* delegate =[SWTWindowDelegate alloc];
    [delegate init];
    delegate->widget = widget;
    return delegate;
}
NSWindow* SWTWindow_alloc(w_widget* widget){
    SWTWindow* window =[SWTWindow alloc];
    window->widget = widget;
    return window;
}
NSWindow* SWTPanel_alloc(w_widget* widget){
    SWTPanel* panel =[SWTPanel alloc];
    panel->widget = widget;
    return panel;
}
void NSPanel_setBecomesKeyOnlyIfNeeded(NSPanel* panel,int becomesKeyOnlyIfNeeded) {
    [panel setBecomesKeyOnlyIfNeeded:becomesKeyOnlyIfNeeded];
}
void NSPanel_setFloatingPanel(NSPanel* panel,int floatingPanel) {
    [panel setFloatingPanel:floatingPanel];
}
void NSPanel_setWorksWhenModal(NSPanel* panel,int worksWhenModal) {
    [panel setWorksWhenModal:worksWhenModal];
}
CGFloat NSPanel_minFrameWidthWithTitle(NSString* aTitle, int aStyle) {
    return [NSPanel minFrameWidthWithTitle:aTitle styleMask:aStyle];
}
int NSPanel_windowNumberAtPoint(NSPoint* point, int windowNumber) {
    return [NSPanel windowNumberAtPoint:*point belowWindowWithWindowNumber:windowNumber];
}
/*
 * NSWindow
 */
void NSWindow_addChildWindow(NSWindow* window,NSWindow* childWin,int place) {
    [window addChildWindow:childWin ordered:place];
}

CGFloat NSWindow_alphaValue(NSWindow* window) {
    return [window alphaValue];
}

int NSWindow_areCursorRectsEnabled(NSWindow* window) {
    return [window areCursorRectsEnabled];
}

void NSWindow_becomeKeyWindow(NSWindow* window) {
    [window becomeKeyWindow];
}

int NSWindow_canBecomeKeyWindow(NSWindow* window) {
    return [window canBecomeKeyWindow];
}

void NSWindow_cascadeTopLeftFromPoint(NSWindow* window,NSPoint* result,NSPoint* topLeftPoint) {
    *result = [window cascadeTopLeftFromPoint:*topLeftPoint];
}

void NSWindow_close(NSWindow* window) {
    [window close];
}

int NSWindow_collectionBehavior(NSWindow* window) {
    return window.collectionBehavior;
}

NSView* NSWindow_contentView(NSWindow* window) {
    return window.contentView;
}

void NSWindow_convertBaseToScreen(NSWindow* window,NSPoint* result,NSPoint* aPoint) {
    *result = [window convertBaseToScreen:*aPoint];
}

void NSWindow_convertScreenToBase(NSWindow* window,NSPoint* result,NSPoint* aPoint) {
    *result = [window convertScreenToBase:*aPoint];
}

NSButtonCell* NSWindow_defaultButtonCell(NSWindow* window) {
    return [window defaultButtonCell];
}

NSObject* NSWindow_delegate(NSWindow* window) {
    return [window delegate];
}

void NSWindow_deminiaturize(NSWindow* window,NSObject* sender) {
    [window deminiaturize:sender];
}

void NSWindow_disableCursorRects(NSWindow* window) {
   [window disableCursorRects];
}

void NSWindow_disableFlushWindow(NSWindow* window) {
    [window disableFlushWindow];
}

void NSWindow_display(NSWindow* window) {
    [window display];
}

void NSWindow_enableCursorRects(NSWindow* window) {
    [window enableCursorRects];
}

void NSWindow_enableFlushWindow(NSWindow* window) {
    [window enableFlushWindow];
}

void NSWindow_endEditingFor(NSWindow* window,NSObject* anObject) {
    [window endEditingFor:anObject];
}

NSText* NSWindow_fieldEditor(NSWindow* window,int createFlag, NSObject* anObject) {
    return [window fieldEditor:createFlag forObject:anObject];
}

NSResponder* NSWindow_firstResponder(NSWindow* window) {
    return [window firstResponder];
}

void NSWindow_flushWindowIfNeeded(NSWindow* window) {
    [window flushWindowIfNeeded];
}

void NSWindow_frame(NSWindow* window,NSRect* result) {
    *result =window.frame;
}

void NSWindow_frameRectForContentRect(NSWindow* window,NSRect* result,NSRect* contentRect) {
    *result =[window frameRectForContentRect:*contentRect];
}

NSGraphicsContext* NSWindow_graphicsContext(NSWindow* window) {
    return [window graphicsContext];
}

int NSWindow_hasShadow(NSWindow* window) {
    return [window hasShadow];
}

NSWindow* NSWindow_initWithContentRect(NSWindow* window,NSRect* contentRect, int aStyle, int bufferingType, int flag) {
    return [window initWithContentRect:*contentRect styleMask:aStyle backing:bufferingType defer:flag];
}

NSWindow* NSWindow_initWithContentRect_0(NSWindow* window,NSRect* contentRect, int aStyle, int bufferingType, int flag, NSScreen* screen) {
    return [window initWithContentRect:*contentRect styleMask:aStyle backing:bufferingType defer:flag screen:screen];
}

void NSWindow_invalidateShadow(NSWindow* window) {
    [window invalidateShadow];
}

int NSWindow_isDocumentEdited(NSWindow* window) {
    return [window isDocumentEdited];
}

int NSWindow_isKeyWindow(NSWindow* window) {
    return [window isKeyWindow];
}

int NSWindow_isMainWindow(NSWindow* window) {
    return [window isMainWindow];
}

int NSWindow_isMiniaturized(NSWindow* window) {
    return [window isMiniaturized];
}

int NSWindow_isSheet(NSWindow* window) {
    return [window isSheet];
}

int NSWindow_isVisible(NSWindow* window) {
    return [window isVisible];
}

int NSWindow_isZoomed(NSWindow* window) {
    return [window isZoomed];
}

int NSWindow_makeFirstResponder(NSWindow* window,NSResponder* aResponder) {
    return [window makeFirstResponder:aResponder];
}

void NSWindow_makeKeyAndOrderFront(NSWindow* window,NSObject* sender) {
   [window makeKeyAndOrderFront:sender];
}

CGFloat NSWindow_minFrameWidthWithTitle(NSString* aTitle, int aStyle) {
    return [NSWindow minFrameWidthWithTitle:aTitle styleMask:aStyle];
}

void NSWindow_minSize(NSWindow* window,NSSize* result) {
    *result = window.minSize;
}

void NSWindow_miniaturize(NSWindow* window,NSObject* sender) {
    [window miniaturize:sender];
}
void NSWindow_mouseLocationOutsideOfEventStream(NSWindow* window,NSPoint* result) {
    *result = window.mouseLocationOutsideOfEventStream;
}

void NSWindow_orderBack(NSWindow* window,NSObject* sender) {
    [window orderBack:sender];
}

void NSWindow_orderFront(NSWindow* window,NSObject* sender) {
    [window orderFront:sender];
}

void NSWindow_orderFrontRegardless(NSWindow* window) {
    [window orderFrontRegardless];
}

void NSWindow_orderOut(NSWindow* window,NSObject* sender) {
    [window orderOut:sender];
}

void NSWindow_orderWindow(NSWindow* window,int place, int otherWin) {
    [window orderWindow:place relativeTo:otherWin];
}

NSWindow* NSWindow_parentWindow(NSWindow* window) {
    return [window parentWindow];
}

void NSWindow_removeChildWindow(NSWindow* window,NSWindow* childWin) {
    [window removeChildWindow:childWin];
}

NSScreen* NSWindow_screen(NSWindow* window) {
    return [window screen];
}

void NSWindow_sendEvent(NSWindow* window,NSEvent* theEvent) {
    [window sendEvent:theEvent];
}
void NSWindow_setAllowsAutomaticWindowTabbing(int tabbing) {
    [NSWindow setAllowsAutomaticWindowTabbing:tabbing];
}
void NSWindow_setAcceptsMouseMovedEvents(NSWindow* window,int acceptsMouseMovedEvents) {
    [window setAcceptsMouseMovedEvents:acceptsMouseMovedEvents];
}

void NSWindow_setAlphaValue(NSWindow* window,CGFloat alphaValue) {
    [window setAlphaValue:alphaValue];
}

void NSWindow_setBackgroundColor(NSWindow* window,NSColor* backgroundColor) {
    [window setBackgroundColor:backgroundColor];
}

void NSWindow_setCollectionBehavior(NSWindow* window,int collectionBehavior) {
    [window setCollectionBehavior:collectionBehavior];
}

void NSWindow_setContentView(NSWindow* window,NSView* contentView) {
    [window setContentView:contentView];
}

void NSWindow_setDefaultButtonCell(NSWindow* window,NSButtonCell* defaultButtonCell) {
    [window setDefaultButtonCell:defaultButtonCell];
}
NSObject* NSWindow_getDelegate(NSWindow* window) {
    return window.delegate;
}
void NSWindow_setDelegate(NSWindow* window,NSObject* delegate) {
    [window setDelegate:(id<NSWindowDelegate>)delegate];
}

void NSWindow_setDocumentEdited(NSWindow* window,int documentEdited) {
    [window setDocumentEdited:documentEdited];
}

void NSWindow_setFrame(NSWindow* window,NSRect* frameRect, int flag) {
    [window setFrame:*frameRect display:flag];
}

void NSWindow_setFrame_0(NSWindow* window,NSRect* frameRect, int displayFlag, int animateFlag) {
    [window setFrame:*frameRect display:displayFlag animate:animateFlag];
}

void NSWindow_setHasShadow(NSWindow* window,int hasShadow) {
    [window setHasShadow:hasShadow];
}

void NSWindow_setHidesOnDeactivate(NSWindow* window,int hidesOnDeactivate) {
    [window setHidesOnDeactivate:hidesOnDeactivate];
}

void NSWindow_setLevel(NSWindow* window,int level) {
    [window setLevel:level];
}

void NSWindow_setMinSize(NSWindow* window,NSSize* minSize) {
    NSSize _minSize = NSMakeSize(minSize->width, minSize->height);
    [window setMinSize:_minSize];
}

void NSWindow_setMovable(NSWindow* window,int movable) {
    [window setMovable:movable];
}

void NSWindow_setOpaque(NSWindow* window,int opaque) {
    [window setOpaque:opaque];
}

void NSWindow_setReleasedWhenClosed(NSWindow* window,int releasedWhenClosed) {
    [window setReleasedWhenClosed:releasedWhenClosed];
}

void NSWindow_setRepresentedFilename(NSWindow* window,NSString* representedFilename) {
    [window setRepresentedFilename:representedFilename];
}

void NSWindow_setRepresentedURL(NSWindow* window,NSURL* representedURL) {
    [window setRepresentedURL:representedURL];
}

void NSWindow_setShowsResizeIndicator(NSWindow* window,int showsResizeIndicator) {
    [window setShowsResizeIndicator:showsResizeIndicator];
}

void NSWindow_setShowsToolbarButton(NSWindow* window,int showsToolbarButton) {
    [window setShowsToolbarButton:showsToolbarButton];
}

void NSWindow_setTitle(NSWindow* window,NSString* title) {
    [window setTitle:title];
}

void NSWindow_setToolbar(NSWindow* window,NSToolbar* toolbar) {
    [window setToolbar:toolbar];
}

int NSWindow_styleMask(NSWindow* window) {
    return [window styleMask];
}

void NSWindow_toggleFullScreen(NSWindow* window,NSObject* sender) {
    [window toggleFullScreen:sender];
}

NSToolbar* NSWindow_toolbar(NSWindow* window) {
    return window.toolbar;
}

int NSWindow_windowNumber(NSWindow* window) {
    return window.windowNumber;
}

int NSWindow_windowNumberAtPoint(NSPoint* point, int windowNumber) {
    return [NSWindow windowNumberAtPoint:*point belowWindowWithWindowNumber:windowNumber];
}

void NSWindow_zoom(NSWindow* window,NSObject* sender) {
    [window zoom:sender];
}
