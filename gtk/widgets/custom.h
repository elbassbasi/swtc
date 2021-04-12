/*
 * Name:        custom.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2021-01-03
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef GTK_WIDGETS_CUSTOM_H_
#define GTK_WIDGETS_CUSTOM_H_
#include "../graphics/gc.h"
/*
 * signals
 */
typedef enum _gtk_signal_type {
	/*
	 * 2 arguments
	 */
	SIGNAL_2_ARGS = 1, //
	SIGNAL_ACTIVATE = SIGNAL_2_ARGS, //
	SIGNAL_DESTROY, //widget
	SIGNAL_CHANGED,
	SIGNAL_CLICKED,
	SIGNAL_GRAB_FOCUS,
	SIGNAL_HIDE,
	SIGNAL_MAP,
	SIGNAL_OUTPUT,
	SIGNAL_POPUP_MENU,
	SIGNAL_PREEDIT_CHANGED,
	SIGNAL_REALIZE,
	SIGNAL_SELECT,
	SIGNAL_SHOW,
	SIGNAL_UNMAP,
	SIGNAL_UNREALIZE,
	SIGNAL_VALUE_CHANGED,
	SIGNAL_ACTIVATE_INVERSE,
	SIGNAL_DAY_SELECTED,
	SIGNAL_MONTH_CHANGED,
	SIGNAL_DAY_SELECTED_DOUBLE_CLICK,
	SIGNAL_SELECTION_DONE,
	SIGNAL_START_INTERACTIVE_SEARCH,
	SIGNAL_BACKSPACE,
	SIGNAL_BACKSPACE_INVERSE,
	SIGNAL_COPY_CLIPBOARD,
	SIGNAL_COPY_CLIPBOARD_INVERSE,
	SIGNAL_CUT_CLIPBOARD,
	SIGNAL_CUT_CLIPBOARD_INVERSE,
	SIGNAL_PASTE_CLIPBOARD,
	SIGNAL_PASTE_CLIPBOARD_INVERSE,
	SIGNAL_CREATE_MENU_PROXY,
	/*
	 * 3 arguments
	 */
	SIGNAL_3_ARGS,
	SIGNAL_BUTTON_PRESS_EVENT = SIGNAL_3_ARGS, //widget
	SIGNAL_BUTTON_PRESS_EVENT_INVERSE, //widget
	SIGNAL_BUTTON_RELEASE_EVENT, //widget
	SIGNAL_BUTTON_RELEASE_EVENT_INVERSE, //widget
	SIGNAL_COMMIT,
	SIGNAL_CONFIGURE_EVENT,
	SIGNAL_DELETE_EVENT,
	SIGNAL_ENTER_NOTIFY_EVENT,
	SIGNAL_EVENT,
	SIGNAL_EVENT_AFTER,
	SIGNAL_EXPOSE_EVENT,
	SIGNAL_EXPOSE_EVENT_INVERSE,
	SIGNAL_DRAW,
	SIGNAL_FOCUS,
	SIGNAL_FOCUS_IN_EVENT,
	SIGNAL_FOCUS_OUT_EVENT,
	SIGNAL_INPUT,
	SIGNAL_KEY_PRESS_EVENT,
	SIGNAL_KEY_RELEASE_EVENT,
	SIGNAL_LEAVE_NOTIFY_EVENT,
	SIGNAL_MAP_EVENT,
	SIGNAL_MNEMONIC_ACTIVATE,
	SIGNAL_MOTION_NOTIFY_EVENT,
	SIGNAL_MOTION_NOTIFY_EVENT_INVERSE,
	SIGNAL_MOVE_FOCUS,
	SIGNAL_POPULATE_POPUP,
	SIGNAL_SCROLL_EVENT,
	SIGNAL_SHOW_HELP,
	SIGNAL_SIZE_ALLOCATE,
	SIGNAL_STYLE_SET,
	SIGNAL_TOGGLED,
	SIGNAL_UNMAP_EVENT,
	SIGNAL_WINDOW_STATE_EVENT,
	SIGNAL_ROW_DELETED,
	SIGNAL_DIRECTION_CHANGED,
	/*
	 * 4 arguments
	 */
	SIGNAL_4_ARGS,
	SIGNAL_DELETE_RANGE = SIGNAL_4_ARGS,
	SIGNAL_DELETE_TEXT,
	SIGNAL_ROW_ACTIVATED,
	SIGNAL_SCROLL_CHILD,
	SIGNAL_SWITCH_PAGE,
	SIGNAL_TEST_COLLAPSE_ROW,
	SIGNAL_TEST_EXPAND_ROW,
	SIGNAL_STATUS_ICON_POPUP_MENU,
	SIGNAL_ROW_INSERTED,
	SIGNAL_ICON_RELEASE,
	SIGNAL_DELETE_FROM_CURSOR,
	SIGNAL_DELETE_FROM_CURSOR_INVERSE,
	SIGNAL_ROW_HAS_CHILD_TOGGLED,
	/*
	 * 5 arguments
	 */
	SIGNAL_5_ARGS,
	SIGNAL_CHANGE_VALUE = SIGNAL_5_ARGS,
	SIGNAL_EXPAND_COLLAPSE_CURSOR_ROW,
	SIGNAL_INSERT_TEXT,
	SIGNAL_TEXT_BUFFER_INSERT_TEXT,
	SIGNAL_MOVE_CURSOR,
	SIGNAL_MOVE_CURSOR_INVERSE,
	/*
	 * last
	 */
	SIGNAL_6_ARGS,
	/*
	 * drag and drop
	 */
	SIGNAL_DRAG_DATA_GET,
	SIGNAL_DRAG_MOTION,
	SIGNAL_DRAG_DROP,
	SIGNAL_DRAG_LEAVE,
	SIGNAL_DRAG_DATA_RECEIVED,
	SIGNAL_DRAG_END,
	SIGNAL_DRAG_DATA_DELETE,
	/*
	 *
	 */
	SIGNAL_LAST,
	GDK_FILTER_PROC,
} _gtk_signal_type;

/**
 * fixed widget
 */
extern GType _w_fixed_type;
typedef struct _w_fixed {
	GtkContainer container;
	GtkAdjustment *hadjustment;
	GtkAdjustment *vadjustment;
	struct _w_fixed *next;
	struct _w_fixed *prev;
	struct _w_fixed *first;
	/* Accessibility */
	AtkObject *accessible;
	GtkWidget *child;
	guint count :28;
	guint hscroll_policy :1;
	guint vscroll_policy :1;
	guint use_as_child :1;
	guint ignore_fixed :1;
	GtkAllocation alloc;
} _w_fixed;
#define _W_TYPE_FIXED             (_w_fixed_type)
#define _W_FIXED(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), _W_TYPE_FIXED, _w_fixed))
#define _W_IS_FIXED(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), _W_TYPE_FIXED))
GtkWidget* _w_fixed_new();
void _w_fixed_move(GtkWidget *child, gint x, gint y);
void _w_fixed_resize(GtkWidget *child, gint width, gint height);
_w_fixed* _w_fixed_get(GtkWidget *widget);
void _w_fixed_add_remove_child(_w_fixed *fixed, GtkWidget *child);
void _w_fixed_restack(GtkWidget *fixed, GtkWidget *child, GtkWidget *sibling,
		gboolean above);
/**
 * image widget
 */
extern GType _w_image_widget_type;
typedef wresult (*__get_image_list)(w_widget *widget, w_imagelist **imagelist);
typedef struct _w_image_widget {
	GtkImage image;
	w_widget *parent;
	__get_image_list get_image_list;
	int index;
} _w_image_widget;

#define _W_TYPE_IMAGE_WIDGET             (_w_image_widget_type)
#define _W_IMAGE_WIDGET(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), _W_TYPE_IMAGE_WIDGET, _w_image_widget))
#define _W_IS_IMAGE_WIDGET(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), _W_TYPE_IMAGE_WIDGET))
_w_image_widget* _w_image_widget_new();
#endif /* GTK_WIDGETS_CUSTOM_H_ */
