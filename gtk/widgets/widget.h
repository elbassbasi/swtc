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
#define STATE_WIDGET_END (1)
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
} _w_event_platform;
#define _W_EVENT_PLATFORM(x) ((_w_event_platform*)x)

typedef struct _w_widget {
	void *v_table; //used in c++ as virtual table
	struct _w_widget_class *clazz;
	volatile int ref;
	wuint id;
	wuint64 style;
	GtkWidget *handle;
	w_widget_post_event_proc post_event;
	void *data[5];
	wuint state;
} _w_widget;
#define _W_WIDGET(x) ((_w_widget*)x)
/*
 * private
 */
typedef struct _w_widget_priv _w_widget_priv;
typedef struct _w_control_priv _w_control_priv;
typedef gboolean (*_gtk_signal)(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
typedef wresult (*__compute_size)(w_widget *widget, w_event_compute_size *e,
		_w_control_priv *priv);
typedef wresult (*__get_client_area)(w_widget *widget, w_event_client_area *e,
		_w_control_priv *priv);
typedef wresult (*__compute_trim)(w_widget *widget, w_event_compute_trim *e,
		_w_control_priv *priv);
struct _w_widget_priv {
	_gtk_signal signals[SIGNAL_LAST];
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
};
#define _W_WIDGET_PRIV(x) ((_w_widget_priv*)x)
#define _W_WIDGET_GET_PRIV(x) ((_w_widget_priv*)_w_widget_get_priv(W_WIDGET(x)))
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
void _w_widget_get_handles(GtkWidget* handle,_w_widget_handles *handles);
/*
 * functions
 */
GdkWindow* _gdk_window_get_device_position(GdkWindow *window, gint *x, gint *y,
		GdkModifierType *mask);
_w_widget_priv* _w_widget_get_priv(w_widget *widget);
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
wresult _w_widget_send_event(w_widget *widget, w_event *event);
void _w_widget_connect(GtkWidget *widget, wushort signal, wushort signal_id,
		gboolean after);
int _w_widget_set_input_state(int state);
int _w_translate_key(int key);
int _w_untranslate_key(int key);
void _w_widget_class_init(struct _w_widget_class *clazz);
#endif /* GTK_WIDGETS_WIDGET_H_ */
