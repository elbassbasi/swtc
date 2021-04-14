/*
 * swt.h
 *
 *  Created on: Dec 30, 2020
 *      Author: azeddine
 */

#ifndef OSX_OBJC_SWT_H_
#define OSX_OBJC_SWT_H_
#include <swtc.h>
#ifdef __OBJC__
#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#else
#import <objc/NSObjCRuntime.h>
#import <objc/NSObject.h>
#import <objc/runtime.h>
typedef struct NSObject NSObject;
typedef struct NSView NSView;
#endif
/*
 * event for platform
 */
typedef union _w_arg {
	wuint64 _int;
	void *_ptr;
	NSObject *_id;
} _w_arg;
#define _W_EVENT_PLATFORM_SUPER (1 << 0)
#define _W_EVENT_PLATFORM_INVERSE (1 << 1)
typedef struct _w_event_platform {
	w_event event;
	wushort msg;
	wushort flags;
	NSView *handle;
	_w_arg result;
	_w_arg args[3];
	void *reserved[4];
} _w_event_platform;
#define _W_EVENT_PLATFORM(x) ((_w_event_platform*)x)
void _w_event_platform_init(_w_event_platform *e, w_widget *widget,
		void *handle, int msg);
wresult _w_widget_send_event(w_widget *widget, w_event *event);
wresult _w_toolkit_notify(int msg,void* obj);
enum {
	_NS_CLASS_VIEW,
	_NS_CLASS_SCROLLVIEW,
	_NS_CLASS_BUTTON,
	_NS_CLASS_OUTLINEVIEW,
	_NS_CLASS_BOX,
	_NS_CLASS_LAST
};
typedef w_widget* (*ns_get_widget)(NSView *view);
void _w_toolkit_registre_class(int _id, Class clazz, ns_get_widget funct);
enum {
	_NS_UNKNOWN = 0,
	/*
	 VIEW
	 */
	_NS_resignFirstResponder,
	_NS_becomeFirstResponder,
	_NS_resetCursorRects,
	_NS_updateTrackingAreas,
	_NS_mouseDownCanMoveWindow,
	_NS_mouseDown,
	_NS_mouseUp,
	_NS_scrollWheel,
	_NS_rightMouseDown,
	_NS_rightMouseUp,
	_NS_rightMouseDragged,
	_NS_otherMouseDown,
	_NS_otherMouseUp,
	_NS_otherMouseDragged,
	_NS_mouseDragged,
	_NS_mouseMoved,
	_NS_mouseEntered,
	_NS_mouseExited,
	_NS_menuForEvent,
	_NS_keyDown,
	_NS_keyUp,
	_NS_flagsChanged,
	_NS_cursorUpdate,
	_NS_setNeedsDisplay,
	_NS_shouldDelayWindowOrderingForEvent,
	_NS_acceptsFirstMouse,
	_NS_changeColor,
	_NS_cancelOperation,
	_NS_touchesBeganWithEvent,
	_NS_touchesMovedWithEvent,
	_NS_touchesEndedWithEvent,
	_NS_touchesCancelledWithEvent,
	_NS_swipeWithEvent,
	_NS_rotateWithEvent,
	_NS_magnifyWithEvent,
	_NS_setNeedsDisplayInRect,
	_NS_drawRect,
	_NS_hitTest,
	_NS_VIEW_LAST,
	/*

	 */
	_NS_WILL_MOVE_TO_WINDOW = _NS_hitTest,
	_NS_outlineViewSelectionDidChange,
	_NS_outlineViewColumnDidMove,
	_NS_outlineViewColumnDidResize,
	_NS_outlineViewSelectionIsChanging,
	_NS_outlineViewItemWillExpand,
	_NS_outlineViewItemDidExpand,
	_NS_outlineViewItemWillCollapse,
	_NS_outlineViewItemDidCollapse,
	/*
	 *
	 */
	_NS_selection,
	/*
	 window
	 */
	_NS_windowDidBecomeKey,
	_NS_WINDOW_DID_DEMINITURIZE,
	_NS_WINDOW_DID_MINITURIZE,
	_NS_WINDOW_DID_MOVE,
	_NS_windowDidResize,
	_NS_WINDOW_DID_RESIGN_KEY,
	_NS_WINDOW_SEND_EVENT,
	_NS_windowWillClose,
	_NS_windowShouldClose,
	_NS_LAST_MSG,
    _NS_applicationDidFinishLaunching=1,
    _NS_applicationWillTerminate,
    _NS_applicationDidBecomeActive,

};
#define _NSMETHOD_EVENT(superclass,method) -(void)method:(NSEvent *)event{\
    _w_event_platform e;\
    _w_event_platform_init(&e, widget, self, _NS_##method);\
    e.args[0]._id =(NSObject*) event;\
    _w_widget_send_event(widget,W_EVENT(&e));\
    if(e.flags & _W_EVENT_PLATFORM_SUPER){ \
		[super method:event];\
        e.flags |= _W_EVENT_PLATFORM_INVERSE;\
        _w_widget_send_event(widget,W_EVENT(&e));\
	}\
}
#define _NSMETHOD_EVENT_BOOL(superclass,method) -(BOOL)method:(NSEvent *)event{\
    _w_event_platform e;\
    _w_event_platform_init(&e, widget, self, _NS_##method);\
    e.args[0]._id =(NSObject*) event;\
    _w_widget_send_event(widget,W_EVENT(&e));\
    if(e.flags & _W_EVENT_PLATFORM_SUPER){ \
		[super method:event];\
        e.flags |= _W_EVENT_PLATFORM_INVERSE;\
        _w_widget_send_event(widget,W_EVENT(&e));\
	}\
    return (int)e.result._int;\
}
#define _NSMETHOD_BOOL(superclass,method) -(BOOL)method{\
    _w_event_platform e;\
    _w_event_platform_init(&e, widget, self, _NS_##method);\
    _w_widget_send_event(widget,W_EVENT(&e));\
    if(e.flags & _W_EVENT_PLATFORM_SUPER){ \
		[super method];\
        e.flags |= _W_EVENT_PLATFORM_INVERSE;\
        _w_widget_send_event(widget,W_EVENT(&e));\
	}\
    return (int)e.result._int;\
}
#define _NSMETHOD_mouseDown(superclass) _NSMETHOD_EVENT(superclass,mouseDown)
#define _NSMETHOD_mouseUp(superclass) _NSMETHOD_EVENT(superclass,mouseUp)
#define _NSMETHOD_scrollWheel(superclass) _NSMETHOD_EVENT(superclass,scrollWheel)
#define _NSMETHOD_rightMouseDown(superclass) _NSMETHOD_EVENT(superclass,rightMouseDown)
#define _NSMETHOD_rightMouseUp(superclass) _NSMETHOD_EVENT(superclass,rightMouseUp)
#define _NSMETHOD_rightMouseDragged(superclass) _NSMETHOD_EVENT(superclass,rightMouseDragged)
#define _NSMETHOD_otherMouseDown(superclass) _NSMETHOD_EVENT(superclass,otherMouseDown)
#define _NSMETHOD_otherMouseUp(superclass) _NSMETHOD_EVENT(superclass,otherMouseUp)
#define _NSMETHOD_otherMouseDragged(superclass) _NSMETHOD_EVENT(superclass,otherMouseDragged)
#define _NSMETHOD_mouseDragged(superclass) _NSMETHOD_EVENT(superclass,mouseDragged)
#define _NSMETHOD_mouseMoved(superclass) _NSMETHOD_EVENT(superclass,mouseMoved)
#define _NSMETHOD_mouseEntered(superclass) _NSMETHOD_EVENT(superclass,mouseEntered)
#define _NSMETHOD_mouseExited(superclass) _NSMETHOD_EVENT(superclass,mouseExited)
#define _NSMETHOD_menuForEvent(superclass) -(nullable NSMenu *)menuForEvent:(NSEvent *)event{\
	    _w_event_platform e;\
	    _w_event_platform_init(&e, widget, self, _NS_##menuForEvent);\
	    e.args[0]._id =(NSObject*) event;\
	    _w_widget_send_event(widget,W_EVENT(&e));\
        if(e.flags & _W_EVENT_PLATFORM_SUPER){ \
			[super menuForEvent:event];\
            e.flags |= _W_EVENT_PLATFORM_INVERSE;\
            _w_widget_send_event(widget,W_EVENT(&e));\
		}\
	    return (NSMenu *)e.result._id;\
}
#define _NSMETHOD_keyDown(superclass) _NSMETHOD_EVENT(superclass,keyDown)
#define _NSMETHOD_keyUp(superclass) _NSMETHOD_EVENT(superclass,keyUp)
#define _NSMETHOD_flagsChanged(superclass) _NSMETHOD_EVENT(superclass,flagsChanged)
#define _NSMETHOD_cursorUpdate(superclass) _NSMETHOD_EVENT(superclass,cursorUpdate)
#define _NSMETHOD_setNeedsDisplay(superclass) -(void)setNeedsDisplay:(BOOL)display{\
		_w_event_platform e;\
		_w_event_platform_init(&e, widget, self, _NS_setNeedsDisplay);\
		e.args[0]._int =display;\
		_w_widget_send_event(widget,W_EVENT(&e));\
        if(e.flags & _W_EVENT_PLATFORM_SUPER){ \
			[super setNeedsDisplay:display];\
            e.flags |= _W_EVENT_PLATFORM_INVERSE;\
            _w_widget_send_event(widget,W_EVENT(&e));\
		}\
}
#define _NSMETHOD_shouldDelayWindowOrderingForEvent(superclass) _NSMETHOD_EVENT_BOOL(superclass,shouldDelayWindowOrderingForEvent)
#define _NSMETHOD_acceptsFirstMouse(superclass) _NSMETHOD_EVENT_BOOL(superclass,acceptsFirstMouse)
#define _NSMETHOD_changeColor(superclass) _NSMETHOD_EVENT(superclass,changeColor)
#define _NSMETHOD_cancelOperation(superclass) _NSMETHOD_EVENT(superclass,cancelOperation)
#define _NSMETHOD_touchesBeganWithEvent(superclass) _NSMETHOD_EVENT(superclass,touchesBeganWithEvent)
#define _NSMETHOD_touchesMovedWithEvent(superclass) _NSMETHOD_EVENT(superclass,touchesMovedWithEvent)
#define _NSMETHOD_touchesEndedWithEvent(superclass) _NSMETHOD_EVENT(superclass,touchesEndedWithEvent)
#define _NSMETHOD_touchesCancelledWithEvent(superclass) _NSMETHOD_EVENT(superclass,touchesCancelledWithEvent)
#define _NSMETHOD_swipeWithEvent(superclass) _NSMETHOD_EVENT(superclass,swipeWithEvent)
#define _NSMETHOD_rotateWithEvent(superclass) _NSMETHOD_EVENT(superclass,rotateWithEvent)
#define _NSMETHOD_magnifyWithEvent(superclass) _NSMETHOD_EVENT(superclass,magnifyWithEvent)
#define _NSMETHOD_resignFirstResponder(superclass) _NSMETHOD_BOOL(superclass,resignFirstResponder)
#define _NSMETHOD_becomeFirstResponder(superclass) _NSMETHOD_BOOL(superclass,becomeFirstResponder)
#define _NSMETHOD_resetCursorRects(superclass) -(void)resetCursorRects{\
	    _w_event_platform e;\
	    _w_event_platform_init(&e, widget, self, _NS_resetCursorRects);\
	    _w_widget_send_event(widget,W_EVENT(&e));\
        if(e.flags & _W_EVENT_PLATFORM_SUPER){ \
			[super resetCursorRects];\
            e.flags |= _W_EVENT_PLATFORM_INVERSE;\
            _w_widget_send_event(widget,W_EVENT(&e));\
		}\
}
#define _NSMETHOD_updateTrackingAreas(superclass) -(void)updateTrackingAreas{\
    _w_event_platform e;\
    _w_event_platform_init(&e, widget, self, _NS_updateTrackingAreas);\
    _w_widget_send_event(widget,W_EVENT(&e));\
    if(e.flags & _W_EVENT_PLATFORM_SUPER){ \
		[super updateTrackingAreas];\
        e.flags |= _W_EVENT_PLATFORM_INVERSE;\
        _w_widget_send_event(widget,W_EVENT(&e));\
	}\
}
#define _NSMETHOD_mouseDownCanMoveWindow(superclass) _NSMETHOD_BOOL(superclass,mouseDownCanMoveWindow)
#define _NSMETHOD_setNeedsDisplayInRect(superclass) -(void)setNeedsDisplayInRect:(NSRect)invalidRect{\
    _w_event_platform e;\
    _w_event_platform_init(&e, widget, self, _NS_setNeedsDisplayInRect);\
    e.args[0]._ptr =(void*) &invalidRect;\
    _w_widget_send_event(widget,W_EVENT(&e));\
    if(e.flags & _W_EVENT_PLATFORM_SUPER){ \
		[super setNeedsDisplayInRect:invalidRect];\
        e.flags |= _W_EVENT_PLATFORM_INVERSE;\
        _w_widget_send_event(widget,W_EVENT(&e));\
	}\
}
#define _NSMETHOD_drawRect(superclass) -(void)drawRect:(NSRect)dirtyRect{\
		_w_event_platform e;\
		_w_event_platform_init(&e, widget, self, _NS_drawRect);\
		e.args[0]._ptr =(void*) &dirtyRect;\
		_w_widget_send_event(widget,W_EVENT(&e));\
		if(e.flags & _W_EVENT_PLATFORM_SUPER){ \
			[super drawRect:dirtyRect];\
			e.flags |= _W_EVENT_PLATFORM_INVERSE;\
			_w_widget_send_event(widget,W_EVENT(&e));\
		}\
}
#define _NSMETHOD_hitTest(superclass) -(nullable NSView *)hitTest:(NSPoint)point{\
    _w_event_platform e;\
    _w_event_platform_init(&e, widget, self, _NS_hitTest);\
    e.args[0]._ptr =(void*)  &point;\
    _w_widget_send_event(widget,W_EVENT(&e));\
    if(e.flags & _W_EVENT_PLATFORM_SUPER){ \
		[super hitTest:point];\
        e.flags |= _W_EVENT_PLATFORM_INVERSE;\
        _w_widget_send_event(widget,W_EVENT(&e));\
	}\
    return (NSView *)e.result._id;\
}
#define _NSMETHOD_EVENT_3(superclass)\
_NSMETHOD_mouseDown(superclass) \
_NSMETHOD_mouseUp(superclass) \
_NSMETHOD_scrollWheel(superclass) \
_NSMETHOD_rightMouseDown(superclass) \
_NSMETHOD_rightMouseUp(superclass) \
_NSMETHOD_rightMouseDragged(superclass) \
_NSMETHOD_otherMouseDown(superclass) \
_NSMETHOD_otherMouseUp(superclass) \
_NSMETHOD_otherMouseDragged(superclass) \
_NSMETHOD_mouseDragged(superclass) \
_NSMETHOD_mouseMoved(superclass) \
_NSMETHOD_mouseEntered(superclass) \
_NSMETHOD_mouseExited(superclass) \
_NSMETHOD_menuForEvent(superclass) \
_NSMETHOD_keyDown(superclass) \
_NSMETHOD_keyUp(superclass) \
_NSMETHOD_flagsChanged(superclass) \
_NSMETHOD_cursorUpdate(superclass) \
_NSMETHOD_setNeedsDisplay(superclass) \
_NSMETHOD_shouldDelayWindowOrderingForEvent(superclass) \
_NSMETHOD_acceptsFirstMouse(superclass) \
_NSMETHOD_changeColor(superclass) \
_NSMETHOD_cancelOperation(superclass) \
_NSMETHOD_touchesBeganWithEvent(superclass) \
_NSMETHOD_touchesMovedWithEvent(superclass) \
_NSMETHOD_touchesEndedWithEvent(superclass) \
_NSMETHOD_touchesCancelledWithEvent(superclass) \
_NSMETHOD_swipeWithEvent(superclass) \
_NSMETHOD_rotateWithEvent(superclass) \
_NSMETHOD_magnifyWithEvent(superclass) \
/*
 *
 */
#define _NSMETHOD_EVENT_2(superclass) _NSMETHOD_resignFirstResponder(superclass) \
_NSMETHOD_becomeFirstResponder(superclass) \
_NSMETHOD_resetCursorRects(superclass) \
_NSMETHOD_updateTrackingAreas(superclass) \
_NSMETHOD_mouseDownCanMoveWindow(superclass)

#endif /* OSX_OBJC_SWT_H_ */
