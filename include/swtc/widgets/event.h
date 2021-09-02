/*
 * Name:        event.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef SWT_WIDGETS_EVENT_H_
#define SWT_WIDGETS_EVENT_H_
#include "../graphics/textlayout.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct w_toolkit w_toolkit;
typedef struct w_control w_control;
typedef struct w_composite w_composite;
typedef struct w_shell w_shell;
typedef struct w_theme w_theme;
typedef struct w_widget w_widget;
typedef struct w_menu w_menu;
enum {
	/*
	 * event type
	 */
	W_EVENT_NONE = 0,
	W_EVENT_PLATFORM,
	W_EVENT_NOTIFYCHANGE,
	W_EVENT_FREE_MEMORY,
	W_EVENT_DISPOSE,
	W_EVENT_KEYDOWN,
	W_EVENT_KEYUP,
	W_EVENT_MOUSEDOWN,
	W_EVENT_MOUSEUP,
	W_EVENT_MOUSEMOVE,
	W_EVENT_MOUSEENTER,
	W_EVENT_MOUSEEXIT,
	W_EVENT_MOUSEDOUBLECLICK,
	W_EVENT_PAINT,
	W_EVENT_MOVE,
	W_EVENT_RESIZE,
	W_EVENT_SELECTION,
	W_EVENT_DEFAULTSELECTION,
	W_EVENT_FOCUSIN,
	W_EVENT_FOCUSOUT,
	W_EVENT_TOOLTIP_TEXT,
	W_EVENT_ICONIFY,
	W_EVENT_DEICONIFY,
	W_EVENT_CLOSE,
	W_EVENT_SHOW,
	W_EVENT_HIDE,
	W_EVENT_MODIFY,
	W_EVENT_VERIFY,
	W_EVENT_ACTIVATE,
	W_EVENT_DEACTIVATE,
	W_EVENT_HELP,
	W_EVENT_DRAGDETECT,
	W_EVENT_ARM,
	W_EVENT_TRAVERSE,
	W_EVENT_MOUSEHOVER,
	W_EVENT_HARDKEYDOWN,
	W_EVENT_HARDKEYUP,
	W_EVENT_MENUDETECT,
	W_EVENT_SETDATA,
	W_EVENT_MOUSEVERTICALWHEEL,
	W_EVENT_MOUSEHORIZONTALWHEEL,
	W_EVENT_MOUSEWHEEL = W_EVENT_MOUSEVERTICALWHEEL,
	W_EVENT_SETTINGS,
	W_EVENT_IMECOMPOSITION,
	W_EVENT_ORIENTATIONCHANGE,
	W_EVENT_SKIN,
	W_EVENT_OPENDOCUMENT,
	W_EVENT_TOUCH,
	W_EVENT_GESTURE,
	W_EVENT_ITEM_GET_TEXT,
	W_EVENT_ITEM_SET_TEXT,
	W_EVENT_ITEM_GET_ATTR,
	W_EVENT_ITEM_SET_ATTR,
	W_EVENT_ITEM_SELECTION,
	W_EVENT_ITEM_DEFAULTSELECTION,
	W_EVENT_ITEM_HELP,
	W_EVENT_ITEM_DISPOSE,
	W_EVENT_ITEM_ARM,
	W_EVENT_ITEM_GET_CONTROL,
	W_EVENT_ITEM_SET_CONTROL,
	W_EVENT_ITEM_GET_MENU,
	W_EVENT_ITEM_SET_MENU,
	W_EVENT_ITEM_MEASURE,
	W_EVENT_ITEM_ERASE,
	W_EVENT_ITEM_PAINT,
	W_EVENT_ITEM_CLOSE,
	W_EVENT_ITEM_EXPAND,
	W_EVENT_ITEM_COLLAPSE,
	W_EVENT_ITEM_RESIZE,
	W_EVENT_ITEM_SHOW,
	W_EVENT_ITEM_HIDE,
	W_EVENT_ITEM_MOVE,
	W_EVENT_HSCROLL,
	W_EVENT_VSCROLL,
	W_EVENT_COMPUTE_SIZE,
	W_EVENT_COMPUTE_TRIM,
	W_EVENT_CLIENT_AREA,
	W_EVENT_LAYOUTDETECT,
	W_EVENT_TIMER,
	W_EVENT_NOTIFY,
	/*
	 * Event Details
	 *
	 **/
	W_COMPOSITION_CHANGED = 1,
	W_COMPOSITION_OFFSET = 2,
	W_COMPOSITION_SELECTION = 3,
	W_DRAG = 1,
	W_TRAVERSE_MASK = 0xFFFC,
	W_TRAVERSE_NONE = 0,
	W_TRAVERSE_ESCAPE = 1 << 2,
	W_TRAVERSE_RETURN = 1 << 3,
	W_TRAVERSE_TAB_PREVIOUS = 1 << 4,
	W_TRAVERSE_TAB_NEXT = 1 << 5,
	W_TRAVERSE_ARROW_PREVIOUS = 1 << 6,
	W_TRAVERSE_ARROW_NEXT = 1 << 7,
	W_TRAVERSE_MNEMONIC = 1 << 8,
	W_TRAVERSE_PAGE_PREVIOUS = 1 << 9,
	W_TRAVERSE_PAGE_NEXT = 1 << 10,
	W_GESTURE_BEGIN = 1 << 1,
	W_GESTURE_END = 1 << 2,
	W_GESTURE_ROTATE = 1 << 3,
	W_GESTURE_SWIPE = 1 << 4,
	W_GESTURE_MAGNIFY = 1 << 5,
	W_GESTURE_PAN = 1 << 6,
	W_TOUCHSTATE_DOWN = 1 << 0,
	W_TOUCHSTATE_MOVE = 1 << 1,
	W_TOUCHSTATE_UP = 1 << 2,
	W_MENU_MOUSE = 0,
	W_MENU_KEYBOARD = 1,
	W_CHANGED = 1 << 1,
	W_DEFER = 1 << 2,
	W_NONE = 0,
	W_DEFAULT = -1,
	W_OFF = 0,
	W_ON = 1,
	W_LOW = 1,
	W_HIGH = 2,
};
typedef struct w_event {
	wuint type;
	wuint time;
	struct w_event_platform *platform_event;
	struct w_widget *widget;
	void *data; //reserved
} w_event;
#define W_EVENT(x) ((w_event*)x)
typedef struct w_event_platform {
	w_event event;
	unsigned int msg;
	unsigned int result;
	void *handle;
	void *args[3];
} w_event_platform;
#define _EVENT_PLATFORM(x) ((w_event_platform*)x)
enum {
	W_NOTIFYCHANGE_UNKNOWN = 0, W_NOTIFYCHANGE_FONT = 1,

};
typedef struct w_event_notifychange {
	w_event event;
	int id;
	union {
		void *info[3];
		w_font *font;
		w_color color;
	};
} w_event_notifychange;
typedef struct w_event_mouse {
	w_event event;
	union {
		struct {
			unsigned doit :1;
			unsigned unused :14;
			unsigned alt :1;
			unsigned shift :1;
			unsigned ctrl :1;
			unsigned button1 :1;
			unsigned button2 :1;
			unsigned button3 :1;
			unsigned command :1;
			unsigned button4 :1;
			unsigned button5 :1;
		};
		int detail;
	};
	wushort clickcount;
	wushort button;
	int x;
	int y;
} w_event_mouse;
typedef struct w_event_key {
	w_event event;
	union {
		struct {
			unsigned doit :1;
			unsigned unused :14;
			unsigned alt :1;
			unsigned shift :1;
			unsigned ctrl :1;
			unsigned button1 :1;
			unsigned button2 :1;
			unsigned button3 :1;
			unsigned command :1;
			unsigned button4 :1;
			unsigned button5 :1;
		};
		int detail;
	};
	int character;
	int keycode;
	int keylocation;
} w_event_key;
typedef struct w_event_selection {
	w_event event;
	union {
		struct {
			unsigned doit :1;
		};
		int detail;
	};
} w_event_selection;
typedef struct w_event_compute_size {
	w_event event;
	w_size *size;
	int wHint;
	int hHint;
	int changed;
} w_event_compute_size;
typedef struct w_event_paint {
	w_event event;
	w_rect bounds;
	w_graphics *gc;
} w_event_paint;
typedef struct w_event_time {
	w_event event;
	wushort id;
} w_event_time;
typedef struct w_event_menu_detect {
	w_event event;
	int detail;
	w_point location;
	w_menu *menu;
} w_event_menu_detect;
typedef struct w_touch {
	void *handle;
	void *id;
	union {
		struct {
			unsigned doit :1;
		};
		int detail;
	};
	w_rect bounds;
	w_point location;
} w_touch;
typedef struct w_event_touch {
	w_event event;
	union {
		struct {
			unsigned doit :1;
		};
		int detail;
	};
	void *touches;
	w_point location;
} w_event_touch;
SWT_PUBLIC wresult w_event_touch_get_touches(w_event_touch *event,
		w_iterator *touches);
typedef struct w_event_tooltip_text {
	w_event event;
	union {
		struct {
			unsigned doit :1;
		};
		int detail;
	};
	int enc;
	w_alloc alloc;
	void *user_data;
	w_point location;
} w_event_tooltip_text;
#ifdef __cplusplus
}
#endif
#endif /* SWT_WIDGETS_EVENT_H_ */
