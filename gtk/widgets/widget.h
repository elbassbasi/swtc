/*
 * Name:        widget.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef GTK_WIDGETS_WIDGET_H_
#define GTK_WIDGETS_WIDGET_H_
#include "custom.h"
/*
 * states
 */
#define STATE_HANDLE (1 << 0)
#define STATE_OBSCURED (1 << 1)
#define STATE_DESTROYED (1 << 2)
#define STATE_WIDGET_END (2)
/*
 *
 */
#define HAS_AUTO_DIRECTION  (0)
#define AUTO_TEXT_DIRECTION  (W_LEFT_TO_RIGHT | W_RIGHT_TO_LEFT)
#define DEFAULT_WIDTH  (64)
#define DEFAULT_HEIGHT  (64)
/*
 * platform event
 */
typedef struct _w_event_platform {
	w_event event;
	int msg;
	int result;
	GtkWidget *widget;
	void *args[6];
	_gtk_signal *signal;
} _w_event_platform;
#define _W_EVENT_PLATFORM(x) ((_w_event_platform*)x)

typedef struct _w_widget {
	void *v_table; //used in c++ as virtual table
	struct _w_widget_class *clazz;
	w_widget_post_event_proc post_event;
	GtkWidget *handle;
	wuint64 style;
	wuint state;
	volatile int ref;
	wuint id;
	wushort state0;
	wushort children_count;
	w_widget *parent;
	w_widget *first_child;
	w_link_0 sibling;
	w_theme *theme;
	void *data[5];
} _w_widget;
#define _W_WIDGET(x) ((_w_widget*)x)
/*
 * private
 */
typedef struct _w_widget_priv _w_widget_priv;
typedef struct _w_control_priv _w_control_priv;
typedef struct _gtk_signal_info {
	wchar msg;
	wuchar args;
	const char *name;
} _gtk_signal_info;
typedef gboolean (*_gtk_signal_fn)(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
typedef wresult (*__compute_size)(w_widget *widget, w_event_compute_size *e,
		_w_control_priv *priv);
typedef wresult (*__get_client_area)(w_widget *widget, w_event_client_area *e,
		_w_control_priv *priv);
typedef wresult (*__compute_trim)(w_widget *widget, w_event_compute_trim *e,
		_w_control_priv *priv);
struct _w_widget_priv {
	unsigned init :1;
	_gtk_signal_fn signals[SIGNAL_LAST];
	__compute_size compute_size;
	__get_client_area get_client_area;
	__compute_trim compute_trim;
	GtkWidget* (*handle_top)(w_widget *control, _w_control_priv *priv);
	wuint64 (*check_style)(w_widget *control, wuint64 style);
	void (*check_open)(w_widget *widget, _w_control_priv *priv);
	wresult (*create_widget)(w_widget *widget, _w_control_priv *priv);
	wresult (*create_handle)(w_widget *widget, _w_control_priv *priv);
	void (*hook_events)(w_widget *widget, _w_control_priv *priv);
	void (*check_orientation)(w_widget *widget, w_widget *parent,
			_w_control_priv *priv);
	void (*set_orientation)(w_widget *widget, int create,
			_w_control_priv *priv);
	gboolean (*send_key_event)(w_widget *widget, _w_event_platform *e,
			_w_control_priv *priv);
};
#define _W_WIDGET_PRIV(x) ((_w_widget_priv*)x)
#define _W_WIDGET_GET_PRIV(x) ((_w_widget_priv*)W_WIDGET_GET_CLASS(x)->platformPrivate)
/*
 * find
 */
typedef struct _w_widget_find_child {
	int index;
	int count;
	GtkWidget *widget;
	GtkWidget *prev;
	GtkWidget *next;
} _w_widget_find_child;
typedef struct _w_widget_handles {
	GtkWidget *handle;
	GtkWidget *box;
	GtkWidget *label;
	GtkWidget *image;
	GtkWidget *arrow;
} _w_widget_handles;
void _w_widget_find_children(GtkWidget *widget, gpointer data);
void _w_widget_children_count(GtkWidget *widget, gpointer data);
void _w_widget_get_handles_callback(GtkWidget *widget, gpointer data);
void _w_widget_get_handles(GtkWidget *handle, _w_widget_handles *handles);
/*
 * functions
 */
GdkWindow* _gdk_window_get_device_position(GdkWindow *window, gint *x, gint *y,
		GdkModifierType *mask);
void _w_widget_set_control(void *handle, w_widget *widget);
w_widget* _w_widget_find_control(void *handle);
GtkWidget* _w_widget_h0(w_widget *widget, _w_control_priv *priv);
GtkWidget* _w_widget_h(w_widget *widget, _w_control_priv *priv);
GtkWidget* _w_widget_hp(w_widget *widget, _w_control_priv *priv);
GtkWidget* _w_widget_hpp(w_widget *widget, _w_control_priv *priv);
GtkWidget* _w_widget_hppp(w_widget *widget, _w_control_priv *priv);
void _w_widget_check_orientation(w_widget *widget, w_widget *parent,
		_w_control_priv *priv);
wresult _w_widget_create_widget(w_widget *widget, _w_control_priv *priv);
wresult _w_widget_create_handle(w_widget *widget, _w_control_priv *priv);
void _w_widget_set_orientation(w_widget *widget, int create,
		_w_control_priv *priv);
void _w_widget_hook_events(w_widget *widget, _w_control_priv *priv);
wuint64 _w_widget_check_bits(wuint64 style, int int0, int int1, int int2,
		int int3, int int4, int int5);
wresult _w_widget_send_event(w_widget *widget, w_event *event,int flags);
gboolean _w_widget_send_IM_key_event(w_widget *widget, _w_event_platform *e,
		int type, GdkEventKey *keyEvent, const char *chars, int length);
void _w_widget_init_signal_0();
void _w_widget_init_signal(_gtk_signal *signals, _gtk_signal_info *info,
		int length);
gulong _w_widget_connect(void *widget, _gtk_signal *signal, gboolean after);
int _w_widget_set_input_state(int state);
int _w_translate_key(int key);
int _w_untranslate_key(int key);
void _w_widget_set_font_description(w_widget *control, GtkWidget *widget,
		PangoFontDescription *font, _w_control_priv *priv);
wresult _w_widget_post_event(w_widget *widget, w_event *ee, int flags);
wresult _w_widget_init_themedata(w_widget *widget, w_themedata *data);
wresult _w_widget_dispose_class(struct _w_widget_class *clazz);
void _w_widget_class_init(w_toolkit *toolkit, wushort classId,
		struct _w_widget_class *clazz);
#endif /* GTK_WIDGETS_WIDGET_H_ */
