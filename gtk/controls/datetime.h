/*
 * datetime.h
 *
 *  Created on: 3 avr. 2021
 *      Author: azeddine
 */

#ifndef GTK_CONTROLS_DATETIME_H_
#define GTK_CONTROLS_DATETIME_H_
#include "../widgets/shell.h"

typedef struct _w_datetime {
	_w_composite composite;
	GtkWidget *down;
	w_size prefferedSize;
	unsigned year :11;
	unsigned month :4;
	unsigned day :5;
	unsigned hours :5;
	unsigned minutes :6;
	unsigned seconds :6;
} _w_datetime;

typedef struct _w_datetime_priv {
	_w_composite_priv composite;
	guint signal_day_selected;
	guint signal_day_selected_double_click;
	guint signal_month_changed;
	guint signal_output;
} _w_datetime_priv;
#define _W_DATETIME(x) ((_w_datetime*)x)
#define _W_DATETIME_PRIV(x) ((_w_datetime_priv*)x)

GtkWidget* _w_datetime_get_entry(w_widget *widget);
void _w_datetime_get_formatted_string(w_widget *widget, char *text);
void _w_datetime_update_control(w_datetime *datetime);
void _w_datetime_class_init(struct _w_datetime_class *clazz);

#endif /* GTK_CONTROLS_DATETIME_H_ */
