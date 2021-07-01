/*
 * datetime.h
 *
 *  Created on: 3 avr. 2021
 *      Author: azeddine
 */

#ifndef WIN32_CONTROLS_DATETIME_H_
#define WIN32_CONTROLS_DATETIME_H_
#include "../widgets/shell.h"
#define STATE_DATETIME_DOUBLECLICK (1 << (STATE_COMPOSITE_END + 0))
#define STATE_DATETIME_IGNORESELECTION (1 << (STATE_COMPOSITE_END + 1))
#define STATE_DATETIME_LASTSYSTEMTIME (1 << (STATE_COMPOSITE_END + 2))
typedef struct _w_datetime {
	_w_composite composite;
	unsigned wHour :5;
	unsigned wMinute :6;
	unsigned wSecond :6;
	SYSTEMTIME lastSystemTime;
} _w_datetime;

typedef struct _w_datetime_priv {
	_w_composite_priv composite;
	WNDPROC calendarProc;
} _w_datetime_priv;
#define _W_DATETIME(x) ((_w_datetime*)x)
#define _W_DATETIME_PRIV(x) ((_w_datetime_priv*)x)

int _w_datetime_get_custom_short_time_format(WCHAR *lpFormat, int _length);
void _w_datetime_class_init(w_toolkit *toolkit, wushort classId,struct _w_datetime_class *clazz);

#endif /* WIN32_CONTROLS_DATETIME_H_ */
