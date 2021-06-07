/*
 * Name:        tooltip.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef WIN32_WIDGETS_TOOLTIP_H_
#define WIN32_WIDGETS_TOOLTIP_H_
#include "widget.h"
#define TIMER_ID 100
typedef struct _w_tooltip {
	struct _w_widget widget;
	struct _w_tooltip *next;
	struct _w_tooltip *prev;
	w_shell* parent;
	w_trayitem* item;
	char* text;
	char* message;
	int id;
	w_point location;
	unsigned visible : 1;
	unsigned hasLocation : 1;
	unsigned autoHide : 1;
} _w_tooltip;

typedef struct _w_tooltip_priv {
	_w_widget_priv widget;

} _w_tooltip_priv;
#define _W_TOOLTIP(x) ((_w_tooltip*)x)
#define _W_TOOLTIP_PRIV(x) ((_w_tooltip_priv*)x)
void _w_tooltip_update_message (w_tooltip *tooltip);
void _w_tooltip_class_init(struct _w_tooltip_class *clazz);
#endif /* WIN32_WIDGETS_TOOLTIP_H_ */
