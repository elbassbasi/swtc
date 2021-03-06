#import "swt.h"
@interface AppDelegate : NSObject <NSApplicationDelegate>
@end
@implementation AppDelegate
- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    _w_toolkit_notify(_NS_applicationDidFinishLaunching,aNotification);
}
- (void)applicationWillTerminate:(NSNotification *)aNotification {
     _w_toolkit_notify(_NS_applicationWillTerminate,aNotification);
}
- (void)applicationDidBecomeActive:(NSNotification *)aNotification{
    _w_toolkit_notify(_NS_applicationDidBecomeActive,aNotification);
}
@end
void NSApp_Init(){
    AppDelegate *appDelegate = [[AppDelegate alloc] init];
    [NSApp setDelegate:appDelegate];
}
void NSApp_run(){
    [NSApp run];
}
NSAutoreleasePool* NSAutoreleasePool_New(){
    return [[NSAutoreleasePool alloc] init];
}
void NSAutoreleasePool_release(NSAutoreleasePool* pool){
    [pool release];
}
int NSThread_isMainThread(){
    return [NSThread isMainThread];
}
/*
 *  NSDate
 */
NSCalendarDate* NSDate_dateWithCalendarFormat(NSDate* date,NSString* format, NSTimeZone* aTimeZone) {
    return [date dateWithCalendarFormat:format timeZone:aTimeZone];
}

NSDate* NSDate_dateWithTimeIntervalSinceNow(CGFloat secs) {
    return [NSDate dateWithTimeIntervalSinceNow:secs];
}

NSDate* NSDate_distantFuture() {
    return [NSDate distantFuture];
}
/*
 *  NSRunLoop
 */
void NSRunLoop_addTimer(NSRunLoop* runloop,NSTimer* timer, NSString* mode) {
    [runloop addTimer:timer forMode:mode];
}

NSRunLoop* NSRunLoop_currentRunLoop() {
    return [NSRunLoop currentRunLoop];
}

int NSRunLoop_runMode(NSRunLoop* runloop,NSString* mode, NSDate* limitDate) {
    return [runloop runMode:mode beforeDate:limitDate];
}
/*
 *  NSApplication
 */
 void NSApplication_activateIgnoringOtherApps(NSApplication *application,int flag) {
     [application activateIgnoringOtherApps:flag];
}

 NSImage* NSApplication_applicationIconImage(NSApplication *application) {
    return  [application applicationIconImage];
}

 void NSApplication_arrangeInFront(NSApplication *application,NSObject* sender) {
     [application arrangeInFront:sender];
}

 void NSApplication_beginSheet(NSApplication *application,NSWindow* sheet, NSWindow* docWindow, NSObject* modalDelegate, int didEndSelector, void* contextInfo) {
 [application beginSheet:sheet modalForWindow:docWindow modalDelegate:modalDelegate didEndSelector:didEndSelector contextInfo:contextInfo];
}

 NSEvent* NSApplication_currentEvent(NSApplication *application) {
    return  [application currentEvent];
}

 NSDockTile* NSApplication_dockTile(NSApplication *application) {
    return  [application dockTile];
}

 void NSApplication_endSheet(NSApplication *application,NSWindow* sheet, int returnCode) {
    [application endSheet:returnCode];
}

 void NSApplication_finishLaunching(NSApplication *application) {
     [application finishLaunching];
}

 void NSApplication_hide(NSApplication *application,NSObject* sender) {
     [application hide:sender];
}

 void NSApplication_hideOtherApplications(NSApplication *application,NSObject* sender) {
     [application hideOtherApplications:sender];
}

 int NSApplication_isActive(NSApplication *application) {
    return  [application isActive];
}

 int NSApplication_isRunning(NSApplication *application) {
    return  [application isRunning];
}

 NSWindow* NSApplication_keyWindow(NSApplication *application) {
    return  [application keyWindow];;
}

 NSMenu* NSApplication_mainMenu(NSApplication *application) {
    return [application mainMenu];
}

 NSEvent* NSApplication_nextEventMatchingMask(NSApplication *application,int mask, NSDate* expiration, NSString* mode, int deqFlag) {
   return [application nextEventMatchingMask:mask untilDate:expiration inMode:mode dequeue:deqFlag];
}

 void NSApplication_orderFrontStandardAboutPanel(NSApplication *application,NSObject* sender) {
     [application orderFrontStandardAboutPanel:sender];
}

 void NSApplication_postEvent(NSApplication *application,NSEvent* event, int flag) {
    [application postEvent:event atStart:flag];
}

 void NSApplication_replyToOpenOrPrint(NSApplication *application,int reply) {
    [application replyToOpenOrPrint:reply];
}

 NSModalResponse* NSApplication_runModalForWindow(NSApplication *application,NSWindow* theWindow) {
    return [application runModalForWindow:theWindow];
}

 int NSApplication_sendAction(NSApplication *application,int theAction, NSObject* theTarget, NSObject* sender) {
    return [application sendAction:theAction to:theTarget from:sender];
}

 void NSApplication_sendEvent(NSApplication *application,NSEvent* theEvent) {
    [application sendEvent:theEvent];
}

 int NSApplication_setActivationPolicy(NSApplication *application,int activationPolicy) {
    return [application setActivationPolicy:activationPolicy];
}

void NSApplication_setApplicationIconImage(NSApplication *application,NSImage* applicationIconImage) {
     [application setApplicationIconImage:applicationIconImage];
}

void NSApplication_setDelegate(NSApplication *application,NSObject* delegate) {
 [application setDelegate:delegate];
}

void NSApplication_setHelpMenu(NSApplication *application,NSMenu* helpMenu) {
  [application setHelpMenu:helpMenu];
}

void NSApplication_setMainMenu(NSApplication *application,NSMenu* mainMenu) {
  [application setMainMenu:mainMenu];
}

void NSApplication_setServicesMenu(NSApplication *application,NSMenu* servicesMenu) {
 [application setServicesMenu:servicesMenu];
}

NSApplication* NSApplication_sharedApplication() {
    return  [NSApplication sharedApplication];
}

void NSApplication_stop(NSApplication *application,NSObject* sender) {
    [application stop:sender];
}

void NSApplication_stopModal(NSApplication *application) {
     [application stopModal];
}

void NSApplication_terminate(NSApplication *application,NSObject* sender) {
     [application terminate:sender];
}

void NSApplication_unhideAllApplications(NSApplication *application,NSObject* sender) {
     [application unhideAllApplications:sender];
}
NSWindow* NSApplication_windowWithWindowNumber(NSApplication *application,int windowNum) {
     return [application windowWithWindowNumber:windowNum];
}
NSArray* NSApplication_windows(NSApplication *application) {
     return [application windows];
}
/*
 * NSScreen
 */
CGFloat NSScreen_backingScaleFactor(NSScreen* screen) {
    return [screen backingScaleFactor];
}
int NSScreen_depth(NSScreen* screen) {
    return [screen depth];
}
NSDictionary* NSScreen_deviceDescription(NSScreen* screen) {
    return [screen deviceDescription];
}
void NSScreen_frame(NSScreen* screen,NSRect* result) {
    *result = screen.frame;
}
NSScreen* NSScreen_mainScreen() {
    return [NSScreen mainScreen];
}
NSArray* NSScreen_screens() {
    return [NSScreen screens];
}
void NSScreen_visibleFrame(NSScreen* screen,NSRect* result) {
    *result = screen.visibleFrame;
}

