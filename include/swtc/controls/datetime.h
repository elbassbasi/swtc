/*
 * Name:        datetime.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef SWTC_CONTROLS_DATETIME_H_
#define SWTC_CONTROLS_DATETIME_H_
#include "../widgets/composite.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct w_datetime {
	w_composite composite;
	void *handle[0x10];
} w_datetime;

#define W_DATETIME(x) ((w_datetime*)x)

struct _w_datetime_class {
	struct _w_composite_class composite;
	int (*get_day)(w_datetime *datetime);
	int (*get_hours)(w_datetime *datetime);
	int (*get_minutes)(w_datetime *datetime);
	int (*get_month)(w_datetime *datetime);
	int (*get_seconds)(w_datetime *datetime);
	int (*get_year)(w_datetime *datetime);
	wresult (*set_date)(w_datetime *datetime, int year, int month, int day);
	wresult (*set_day)(w_datetime *datetime, int day);
	wresult (*set_hours)(w_datetime *datetime, int hours);
	wresult (*set_minutes)(w_datetime *datetime, int minutes);
	wresult (*set_month)(w_datetime *datetime, int month);
	wresult (*set_seconds)(w_datetime *datetime, int seconds);
	wresult (*set_time)(w_datetime *datetime, int hours, int minutes,
			int seconds);
	wresult (*set_year)(w_datetime *datetime, int year);
};
SWT_PUBLIC wresult w_datetime_create(w_datetime *datetime, struct w_toolkit *toolkit,
		w_composite *parent, wuint64 style, w_widget_post_event_proc post_event);
SWT_PUBLIC w_datetime* w_datetime_new(struct w_toolkit *toolkit, w_composite *parent,
                                      wuint64 style, w_widget_post_event_proc post_event);
SWT_PUBLIC int w_datetime_get_day(w_datetime *datetime);
SWT_PUBLIC int w_datetime_get_hours(w_datetime *datetime);
SWT_PUBLIC int w_datetime_get_minutes(w_datetime *datetime);
SWT_PUBLIC int w_datetime_get_month(w_datetime *datetime);
SWT_PUBLIC int w_datetime_get_seconds(w_datetime *datetime);
SWT_PUBLIC int w_datetime_get_year(w_datetime *datetime);
SWT_PUBLIC wresult w_datetime_set_date(w_datetime *datetime, int year, int month,
		int day);
SWT_PUBLIC wresult w_datetime_set_day(w_datetime *datetime, int day);
SWT_PUBLIC wresult w_datetime_set_hours(w_datetime *datetime, int hours);
SWT_PUBLIC wresult w_datetime_set_minutes(w_datetime *datetime, int minutes);
SWT_PUBLIC wresult w_datetime_set_month(w_datetime *datetime, int month);
SWT_PUBLIC wresult w_datetime_set_seconds(w_datetime *datetime, int seconds);
SWT_PUBLIC wresult w_datetime_set_time(w_datetime *datetime, int hours, int minutes,
		int seconds);
SWT_PUBLIC wresult w_datetime_set_year(w_datetime *datetime, int year);

#define W_DATETIME_CLASS(x) ((struct _w_datetime_class*)x)
#define W_DATETIME_GET_CLASS(x) ((struct _w_datetime_class*)W_WIDGET_GET_CLASS(x))

#ifdef __cplusplus
}
#endif
#endif /* SWTC_CONTROLS_DATETIME_H_ */
