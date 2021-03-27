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
			w_cursor *cursor = priv->find_cursor(currentControl, priv);
			if (cursor != 0 && _W_CURSOR(cursor)->handle != (NSCursor*) id)
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
NSMenuItem* _w_toolkit_create_item(char *title, char *action, char *key) {
	NSString *_title = NSString_stringWithUTF8String(title);
	return NSMenuItem_initWithTitle(_title, 0, mac_toolkit->emptyString);
}
void _w_toolkit_add_item(NSMenu *menu, char *title, char *action, char *key) {
	NSString *_title = NSString_stringWithUTF8String(title);
	NSMenu_addItemWithTitle(menu, _title, 0, mac_toolkit->emptyString);
}
id objc_msgSend(id self, SEL op, ...);
void _w_toolkit_create_mainmenu() {
	NSString *empty;
	NSMenu *mainMenu = NSMenu_initWithTitle(empty); // `title` really doesn't matter.
	NSMenuItem *mainAppMenuItem = _w_toolkit_create_item("Application", 0, ""); // `title` really doesn't matter.
	NSMenu_addItem(mainMenu, mainAppMenuItem);

	NSMenu *appMenu = NSMenu_initWithTitle(empty); // `title` really doesn't matter.
	NSMenuItem_setSubmenu(mainAppMenuItem, appMenu);

	SEL sel = sel_registerName("setAppleMenu:");

	objc_msgSend(NSApplication_sharedApplication(), sel, appMenu);

	NSMenu *appServicesMenu = NSMenu_initWithTitle(empty);

	NSApplication_setServicesMenu(mac_toolkit->application, appServicesMenu);
	_w_toolkit_add_item(appMenu, "About Me", 0, "");
	NSMenu_addItem(appMenu, NSMenuItem_separatorItem());
	_w_toolkit_add_item(appMenu, "Preferences...", 0, "");
	NSMenu_addItem(appMenu, NSMenuItem_separatorItem());
	_w_toolkit_add_item(appMenu, "Hide Me", 0, "");
	_w_toolkit_add_item(appMenu, "Hide Others", 0, "");
	_w_toolkit_add_item(appMenu, "Show All", 0, "");
	NSMenu_addItem(appMenu, NSMenuItem_separatorItem());
	_w_toolkit_add_item(appMenu, "Services", 0, "");
	_w_toolkit_add_item(appMenu, "Quit Me", 0, "");
	NSApplication_setMainMenu(NSApplication_sharedApplication(), mainMenu);
}
void _w_toolkit_set_menubar(w_menu *menu) {
	// If passed a null menu bar don't clear out the menu bar, but switch back to the
	// application menu bar instead, if it exists.  If the app menu bar is already active
	// we jump out without harming the current menu bar.
	if (menu == 0) {
		if (w_widget_is_ok(W_WIDGET(&mac_toolkit->appMenuBar)) > 0) {
			menu = &mac_toolkit->appMenuBar;
		}
	}
	/*if (menu == mac_toolkit->menuBar)
		return;*/
	mac_toolkit->menuBar = menu;
	//remove all existing menu items except the application menu
	NSMenu *menubar = NSApplication_mainMenu(mac_toolkit->application);
	/*
	 * For some reason, NSMenu.cancelTracking() does not dismisses
	 * the menu right away when the menu bar is set in a stacked
	 * event loop. The fix is to use CancelMenuTracking() instead.
	 */
//	menubar.cancelTracking();
	NSMenu_cancelTracking(menubar);
	NSInteger count = NSMenu_numberOfItems(menubar);
	while (count > 1) {
		NSMenu_removeItemAtIndex(menubar, count - 1);
		count--;
	}
	//set parent of each item to NULL and add them to menubar
	if (menu != 0) {
		NSMenu *hmenu = (NSMenu*) _W_WIDGET(menu)->handle;
		count = NSMenu_numberOfItems(hmenu);
		for (int i = 0; i < count; i++) {
			NSMenuItem *nsItem = NSMenu_itemAtIndex(hmenu, i);
			NSMenuItem_setMenu(nsItem, 0);
			NSMenu_addItem(menubar, nsItem);
		}
	}
}
wresult _w_toolkit_notify(int msg,void* obj){
	switch (msg) {
		case _NS_applicationDidBecomeActive:{
			NSWindow* keyWindow = NSApplication_keyWindow(mac_toolkit->application);
			if (keyWindow != 0) {
				NSWindow_orderFrontRegardless(keyWindow);
			} else {
				_w_toolkit_set_menubar(mac_toolkit->menuBar);
			}
			/*checkFocus();
			checkEnterExit(findControl(true), null, false);*/
		}
			break;
		default:
			break;
	}
	return W_FALSE;
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
	W_WIDGET_CLASS(&toolkit->class_tree)->init_class =
			(w_widget_init_class) _w_treeview_class_init;
	W_WIDGET_CLASS(&toolkit->class_tree)->reserved[0] =
			&toolkit->class_tree_priv;
	W_LISTVIEWBASE_CLASS(&toolkit->class_tree)->class_item = W_ITEM_CLASS(
			&toolkit->class_treeitem);
	W_LISTVIEWBASE_CLASS(&toolkit->class_tree)->class_column =
			&toolkit->class_treecolumn;
	toolkit->classes[_W_CLASS_TREEVIEW] = W_WIDGET_CLASS(&toolkit->class_tree);
	/*
	 * listview
	 */
	W_WIDGET_CLASS(&toolkit->class_listview)->init_class =
			(w_widget_init_class) _w_listview_class_init;
	W_WIDGET_CLASS(&toolkit->class_listview)->reserved[0] =
			&toolkit->class_listview_priv;
	W_LISTVIEWBASE_CLASS(&toolkit->class_listview)->class_item = W_ITEM_CLASS(
			&toolkit->class_listitem);
	W_LISTVIEWBASE_CLASS(&toolkit->class_listview)->class_column =
			&toolkit->class_listcolumn;
	toolkit->classes[_W_CLASS_LISTVIEW] = W_WIDGET_CLASS(
			&toolkit->class_listview);
	/*
	 * sash
	 */
	W_WIDGET_CLASS(&toolkit->class_sash)->init_class =
			(w_widget_init_class) _w_sash_class_init;
	W_WIDGET_CLASS(&toolkit->class_sash)->reserved[0] =
			&toolkit->class_sash_priv;
	toolkit->classes[_W_CLASS_SASH] = W_WIDGET_CLASS(&toolkit->class_sash);
	/*
	 * button
	 */
	W_WIDGET_CLASS(&toolkit->class_button)->init_class =
			(w_widget_init_class) _w_button_class_init;
	W_WIDGET_CLASS(&toolkit->class_button)->reserved[0] =
			&toolkit->class_button_priv;
	toolkit->classes[_W_CLASS_BUTTON] = W_WIDGET_CLASS(&toolkit->class_button);
	/*
	 * label
	 */
	W_WIDGET_CLASS(&toolkit->class_label)->init_class =
			(w_widget_init_class) _w_label_class_init;
	W_WIDGET_CLASS(&toolkit->class_label)->reserved[0] =
			&toolkit->class_label_priv;
	toolkit->classes[_W_CLASS_LABEL] = W_WIDGET_CLASS(&toolkit->class_label);
	/*
	 * textedit
	 */
	W_WIDGET_CLASS(&toolkit->class_textedit)->init_class =
			(w_widget_init_class) _w_textedit_class_init;
	W_WIDGET_CLASS(&toolkit->class_textedit)->reserved[0] =
			&toolkit->class_textedit_priv;
	toolkit->classes[_W_CLASS_TEXTEDIT] = W_WIDGET_CLASS(
			&toolkit->class_textedit);
	/*
	 * progressbar
	 */
	W_WIDGET_CLASS(&toolkit->class_progressbar)->init_class =
			(w_widget_init_class) _w_progressbar_class_init;
	W_WIDGET_CLASS(&toolkit->class_progressbar)->reserved[0] =
			&toolkit->class_progressbar_priv;
	toolkit->classes[_W_CLASS_PROGRESSBAR] = W_WIDGET_CLASS(
			&toolkit->class_progressbar);
	/*
	 * groupbox
	 */
	W_WIDGET_CLASS(&toolkit->class_groupbox)->init_class =
			(w_widget_init_class) _w_groupbox_class_init;
	W_WIDGET_CLASS(&toolkit->class_groupbox)->reserved[0] =
			&toolkit->class_groupbox_priv;
	toolkit->classes[_W_CLASS_GROUPBOX] = W_WIDGET_CLASS(
			&toolkit->class_groupbox);
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
	toolkit->emptyString = NSString_stringWithUTF8String("");
	NSObject_retain(NSOBJECT(toolkit->emptyString));
	_w_toolkit_create_mainmenu();
}
void _w_toolkit_dispose(w_disposable *disposable) {
	_w_toolkit *toolkit = _W_TOOLKIT(disposable);
	NSObject_release(NSOBJECT(toolkit->isPainting));
}
