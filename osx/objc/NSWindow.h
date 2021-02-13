/*
 * NSWindow.h
 *
 *  Created on: Dec 31, 2020
 *      Author: azeddine
 */

#ifndef OSX_OBJC_NSWINDOW_H_
#define OSX_OBJC_NSWINDOW_H_
#include "NSView.h"
/*
 SWTWindow
 */
typedef struct NSWindow NSWindow;
typedef struct NSButtonCell NSButtonCell;
typedef struct NSText NSText;
typedef struct NSResponder NSResponder;
typedef struct NSURL NSURL;
typedef struct NSToolbar NSToolbar;
typedef struct NSString NSString;
NSObject* SWTWindowDelegate_alloc(w_widget* widget);
NSWindow* SWTWindow_alloc(w_widget* widget);
NSWindow* SWTPanel_alloc(w_widget* widget);
typedef struct NSPanel NSPanel;
void NSPanel_setBecomesKeyOnlyIfNeeded(NSPanel* panel,int becomesKeyOnlyIfNeeded);
void NSPanel_setFloatingPanel(NSPanel* panel,int floatingPanel);
void NSPanel_setWorksWhenModal(NSPanel* panel,int worksWhenModal);
CGFloat NSPanel_minFrameWidthWithTitle(NSString* aTitle, int aStyle);
int NSPanel_windowNumberAtPoint(NSPoint* point, int windowNumber);
/*
 * NSWindow
 */
typedef struct NSScreen NSScreen;
void NSWindow_addChildWindow(NSWindow* window,NSWindow* childWin, int place);
CGFloat NSWindow_alphaValue(NSWindow* window);
int NSWindow_areCursorRectsEnabled(NSWindow* window);
void NSWindow_becomeKeyWindow(NSWindow* window);
int NSWindow_canBecomeKeyWindow(NSWindow* window);
void NSWindow_cascadeTopLeftFromPoint(NSWindow* window,NSPoint* result,NSPoint* topLeftPoint);
void NSWindow_close(NSWindow* window);
int NSWindow_collectionBehavior(NSWindow* window);
NSView* NSWindow_contentView(NSWindow* window);
void NSWindow_convertBaseToScreen(NSWindow* window,NSPoint* result,NSPoint* aPoint);
void NSWindow_convertScreenToBase(NSWindow* window,NSPoint* result,NSPoint* aPoint);
NSButtonCell* NSWindow_defaultButtonCell(NSWindow* window);
NSObject* NSWindow_delegate(NSWindow* window);
void NSWindow_deminiaturize(NSWindow* window,NSObject* sender);
void NSWindow_disableCursorRects(NSWindow* window);
void NSWindow_disableFlushWindow(NSWindow* window);
void NSWindow_display(NSWindow* window);
void NSWindow_enableCursorRects(NSWindow* window);
void NSWindow_enableFlushWindow(NSWindow* window);
void NSWindow_endEditingFor(NSWindow* window,NSObject* anObject);
NSText* NSWindow_fieldEditor(NSWindow* window,int createFlag, NSObject* anObject);
NSResponder* NSWindow_firstResponder(NSWindow* window);
void NSWindow_flushWindowIfNeeded(NSWindow* window);
void NSWindow_frame(NSWindow* window,NSRect* result);
void NSWindow_frameRectForContentRect(NSWindow* window,NSRect* result,NSRect* contentRect);
NSGraphicsContext* NSWindow_graphicsContext(NSWindow* window);
int NSWindow_hasShadow(NSWindow* window);
NSWindow* NSWindow_initWithContentRect(NSWindow* window,NSRect* contentRect, int aStyle, int bufferingType, int flag);
NSWindow* NSWindow_initWithContentRect_0(NSWindow* window,NSRect* contentRect, int aStyle, int bufferingType, int flag, NSScreen* screen);
void NSWindow_invalidateShadow(NSWindow* window);
int NSWindow_isDocumentEdited(NSWindow* window);
int NSWindow_isKeyWindow(NSWindow* window);
int NSWindow_isMainWindow(NSWindow* window);
int NSWindow_isMiniaturized(NSWindow* window);
int NSWindow_isSheet(NSWindow* window);
int NSWindow_isVisible(NSWindow* window);
int NSWindow_isZoomed(NSWindow* window);
int NSWindow_makeFirstResponder(NSWindow* window,NSResponder* aResponder);
void NSWindow_makeKeyAndOrderFront(NSWindow* window,NSObject* sender);
CGFloat NSWindow_minFrameWidthWithTitle(NSString* aTitle, int aStyle);
void NSWindow_minSize(NSWindow* window,NSSize* result);
void NSWindow_miniaturize(NSWindow* window,NSObject* sender);
void NSWindow_mouseLocationOutsideOfEventStream(NSWindow* window,NSPoint* result);
void NSWindow_orderBack(NSWindow* window,NSObject* sender);
void NSWindow_orderFront(NSWindow* window,NSObject* sender);
void NSWindow_orderFrontRegardless(NSWindow* window);
void NSWindow_orderOut(NSWindow* window,NSObject* sender);
void NSWindow_orderWindow(NSWindow* window,int place, int otherWin);
NSWindow* NSWindow_parentWindow(NSWindow* window);
void NSWindow_removeChildWindow(NSWindow* window,NSWindow* childWin);
NSScreen* NSWindow_screen(NSWindow* window);
void NSWindow_sendEvent(NSWindow* window,NSEvent* theEvent);
void NSWindow_setAllowsAutomaticWindowTabbing(int tabbing);
void NSWindow_setAcceptsMouseMovedEvents(NSWindow* window,int acceptsMouseMovedEvents);
void NSWindow_setAlphaValue(NSWindow* window,CGFloat alphaValue);
void NSWindow_setBackgroundColor(NSWindow* window,NSColor* backgroundColor);
void NSWindow_setCollectionBehavior(NSWindow* window,int collectionBehavior);
void NSWindow_setContentView(NSWindow* window,NSView* contentView);
void NSWindow_setDefaultButtonCell(NSWindow* window,NSButtonCell* defaultButtonCell);
NSObject* NSWindow_getDelegate(NSWindow* window);
void NSWindow_setDelegate(NSWindow* window,NSObject* delegate);
void NSWindow_setDocumentEdited(NSWindow* window,int documentEdited);
void NSWindow_setFrame(NSWindow* window,NSRect* frameRect, int flag);
void NSWindow_setFrame_0(NSWindow* window,NSRect* frameRect, int displayFlag, int animateFlag);
void NSWindow_setHasShadow(NSWindow* window,int hasShadow);
void NSWindow_setHidesOnDeactivate(NSWindow* window,int hidesOnDeactivate);
void NSWindow_setLevel(NSWindow* window,int level);
void NSWindow_setMinSize(NSWindow* window,NSSize* minSize);
void NSWindow_setMovable(NSWindow* window,int movable);
void NSWindow_setOpaque(NSWindow* window,int opaque);
void NSWindow_setReleasedWhenClosed(NSWindow* window,int releasedWhenClosed);
void NSWindow_setRepresentedFilename(NSWindow* window,NSString* representedFilename);
void NSWindow_setRepresentedURL(NSWindow* window,NSURL* representedURL);
void NSWindow_setShowsResizeIndicator(NSWindow* window,int showsResizeIndicator);
void NSWindow_setShowsToolbarButton(NSWindow* window,int showsToolbarButton);
void NSWindow_setTitle(NSWindow* window,NSString* title);
void NSWindow_setToolbar(NSWindow* window,NSToolbar* toolbar);
int NSWindow_styleMask(NSWindow* window);
void NSWindow_toggleFullScreen(NSWindow* window,NSObject* sender);
NSToolbar* NSWindow_toolbar(NSWindow* window);
int NSWindow_windowNumber(NSWindow* window);
int NSWindow_windowNumberAtPoint(NSPoint* point, int windowNumber);
void NSWindow_zoom(NSWindow* window,NSObject* sender);


#endif /* OSX_OBJC_NSWINDOW_H_ */
