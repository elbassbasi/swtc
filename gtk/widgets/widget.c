/*
 * Name:        widget.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "widget.h"
#include "toolkit.h"
#if defined(__GNUC__) || defined(__GNUG__)
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
GdkWindow* _gdk_window_get_device_position(GdkWindow *window, gint *x, gint *y,
		GdkModifierType *mask) {
#if GTK3
	GdkDisplay *display = 0;
	if (window != 0) {
		display = gdk_window_get_display(window);
	} else {
		window = gdk_get_default_root_window();
		display = gdk_window_get_display(window);
	}
	GdkDeviceManager *device_manager = gdk_display_get_device_manager(display);
	GdkDevice *pointer = gdk_device_manager_get_client_pointer(device_manager);
	return gdk_window_get_device_position(window, pointer, x, y, mask);
#endif
#if GTK2
		return gdk_window_get_pointer(window, x, y, mask);
#endif
}
void _w_widget_set_control(void *handle, w_widget *widget) {
	g_object_set_qdata(G_OBJECT(handle), gtk_toolkit->quark[SWT_GQUARK_WIDGET],
			widget);
}
w_widget* _w_widget_find_control(void *handle) {
	return (w_widget*) g_object_get_qdata(G_OBJECT(handle),
			gtk_toolkit->quark[SWT_GQUARK_WIDGET]);
}
GtkWidget* _w_widget_h0(w_widget *widget, _w_control_priv *priv) {
	return 0;
}
GtkWidget* _w_widget_h(w_widget *widget, _w_control_priv *priv) {
	return _W_WIDGET(widget)->handle;
}
GtkWidget* _w_widget_hp(w_widget *widget, _w_control_priv *priv) {
	GtkWidget *handle = _W_WIDGET(widget)->handle;
	return gtk_widget_get_parent(handle);
}
GtkWidget* _w_widget_hpp(w_widget *widget, _w_control_priv *priv) {
	GtkWidget *handle = _W_WIDGET(widget)->handle;
	return gtk_widget_get_parent(gtk_widget_get_parent(handle));
}
GtkWidget* _w_widget_hppp(w_widget *widget, _w_control_priv *priv) {
	GtkWidget *handle = _W_WIDGET(widget)->handle;
	return gtk_widget_get_parent(
			gtk_widget_get_parent(gtk_widget_get_parent(handle)));
}
void _w_widget_check_open(w_widget *widget, _w_control_priv *priv) {

}
void _w_widget_check_orientation(w_widget *widget, w_widget *parent,
		_w_control_priv *priv) {
	_W_WIDGET(widget)->style &= ~W_MIRRORED;
	if ((_W_WIDGET(widget)->style & (W_LEFT_TO_RIGHT | W_RIGHT_TO_LEFT)) == 0) {
		if (_W_WIDGET(widget)->parent != 0) {
			if ((_W_WIDGET(_W_WIDGET(widget)->parent)->style & W_LEFT_TO_RIGHT)
					!= 0)
				_W_WIDGET(widget)->style |= W_LEFT_TO_RIGHT;
			if ((_W_WIDGET(_W_WIDGET(widget)->parent)->style & W_RIGHT_TO_LEFT)
					!= 0)
				_W_WIDGET(widget)->style |= W_RIGHT_TO_LEFT;
		}
	}
	_W_WIDGET(widget)->style = _w_widget_check_bits(_W_WIDGET(widget)->style,
			W_LEFT_TO_RIGHT, W_RIGHT_TO_LEFT, 0, 0, 0, 0);
}
wuint64 _w_widget_check_style(w_widget *control, wuint64 style) {
	return style;
}
wresult _w_widget_create_widget(w_widget *widget, _w_control_priv *priv) {
	wresult ret = _W_WIDGET_PRIV(priv)->create_handle(widget, priv);
	if (ret < 0)
		return ret;
	_W_WIDGET_PRIV(priv)->set_orientation(widget, W_TRUE, priv);
	_W_WIDGET_PRIV(priv)->hook_events(widget, priv);
	return ret;
}
wresult _w_widget_create_handle(w_widget *widget, _w_control_priv *priv) {
	return W_TRUE;
}
void _w_widget_set_orientation(w_widget *widget, int create,
		_w_control_priv *priv) {
}
void _w_widget_hook_events(w_widget *widget, _w_control_priv *priv) {
}
wuint64 _w_widget_check_bits(wuint64 style, int int0, int int1, int int2,
		int int3, int int4, int int5) {
	int mask = int0 | int1 | int2 | int3 | int4 | int5;
	if ((style & mask) == 0)
		style |= int0;
	if ((style & int0) != 0)
		style = (style & ~mask) | int0;
	if ((style & int1) != 0)
		style = (style & ~mask) | int1;
	if ((style & int2) != 0)
		style = (style & ~mask) | int2;
	if ((style & int3) != 0)
		style = (style & ~mask) | int3;
	if ((style & int4) != 0)
		style = (style & ~mask) | int4;
	if ((style & int5) != 0)
		style = (style & ~mask) | int5;
	return style;
}
wresult _w_widget_is_ok(w_widget *obj) {
	return W_TRUE;
}
wresult _w_widget_get_theme(w_widget *widget, w_theme **theme) {
	if (_W_WIDGET(widget)->theme != 0) {
		*theme = _W_WIDGET(widget)->theme;
		return W_TRUE;
	}
	w_toolkit *toolkit = w_widget_get_toolkit(widget);
	*theme = w_toolkit_get_theme(toolkit);
	return W_TRUE;
}
wresult _w_widget_set_theme(w_widget *widget, w_theme *theme) {
	_W_WIDGET(widget)->theme = theme;
	return W_TRUE;
}
int _w_widget_set_input_state(int state) {
	int statemask = 0;
	if ((state & GDK_MOD1_MASK) != 0)
		statemask |= W_ALT;
	if ((state & GDK_SHIFT_MASK) != 0)
		statemask |= W_SHIFT;
	if ((state & GDK_CONTROL_MASK) != 0)
		statemask |= W_CTRL;
	if ((state & GDK_BUTTON1_MASK) != 0)
		statemask |= W_BUTTON1;
	if ((state & GDK_BUTTON2_MASK) != 0)
		statemask |= W_BUTTON2;
	if ((state & GDK_BUTTON3_MASK) != 0)
		statemask |= W_BUTTON3;
	return statemask;
}
int KeyTable[][2] = {

/* Keyboard and Mouse Masks */
{ GDK_KEY_Alt_L, W_ALT }, //
		{ GDK_KEY_Alt_R, W_ALT }, //
		{ GDK_KEY_Meta_L, W_ALT }, //
		{ GDK_KEY_Meta_R, W_ALT }, //
		{ GDK_KEY_Shift_L, W_SHIFT }, //
		{ GDK_KEY_Shift_R, W_SHIFT }, //
		{ GDK_KEY_Control_L, W_CTRL }, //
		{ GDK_KEY_Control_R, W_CTRL }, //
		{ GDK_KEY_ISO_Level3_Shift, W_ALT }, //
//		{OS.GDK_KEY_????,		W_COMMAND},
//		{OS.GDK_KEY_????,		W_COMMAND},

		/* Non-Numeric Keypad Keys */
		{ GDK_KEY_Up, W_ARROW_UP }, //
		{ GDK_KEY_KP_Up, W_ARROW_UP }, //
		{ GDK_KEY_Down, W_ARROW_DOWN }, //
		{ GDK_KEY_KP_Down, W_ARROW_DOWN }, //
		{ GDK_KEY_Left, W_ARROW_LEFT }, //
		{ GDK_KEY_KP_Left, W_ARROW_LEFT }, //
		{ GDK_KEY_Right, W_ARROW_RIGHT }, //
		{ GDK_KEY_KP_Right, W_ARROW_RIGHT }, //
		{ GDK_KEY_Page_Up, W_PAGE_UP }, //
		{ GDK_KEY_KP_Page_Up, W_PAGE_UP }, //
		{ GDK_KEY_Page_Down, W_PAGE_DOWN }, //
		{ GDK_KEY_KP_Page_Down, W_PAGE_DOWN }, //
		{ GDK_KEY_Home, W_HOME }, //
		{ GDK_KEY_KP_Home, W_HOME }, //
		{ GDK_KEY_End, W_END }, //
		{ GDK_KEY_KP_End, W_END }, //
		{ GDK_KEY_Insert, W_INSERT }, //
		{ GDK_KEY_KP_Insert, W_INSERT }, //

		/* Virtual and Ascii Keys */
		{ GDK_KEY_BackSpace, W_BS }, //
		{ GDK_KEY_Return, W_CR }, //
		{ GDK_KEY_Delete, W_DEL }, //
		{ GDK_KEY_KP_Delete, W_DEL }, //
		{ GDK_KEY_Escape, W_ESC }, //
		{ GDK_KEY_Linefeed, W_LF }, //
		{ GDK_KEY_Tab, W_TAB }, //
		{ GDK_KEY_ISO_Left_Tab, W_TAB }, //

		/* Functions Keys */
		{ GDK_KEY_F1, W_F1 }, //
		{ GDK_KEY_F2, W_F2 }, //
		{ GDK_KEY_F3, W_F3 }, //
		{ GDK_KEY_F4, W_F4 }, //
		{ GDK_KEY_F5, W_F5 }, //
		{ GDK_KEY_F6, W_F6 }, //
		{ GDK_KEY_F7, W_F7 }, //
		{ GDK_KEY_F8, W_F8 }, //
		{ GDK_KEY_F9, W_F9 }, //
		{ GDK_KEY_F10, W_F10 }, //
		{ GDK_KEY_F11, W_F11 }, //
		{ GDK_KEY_F12, W_F12 }, //
		{ GDK_KEY_F13, W_F13 }, //
		{ GDK_KEY_F14, W_F14 }, //
		{ GDK_KEY_F15, W_F15 }, //
		{ GDK_KEY_F16, W_F16 }, //
		{ GDK_KEY_F17, W_F17 }, //
		{ GDK_KEY_F18, W_F18 }, //
		{ GDK_KEY_F19, W_F19 }, //
		{ GDK_KEY_F20, W_F20 }, //

		/* Numeric Keypad Keys */
		{ GDK_KEY_KP_Multiply, W_KEYPAD_MULTIPLY }, //
		{ GDK_KEY_KP_Add, W_KEYPAD_ADD }, //
		{ GDK_KEY_KP_Enter, W_KEYPAD_CR }, //
		{ GDK_KEY_KP_Subtract, W_KEYPAD_SUBTRACT }, //
		{ GDK_KEY_KP_Decimal, W_KEYPAD_DECIMAL }, //
		{ GDK_KEY_KP_Divide, W_KEYPAD_DIVIDE }, //
		{ GDK_KEY_KP_0, W_KEYPAD_0 }, //
		{ GDK_KEY_KP_1, W_KEYPAD_1 }, //
		{ GDK_KEY_KP_2, W_KEYPAD_2 }, //
		{ GDK_KEY_KP_3, W_KEYPAD_3 }, //
		{ GDK_KEY_KP_4, W_KEYPAD_4 }, //
		{ GDK_KEY_KP_5, W_KEYPAD_5 }, //
		{ GDK_KEY_KP_6, W_KEYPAD_6 }, //
		{ GDK_KEY_KP_7, W_KEYPAD_7 }, //
		{ GDK_KEY_KP_8, W_KEYPAD_8 }, //
		{ GDK_KEY_KP_9, W_KEYPAD_9 }, //
		{ GDK_KEY_KP_Equal, W_KEYPAD_EQUAL }, //

		/* Other keys */
		{ GDK_KEY_Caps_Lock, W_CAPS_LOCK }, //
		{ GDK_KEY_Num_Lock, W_NUM_LOCK }, //
		{ GDK_KEY_Scroll_Lock, W_SCROLL_LOCK }, //
		{ GDK_KEY_Pause, W_PAUSE }, //
		{ GDK_KEY_Break, W_BREAK }, //
		{ GDK_KEY_Print, W_PRINT_SCREEN }, //
		{ GDK_KEY_Help, W_HELP }, //

		};
int _w_translate_key(int key) {
	for (int i = 0; i < (int) (sizeof(KeyTable) / sizeof(KeyTable[0])); i++) {
		if (KeyTable[i][0] == key)
			return KeyTable[i][1];
	}
	return 0;
}

int _w_untranslate_key(int key) {
	for (int i = 0; i < (int) (sizeof(KeyTable) / sizeof(KeyTable[0])); i++) {
		if (KeyTable[i][1] == key)
			return KeyTable[i][0];
	}
	return 0;
}
void _w_widget_find_children(GtkWidget *widget, gpointer data) {
	_w_widget_find_child *i = (_w_widget_find_child*) data;
	if (i->index >= 0) {
		if (i->count == i->index) {
			i->widget = widget;
		}
		if (i->count == i->index - 1) {
			i->prev = widget;
		}
		if (i->count == i->index + 1) {
			i->next = widget;
		}
	} else {
		i->prev = i->widget;
		i->widget = widget;
	}
	i->count++;
}
void _w_widget_children_count(GtkWidget *widget, gpointer data) {
	((_w_widget_find_child*) data)->count++;
}
void _w_widget_get_handles_callback(GtkWidget *widget, gpointer data) {
	if (GTK_IS_BOX(widget)) {
		((_w_widget_handles*) data)->box = widget;
		return;
	}
	if (GTK_IS_IMAGE(widget)) {
		((_w_widget_handles*) data)->image = widget;
		return;
	}
	if (GTK_IS_LABEL(widget)) {
		((_w_widget_handles*) data)->label = widget;
		return;
	}
	if (GTK_IS_ARROW(widget)) {
		((_w_widget_handles*) data)->arrow = widget;
		return;
	}
}
void _w_widget_get_handles(GtkWidget *handle, _w_widget_handles *handles) {
	memset(handles, 0, sizeof(_w_widget_handles));
	handles->handle = handle;
	if (GTK_IS_CONTAINER(handles->handle)) {
		gtk_container_forall(GTK_CONTAINER(handles->handle),
				_w_widget_get_handles_callback, handles);
	}
	if (handles->box != 0) {
		gtk_container_forall(GTK_CONTAINER(handles->box),
				_w_widget_get_handles_callback, handles);
	}
}
void _w_widget_set_location_state(w_event_key *event, GdkEventKey *keyEvent) {
	switch (keyEvent->keyval) {
	case GDK_KEY_Alt_L:
	case GDK_KEY_Shift_L:
	case GDK_KEY_Control_L:
		event->keylocation = W_LEFT;
		break;
	case GDK_KEY_Alt_R:
	case GDK_KEY_Shift_R:
	case GDK_KEY_Control_R:
		event->keylocation = W_RIGHT;
		break;
	case GDK_KEY_KP_0:
	case GDK_KEY_KP_1:
	case GDK_KEY_KP_2:
	case GDK_KEY_KP_3:
	case GDK_KEY_KP_4:
	case GDK_KEY_KP_5:
	case GDK_KEY_KP_6:
	case GDK_KEY_KP_7:
	case GDK_KEY_KP_8:
	case GDK_KEY_KP_9:
	case GDK_KEY_KP_Add:
	case GDK_KEY_KP_Decimal:
	case GDK_KEY_KP_Delete:
	case GDK_KEY_KP_Divide:
	case GDK_KEY_KP_Down:
	case GDK_KEY_KP_End:
	case GDK_KEY_KP_Enter:
	case GDK_KEY_KP_Equal:
	case GDK_KEY_KP_Home:
	case GDK_KEY_KP_Insert:
	case GDK_KEY_KP_Left:
	case GDK_KEY_KP_Multiply:
	case GDK_KEY_KP_Page_Down:
	case GDK_KEY_KP_Page_Up:
	case GDK_KEY_KP_Right:
	case GDK_KEY_KP_Subtract:
	case GDK_KEY_KP_Up:
	case GDK_KEY_Num_Lock:
		event->keylocation = W_KEYPAD;
		break;
	}
}
gboolean _w_widget_set_key_state(w_event_key *event, GdkEventKey *keyEvent,
		int unicode_length) {
	if (keyEvent->string != 0 && unicode_length > 1)
		return FALSE;
	gboolean isNull = FALSE;
	event->keycode = _w_translate_key(keyEvent->keyval);
	switch (keyEvent->keyval) {
	case GDK_KEY_BackSpace:
		event->character = W_BS;
		break;
	case GDK_KEY_Linefeed:
		event->character = W_LF;
		break;
	case GDK_KEY_KP_Enter:
	case GDK_KEY_Return:
		event->character = W_CR;
		break;
	case GDK_KEY_KP_Delete:
	case GDK_KEY_Delete:
		event->character = W_DEL;
		break;
	case GDK_KEY_Escape:
		event->character = W_ESC;
		break;
	case GDK_KEY_Tab:
	case GDK_KEY_ISO_Left_Tab:
		event->character = W_TAB;
		break;
	default: {
		if (event->keycode == 0) {
			guint keyval = 0;
			gint effective_group, level;
			GdkModifierType consumed_modifiers;
			if (gdk_keymap_translate_keyboard_state(gdk_keymap_get_default(),
					keyEvent->hardware_keycode, (GdkModifierType) 0,
					gtk_toolkit->latinKeyGroup, &keyval, &effective_group,
					&level, &consumed_modifiers)) {
				event->keycode = (int) gdk_keyval_to_unicode(keyval);
			}
		}
		int key = keyEvent->keyval;
		if ((keyEvent->state & GDK_CONTROL_MASK) != 0
				&& (0 <= key && key <= 0x7F)) {
			if ('a' <= key && key <= 'z')
				key -= 'a' - 'A';
			if (64 <= key && key <= 95)
				key -= 64;
			event->character = key;
			isNull = keyEvent->keyval == '@' && key == 0;
		} else {
			if (keyEvent->string != 0) {
				event->character = g_utf8_get_char(keyEvent->string);
			} else {
				event->character = gdk_keyval_to_unicode(key);
			}
		}
	}
	}
	_w_widget_set_location_state(event, keyEvent);
	if (event->keycode == 0 && event->character == 0) {
		if (!isNull)
			return FALSE;
	}
	event->detail = _w_widget_set_input_state(keyEvent->state);
	return TRUE;
}

gboolean _w_widget_send_IM_key_event(w_widget *widget, _w_event_platform *e,
		int type, GdkEventKey *keyEvent, const char *chars, int length) {
	GdkEventKey *ptr = 0;
	int index = 0, count = 0;
	GdkModifierType state = 0;
	w_event_key event;
	if (keyEvent == 0) {
		keyEvent = (GdkEventKey*) gtk_get_current_event();
		ptr = keyEvent;
		if (keyEvent != 0) {
			switch (keyEvent->type) {
			case GDK_KEY_PRESS:
			case GDK_KEY_RELEASE:
				state = keyEvent->state;
				break;
			default:
				keyEvent = 0;
				break;
			}
		}
	}
	if (keyEvent == 0) {
		gtk_get_current_event_state(&state);
	}
	gunichar c;
	while (index < length) {
		c = g_utf8_get_char(&chars[index]);
		if (c == 0)
			break;
		event.event.type = type;
		event.event.time = keyEvent->time;
		event.event.platform_event = (w_event_platform*) e;
		event.event.widget = widget;
		event.event.time = keyEvent->time;
		if (keyEvent != 0 && keyEvent->length <= 1) {
			_w_widget_set_key_state(&event, keyEvent, length);
		} else {
			event.detail = _w_widget_set_input_state(state);
		}
		event.character = c;
		int doit = _w_widget_send_event(widget, (w_event*) &event,
				W_EVENT_SEND);

		/*
		 * It is possible (but unlikely), that application
		 * code could have disposed the widget in the key
		 * events.  If this happens, end the processing of
		 * the key by returning null.
		 */
		//if (event.doit) chars [count++] = chars [index];
		if (doit)
			count++;
		index += g_utf8_skip[*(const guchar*) (&chars[index])];
	}
	if (ptr != 0)
		gdk_event_free((GdkEvent*) ptr);
	if (count == 0)
		return FALSE;
	return TRUE;
}
gboolean _w_widget_send_key_event(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	GdkEventKey *keyEvent = (GdkEventKey*) e->args[0];
	w_event_key event;
	int length = keyEvent->length;
	int unicode_length = w_utf8_to_utf16(keyEvent->string, length, 0, 0);
	if (e->msg == SIGNAL_KEY_PRESS_EVENT) {
		event.event.type = W_EVENT_KEYDOWN;
	} else {
		event.event.type = W_EVENT_KEYUP;
	}
	event.detail = 0;
	if (keyEvent->string == 0 || unicode_length <= 1) {
		event.event.time = keyEvent->time;
		event.event.platform_event = (struct w_event_platform*) e;
		event.event.widget = widget;
		event.event.time = keyEvent->time;
		if (!_w_widget_set_key_state(&event, keyEvent, unicode_length))
			return TRUE;
		return _w_widget_send_event(widget, (w_event*) &event, W_EVENT_SEND);
		// widget could be disposed at this point

		/*
		 * It is possible (but unlikely), that application
		 * code could have disposed the widget in the key
		 * events.  If this happens, end the processing of
		 * the key by returning false.
		 */
		//if (isDisposed ()) return false;
		//return event.doit;
	}
	return _w_widget_send_IM_key_event(widget, e, event.event.type, keyEvent,
			keyEvent->string, unicode_length);
}
/*
 * signals
 */
wresult _w_widget_send_event(w_widget *widget, w_event *ee, int flags) {
	return W_WIDGET_GET_CLASS(widget)->post_event(widget, ee, flags);
}
wresult _w_widget_post_event(w_widget *widget, w_event *ee, int flags) {
	if (widget->post_event != 0) {
		widget->post_event(widget, ee);
	}
	_w_widget_priv *priv;
	switch (ee->type) {
	case W_EVENT_PLATFORM: {
		priv = _W_WIDGET_GET_PRIV(widget);
		_w_event_platform *e = _W_EVENT_PLATFORM(ee);
		e->result = priv->signals[e->msg](widget, e, _W_CONTROL_PRIV(priv));
		return W_TRUE;
	}
		break;
	case W_EVENT_COMPUTE_SIZE:
		priv = _W_WIDGET_GET_PRIV(widget);
		return priv->compute_size(widget, (w_event_compute_size*) ee,
				_W_CONTROL_PRIV(priv));
		break;
	case W_EVENT_CLIENT_AREA:
		priv = _W_WIDGET_GET_PRIV(widget);
		return priv->get_client_area(widget, (w_event_client_area*) ee,
				_W_CONTROL_PRIV(priv));
		break;
	case W_EVENT_COMPUTE_TRIM:
		priv = _W_WIDGET_GET_PRIV(widget);
		return priv->compute_trim(widget, (w_event_compute_trim*) ee,
				_W_CONTROL_PRIV(priv));
		break;
	case W_EVENT_LAYOUTDETECT: {
		w_layout *layout;
		w_layout_fill filllayout;
		w_composite_get_layout(W_COMPOSITE(widget), &layout);
		/*if (layout == 0) {
		 w_layout_fill_init(&filllayout, W_VERTICAL);
		 layout = (w_layout*) &filllayout;
		 }*/
		if (layout != 0) {
			w_layout_do_layout(layout, W_COMPOSITE(widget), W_FALSE);
		}
		return W_TRUE;
	}
		break;
	}
	return W_FALSE;
}
gboolean _w_widget_proc(GtkWidget *widget, _gtk_signal *signal, void *args0,
		void *args1, void *args2) {
	w_widget *cc = (w_widget*) g_object_get_qdata(G_OBJECT(widget),
			gtk_toolkit->quark[0]);
	if (cc == 0)
		return FALSE;
	else {
		struct _w_event_platform e;
		e.event.type = W_EVENT_PLATFORM;
		e.event.widget = cc;
		e.event.platform_event = 0;
		e.event.data = 0;
		e.msg = signal->msg;
		e.signal = signal;
		e.result = FALSE;
		e.widget = widget;
		e.args[0] = args0;
		e.args[1] = args1;
		e.args[2] = args2;
		_w_widget_send_event(cc, (w_event*) &e, W_EVENT_SEND);
		return e.result;
	}
}
gboolean _w_widget_proc_2(GtkWidget *widget, _gtk_signal *signal) {
	return _w_widget_proc(widget, signal, 0, 0, 0);
}
gboolean _w_widget_proc_3(GtkWidget *widget, void *args0, _gtk_signal *signal) {
	return _w_widget_proc(widget, signal, args0, 0, 0);
}
gboolean _w_widget_proc_4(GtkWidget *widget, void *args0, void *args1,
		_gtk_signal *signal) {
	return _w_widget_proc(widget, signal, args0, args1, 0);
}
gboolean _w_widget_proc_5(GtkWidget *widget, void *args0, void *args1,
		void *args2, _gtk_signal *signal) {
	return _w_widget_proc(widget, signal, args0, args1, args2);
}
_gtk_signal_info _gtk_signal_lookup[] = { //
		{ SIGNAL_DESTROY, 2, "destroy" }, //
				{ SIGNAL_GRAB_FOCUS, 2, "grab-focus" }, //
				{ SIGNAL_HIDE, 2, "hide" }, //
				{ SIGNAL_MAP, 2, "map" }, //
				{ SIGNAL_POPUP_MENU, 2, "popup-menu" }, //
				{ SIGNAL_REALIZE, 2, "realize" }, //
				{ SIGNAL_SHOW, 2, "show" }, //
				{ SIGNAL_UNMAP, 2, "unmap" }, //
				{ SIGNAL_UNREALIZE, 2, "unrealize" }, //
				{ SIGNAL_BUTTON_PRESS_EVENT, 3, "button-press-event" }, //
				{ SIGNAL_BUTTON_RELEASE_EVENT, 3, "button-release-event" }, //
				{ SIGNAL_CONFIGURE_EVENT, 3, "configure-event" }, //
				{ SIGNAL_DELETE_EVENT, 3, "delete-event" }, //
				{ SIGNAL_ENTER_NOTIFY_EVENT, 3, "enter-notify-event" }, //
				{ SIGNAL_EVENT_AFTER, 3, "event-after" }, //
#ifdef GTK3
				{ SIGNAL_EXPOSE_EVENT, 3, "draw" }, //
				{ SIGNAL_EXPOSE_EVENT_INVERSE, 3, "draw" }, //
				{ SIGNAL_DRAW, 3, "draw" }, //
#endif
				{ SIGNAL_FOCUS, 3, "focus" }, //
				{ SIGNAL_FOCUS_IN_EVENT, 3, "focus-in-event" }, //
				{ SIGNAL_FOCUS_OUT_EVENT, 3, "focus-out-event" }, //
				{ SIGNAL_KEY_PRESS_EVENT, 3, "key-press-event" }, //
				{ SIGNAL_KEY_RELEASE_EVENT, 3, "key-release-event" }, //
				{ SIGNAL_LEAVE_NOTIFY_EVENT, 3, "leave-notify-event" }, //
				{ SIGNAL_MAP_EVENT, 3, "map-event" }, //
				{ SIGNAL_MNEMONIC_ACTIVATE, 3, "mnemonic-activate" }, //
				{ SIGNAL_MOTION_NOTIFY_EVENT, 3, "motion-notify-event" }, //
				{ SIGNAL_SCROLL_EVENT, 3, "scroll-event" }, //
				{ SIGNAL_SHOW_HELP, 3, "show-help" }, //
				{ SIGNAL_SIZE_ALLOCATE, 3, "size-allocate" }, //
				{ SIGNAL_STYLE_SET, 3, "style-set" }, //
				{ SIGNAL_UNMAP_EVENT, 3, "unmap-event" }, //
				{ SIGNAL_WINDOW_STATE_EVENT, 3, "window-state-event" }, //
				/* */
				{ SIGNAL_CHANGE_VALUE, 3, "change-value" }, //
				{ SIGNAL_VALUE_CHANGED, 2, "value-changed" }, //
				/* */
				{ SIGNAL_SCROLL_CHILD, 3, "scroll-child" }, //
				{ SIGNAL_DRAG_DATA_GET, 5, "drag_data_get" }, //
				{ SIGNAL_DRAG_MOTION, 5, "drag_motion" }, //
				{ SIGNAL_DRAG_DROP, 5, "drag_drop" }, //
				{ SIGNAL_DRAG_LEAVE, 5, "drag_leave" }, //
				{ SIGNAL_DRAG_DATA_RECEIVED, 5, "drag_data_received" }, //
				{ SIGNAL_DRAG_END, 5, "drag_end" }, //
				{ SIGNAL_DRAG_DATA_DELETE, 5, "drag_data_delete" }, };
void _w_widget_init_signal_0() {
	_gtk_signal *signals = gtk_toolkit->signals;
	const int length = sizeof(_gtk_signal_lookup)
			/ sizeof(_gtk_signal_lookup[0]);
	for (int i = 0; i < length; i++) {
		_gtk_signal_info *st = &_gtk_signal_lookup[i];
		_gtk_signal *signal = &signals[st->msg];
		signal->msg = st->msg;
		signal->name = st->name;
		signal->number_of_args = st->args;
	}
}
void _w_widget_init_signal(_gtk_signal *signals, _gtk_signal_info *info,
		int length) {
	for (int i = 0; i < length; i++) {
		signals[i].msg = info[i].msg;
		signals[i].name = info[i].name;
		signals[i].number_of_args = info[i].args;
	}
}
void _w_widget_connect(void *widget, _gtk_signal *signal, gboolean after) {
	if (signal->closure == 0) {
		GType type = ((GTypeInstance*) widget)->g_class->g_type;
		signal->id = g_signal_lookup(signal->name, type);
		if (signal->callback == 0) {
			GCallback callback;
			switch (signal->number_of_args) {
			case 2:
				callback = (GCallback) _w_widget_proc_2;
				break;
			case 3:
				callback = (GCallback) _w_widget_proc_3;
				break;
			case 4:
				callback = (GCallback) _w_widget_proc_4;
				break;
			case 5:
				callback = (GCallback) _w_widget_proc_5;
				break;
			default:
				callback = (GCallback) _w_widget_proc_2;
				break;
			}
			signal->callback = callback;
		}
		signal->closure = g_cclosure_new(signal->callback, signal, 0);
	}
	g_signal_connect_closure_by_id(widget, signal->id, 0, signal->closure,
			after);
}
gboolean _gtk_signal_null(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return FALSE;
}
gboolean _gtk_widget_expose_inverse(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	if (GTK_IS_CONTAINER(e->widget)) {
		return _W_WIDGET_PRIV(priv)->signals[SIGNAL_DRAW](widget, e, priv);
	}
	return FALSE;
}
gboolean _gtk_widget_expose(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	if (!GTK_IS_CONTAINER(e->widget)) {
		return _W_WIDGET_PRIV(priv)->signals[SIGNAL_DRAW](widget, e, priv);
	}
	return FALSE;
}
wresult _w_widget_dispose(w_widget *widget) {
	_w_widget_priv *priv = _W_WIDGET_GET_PRIV(widget);
	GtkWidget *topHandle = priv->handle_top(W_WIDGET(widget),
			_W_CONTROL_PRIV(priv));
	gtk_widget_destroy(topHandle);
	widget->clazz = 0;
	return W_TRUE;
}
void _w_widget_set_font_description(w_widget *control, GtkWidget *widget,
		PangoFontDescription *font, _w_control_priv *priv) {
#if GTK3
	gtk_widget_override_font(widget, font);
	GtkStyleContext *context = gtk_widget_get_style_context(widget);
	gtk_style_context_invalidate(context);
#endif
#if GTK2
#endif
}
wresult _w_widget_get_shell(w_widget *widget, w_shell **shell) {
	*shell = 0;
	return W_FALSE;
}
wresult _w_widget_get_parent(w_widget *control, w_widget **parent) {
	*parent = _W_WIDGET(control)->parent;
	return W_TRUE;
}
void _w_widget_class_init(w_toolkit *toolkit, wushort classId,
		struct _w_widget_class *clazz) {
	clazz->is_ok = _w_widget_is_ok;
	clazz->dispose = _w_widget_dispose;
	clazz->post_event = _w_widget_post_event;
	clazz->toolkit = W_TOOLKIT(gtk_toolkit);
	clazz->get_theme = _w_widget_get_theme;
	clazz->set_theme = _w_widget_set_theme;
	clazz->get_parent = _w_widget_get_parent;
	clazz->toolkit = toolkit;
	/*
	 * public function
	 */
	clazz->get_shell = _w_widget_get_shell;
	/*
	 * private
	 */
	_w_control_priv *priv = _W_CONTROL_PRIV(
			W_WIDGET_CLASS(clazz)->platformPrivate);
	if (priv != 0 && _W_WIDGET_PRIV(priv)->init == 0) {
		_W_WIDGET_PRIV(priv)->check_open = _w_widget_check_open;
		_W_WIDGET_PRIV(priv)->check_orientation = _w_widget_check_orientation;
		_W_WIDGET_PRIV(priv)->check_style = _w_widget_check_style;
		_W_WIDGET_PRIV(priv)->create_handle = _w_widget_create_handle;
		_W_WIDGET_PRIV(priv)->create_widget = _w_widget_create_widget;
		_W_WIDGET_PRIV(priv)->set_orientation = _w_widget_set_orientation;
		_W_WIDGET_PRIV(priv)->hook_events = _w_widget_hook_events;
		_W_WIDGET_PRIV(priv)->send_key_event = _w_widget_send_key_event;
		/*
		 * signals
		 */
		for (int i = 0; i < SIGNAL_LAST; i++) {
			_W_WIDGET_PRIV(priv)->signals[i] = _gtk_signal_null;
		}
		_W_WIDGET_PRIV(priv)->compute_size = (__compute_size ) _gtk_signal_null;
		_W_WIDGET_PRIV(priv)->get_client_area = (__get_client_area) _gtk_signal_null;
		_W_WIDGET_PRIV(priv)->compute_trim = (__compute_trim) _gtk_signal_null;
		_W_WIDGET_PRIV(priv)->signals[SIGNAL_EXPOSE_EVENT] = _gtk_widget_expose;
		_W_WIDGET_PRIV(priv)->signals[SIGNAL_EXPOSE_EVENT_INVERSE] =
				_gtk_widget_expose_inverse;
	}
}
