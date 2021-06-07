/*
 * Name:        tray.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef WIN32_WIDGETS_TRAY_H_
#define WIN32_WIDGETS_TRAY_H_
#include "widget.h"
typedef struct _w_trayitem {
	_w_widget widget;
	struct _w_trayitem *next;
	struct _w_trayitem *prev;
	w_widget *parent;
	w_tooltip *tooltip;
	int id;
	unsigned visible :1;
	HICON icon;
	char* tooltiptext;
} _w_trayitem;
#define _W_TRAYITEM(x) ((_w_trayitem*)x)
typedef struct _w_tray {
	_w_widget widget;
	_w_trayitem *items;
	int count;
} _w_tray;

typedef struct _w_tray_priv {
	_w_widget_priv widget;
} _w_tray_priv;
#define _W_TRAY(x) ((_w_tray*)x)
#define _W_TRAY_PRIV(x) ((_w_tray_priv*)x)
void _w_tray_class_init(struct _w_tray_class *clazz);
void _w_trayitem_class_init(struct _w_trayitem_class *item);

#endif /* WIN32_WIDGETS_TRAY_H_ */
