/*
 * Name:        toolkit1.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "toolkit.h"
#include <pthread.h>
void* _w_toolkit_malloc(size_t size) {
	if ((mac_toolkit->tmp_length + size) < mac_toolkit->tmp_alloc) {
		int i = mac_toolkit->tmp_length;
		mac_toolkit->tmp_length += size;
		return &mac_toolkit->tmp[i];
	} else {
		return malloc(size);
	}
}
void* _w_toolkit_malloc_all(size_t *size) {
	*size = mac_toolkit->tmp_alloc - mac_toolkit->tmp_length;
	int i = mac_toolkit->tmp_length;
	mac_toolkit->tmp_length += *size;
	return &mac_toolkit->tmp[i];
}
void _w_toolkit_free(void *ptr, size_t size) {
	wintptr diff = ptr - (void*) mac_toolkit->tmp;
	if (diff >= 0 && diff < mac_toolkit->tmp_alloc) {
		mac_toolkit->tmp_length -= size;
	} else
		free(ptr);
}
void _w_toolkit_add_shell(_w_shell *shell) {
	shell->next = 0;
	if (mac_toolkit->shells == 0) {
		mac_toolkit->shells = shell;
		shell->prev = shell; //last
	} else {
		_w_shell *last = mac_toolkit->shells->prev;
		last->next = shell;
		shell->prev = last;
		mac_toolkit->shells->prev = shell;
	}
	mac_toolkit->shells_count++;
}
void _w_toolkit_remove_shell(_w_shell *shell) {
	if (shell == mac_toolkit->shells) {
		mac_toolkit->shells = shell->next;
		if (mac_toolkit->shells != 0)
			mac_toolkit->shells->prev = shell->prev; //last
	} else {
		if (shell->next == 0) {
			mac_toolkit->shells->prev = shell->prev;
		} else {
			shell->next->prev = shell->prev;
		}
		shell->prev->next = shell->next;
	}
	mac_toolkit->shells_count--;
}
void _w_toolkit_registre_class(int _id, Class clazz, ns_get_widget funct) {
	if (mac_toolkit->nsclasses[_id] == 0) {
		mac_toolkit->nsclasses[_id] = clazz;
		mac_toolkit->_ns_get_widget[_id] = funct;
	}
}
IMP oldCursorSetProc;
void* _w_toolkit_oldcursor_set_proc_call(void *id, void *sel) {
	return ((void* (*)(void*, void*)) oldCursorSetProc)(id, sel);
}
void* _w_toolkit_cursor_set_proc(NSObject *id, SEL sel) {
	if (mac_toolkit->lockCursor) {
		w_control *currentControl = mac_toolkit->currentControl;
		if (w_widget_is_ok(W_WIDGET(currentControl)) > 0) {
			_w_control_priv *priv = _W_CONTROL_GET_PRIV(currentControl);
			w_cursor *cursor = priv->find_cursor(W_WIDGET(currentControl),
					priv);
			if (cursor != 0 && _W_CURSOR(cursor)->handle != id)
				return 0;
		}
	}
	return _w_toolkit_oldcursor_set_proc_call(id, sel);
}
void NSCursor_init() {
	Class cls = objc_lookUpClass("NSCursor");
	SEL sel = sel_getUid("set");
	Method method = class_getInstanceMethod(cls, sel);
	if (method != 0)
		oldCursorSetProc = method_setImplementation(method,
				(IMP) _w_toolkit_cursor_set_proc);
}
void _w_toolkit_set_cursor(w_control *control) {
	w_cursor *cursor = 0;
	if (w_widget_is_ok(W_WIDGET(control)) > 0) {
		_w_control_priv *priv = _W_CONTROL_GET_PRIV(control);
		cursor = priv->find_cursor(control, priv);
	}
	if (cursor == 0) {
		NSWindow *window = NSApplication_keyWindow(mac_toolkit->application);
		if (window != 0) {
			if (NSWindow_areCursorRectsEnabled(window)) {
				NSWindow_disableCursorRects(window);
				NSWindow_enableCursorRects(window);
			}
			return;
		}
		cursor = _w_toolkit_get_system_cursor(W_TOOLKIT(mac_toolkit),
				W_CURSOR_ARROW);
	}
	mac_toolkit->lockCursor = W_FALSE;
	NSCursor_set(_W_CURSOR(cursor)->handle);
	mac_toolkit->lockCursor = W_TRUE;
}
void _w_toolkit_widget_class_init(_w_toolkit *toolkit) {
	/*
	 * shell
	 */
	W_WIDGET_CLASS(&toolkit->class_shell)->init_class =
			(w_widget_init_class) _w_shell_class_init;
	W_WIDGET_CLASS(&toolkit->class_shell)->reserved[0] =
			&toolkit->class_shell_priv;
	toolkit->classes[_W_CLASS_SHELL] = W_WIDGET_CLASS(&toolkit->class_shell);
	/*
	 * canvas
	 */
	W_WIDGET_CLASS(&toolkit->class_canvas)->init_class =
			(w_widget_init_class) _w_canvas_class_init;
	W_WIDGET_CLASS(&toolkit->class_canvas)->reserved[0] =
			&toolkit->class_canvas_priv;
	toolkit->classes[_W_CLASS_CANVAS] = W_WIDGET_CLASS(&toolkit->class_canvas);
	/*
	 * composite
	 */
	W_WIDGET_CLASS(&toolkit->class_composite)->init_class =
			(w_widget_init_class) _w_composite_class_init;
	W_WIDGET_CLASS(&toolkit->class_composite)->reserved[0] =
			&toolkit->class_composite_priv;
	toolkit->classes[_W_CLASS_COMPOSITE] = W_WIDGET_CLASS(
			&toolkit->class_composite);
	/*
	 * menu
	 */
	W_WIDGET_CLASS(&toolkit->class_menu)->init_class =
			(w_widget_init_class) _w_menu_class_init;
	W_WIDGET_CLASS(&toolkit->class_menu)->reserved[0] =
			&toolkit->class_menu_priv;
	W_MENU_CLASS(&toolkit->class_menu)->class_menuitem =
			&toolkit->class_menuitem;
	toolkit->classes[_W_CLASS_MENU] = W_WIDGET_CLASS(&toolkit->class_menu);
	/*
	 * treeview
	 */
	W_WIDGET_CLASS(&toolkit->class_treeview)->init_class =
			(w_widget_init_class) _w_treeview_class_init;
	W_WIDGET_CLASS(&toolkit->class_treeview)->reserved[0] =
			&toolkit->class_treeview_priv;
	W_LISTVIEWBASE_CLASS(&toolkit->class_treeview)->class_item = W_ITEM(
			&toolkit->class_treeitem);
	W_LISTVIEWBASE_CLASS(&toolkit->class_treeview)->class_column =
			&toolkit->class_treecolumn;
	toolkit->classes[_W_CLASS_TREEVIEW] = W_WIDGET_CLASS(
			&toolkit->class_treeview);
}
void _w_toolkit_init_thread(_w_toolkit *toolkit) {
	toolkit->thread.id = (wintptr) pthread_self();
}
void _w_toolkit_init(_w_toolkit *toolkit) {
	_w_toolkit_class_init(toolkit);
	_w_toolkit_widget_class_init(toolkit);
	_w_theme_init();
	_W_TOOLKIT(toolkit)->pool = NSAutoreleasePool_New();
	_W_TOOLKIT(toolkit)->application = NSApplication_sharedApplication();
	toolkit->isPainting = NSMutableArray_initWithCapacity(12);
	toolkit->SYNTHETIC_BOLD = NSNumber_numberWithDouble(-2.5);
	toolkit->SYNTHETIC_ITALIC = NSNumber_numberWithDouble(0.2);
	CGFloat systemFontSize = NSFont_systemFontSize();
	NSFont *font = NSFont_systemFontOfSize(systemFontSize);
	NSObject_retain(NSOBJECT(font));
	toolkit->systemFont.handle = font;
	NSApp_Init();
	NSCursor_init();
}
void _w_toolkit_dispose(w_disposable *disposable) {
	_w_toolkit *toolkit = _W_TOOLKIT(disposable);
	NSObject_release(NSOBJECT(toolkit->isPainting));
}
