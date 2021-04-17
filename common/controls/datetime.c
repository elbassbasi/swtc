/*
 * datetime.c
 *
 *  Created on: 26 août 2019
 *      Author: azeddine El Bassbasi
 */
#include <swtc/swtc.h>
wresult w_datetime_create(w_datetime *datetime, struct w_toolkit *toolkit,
		w_composite *parent, wuint64 style,
		w_widget_post_event_proc post_event) {
	return _w_widget_create(W_WIDGET(datetime), toolkit, W_WIDGET(parent),
			style, _W_CLASS_DATETIME, post_event);
}
w_datetime* w_datetime_new(struct w_toolkit *toolkit, w_composite *parent,
		wuint64 style, w_widget_post_event_proc post_event) {
	return W_DATETIME(
			_w_widget_new(toolkit,W_WIDGET(parent),style,_W_CLASS_DATETIME,post_event));
}
wresult w_datetime_get_day(w_datetime *datetime) {
	wresult result = W_WIDGET_CHECK0(datetime);
	if (result > 0) {
		return W_DATETIME_GET_CLASS(datetime)->get_day(datetime);
	} else
		return result;
}
wresult w_datetime_get_hours(w_datetime *datetime) {
	wresult result = W_WIDGET_CHECK0(datetime);
	if (result > 0) {
		return W_DATETIME_GET_CLASS(datetime)->get_hours(datetime);
	} else
		return result;
}
wresult w_datetime_get_minutes(w_datetime *datetime) {
	wresult result = W_WIDGET_CHECK0(datetime);
	if (result > 0) {
		return W_DATETIME_GET_CLASS(datetime)->get_minutes(datetime);
	} else
		return result;
}
wresult w_datetime_get_month(w_datetime *datetime) {
	wresult result = W_WIDGET_CHECK0(datetime);
	if (result > 0) {
		return W_DATETIME_GET_CLASS(datetime)->get_month(datetime);
	} else
		return result;
}
wresult w_datetime_get_seconds(w_datetime *datetime) {
	wresult result = W_WIDGET_CHECK0(datetime);
	if (result > 0) {
		return W_DATETIME_GET_CLASS(datetime)->get_seconds(datetime);
	} else
		return result;
}
wresult w_datetime_get_year(w_datetime *datetime) {
	wresult result = W_WIDGET_CHECK0(datetime);
	if (result > 0) {
		return W_DATETIME_GET_CLASS(datetime)->get_year(datetime);
	} else
		return result;
}
wresult w_datetime_set_date(w_datetime *datetime, int year, int month,
		int day) {
	wresult result = W_WIDGET_CHECK0(datetime);
	if (result > 0) {
		return W_DATETIME_GET_CLASS(datetime)->set_date(datetime, year, month,
				day);
	} else
		return result;
}
wresult w_datetime_set_day(w_datetime *datetime, int day) {
	wresult result = W_WIDGET_CHECK0(datetime);
	if (result > 0) {
		return W_DATETIME_GET_CLASS(datetime)->set_day(datetime, day);
	} else
		return result;
}
wresult w_datetime_set_hours(w_datetime *datetime, int hours) {
	wresult result = W_WIDGET_CHECK0(datetime);
	if (result > 0) {
		return W_DATETIME_GET_CLASS(datetime)->set_hours(datetime, hours);
	} else
		return result;
}
wresult w_datetime_set_minutes(w_datetime *datetime, int minutes) {
	wresult result = W_WIDGET_CHECK0(datetime);
	if (result > 0) {
		return W_DATETIME_GET_CLASS(datetime)->set_minutes(datetime, minutes);
	} else
		return result;
}
wresult w_datetime_set_month(w_datetime *datetime, int month) {
	wresult result = W_WIDGET_CHECK0(datetime);
	if (result > 0) {
		return W_DATETIME_GET_CLASS(datetime)->set_month(datetime, month);
	} else
		return result;
}
wresult w_datetime_set_seconds(w_datetime *datetime, int seconds) {
	wresult result = W_WIDGET_CHECK0(datetime);
	if (result > 0) {
		return W_DATETIME_GET_CLASS(datetime)->set_seconds(datetime, seconds);
	} else
		return result;
}
wresult w_datetime_set_time(w_datetime *datetime, int hours, int minutes,
		int seconds) {
	wresult result = W_WIDGET_CHECK0(datetime);
	if (result > 0) {
		return W_DATETIME_GET_CLASS(datetime)->set_time(datetime, hours,
				minutes, seconds);
	} else
		return result;
}
wresult w_datetime_set_year(w_datetime *datetime, int year) {
	wresult result = W_WIDGET_CHECK0(datetime);
	if (result > 0) {
		return W_DATETIME_GET_CLASS(datetime)->set_year(datetime, year);
	} else
		return result;
}
