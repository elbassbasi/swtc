/*
 * datetime.c
 *
 *  Created on: 3 avr. 2021
 *      Author: azeddine
 */
#include "datetime.h"
#include "../widgets/toolkit.h"
#define MIN_YEAR 1752 // Gregorian switchover in North America: September 19, 1752
#define MAX_YEAR 9999
#define SPACE_FOR_CURSOR 1
#define GTK2_MANUAL_BORDER_PADDING 2
wuint64 _w_datetime_check_style(w_widget *control, wuint64 style) {
	/*
	 * Even though it is legal to create this widget
	 * with scroll bars, they serve no useful purpose
	 * because they do not automatically scroll the
	 * widget's client area.  The fix is to clear
	 * the SWT style.
	 */
	style &= ~(W_HSCROLL | W_VSCROLL);

	style = _w_widget_check_bits(style, W_DATE, W_TIME, W_CALENDAR, 0, 0, 0);
	if ((style & W_DATE) == 0)
		style &= ~W_DROP_DOWN;
	return _w_widget_check_bits(style, W_MEDIUM, W_SHORT, W_LONG, 0, 0, 0);
}
wresult _w_datetime_compute_trim(w_widget *widget, w_event_compute_trim *e,
		_w_control_priv *priv) {
	wuint64 style = _W_WIDGET(widget)->style;
	if ((style & W_CALENDAR) != 0) {
		return _w_scrollable_compute_trim(widget, e, priv);
	}
	wresult ret = _w_scrollable_compute_trim(widget, e, priv);
	int xborder = 0, yborder = 0;
#if GTK3
	GtkBorder tmp;
	GtkWidget *textEntryHandle = _w_datetime_get_entry(widget);
	GtkStyleContext *context = gtk_widget_get_style_context(textEntryHandle);
	if (GTK_VERSION < VERSION(3, 18, 0)) {
		gtk_style_context_get_padding(context, GTK_STATE_FLAG_NORMAL, &tmp);
	} else {
		gtk_style_context_get_padding(context,
				gtk_widget_get_state_flags(textEntryHandle), &tmp);
	}
	e->result->x -= tmp.left;
	e->result->y -= tmp.top;
	e->result->width += tmp.left + tmp.right;
	e->result->height += tmp.top + tmp.bottom;
	if ((style & W_BORDER) != 0) {
		if (GTK_VERSION < VERSION(3, 18, 0)) {
			gtk_style_context_get_border(context, GTK_STATE_FLAG_NORMAL, &tmp);
		} else {
			gtk_style_context_get_border(context,
					gtk_widget_get_state_flags(textEntryHandle), &tmp);
		}
		e->result->x -= tmp.left;
		e->result->y -= tmp.top;
		e->result->width += tmp.left + tmp.right;
		e->result->height += tmp.top + tmp.bottom;
	}
#endif
	e->result->x -= xborder;
	e->result->y -= yborder;
	e->result->width += 2 * xborder;
	e->result->height += 2 * yborder;
	e->result->width += SPACE_FOR_CURSOR;
	return ret;
}
wresult _w_datetime_compute_size(w_widget *widget, w_event_compute_size *e,
		_w_control_priv *priv) {
	wuint64 style = _W_WIDGET(widget)->style;
	int wHint = e->wHint;
	int hHint = e->hHint;
	if (wHint != W_DEFAULT && wHint < 0)
		wHint = 0;
	if (hHint != W_DEFAULT && hHint < 0)
		hHint = 0;
	int width = 0, height = 0;
	//For Date and Time, we cache the preffered size as there is no need to recompute it.
	w_size *prefferedSize = &_W_DATETIME(widget)->prefferedSize;
	if (!e->changed && ((style & W_DATE) || (style & W_TIME))
			&& prefferedSize->width != 0 && prefferedSize->height != 0) {
		width = (wHint != W_DEFAULT) ? wHint : prefferedSize->width;
		height = (hHint != W_DEFAULT) ? hHint : prefferedSize->height;
		e->size->width = width;
		e->size->height = height;
	} else {

		if (wHint == W_DEFAULT || hHint == W_DEFAULT) {
			if (style & W_CALENDAR) {
				GtkWidget *containerHandle = _W_WIDGET(widget)->handle;
				_w_control_compute_native_size(widget, containerHandle, e,
						priv);
				width = e->size->width;
				height = e->size->height;
			} else {
				GtkWidget *textEntryHandle = _w_datetime_get_entry(widget);
				_w_control_compute_native_size(widget, textEntryHandle, e,
						priv);
				w_event_compute_trim ee;
				w_rect rect, trim;
				ee.rect = &rect;
				ee.result = &trim;
				rect.x = rect.y = 0;
				rect.width = e->size->width;
				rect.height = e->size->height;
				_w_datetime_compute_trim(widget, &ee, priv);
				if (((style & W_DROP_DOWN) != 0 && (style & W_DATE) != 0)) {
					GtkWidget *down;
					int wHint = e->wHint;
					int hHint = e->hHint;
					e->wHint = e->hHint = W_DEFAULT;
					_w_control_compute_native_size(widget, down, e, priv);
					e->wHint = wHint;
					e->hHint = hHint;
					width = trim.width + e->size->width;
					height = WMAX(trim.height, e->size->height);
				} else if (((style & W_DATE) != 0) || ((style & W_TIME) != 0)) {
#if GTK3
					width = trim.width;
					height = trim.height;
#endif
				}
			}
		}
		if (width == 0)
			width = DEFAULT_WIDTH;
		if (height == 0)
			height = DEFAULT_HEIGHT;
		if (e->wHint != W_DEFAULT)
			width = e->wHint;
		if (e->hHint != W_DEFAULT)
			height = e->hHint;
		int borderWidth = _w_control_get_border_width(W_CONTROL(widget));
		e->size->width = width + 2 * borderWidth;
		e->size->height = height + 2 * borderWidth;
	}
	return TRUE;
}
wresult _w_datetime_create_handle(w_widget *widget, _w_control_priv *priv) {
	wuint64 style = _W_WIDGET(widget)->style;
	GtkWidget *fixedHandle = 0, *textEntryHandle = 0, *handle;
	if (style & W_CALENDAR) {
		_W_WIDGET(widget)->state |= STATE_HANDLE;
		fixedHandle = _w_fixed_new(widget);
		if (fixedHandle == 0)
			goto _err;
		gtk_widget_set_has_window(fixedHandle, TRUE);
		handle = gtk_calendar_new();
		if (handle == 0)
			goto _err;

		//Calenadar becomes container in this case.
		//containerHandle = handle;
		_w_fixed_set_child(fixedHandle, handle);

		GtkCalendarDisplayOptions flags = GTK_CALENDAR_SHOW_HEADING
				| GTK_CALENDAR_SHOW_DAY_NAMES;
		if ((style & W_CALENDAR_WEEKNUMBERS) != 0) {
			flags |= GTK_CALENDAR_SHOW_WEEK_NUMBERS;
		}
		gtk_calendar_set_display_options(GTK_CALENDAR(handle), flags);

	} else {
		fixedHandle = _w_fixed_new(widget);
		if (fixedHandle == 0)
			goto _err;
		gtk_widget_set_has_window(fixedHandle, TRUE);
		if (((style & W_DROP_DOWN) != 0 && (style & W_DATE) != 0)) {
			//Create box to put entry and button into box.
#if GTK3
			handle = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
			if (handle == 0)
				goto _err;
			gtk_box_set_homogeneous(GTK_BOX(handle), FALSE);
#endif
			_w_fixed_set_child(fixedHandle, handle);

			//Create entry
			textEntryHandle = gtk_entry_new();
			if (textEntryHandle == 0)
				goto _err;
			gtk_container_add(GTK_CONTAINER(handle), textEntryHandle);
			g_object_set_qdata(G_OBJECT(textEntryHandle), gtk_toolkit->quark[0],
					widget);

			//handle = containerHandle;

			// In GTK 3 font description is inherited from parent widget which is not how SWT has always worked,
			// reset to default font to get the usual behavior
#if GTK3
			//setFontDescription (defaultFont ().handle);
#endif
		} else {
			GtkAdjustment *adjusment = gtk_adjustment_new(0, -9999, 9999, 1, 0,
					0);
			textEntryHandle = gtk_spin_button_new(adjusment, 1, 0);
			if (textEntryHandle == 0)
				goto _err;

			//in this case,the Entry becomes the container.
			handle = textEntryHandle;
			//containerHandle = textEntryHandle;

			gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(textEntryHandle),
			FALSE);
			_w_fixed_set_child(fixedHandle, textEntryHandle);
			gtk_spin_button_set_wrap(GTK_SPIN_BUTTON(textEntryHandle),
					(style & W_WRAP) != 0);
		}
		gtk_editable_set_editable(GTK_EDITABLE(textEntryHandle),
				(style & W_READ_ONLY) == 0);
		if (GTK_VERSION <= VERSION(3, 20, 0)) {
			gtk_entry_set_has_frame(GTK_ENTRY(textEntryHandle),
					(style & W_BORDER) != 0);
		}
	}
	if (((style & W_TIME) != 0 && (style & W_DATE) != 0)) {
		char txt[20];
		_w_datetime_get_formatted_string(widget, txt);
		gtk_entry_set_text(GTK_ENTRY(textEntryHandle), txt);
	}
	if (((style & W_DROP_DOWN) != 0 && (style & W_DATE) != 0)) {
		//createDropDownButton();
		//createPopupShell(-1, -1, -1);
		//Date w/ drop down button is in containers.
		//first time round we set the bounds manually for correct Right_to_left behaviour
		w_size size;
		w_control_compute_size(W_CONTROL(widget), &size, W_DEFAULT, W_DEFAULT);
		w_control_set_bounds(W_CONTROL(widget), 0, &size);
	}
	_w_widget_set_control(handle, widget);
	_w_widget_set_control(fixedHandle, widget);
	_W_WIDGET(widget)->handle = handle;
	gtk_widget_show_all(fixedHandle);
	return W_TRUE;
	_err: return W_ERROR_NO_HANDLES;
}
void _w_datetime_get_entry_callback(GtkWidget *widget, gpointer data) {
	*((GtkWidget**) data) = widget;
}
GtkWidget* _w_datetime_get_entry(w_widget *widget) {
	wuint64 style = _W_WIDGET(widget)->style;
	if (style & W_CALENDAR) {
		return 0;
	} else {
		if (((style & W_DROP_DOWN) != 0 && (style & W_DATE) != 0)) {
			GtkWidget *h = 0;
			gtk_container_forall(GTK_CONTAINER(_W_WIDGET(widget)->handle),
					_w_datetime_get_entry_callback, &h);
			return h;
		} else {
			return _W_WIDGET(widget)->handle;
		}
	}
}
void _w_datetime_get_formatted_string(w_widget *widget, char *text) {
	wuint64 style = _W_WIDGET(widget)->style;
	if (style & W_TIME) {
		int h = _W_DATETIME(widget)->hours;
		if (h == 0)
			h = 12;
		int m = _W_DATETIME(widget)->minutes;
		int s = _W_DATETIME(widget)->seconds;
		if (style & W_SHORT) {
			sprintf(text, "%2d:%2d", h, m);
		} else {
			sprintf(text, "%2d:%2d:%2d", h, m, s);
		}
	} else if (style & W_DATE) {
		int y = _W_DATETIME(widget)->year;
		int m = _W_DATETIME(widget)->month + 1;
		int d = _W_DATETIME(widget)->day;
		if (style & W_SHORT) {
			sprintf(text, "%2d/%4d", m, y);
		} else {
			sprintf(text, "%2d/%2d/%4d", m, d, y);
		}
	}
}
wresult _w_datetime_get_day(w_datetime *datetime) {
	wuint64 style = _W_WIDGET(datetime)->style;
	if (style & W_CALENDAR) {
		GtkWidget *calendarHandle = _W_WIDGET(datetime)->handle;
		guint y, m, d;
		gtk_calendar_get_date(GTK_CALENDAR(calendarHandle), &y, &m, &d);
		return d;
	} else {
		return _W_DATETIME(datetime)->day;
	}
}
wresult _w_datetime_get_hours(w_datetime *datetime) {
	return _W_DATETIME(datetime)->hours;
}
wresult _w_datetime_get_minutes(w_datetime *datetime) {
	return _W_DATETIME(datetime)->minutes;
}
wresult _w_datetime_get_month(w_datetime *datetime) {
	wuint64 style = _W_WIDGET(datetime)->style;
	if (style & W_CALENDAR) {
		GtkWidget *calendarHandle = _W_WIDGET(datetime)->handle;
		guint y, m, d;
		gtk_calendar_get_date(GTK_CALENDAR(calendarHandle), &y, &m, &d);
		return m;
	} else {
		return _W_DATETIME(datetime)->month;
	}
}
wresult _w_datetime_get_seconds(w_datetime *datetime) {
	return _W_DATETIME(datetime)->seconds;
}
wresult _w_datetime_get_year(w_datetime *datetime) {
	wuint64 style = _W_WIDGET(datetime)->style;
	if (style & W_CALENDAR) {
		GtkWidget *calendarHandle = _W_WIDGET(datetime)->handle;
		guint y, m, d;
		gtk_calendar_get_date(GTK_CALENDAR(calendarHandle), &y, &m, &d);
		return y;
	} else {
		return _W_DATETIME(datetime)->year;
	}
}
wresult _w_datetime_set_date(w_datetime *datetime, int year, int month,
		int day) {
	wuint64 style = _W_WIDGET(datetime)->style;
	if (style & W_CALENDAR) {
		GtkWidget *calendarHandle = _W_WIDGET(datetime)->handle;
		guint y, m, d;
		gtk_calendar_select_month(GTK_CALENDAR(calendarHandle), month, year);
		gtk_calendar_select_day(GTK_CALENDAR(calendarHandle), day);
	} else {
		if (year >= 9999 || month >= 12 || day >= 32)
			return W_FALSE;
		_W_DATETIME(datetime)->year = year;
		_W_DATETIME(datetime)->month = month;
		_W_DATETIME(datetime)->day = day;
	}
	return W_TRUE;
}
void _w_datetime_hook_events(w_widget *widget, _w_control_priv *priv) {
	_w_composite_hook_events(widget, priv);
	wuint64 style = _W_WIDGET(widget)->style;
	_w_datetime_priv *tpriv = (_w_datetime_priv*) priv;
	if (style & W_CALENDAR) {
		GtkWidget *calendarHandle = _W_WIDGET(widget)->handle;
		for (int i = 0; i < 3; i++) {
			_w_widget_connect(calendarHandle, &tpriv->signals[i], FALSE);
		}
	} else {
		_gtk_signal *signals = gtk_toolkit->signals;
		GtkWidget *textEntryHandle = _w_datetime_get_entry(widget);
		int eventMask = GDK_POINTER_MOTION_MASK | GDK_BUTTON_PRESS_MASK
				| GDK_BUTTON_RELEASE_MASK;
		gtk_widget_add_events(textEntryHandle, eventMask);
		if ((style & W_DROP_DOWN) == 0) {
			_w_widget_connect(textEntryHandle, &signals[SIGNAL_OUTPUT], TRUE);
			_w_widget_connect(textEntryHandle, &signals[SIGNAL_FOCUS_IN_EVENT],
			TRUE);
		}
		if (G_OBJECT_TYPE (textEntryHandle) == gtk_menu_get_type()) {
			_w_widget_connect(_W_DATETIME(widget)->down,
					&signals[SIGNAL_FOCUS_IN_EVENT], TRUE);
		}
	}
}
wresult _w_datetime_set_day(w_datetime *datetime, int day) {
	wuint64 style = _W_WIDGET(datetime)->style;
	if (style & W_CALENDAR) {
		GtkWidget *calendarHandle = _W_WIDGET(datetime)->handle;
		gtk_calendar_select_day(GTK_CALENDAR(calendarHandle), day);
	} else {
		if (day >= 32)
			return W_FALSE;
		_W_DATETIME(datetime)->day = day;
		_w_datetime_update_control(datetime);
	}
	return W_TRUE;
}
wresult _w_datetime_set_hours(w_datetime *datetime, int hours) {
	if (hours >= 24)
		return W_FALSE;
	_W_DATETIME(datetime)->hours = hours;
	_w_datetime_update_control(datetime);
	return W_TRUE;
}
wresult _w_datetime_set_minutes(w_datetime *datetime, int minutes) {
	if (minutes >= 60)
		return W_FALSE;
	_W_DATETIME(datetime)->minutes = minutes;
	_w_datetime_update_control(datetime);
	return W_TRUE;
}
wresult _w_datetime_set_month(w_datetime *datetime, int month) {
	wuint64 style = _W_WIDGET(datetime)->style;
	if (style & W_CALENDAR) {
		GtkWidget *calendarHandle = _W_WIDGET(datetime)->handle;
		guint y, m, d;
		gtk_calendar_get_date(GTK_CALENDAR(calendarHandle), &y, &m, &d);
		gtk_calendar_select_month(GTK_CALENDAR(calendarHandle), month, y);
	} else {
		if (month >= 12)
			return W_FALSE;
		_W_DATETIME(datetime)->month = month;
		_w_datetime_update_control(datetime);
	}
	return W_TRUE;
}
wresult _w_datetime_set_seconds(w_datetime *datetime, int seconds) {
	if (seconds >= 60)
		return W_FALSE;
	_W_DATETIME(datetime)->seconds = seconds;
	_w_datetime_update_control(datetime);
	return W_TRUE;
}
wresult _w_datetime_set_time(w_datetime *datetime, int hours, int minutes,
		int seconds) {
	if (hours >= 24 || minutes >= 60 || seconds >= 60)
		return W_FALSE;
	_W_DATETIME(datetime)->hours = hours;
	_W_DATETIME(datetime)->minutes = minutes;
	_W_DATETIME(datetime)->seconds = seconds;
	return W_TRUE;
}
wresult _w_datetime_set_year(w_datetime *datetime, int year) {
	wuint64 style = _W_WIDGET(datetime)->style;
	if (style & W_CALENDAR) {
		GtkWidget *calendarHandle = _W_WIDGET(datetime)->handle;
		guint y, m, d;
		gtk_calendar_get_date(GTK_CALENDAR(calendarHandle), &y, &m, &d);
		gtk_calendar_select_month(GTK_CALENDAR(calendarHandle), m, year);
	} else {
		if (year >= 9999)
			return W_FALSE;
		_W_DATETIME(datetime)->year = year;
		_w_datetime_update_control(datetime);
	}
	return W_TRUE;
}
void _w_datetime_update_control(w_datetime *datetime) {

}
_gtk_signal_info _gtk_datetime_signal_lookup[_W_DATETIME_SIGNAL_COUNT] = { //
		{ SIGNAL_DAY_SELECTED, 2, "day-selected" }, //
				{ SIGNAL_DAY_SELECTED_DOUBLE_CLICK, 2,
						"day-selected-double-click" }, //
				{ SIGNAL_MONTH_CHANGED, 2, "month-changed" }, //
				{ SIGNAL_OUTPUT, 2, "output" } };
void _w_datetime_class_init(w_toolkit *toolkit, wushort classId,
		struct _w_datetime_class *clazz) {
	if (classId == _W_CLASS_DATETIME) {
		W_WIDGET_CLASS(clazz)->platformPrivate =
				&gtk_toolkit->class_datetime_priv;
	}
	_w_composite_class_init(toolkit, classId, W_COMPOSITE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_DATETIME;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_datetime_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_datetime);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_datetime);
	/*
	 * public function
	 */
	clazz->get_day = _w_datetime_get_day;
	clazz->get_hours = _w_datetime_get_hours;
	clazz->get_minutes = _w_datetime_get_minutes;
	clazz->get_month = _w_datetime_get_month;
	clazz->get_seconds = _w_datetime_get_seconds;
	clazz->get_year = _w_datetime_get_year;
	clazz->set_date = _w_datetime_set_date;
	clazz->set_day = _w_datetime_set_day;
	clazz->set_hours = _w_datetime_set_hours;
	clazz->set_minutes = _w_datetime_set_minutes;
	clazz->set_month = _w_datetime_set_month;
	clazz->set_seconds = _w_datetime_set_seconds;
	clazz->set_time = _w_datetime_set_time;
	clazz->set_year = _w_datetime_set_year;
	/*
	 * private
	 */
	_w_control_priv *priv = _W_CONTROL_PRIV(
			W_WIDGET_CLASS(clazz)->platformPrivate);
	if (_W_WIDGET_PRIV(priv)->init == 0) {
		if (classId == _W_CLASS_DATETIME) {
			_W_WIDGET_PRIV(priv)->init = 1;
		}
		priv->widget.handle_top = _w_widget_hp;
		priv->handle_fixed = _w_widget_hp;
		priv->widget.compute_size = _w_datetime_compute_size;
		priv->widget.compute_trim = _w_datetime_compute_trim;
		priv->widget.check_style = _w_datetime_check_style;
		priv->widget.create_handle = _w_datetime_create_handle;
		priv->widget.hook_events = _w_datetime_hook_events;
		_w_widget_init_signal(_W_DATETIME_PRIV(priv)->signals,
				_gtk_datetime_signal_lookup, _W_DATETIME_SIGNAL_COUNT);
		/*
		 * signals
		 */
		_gtk_signal_fn *signals = priv->widget.signals;
		signals[SIGNAL_CLICKED] = _gtk_button_clicked;
	}
}
