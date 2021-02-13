/*
 * NSApplication.h
 *
 *  Created on: Dec 30, 2020
 *      Author: azeddine
 */

#ifndef OSX_OBJC_NSAPPLICATION_H_
#define OSX_OBJC_NSAPPLICATION_H_
#include "NSView.h"
typedef struct NSApplication NSApplication;
typedef struct NSDockTile NSDockTile;
typedef struct NSDate NSDate;
typedef struct NSModalResponse NSModalResponse;
typedef struct NSRunLoop NSRunLoop;
typedef struct NSTimer NSTimer;
typedef struct NSCalendarDate NSCalendarDate;
typedef struct NSTimeZone NSTimeZone;
typedef struct NSString NSString;

/*
 *  NSDate
 */
NSCalendarDate* NSDate_dateWithCalendarFormat(NSDate* date,NSString* format, NSTimeZone* aTimeZone);
NSDate* NSDate_dateWithTimeIntervalSinceNow(CGFloat secs);
NSDate* NSDate_distantFuture();

extern NSString* const NSDefaultRunLoopMode;
extern NSString* const NSRunLoopCommonModes;
/*
 * NSRunLoop
 */
void NSRunLoop_addTimer(NSRunLoop* runloop,NSTimer* timer, NSString* mode);
NSRunLoop* NSRunLoop_currentRunLoop();
int NSRunLoop_runMode(NSRunLoop* runloop,NSString* mode, NSDate* limitDate);
/*
 * NSApplication
 */
void NSApplication_activateIgnoringOtherApps(NSApplication *application,int flag);
NSImage* NSApplication_applicationIconImage(NSApplication *application);
void NSApplication_arrangeInFront(NSApplication *application,NSObject* sender);
void NSApplication_beginSheet(NSApplication *application,NSWindow* sheet, NSWindow* docWindow, NSObject* modalDelegate, int didEndSelector, void* contextInfo);
NSEvent* NSApplication_currentEvent(NSApplication *application);
NSDockTile* NSApplication_dockTile(NSApplication *application);
void NSApplication_endSheet(NSApplication *application,NSWindow* sheet, int returnCode);
void NSApplication_finishLaunching(NSApplication *application);
void NSApplication_hide(NSApplication *application,NSObject* sender);
void NSApplication_hideOtherApplications(NSApplication *application,NSObject* sender);
int NSApplication_isActive(NSApplication *application);
int NSApplication_isRunning(NSApplication *application);
NSWindow* NSApplication_keyWindow(NSApplication *application);
NSMenu* NSApplication_mainMenu(NSApplication *application);
NSEvent* NSApplication_nextEventMatchingMask(NSApplication *application,int mask, NSDate* expiration, NSString* mode, int deqFlag);
void NSApplication_orderFrontStandardAboutPanel(NSApplication *application,NSObject* sender);
void NSApplication_postEvent(NSApplication *application,NSEvent* event, int flag);
void NSApplication_replyToOpenOrPrint(NSApplication *application,int reply);
NSModalResponse* NSApplication_runModalForWindow(NSApplication *application,NSWindow* theWindow);
int NSApplication_sendAction(NSApplication *application,int theAction, NSObject* theTarget, NSObject* sender);
void NSApplication_sendEvent(NSApplication *application,NSEvent* theEvent);
int NSApplication_setActivationPolicy(NSApplication *application,int activationPolicy);
void NSApplication_setApplicationIconImage(NSApplication *application,NSImage* applicationIconImage);
void NSApplication_setDelegate(NSApplication *application,NSObject* delegate);
void NSApplication_setHelpMenu(NSApplication *application,NSMenu* helpMenu) ;
void NSApplication_setMainMenu(NSApplication *application,NSMenu* mainMenu) ;
void NSApplication_setServicesMenu(NSApplication *application,NSMenu* servicesMenu);
NSApplication* NSApplication_sharedApplication();
void NSApplication_stop(NSApplication *application,NSObject* sender);
void NSApplication_stopModal(NSApplication *application);
void NSApplication_terminate(NSApplication *application,NSObject* sender);
void NSApplication_unhideAllApplications(NSApplication *application,NSObject* sender);
NSWindow* NSApplication_windowWithWindowNumber(NSApplication *application,int windowNum);
NSArray* NSApplication_windows(NSApplication *application);
/*
 * NSScreen
 */
typedef struct NSDictionary NSDictionary;
typedef struct NSScreen NSScreen;
CGFloat NSScreen_backingScaleFactor(NSScreen* screen);
int NSScreen_depth(NSScreen* screen);
NSDictionary* NSScreen_deviceDescription(NSScreen* screen);
void NSScreen_frame(NSScreen* screen,NSRect* result);
NSScreen* NSScreen_mainScreen();
NSArray* NSScreen_screens();
void NSScreen_visibleFrame(NSScreen* screen,NSRect* result);

/*
NSAutoreleasePool
 */
typedef struct NSAutoreleasePool NSAutoreleasePool;
NSAutoreleasePool* NSAutoreleasePool_New();
void NSAutoreleasePool_release(NSAutoreleasePool* pool);
void NSApp_Init();
int NSThread_isMainThread();


#endif /* OSX_OBJC_NSAPPLICATION_H_ */
