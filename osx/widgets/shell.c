/*
 * Name:        shell.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "shell.h"
#include "toolkit.h"
wuint64 _w_shell_check_style(w_widget *control, wuint64 style) {
	if ((style & W_NO_TRIM) != 0) {
		style &= ~(W_CLOSE | W_TITLE | W_MIN | W_MAX | W_RESIZE | W_BORDER);
	} else if ((style & W_NO_MOVE) != 0) {
		style |= W_TITLE;
	}
	if ((style & (W_MENU | W_MIN | W_MAX | W_CLOSE)) != 0) {
		style |= W_TITLE;
	}
	style &= ~W_TRANSPARENT;
	int mask = W_SYSTEM_MODAL | W_APPLICATION_MODAL | W_PRIMARY_MODAL;
	if ((style & W_SHEET) != 0) {
		w_composite *parent = _W_CONTROL(control)->parent;
		if (mac_toolkit->sheet) {
			style &= ~(W_CLOSE | W_TITLE | W_MIN | W_MAX);
			if (parent == 0) {
				style &= ~W_SHEET;
				style |= W_FRAME_TRIM;
			}
		} else {
			style &= ~W_SHEET;
			style |= parent == 0 ? W_FRAME_TRIM : W_DIALOG_TRIM;
		}
		style |= parent == 0 ? W_FRAME_TRIM : W_DIALOG_TRIM;
		if ((style & mask) == 0) {
			style |= parent == 0 ? W_APPLICATION_MODAL : W_PRIMARY_MODAL;
		}
	}
	int bits = style & ~mask;
	if ((style & W_SYSTEM_MODAL) != 0)
		return bits | W_SYSTEM_MODAL;
	if ((style & W_APPLICATION_MODAL) != 0)
		return bits | W_APPLICATION_MODAL;
	if ((style & W_PRIMARY_MODAL) != 0)
		return bits | W_PRIMARY_MODAL;
	return bits;
}
wresult _w_shell_create_embedded(w_widget *widget, w_widget *parent,
		wuint64 style, w_widget_post_event_proc post_event, void *handle,
		int embedded) {
	if (parent != 0) {
		if (!w_widget_is_ok(parent))
			return W_ERROR_INVALID_ARGUMENT;
		if (w_widget_class_id(parent) != _W_CLASS_SHELL)
			return W_ERROR_INVALID_ARGUMENT;
	}
	_W_CONTROL(widget)->parent = W_COMPOSITE(parent);
	if (!mac_toolkit->sheet) {
		_W_SHELL(widget)->center = parent != 0 && (style & W_SHEET) != 0;
	}
	_W_WIDGET(widget)->post_event = post_event;
	_W_WIDGET(widget)->style = _w_shell_check_style(widget, style);
	if (handle != 0) {
		if (embedded) {
			_W_WIDGET(widget)->handle = handle;
		} else {
			_W_SHELL(widget)->window = handle;
			_W_WIDGET(widget)->state |= STATE_FOREIGN_HANDLE;
		}
	}
	_w_widget_priv *priv = _w_widget_get_priv(widget);
	wresult result = priv->create_widget(widget, _W_CONTROL_PRIV(priv));
	if (result > 0) {
		_w_toolkit_add_shell(_W_SHELL(widget));
	}
	return result;
}
wresult _w_shell_create(w_widget *widget, w_widget *parent, wuint64 style,
		w_widget_post_event_proc post_event) {
	return _w_shell_create_embedded(widget, parent, style, post_event, 0,
			W_FALSE);
}
NSWindow* _w_shell_parent_window(w_shell *shell) {
	w_shell *parent = W_SHELL(_W_CONTROL(shell)->parent);
	if (parent == 0)
		return 0;
	return NSView_window(_W_WIDGET(parent)->handle);
}
wresult _w_shell_fix_resize(w_shell *shell) {
	/*
	 * Feature in Cocoa.  It is not possible to have a resizable window
	 * without the title bar.  The fix is to resize the content view on
	 * top of the title bar.
	 *
	 * Never do this when the shell is embedded, because the window belongs to the AWT.
	 */
	if (_W_SHELL(shell)->window == 0)
		return W_FALSE;
	if ((_W_WIDGET(shell)->style & W_NO_TRIM) == 0) {
		if ((_W_WIDGET(shell)->style & W_RESIZE) != 0
				&& (_W_WIDGET(shell)->style
						& (W_SHEET | W_TITLE | W_CLOSE | W_MIN | W_MAX)) == 0) {
			return W_TRUE;
		}
	}
	return W_FALSE;
}
wresult _w_shell_create_handle(w_widget *widget, _w_control_priv *priv) {
	_W_WIDGET(widget)->state |= STATE_HIDDEN;
	NSWindow *window = 0;
	NSRect frame;
	if (_W_SHELL(widget)->window == 0 && _W_WIDGET(widget)->handle == 0) {
		int styleMask = NSBorderlessWindowMask;
		if ((_W_WIDGET(widget)->style & (W_TOOL | W_SHEET)) != 0) {
			window = SWTPanel_alloc(widget);
			if ((_W_WIDGET(widget)->style & W_SHEET) != 0) {
				styleMask |= NSDocModalWindowMask;
			} else {
				styleMask |= NSUtilityWindowMask | NSNonactivatingPanelMask;
			}
		} else {
			window = SWTWindow_alloc(widget);
		}
		_W_SHELL(widget)->window = window;
		if ((_W_WIDGET(widget)->style & W_NO_TRIM) == 0) {
			if ((_W_WIDGET(widget)->style & W_TITLE) != 0)
				styleMask |= NSTitledWindowMask;
			if ((_W_WIDGET(widget)->style & W_CLOSE) != 0)
				styleMask |= NSClosableWindowMask;
			if ((_W_WIDGET(widget)->style & W_MIN) != 0)
				styleMask |= NSMiniaturizableWindowMask;
			if ((_W_WIDGET(widget)->style & W_MAX) != 0)
				styleMask |= NSResizableWindowMask;
			if ((_W_WIDGET(widget)->style & W_RESIZE) != 0)
				styleMask |= NSResizableWindowMask;
		}
		NSScreen *screen = 0;
		NSArray *screens = NSScreen_screens();
		NSScreen *primaryScreen = (NSScreen*) NSArray_objectAtIndex(screens, 0);
		if (_W_CONTROL(widget)->parent != 0) {
			NSWindow *parentWindow = _w_shell_parent_window(W_SHELL(widget));
			screen = NSWindow_screen(parentWindow);
		}
		if (screen == 0)
			screen = primaryScreen;
		memset(&frame, 0, sizeof(frame));
		window = NSWindow_initWithContentRect_0(window, &frame, styleMask,
				NSBackingStoreBuffered,
				(_W_WIDGET(widget)->style & W_ON_TOP) != 0, screen);
		if ((_W_WIDGET(widget)->style & (W_NO_TRIM | W_BORDER | W_FRAME_TRIM))
				== 0 || (_W_WIDGET(widget)->style & (W_TOOL | W_SHEET)) != 0) {
			NSWindow_setHasShadow(window, W_TRUE);
		}
		if ((_W_WIDGET(widget)->style & W_NO_MOVE) != 0) {
			NSWindow_setMovable(window, W_FALSE);
		}
		if ((_W_WIDGET(widget)->style & W_TOOL) != 0) {
			// Feature in Cocoa: NSPanels that use NSUtilityWindowMask are always promoted to the floating window layer.
			// Fix is to call setFloatingPanel:NO, which turns off this behavior.
			NSPanel_setFloatingPanel((NSPanel*) window, W_FALSE);
			// By default, panels hide on deactivation.
			NSWindow_setHidesOnDeactivate(window, W_FALSE);
			// Normally a panel doesn't become key unless something inside it needs to be first responder.
			// TOOL shells always become key, so disable that behavior.
			NSPanel_setBecomesKeyOnlyIfNeeded((NSPanel*) window, W_FALSE);
		}
		NSWindow_setReleasedWhenClosed(window, W_TRUE);
		if ((_W_WIDGET(widget)->style & W_NO_TRIM) == 0) {
			NSWindow_minSize(window, &frame.size);
			NSString *str = NSString_stringWithUTF8String("");
			frame.size.width = NSWindow_minFrameWidthWithTitle(str, styleMask);
			NSObject_release(NSOBJECT(str));
			NSWindow_setMinSize(window, &frame.size);
		}
		if (_w_shell_fix_resize(W_SHELL(widget)) > 0) {
			//if (window.respondsToSelector(OS.sel_setMovable_)) {
			NSWindow_setMovable(window, 0);
			//}
		}
		if (MAC_VERSION >= VERSION(10, 12, 0)) {
			/*
			 * In macOS 10.12, a new system preference "prefer tabs when opening documents"
			 * has been added which causes automatic tabbing of windows in Eclipse.
			 * Disable automatic window tabbing, but setting the NSWindow.allowsAutomaticWindowTabbing
			 * property to false.
			 */
			NSWindow_setAllowsAutomaticWindowTabbing(false);
		}
		//display.cascadeWindow(window, screen);
		NSRect screenFrame;
		NSScreen_frame(screen, &screenFrame);
		CGFloat width = screenFrame.width * 5 / 8, height = screenFrame.height
				* 5 / 8;
		NSWindow_frame(window, &frame);
		NSRect primaryFrame;
		NSScreen_frame(primaryScreen, &primaryFrame);
		frame.origin.y = primaryFrame.height
				- ((primaryFrame.height - (frame.y + frame.height)) + height);
		frame.width = width;
		frame.height = height;
		NSWindow_setFrame(window, &frame, W_FALSE);
		if ((_W_WIDGET(widget)->style & W_ON_TOP) != 0) {
			NSWindow_setLevel(window, NSStatusWindowLevel);
		}
		_w_composite_create_handle(widget, priv);
		NSView *topView = priv->top_view(widget);
		NSView_setHidden(topView, W_TRUE);
	} else {
		_W_WIDGET(widget)->state &= ~STATE_HIDDEN;

		if (_W_SHELL(widget)->window != 0) {
			// In the FOREIGN_HANDLE case, 'window' is an NSWindow created on our behalf.
			// It may already have a content view, so if it does, grab and retain, since we release()
			// the view at disposal time.  Otherwise, create a new 'view' that will be used as the window's
			// content view in setZOrder.
			_W_WIDGET(widget)->handle = NSWindow_contentView(
			_W_SHELL(widget)->window);

			if (_W_WIDGET(widget)->handle == 0) {
				_w_composite_create_handle(widget, priv);
			} else {
				NSObject_retain(NSOBJECT(_W_WIDGET(widget)->handle));
			}
		} else {
			// In the embedded case, 'view' is already set to the NSView we should add the window's content view to as a subview.
			// In that case we will hold on to the foreign view, create our own SWTCanvasView (which overwrites 'view') and then
			// add it to the foreign view.
			NSView *parentView = _W_WIDGET(widget)->handle;
			_w_composite_create_handle(widget, priv);
			NSView *topView = priv->top_view(widget);
			NSView_addSubview(parentView, topView);
		}

		_W_WIDGET(widget)->style |= W_NO_BACKGROUND;
	}

	if (_W_SHELL(widget)->window == 0) {
		NSView *view = priv->get_view(widget);
		NSWindow *hostWindow = NSView_window(view);
		//attachObserversToWindow(hostWindow);
	} else {
		NSObject *windowDelegate = SWTWindowDelegate_alloc(widget);
		int behavior;
		if (_W_CONTROL(widget)->parent != 0) {
			behavior = NSWindowCollectionBehaviorMoveToActiveSpace;
		} else if ((_W_WIDGET(widget)->style & W_TOOL) != 0) {
			behavior = NSWindowCollectionBehaviorFullScreenAuxiliary;
		} else {
			behavior = NSWindowCollectionBehaviorFullScreenPrimary;
		}
		NSWindow_setCollectionBehavior(window, behavior);
		NSWindow_setAcceptsMouseMovedEvents(window, W_TRUE);
		NSWindow_setDelegate(window, windowDelegate);
	}
	return W_TRUE;
}
void _w_shell_bring_totop(w_shell *shell, int force) {
	//if (getMinimized ()) return;
	if (force) {
		_w_shell_force_active(shell);
	} else {
		_w_shell_set_active(shell);
	}
}
void _w_shell_center(w_shell *shell) {
	if (_W_CONTROL(shell)->parent == 0)
		return;
}
wresult _w_shell_get_shell(w_control *control, w_shell **shell) {
	*shell = W_SHELL(control);
	return W_TRUE;
}
w_composite* _w_shell_find_deferred_control(w_composite *composite,
		_w_composite_priv *prriv) {
	return _W_COMPOSITE(composite)->layoutCount > 0 ? composite : 0;
}
wresult _w_shell_get_bounds(w_control *control, w_point *location,
		w_size *size) {
	NSRect frame;
	int y;
	if (_W_SHELL(control)->window != 0) {
		if (location != 0) {
			NSArray *screens = NSScreen_screens();
			if (screens != 0) {
				NSScreen *screen = (NSScreen*) NSArray_objectAtIndex(screens,
						0);
				NSScreen_frame(screen, &frame);
				y = frame.height;
			} else
				y = 0;
			NSWindow_frame(_W_SHELL(control)->window, &frame);
			y = y - (frame.y + frame.height);
			location->x = frame.x;
			location->y = y;
		} else {
			NSWindow_frame(_W_SHELL(control)->window, &frame);
		}
		if (size != 0) {
			size->width = frame.width;
			size->height = frame.height;
		}
	} else {
		_w_control_priv *priv = _W_CONTROL_GET_PRIV(control);
		NSView *view = priv->get_view(W_WIDGET(control));
		if (location != 0) {
			location->x = 0;
			location->y = 0;
			NSArray *screens = NSScreen_screens();
			if (screens != 0) {
				NSScreen *screen = (NSScreen*) NSArray_objectAtIndex(screens,
						0);
				NSScreen_frame(screen, &frame);
				y = frame.height;
			} else
				y = 0;
			if (!NSView_isFlipped(view)) {
				NSView_bounds(view, &frame);
				location->y = frame.height - location->y;
			}
			NSPoint _loc;
			_loc.x = location->x;
			_loc.y = location->y;
			NSView_convertPoint_toView_(view, &frame.origin, &_loc, 0);
			NSWindow *_window = NSView_window(view);
			NSWindow_convertBaseToScreen(_window, &_loc, &frame.origin);
			location->x = _loc.x;
			location->y = _loc.y;
			location->y = y - location->y;
		}
		if (size != 0) {
			NSView_frame(view, &frame);
			size->width = frame.width;
			size->height = frame.height;
		}
	}
	return W_TRUE;
}
wresult _w_shell_get_client_area(w_widget *widget, w_event_client_area *e,
		_w_control_priv *priv) {
	NSRect frame;
	if (_W_SHELL(widget)->window != 0) {
		if (!_w_shell_fix_resize(W_SHELL(widget))) {
			NSView *contentView = NSWindow_contentView(
			_W_SHELL(widget)->window);
			NSView_frame(contentView, &frame);
		} else {
			NSWindow_frame(_W_SHELL(widget)->window, &frame);
		}
	} else {
		NSView *topView = priv->top_view(widget);
		NSView_frame(topView, &frame);
	}
	e->rect->x = 0;
	e->rect->y = 0;
	e->rect->width = frame.width;
	e->rect->height = frame.height;
	NSView *scrollView = _W_SCROLLABLE_PRIV(priv)->scroll_view(widget);
	if (scrollView != 0) {
		wuint64 style = _W_WIDGET(widget)->style;
		NSScrollView_contentSizeForFrameSize(NSSCROLLVIEW(scrollView),
				&e->rect->sz, (style & W_HSCROLL) != 0,
				(style & W_VSCROLL) != 0, NSNoBorder);
	}
	return W_TRUE;
}
wresult _w_shell_close_widget(w_shell *shell, _w_event_platform *e) {
	wuint64 style = w_widget_get_style(W_WIDGET(shell));
	w_event event;
	event.type = W_EVENT_CLOSE;
	event.platform_event = _EVENT_PLATFORM(e);
	event.widget = W_WIDGET(shell);
	event.data = 0;
	_w_widget_send_event(W_WIDGET(shell), &event);
	if (style & W_DISPOSE_ON_CLOSE) {
		w_widget_dispose(W_WIDGET(shell));
	}
	return W_TRUE;
}
wresult _w_shell_close(w_shell *shell) {
	_w_shell_close_widget(shell, 0);
	return W_TRUE;
}
w_cursor* _w_shell_find_cursor(w_control *control, _w_control_priv *priv) {
	return _W_CONTROL(control)->cursor;
}
wresult _w_shell_force_active(w_shell *shell) {
	//if (_w_shell_is_visible(W_CONTROL(shell))) return W_TRUE;
	if (_W_SHELL(shell)->window == 0)
		return W_TRUE;
	_w_shell_make_key_and_order_front(shell);
	NSApplication *application = NSApplication_sharedApplication();
	NSApplication_activateIgnoringOtherApps(application, W_TRUE);
	return W_TRUE;
}
wresult _w_shell_get_default_button(w_shell *shell, w_button **button) {
	return W_FALSE;
}
wresult _w_shell_get_alpha(w_shell *shell) {
	return W_FALSE;
}
wresult _w_shell_get_full_screen(w_shell *shell) {
	return W_FALSE;
}
wresult _w_shell_get_minimum_size(w_shell *shell, w_size *size) {
	return W_FALSE;
}
wresult _w_shell_get_modified(w_shell *shell) {
	return W_FALSE;
}
wresult _w_shell_get_images(w_shell *shell, w_image *image, size_t length) {
	return W_FALSE;
}
wresult _w_shell_get_ime_input_mode(w_shell *shell) {
	return W_FALSE;
}
wresult _w_shell_get_maximized(w_shell *shell) {
	return W_FALSE;
}
wresult _w_shell_get_menu_bar(w_shell *shell, w_menu **menu) {
	return W_FALSE;
}
wresult _w_shell_get_minimized(w_shell *shell) {
	return W_FALSE;
}
wresult _w_shell_get_shells(w_shell *shell, w_iterator *iterator) {
	return W_FALSE;
}
wresult _w_shell_get_text(w_shell *shell, w_alloc alloc, void *user_data,
		int enc) {
	return W_FALSE;
}
wresult _w_shell_get_toolbar(w_shell *shell, w_toolbar **toolbar) {
	return W_FALSE;
}
void _w_shell_make_key_and_order_front(w_shell *shell) {
	/*
	 * Bug in Cocoa.  If a child window becomes the key window when its
	 * parent window is miniaturized, the parent window appears as if
	 * restored to its full size without actually being restored. In this
	 * case the parent window does become active when its child is closed
	 * and the user is forced to restore the window from the dock.
	 * The fix is to be sure that the parent window is deminiaturized before
	 * making the child a key window.
	 */
	if (_W_CONTROL(shell)->parent != 0) {
		NSWindow *parentWindow = _w_shell_parent_window(shell);
		if (parentWindow != 0) {
			if (NSWindow_isMiniaturized(parentWindow)) {
				NSWindow_deminiaturize(parentWindow, 0);
			}
		}
	}
	NSWindow_makeKeyAndOrderFront(_W_SHELL(shell)->window, 0);
}
wresult _w_shell_set_window_visible(w_shell *shell, int visible, int key,
		_w_control_priv *priv) {
	w_event e;
	if (visible) {
		if ((_W_WIDGET(shell)->state & STATE_HIDDEN) == 0)
			return W_TRUE;
		_W_WIDGET(shell)->state &= ~STATE_HIDDEN;
	} else {
		if ((_W_WIDGET(shell)->state & STATE_HIDDEN) != 0)
			return W_TRUE;
		_W_WIDGET(shell)->state |= STATE_HIDDEN;
	}
	NSWindow *window = _W_SHELL(shell)->window;
	if (window != 0 && (NSWindow_isVisible(window) == visible))
		return W_TRUE;
	if (visible) {
		//display.clearPool ();
		if (_W_SHELL(shell)->center && !_W_SHELL(shell)->moved) {
			if (w_widget_is_ok(W_WIDGET(shell)) <= 0)
				return W_FALSE;
			_w_shell_center(shell);
		}
		e.type = W_EVENT_SHOW;
		e.platform_event = 0;
		e.time = 0;
		e.widget = W_WIDGET(shell);
		e.data = 0;
		_w_widget_send_event(W_WIDGET(shell), &e);
		if (w_widget_is_ok(W_WIDGET(shell)) <= 0)
			return W_FALSE;
		NSView *topView = priv->top_view(W_WIDGET(shell));
		NSView_setHidden(topView, W_FALSE);
		//invalidateVisibleRegion();
		if (window != 0) {
			NSWindow *parentWindow = _w_shell_parent_window(shell);
			if ((_W_WIDGET(shell)->style & (W_SHEET)) != 0) {
				NSApplication *application = NSApplication_sharedApplication();
				NSApplication_beginSheet(application, parentWindow, 0, 0, 0, 0);
			} else {
				// If the parent window is miniaturized, the window will be shown
				// when its parent is shown.
				int parentMinimized = _W_CONTROL(shell)->parent != 0
						&& NSWindow_isMiniaturized(parentWindow);
				if (!parentMinimized) {
					if (key && (_W_WIDGET(shell)->style & W_NO_FOCUS) == 0) {
						_w_shell_make_key_and_order_front(shell);
					} else {
						NSWindow_orderFront(window, 0);
					}
					if (w_widget_is_ok(W_WIDGET(shell)) <= 0)
						return W_TRUE;
					if (_W_SHELL(shell)->minimized
							!= NSWindow_isMiniaturized(window)) {
						if (_W_SHELL(shell)->minimized) {
							NSWindow_miniaturize(window, 0);
						} else {
							NSWindow_deminiaturize(window, 0);
						}
					}
				}
			}
		}
		if (w_widget_is_ok(W_WIDGET(shell)) <= 0)
			return W_FALSE;
		//updateParent (visible);
		_W_SHELL(shell)->opened = true;
		if (!_W_SHELL(shell)->moved) {
			_W_SHELL(shell)->moved = true;
			e.type = W_EVENT_MOVE;
			e.platform_event = 0;
			e.time = 0;
			e.widget = W_WIDGET(shell);
			e.data = 0;
			_w_widget_send_event(W_WIDGET(shell), &e);
			if (w_widget_is_ok(W_WIDGET(shell)) <= 0)
				return W_FALSE;
		}
		if (!_W_SHELL(shell)->resized) {
			_W_SHELL(shell)->resized = true;
			e.type = W_EVENT_RESIZE;
			e.platform_event = 0;
			e.time = 0;
			e.widget = W_WIDGET(shell);
			e.data = 0;
			_w_widget_send_event(W_WIDGET(shell), &e);
			if (w_widget_is_ok(W_WIDGET(shell)) <= 0)
				return W_FALSE;
			//if (layout != null) {
			priv->mark_layout(W_CONTROL(shell), 0, priv);
			priv->update_layout(W_CONTROL(shell), 0, priv);
			//}
		}
	} else {
		e.type = W_EVENT_HIDE;
		e.platform_event = 0;
		e.time = 0;
		e.widget = W_WIDGET(shell);
		e.data = 0;
		_w_widget_send_event(W_WIDGET(shell), &e);
	}
	return W_TRUE;
}
wresult _w_shell_open(w_shell *shell) {
	_w_control_priv *priv = _W_CONTROL_GET_PRIV(shell);
	//_w_shell_bring_totop(shell, W_FALSE);
	_w_shell_set_window_visible(shell, W_TRUE, W_TRUE, priv);
	return W_TRUE;
}
wresult _w_shell_set_active(w_shell *shell) {
	if (_W_SHELL(shell)->window == 0)
		return W_TRUE;
	//if (!isVisible()) return;
	_w_shell_make_key_and_order_front(shell);
	return W_TRUE;
}
wresult _w_shell_set_alpha(w_shell *shell, int alpha) {
	return W_FALSE;
}
wresult _w_shell_set_bounds(w_control *shell, w_point *location, w_size *size) {
	if (_W_SHELL(shell)->window == 0) {
		// Embedded shells aren't movable.
		if (location != 0)
			return W_TRUE;
		if (size) {
			NSSize _sz;
			_sz.width = size->width;
			_sz.height = size->height;
			NSView_setFrameSize(_W_WIDGET(shell)->handle, &_sz);
			return W_TRUE;
		}
	}
	if (_w_shell_get_full_screen(W_SHELL(shell)) > 0) {
		//_w_shell_set_full_screen(W_SHELL(shell), W_FALSE);
	}
	int sheet = NSWindow_isSheet(_W_SHELL(shell)->window);
	if (sheet && location != 0 && size == 0)
		return W_TRUE;
	NSRect frame, screenframe;
	NSArray *screens = NSScreen_screens();
	NSScreen *screen = (NSScreen*) NSArray_objectAtIndex(screens, 0);
	NSScreen_frame(screen, &screenframe);
	CGFloat screenHeight = screenframe.height;
	NSWindow_frame(_W_SHELL(shell)->window, &frame);
	CGFloat x, y, width, height;
	if (location == 0) {
		x = frame.x;
		y = screenHeight - (frame.y + frame.height);
	} else {
		x = location->x;
		y = location->y;
	}
	if (size != 0) {
		NSSize minSize;
		NSWindow_minSize(_W_SHELL(shell)->window, &minSize);
		width = WMAX(size->width, minSize.width);
		height = WMAX(size->height, minSize.height);
	} else {
		width = frame.width;
		height = frame.height;
	}
	if (sheet) {
		y = screenHeight - (frame.y + frame.height);
		NSRect parentRect;
		w_shell *__shell;
		w_control_get_shell((w_control*) _W_CONTROL(shell)->parent, &__shell);
		NSWindow_frame(_W_SHELL(__shell)->window, &parentRect);
		frame.width = width;
		frame.height = height;
		frame.x = parentRect.x + (parentRect.width - frame.width) / 2;
		frame.y = screenHeight - (int) (y + frame.height);
		NSWindow_setFrame_0(_W_SHELL(shell)->window, &frame,
				1/*_w_shell_is_visible(shell)*/, W_TRUE);
	} else {
		frame.x = x;
		frame.y = screenHeight - (int) (y + height);
		frame.width = width;
		frame.height = height;
		NSWindow_setFrame(_W_SHELL(shell)->window, &frame,
				1/*_w_shell_is_visible(shell)*/);
	}
	return W_TRUE;
}

wresult _w_shell_set_default_button(w_shell *shell, w_button *button) {
	return W_FALSE;
}
wresult _w_shell_set_full_screen(w_shell *shell, int fullScreen) {
	return W_FALSE;
}
wresult _w_shell_set_images(w_shell *shell, w_image *image, size_t length) {
	return W_FALSE;
}
wresult _w_shell_set_ime_input_mode(w_shell *shell, int mode) {
	return W_FALSE;
}
wresult _w_shell_set_maximized(w_shell *shell, int maximized) {
	return W_FALSE;
}
wresult _w_shell_set_menu_bar(w_shell *shell, w_menu *menu) {
	if (_W_SHELL(shell)->menubar == menu)
		return W_TRUE;
	if (menu != 0) {
		if (w_widget_is_ok(W_WIDGET(menu)) <= 0)
			return W_ERROR_INVALID_ARGUMENT;
		if ((_W_WIDGET(menu)->style & W_BAR) == 0)
			return W_ERROR_MENU_NOT_BAR;
		if (_W_MENU(menu)->parent != W_CONTROL(shell))
			return W_ERROR_INVALID_PARENT;
	}
	_W_SHELL(shell)->menubar = menu;
	w_shell *activeshell = _w_toolkit_get_active_shell(W_TOOLKIT(mac_toolkit));
	if (activeshell == shell) {
		_w_toolkit_set_menubar(menu);
	}
	return W_TRUE;
}
wresult _w_shell_set_minimized(w_shell *shell, int minimized) {
	return W_FALSE;
}
wresult _w_shell_set_minimum_size(w_shell *shell, w_size *size) {
	return W_FALSE;
}
wresult _w_shell_set_modified(w_shell *shell, int modified) {
	return W_FALSE;
}
wresult _w_shell_set_text(w_shell *shell, const char *string, size_t length,
		int enc) {
	return W_FALSE;
}
wresult _w_shell_set_zorder(w_widget *widget, _w_control_priv *priv) {
	/*NSScrollView* scrollView =(NSScrollView*) __w_scrollable_scroll_view(_W_WIDGET(control)->handle);
	 if (scrollView != 0){
	 NSScrollView_setDocumentView(scrollView, view);
	 }*/
	NSWindow *window = _W_SHELL(widget)->window;
	if (window == 0)
		return W_TRUE;
	NSWindow_setContentView(window, _W_WIDGET(widget)->handle);
	if (_w_shell_fix_resize(W_SHELL(widget)) > 0) {
		NSRect rect;
		NSWindow_frame(window, &rect);
		rect.x = rect.y = 0;
		NSView_setFrame(NSWindow_contentView(window), &rect);
	}
	return W_TRUE;
}
/*
 * messages
 */
wresult _w_shell_dispose(w_widget *widget) {
	NSObject *windowDelegate = 0;
	if (_W_SHELL(widget)->window != 0) {
		windowDelegate = NSWindow_getDelegate(_W_SHELL(widget)->window);
		NSWindow_setDelegate(_W_SHELL(widget)->window, 0);
	}
	NSObject_release(NSOBJECT(_W_SHELL(widget)->window));
	//removeObserversFromWindow();
	if (windowDelegate != 0) {
		NSObject_release(windowDelegate);
	}

	//super.releaseHandle ();
	//window = null;
	_w_toolkit_remove_shell(_W_SHELL(widget));
	if (mac_toolkit->shells == 0) {
		w_toolkit_post_quit(W_TOOLKIT(mac_toolkit), EXIT_SUCCESS);
	}
	return W_TRUE;
}
wresult _ns_shell_viewWillMoveToWindow(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _ns_shell_windowDidBecomeKey(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	if (_W_SHELL(widget)->window != 0) {
		_w_toolkit_set_menubar(_W_SHELL(widget)->menubar);
	}
	/*sendEvent (SWT.Activate);
	if (isDisposed ()) return;
	if (!restoreFocus () && !traverseGroup (true)) setFocus ();
	if (isDisposed ()) return;
	if ((window.collectionBehavior() & OS.NSWindowCollectionBehaviorFullScreenPrimary) == 0) {
		Shell parentShell = this;
		while (parentShell.parent != null) {
			parentShell = (Shell) parentShell.parent;
			if (parentShell._getFullScreen ()) {
				break;
			}
		}
		if (!parentShell._getFullScreen () || menuBar != null) {
			updateSystemUIMode ();
		} else {
			parentShell.updateSystemUIMode ();
		}
	}*/
	return W_FALSE;
}
wresult _ns_shell_windowDidDeminiturize(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _ns_shell_windowDidMiniturize(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _ns_shell_windowDidMove(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _ns_shell_windowDidResize(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	_W_SHELL(widget)->resized = W_TRUE;
	w_event event;
	event.type = W_EVENT_RESIZE;
	event.time = 0;
	event.widget = widget;
	event.platform_event = (w_event_platform*) e;
	event.data = 0;
	_w_widget_send_event(widget, &event);
	if (!w_widget_is_ok(widget))
		return W_FALSE;
	//if (layout != null) {
	priv->mark_layout(W_CONTROL(widget), 0, priv);
	priv->update_layout(W_CONTROL(widget), 0, priv);
	return W_FALSE;
}
wresult _ns_shell_windowDidResignKey(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _ns_shell_windowSendEvent(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _ns_shell_windowShouldClose(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	//if (_w_control_is_enabled(W_CONTROL(widget))>0){
	_w_shell_close_widget(W_SHELL(widget), e);
	//}
	e->result._id = 0;
	return W_TRUE;
}
wresult _ns_shell_windowWillClose(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return _w_shell_close_widget(W_SHELL(widget), e);
}
void _w_shell_class_init(struct _w_shell_class *clazz) {
	_w_canvas_class_init(W_CANVAS_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_SHELL;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_shell_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_shell);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_shell);
	/*
	 * functions
	 */
	W_WIDGET_CLASS(clazz)->create = _w_shell_create;
	W_WIDGET_CLASS(clazz)->dispose = _w_shell_dispose;
	W_CONTROL_CLASS(clazz)->get_shell = _w_shell_get_shell;
	W_CONTROL_CLASS(clazz)->get_bounds = _w_shell_get_bounds;
	W_CONTROL_CLASS(clazz)->set_bounds = _w_shell_set_bounds;
	clazz->close = _w_shell_close;
	clazz->get_toolbar = _w_shell_get_toolbar;
	clazz->get_alpha = _w_shell_get_alpha;
	clazz->get_full_screen = _w_shell_get_full_screen;
	clazz->get_minimum_size = _w_shell_get_minimum_size;
	clazz->get_modified = _w_shell_get_modified;
	clazz->get_ime_input_mode = _w_shell_get_ime_input_mode;
	clazz->get_shells = _w_shell_get_shells;
	clazz->open = _w_shell_open;
	clazz->set_active = _w_shell_set_active;
	clazz->set_alpha = _w_shell_set_alpha;
	clazz->set_full_screen = _w_shell_set_full_screen;
	clazz->set_ime_input_mode = _w_shell_set_ime_input_mode;
	clazz->set_minimum_size = _w_shell_set_minimum_size;
	clazz->set_modified = _w_shell_set_modified;
	clazz->force_active = _w_shell_force_active;
	clazz->get_default_button = _w_shell_get_default_button;
	clazz->get_images = _w_shell_get_images;
	clazz->get_maximized = _w_shell_get_maximized;
	clazz->get_menu_bar = _w_shell_get_menu_bar;
	clazz->get_minimized = _w_shell_get_minimized;
	clazz->get_text = _w_shell_get_text;
	clazz->set_default_button = _w_shell_set_default_button;
	clazz->set_images = _w_shell_set_images;
	clazz->set_maximized = _w_shell_set_maximized;
	clazz->set_menu_bar = _w_shell_set_menu_bar;
	clazz->set_minimized = _w_shell_set_minimized;
	clazz->set_text = _w_shell_set_text;
	/*
	 * private
	 */
	_w_control_priv *priv = _W_CONTROL_PRIV(W_WIDGET_CLASS(clazz)->reserved[0]);
	_W_WIDGET_PRIV(priv)->create_handle = _w_shell_create_handle;
	_W_WIDGET_PRIV(priv)->get_client_area = _w_shell_get_client_area;
	priv->set_zorder = _w_shell_set_zorder;
	priv->find_cursor = _w_shell_find_cursor;
	_W_COMPOSITE_PRIV(priv)->find_deferred_control =
			_w_shell_find_deferred_control;
	/*
	 * messages
	 */
	dispatch_message *msgs = _W_WIDGET_PRIV(priv)->msgs;
	msgs[_NS_WILL_MOVE_TO_WINDOW] = _ns_shell_viewWillMoveToWindow;
	msgs[_NS_windowDidBecomeKey] = _ns_shell_windowDidBecomeKey;
	msgs[_NS_WINDOW_DID_DEMINITURIZE] = _ns_shell_windowDidDeminiturize;
	msgs[_NS_WINDOW_DID_MINITURIZE] = _ns_shell_windowDidMiniturize;
	msgs[_NS_WINDOW_DID_MOVE] = _ns_shell_windowDidMove;
	msgs[_NS_windowDidResize] = _ns_shell_windowDidResize;
	msgs[_NS_WINDOW_DID_RESIGN_KEY] = _ns_shell_windowDidResignKey;
	msgs[_NS_WINDOW_SEND_EVENT] = _ns_shell_windowSendEvent;
	msgs[_NS_windowShouldClose] = _ns_shell_windowShouldClose;
	msgs[_NS_windowWillClose] = _ns_shell_windowWillClose;
}
