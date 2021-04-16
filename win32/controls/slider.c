/*
 * slider.c
 *
 *  Created on: 3 avr. 2021
 *      Author: azeddine
 */
#include "slider.h"
#include "../widgets/toolkit.h"
wuint64 _w_slider_check_style(w_widget *widget, wuint64 style) {
	return _w_widget_check_bits(style, W_HORIZONTAL, W_VERTICAL, 0, 0, 0, 0);
}
WNDPROC* _w_slider_get_def_window_proc(w_control *control,
		_w_control_priv *priv) {
	if (_W_WIDGET(control)->style & W_SCALE) {
		return &_W_SLIDER_PRIV(priv)->def_scale_proc;
	} else {
		return &priv->def_window_proc;
	}
}
wresult _w_slider_compute_size(w_widget *widget, w_event_compute_size *e,
		_w_control_priv *priv) {
	wuint64 style = _W_WIDGET(widget)->style;
	HWND handle = _W_WIDGET(widget)->handle;
	int wHint = e->wHint;
	int hHint = e->hHint;
	int border = W_CONTROL_GET_CLASS(widget)->get_border_width(
			W_CONTROL(widget));
	int width = border * 2, height = border * 2;
	RECT rect;
	if (style & W_SCALE) {
		SendMessageW(handle, TBM_GETTHUMBRECT, 0, (LPARAM) &rect);
	}
	if ((style & W_HORIZONTAL) != 0) {
		width += GetSystemMetrics(SM_CXHSCROLL) * 10;
		if (style & W_SCALE) {
			int scrollY = GetSystemMetrics(SM_CYHSCROLL);
			height += (rect.top * 2) + scrollY + (scrollY / 3);
		} else {
			height += GetSystemMetrics(SM_CYHSCROLL);
		}
	} else {
		if (style & W_SCALE) {
			int scrollX = GetSystemMetrics(SM_CXVSCROLL);
			width += (rect.left * 2) + scrollX + (scrollX / 3);
			height += GetSystemMetrics(SM_CYVSCROLL) * 10;
		} else {
			width += GetSystemMetrics(SM_CXVSCROLL);
			height += GetSystemMetrics(SM_CYVSCROLL) * 10;
		}
	}
	if (wHint != W_DEFAULT)
		width = wHint + (border * 2);
	if (hHint != W_DEFAULT)
		height = hHint + (border * 2);
	e->size->width = width;
	e->size->height = height;
	return W_TRUE;
}
wresult _w_slider_create_handle(w_control *control, _w_control_priv *priv) {
	wresult result;
	result = _w_control_create_handle(control, priv);
	if (result > 0) {
		wuint64 style = _W_WIDGET(control)->style;
		HWND handle = _W_WIDGET(control)->handle;

		if (style & W_SCALE) {
			_W_WIDGET(control)->state |= STATE_THEME_BACKGROUND
					| STATE_DRAW_BACKGROUND;
			SendMessageW(handle, TBM_SETRANGEMAX, 0, 100);
			SendMessageW(handle, TBM_SETPAGESIZE, 0, 10);
			SendMessageW(handle, TBM_SETTICFREQ, 10, 0);
		} else {
			_W_SLIDER(control)->increment = 1;
			_W_SLIDER(control)->pageIncrement = 10;
			/*
			 * Set the initial values of the maximum
			 * to 100 and the thumb to 10.  Note that
			 * info.nPage needs to be 11 in order to
			 * get a thumb that is 10.
			 */
			SCROLLINFO info;
			memset(&info, 0, sizeof(info));
			info.cbSize = sizeof(info);
			info.fMask = SIF_ALL;
			info.nMax = 100;
			info.nPage = 11;
			SetScrollInfo(handle, SB_CTL, &info, TRUE);
		}
	}
	return result;
}
wresult _w_slider_get_increment(w_slider *slider) {
	HWND handle = _W_WIDGET(slider)->handle;
	if (_W_WIDGET(slider)->style & W_SCALE) {
		return SendMessageW(handle, TBM_GETLINESIZE, 0, 0);
	} else {
		return _W_SLIDER(slider)->increment;
	}
	return W_TRUE;
}
wresult _w_slider_get_maximum(w_slider *slider) {
	HWND handle = _W_WIDGET(slider)->handle;
	if (_W_WIDGET(slider)->style & W_SCALE) {
		return SendMessageW(handle, TBM_GETRANGEMAX, 0, 0);
	} else {
		SCROLLINFO info;
		info.cbSize = sizeof(info);
		info.fMask = SIF_RANGE;
		GetScrollInfo(handle, SB_CTL, &info);
		return info.nMax;
	}
	return W_TRUE;
}
wresult _w_slider_get_minimum(w_slider *slider) {
	HWND handle = _W_WIDGET(slider)->handle;
	if (_W_WIDGET(slider)->style & W_SCALE) {
		return SendMessageW(handle, TBM_GETRANGEMIN, 0, 0);
	} else {
		SCROLLINFO info;
		info.cbSize = sizeof(info);
		info.fMask = SIF_RANGE;
		GetScrollInfo(handle, SB_CTL, &info);
		return info.nMin;
	}
	return W_TRUE;
}
wresult _w_slider_get_page_increment(w_slider *slider) {
	HWND handle = _W_WIDGET(slider)->handle;
	if (_W_WIDGET(slider)->style & W_SCALE) {
		return SendMessageW(handle, TBM_GETPAGESIZE, 0, 0);
	} else {
		return _W_SLIDER(slider)->pageIncrement;
	}
	return W_TRUE;
}
wresult _w_slider_get_selection(w_slider *slider) {
	HWND handle = _W_WIDGET(slider)->handle;
	if (_W_WIDGET(slider)->style & W_SCALE) {
		return SendMessageW(handle, TBM_GETPOS, 0, 0);
	} else {
		SCROLLINFO info;
		info.cbSize = sizeof(info);
		info.fMask = SIF_POS;
		GetScrollInfo(handle, SB_CTL, &info);
		return info.nPos;
	}
	return W_TRUE;
}
wresult _w_slider_get_thumb(w_slider *slider) {
	HWND handle = _W_WIDGET(slider)->handle;
	if (_W_WIDGET(slider)->style & W_SCALE) {
		return W_FALSE;
	} else {
		SCROLLINFO info;
		info.cbSize = sizeof(info);
		info.fMask = SIF_PAGE;
		GetScrollInfo(handle, SB_CTL, &info);
		if (info.nPage != 0)
			--info.nPage;
		return info.nPage;
	}
	return W_TRUE;
}
wresult _w_slider_get_values(w_slider *slider, w_slider_value *value) {
	HWND handle = _W_WIDGET(slider)->handle;
	if (_W_WIDGET(slider)->style & W_SCALE) {
		value->selection = SendMessageW(handle, TBM_GETPOS, 0, 0);
		value->minimum = SendMessageW(handle, TBM_GETRANGEMIN, 0, 0);
		value->maximum = SendMessageW(handle, TBM_GETRANGEMAX, 0, 0);
		value->thumb = 0;
		value->increment = _W_SLIDER(slider)->increment;
		value->pageIncrement = SendMessageW(handle, TBM_GETPAGESIZE, 0, 0);
	} else {
		SCROLLINFO info;
		info.cbSize = sizeof(info);
		info.fMask = SIF_POS | SIF_PAGE | SIF_RANGE | SIF_DISABLENOSCROLL;
		GetScrollInfo(handle, SB_CTL, &info);
		value->selection = info.nPos;
		value->minimum = info.nMin;
		value->maximum = info.nMax;
		value->thumb = info.nPage;
		value->increment = _W_SLIDER(slider)->increment;
		value->pageIncrement = _W_SLIDER(slider)->pageIncrement;
	}
	return W_TRUE;
}
int SetScrollInfo0(w_slider *slider, HWND hwnd, int nBar, LPCSCROLLINFO lpsi,
		WINBOOL redraw) {
	return SetScrollInfo(hwnd, nBar, lpsi, redraw);
}
wresult _w_slider_set_increment(w_slider *slider, int value) {
	HWND handle = _W_WIDGET(slider)->handle;
	if (_W_WIDGET(slider)->style & W_SCALE) {
		if (value < 1)
			return W_FALSE;
		int minimum = SendMessageW(handle, TBM_GETRANGEMIN, 0, 0);
		int maximum = SendMessageW(handle, TBM_GETRANGEMAX, 0, 0);
		if (value > maximum - minimum)
			return W_FALSE;
		SendMessageW(handle, TBM_SETLINESIZE, 0, value);
	} else {
		if (value < 1)
			return W_FALSE;
		_W_SLIDER(slider)->increment = value;
	}
	return W_TRUE;
}
wresult _w_slider_set_maximum(w_slider *slider, int value) {
	HWND handle = _W_WIDGET(slider)->handle;
	if (_W_WIDGET(slider)->style & W_SCALE) {
		int minimum = SendMessageW(handle, TBM_GETRANGEMIN, 0, 0);
		if (0 <= minimum && minimum < value) {
			SendMessageW(handle, TBM_SETRANGEMAX, 1, value);
		}
	} else {
		if (value < 0)
			return W_FALSE;
		SCROLLINFO info;
		info.cbSize = sizeof(info);
		info.fMask = SIF_RANGE | SIF_DISABLENOSCROLL;
		GetScrollInfo(handle, SB_CTL, &info);
		if (value - info.nMin - info.nPage < 1)
			return W_FALSE;
		info.nMax = value;
		SetScrollInfo0(slider, handle, SB_CTL, &info, TRUE);
	}
	return W_TRUE;
}
wresult _w_slider_set_minimum(w_slider *slider, int value) {
	HWND handle = _W_WIDGET(slider)->handle;
	if (_W_WIDGET(slider)->style & W_SCALE) {
		int maximum = SendMessageW(handle, TBM_GETRANGEMAX, 0, 0);
		if (0 <= value && value < maximum) {
			SendMessageW(handle, TBM_SETRANGEMIN, 1, value);
		}
	} else {
		if (value < 0)
			return W_FALSE;
		SCROLLINFO info;
		info.cbSize = sizeof(info);
		info.fMask = SIF_RANGE | SIF_DISABLENOSCROLL;
		GetScrollInfo(handle, SB_CTL, &info);
		if (info.nMax - value - info.nPage < 1)
			return W_FALSE;
		info.nMin = value;
		SetScrollInfo0(slider, handle, SB_CTL, &info, TRUE);
	}
	return W_TRUE;
}
wresult _w_slider_set_page_increment(w_slider *slider, int value) {
	HWND handle = _W_WIDGET(slider)->handle;
	if (_W_WIDGET(slider)->style & W_SCALE) {
		if (value < 1)
			return W_FALSE;
		int minimum = SendMessageW(handle, TBM_GETRANGEMIN, 0, 0);
		int maximum = SendMessageW(handle, TBM_GETRANGEMAX, 0, 0);
		if (value > maximum - minimum)
			return W_FALSE;
		SendMessageW(handle, TBM_SETPAGESIZE, 0, value);
		SendMessageW(handle, TBM_SETTICFREQ, value, 0);
	} else {
		if (value < 1)
			return W_FALSE;
		_W_SLIDER(slider)->pageIncrement = value;
	}
	return W_TRUE;
}
wresult _w_slider_set_selection(w_slider *slider, int value) {
	HWND handle = _W_WIDGET(slider)->handle;
	if (_W_WIDGET(slider)->style & W_SCALE) {
		SendMessageW(handle, TBM_SETPOS, 1, value);
	} else {
		SCROLLINFO info;
		info.cbSize = sizeof(info);
		info.fMask = SIF_POS;
		info.nPos = value;
		SetScrollInfo0(slider, handle, SB_CTL, &info, TRUE);
	}
	return W_TRUE;
}
wresult _w_slider_set_thumb(w_slider *slider, int value) {
	HWND handle = _W_WIDGET(slider)->handle;
	if (_W_WIDGET(slider)->style & W_SCALE) {
		return W_FALSE;
	} else {
		if (value < 1)
			return W_FALSE;
		SCROLLINFO info;
		info.cbSize = sizeof(info);
		info.fMask = SIF_PAGE | SIF_RANGE | SIF_DISABLENOSCROLL;
		GetScrollInfo(handle, SB_CTL, &info);
		info.nPage = value;
		if (info.nPage != 0)
			info.nPage++;
		SetScrollInfo0(slider, handle, SB_CTL, &info, TRUE);
	}
	return W_TRUE;
}
wresult _w_slider_set_values(w_slider *slider, w_slider_value *value) {
	if (value->minimum < 0)
		return W_FALSE;
	if (value->maximum < 0)
		return W_FALSE;
	if (value->thumb < 1)
		return W_FALSE;
	if (value->increment < 1)
		return W_FALSE;
	if (value->pageIncrement < 1)
		return W_FALSE;
	HWND handle = _W_WIDGET(slider)->handle;
	if (_W_WIDGET(slider)->style & W_SCALE) {
		if (value->selection > value->maximum - value->minimum)
			return W_FALSE;
		SendMessageW(handle, TBM_SETPOS, 1, value->selection);
		SendMessageW(handle, TBM_SETPAGESIZE, 0, value->pageIncrement);
		SendMessageW(handle, TBM_SETTICFREQ, value->pageIncrement, 0);
		SendMessageW(handle, TBM_SETRANGEMIN, 1, value->minimum);
		SendMessageW(handle, TBM_SETRANGEMAX, 1, value->maximum);
		SendMessageW(handle, TBM_SETLINESIZE, 0, value->increment);
	} else {
		_W_SLIDER(slider)->increment = value->increment;
		_W_SLIDER(slider)->pageIncrement = value->pageIncrement;
		SCROLLINFO info;
		info.cbSize = sizeof(info);
		info.fMask = SIF_POS | SIF_PAGE | SIF_RANGE | SIF_DISABLENOSCROLL;
		info.nPos = value->selection;
		info.nMin = value->minimum;
		info.nMax = value->maximum;
		info.nPage = value->thumb;
		if (info.nPage != 0)
			info.nPage++;
		SetScrollInfo0(slider, handle, SB_CTL, &info, TRUE);
	}
	return W_TRUE;
}
DWORD _w_slider_widget_style(w_control *control, _w_control_priv *priv) {
	DWORD bits = _w_control_widget_style(control, priv);
	wuint64 style = _W_WIDGET(control)->style;
	if (style & W_SCALE) {
		bits |= WS_TABSTOP | TBS_BOTH | TBS_AUTOTICKS;
		if ((style & W_HORIZONTAL) != 0)
			return bits | TBS_HORZ | TBS_DOWNISLEFT;
		return bits | TBS_VERT;
	} else {
		bits |= WS_TABSTOP;
		/*
		 * Bug in Windows.  If a scroll bar control is given a border,
		 * dragging the scroll bar thumb eats away parts of the border
		 * while the thumb is dragged.  The fix is to clear WS_BORDER.
		 */
		if ((style & W_BORDER) != 0)
			bits &= ~WS_BORDER;
		if ((style & W_HORIZONTAL) != 0)
			return bits | SBS_HORZ;
		return bits | SBS_VERT;
	}
}
DWORD _w_slider_widget_extstyle(w_control *control, _w_control_priv *priv) {
	DWORD bits = _w_control_widget_extstyle(control, priv);
	wuint64 style = _W_WIDGET(control)->style;
	if ((style & W_SCALE) == 0) {
		/*
		 * Bug in Windows.  If a scroll bar control is given a border,
		 * dragging the scroll bar thumb eats away parts of the border
		 * while the thumb is dragged.  The fix is to clear border for
		 * all scroll bars.
		 */
		if ((style & W_BORDER) != 0)
			bits &= ~WS_EX_CLIENTEDGE;
	}
	return bits;
}
const char* _w_slider_window_class(w_control *control, _w_control_priv *priv) {
	if (_W_WIDGET(control)->style & W_SCALE) {
		return TRACKBAR_CLASS;
	} else {
		return WC_SCROLLBARA;
	}
}
/*
 * messages
 */
wresult _SLIDER_WM_SCROLLCHILD(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	w_event_selection event;
	if (_W_WIDGET(widget)->style & W_SCALE) {
		/* Do nothing when scrolling is ending */
		int code = LOWORD(e->wparam);
		switch (code) {
		case TB_ENDTRACK:
		case TB_THUMBPOSITION:
			return W_FALSE;
		}

		if ((_W_WIDGET(widget)->state & STATE_SLIDER_IGNORE_SELECTION) == 0) {
			/*
			 * This code is intentionally commented.  The event
			 * detail field is not currently supported on all
			 * platforms.
			 */
//		switch (code) {
//			case TB_TOP: 		event.detail = W_HOME;  break;
//			case TB_BOTTOM:		event.detail = W_END;  break;
//			case TB_LINEDOWN:	event.detail = W_ARROW_DOWN;  break;
//			case TB_LINEUP: 		event.detail = W_ARROW_UP;  break;
//			case TB_PAGEDOWN: 	event.detail = W_PAGE_DOWN;  break;
//			case TB_PAGEUP: 		event.detail = W_PAGE_UP;  break;
//		}
			/*
			 * Send the event because WM_HSCROLL and WM_VSCROLL
			 * are sent from a modal message loop in windows that
			 * is active when the user is scrolling.
			 */
			event.event.type = W_EVENT_SELECTION;
			event.event.widget = widget;
			event.event.time = 0;
			event.event.platform_event = _EVENT_PLATFORM(e);
			event.event.data = 0;
			event.detail = 0;
			_w_widget_send_event(widget, (w_event*) &event);
			// widget could be disposed at this point
		}
		return W_FALSE;
	} else {

		/* Do nothing when scrolling is ending */
		int code = LOWORD(e->wparam);
		if (code == SB_ENDSCROLL)
			return W_FALSE;

		HWND handle = _W_WIDGET(widget)->handle;
		event.detail = 0;

		/* Move the thumb */
		SCROLLINFO info;
		info.cbSize = sizeof(info);
		info.fMask = SIF_TRACKPOS | SIF_POS | SIF_RANGE;
		GetScrollInfo(handle, SB_CTL, &info);
		info.fMask = SIF_POS;
		switch (code) {
		case SB_THUMBPOSITION:
			event.detail = W_NONE;
			info.nPos = info.nTrackPos;
			break;
		case SB_THUMBTRACK:
			event.detail = W_DRAG;
			info.nPos = info.nTrackPos;
			break;
		case SB_TOP:
			event.detail = W_HOME;
			info.nPos = info.nMin;
			break;
		case SB_BOTTOM:
			event.detail = W_END;
			info.nPos = info.nMax;
			break;
		case SB_LINEDOWN:
			event.detail = W_ARROW_DOWN;
			info.nPos += _W_SLIDER(widget)->increment;
			break;
		case SB_LINEUP:
			event.detail = W_ARROW_UP;
			info.nPos = WMAX(info.nMin,
					info.nPos - _W_SLIDER(widget)->increment);
			break;
		case SB_PAGEDOWN:
			event.detail = W_PAGE_DOWN;
			info.nPos += _W_SLIDER(widget)->pageIncrement;
			break;
		case SB_PAGEUP:
			event.detail = W_PAGE_UP;
			info.nPos = WMAX(info.nMin,
					info.nPos - _W_SLIDER(widget)->pageIncrement);
			break;
		}
		SetScrollInfo(handle, SB_CTL, &info, TRUE);

		/*
		 * Feature in Windows.  Windows runs a modal message
		 * loop when the user drags a scroll bar.  This means
		 * that selection event must be sent because WM_HSCROLL
		 * and WM_VSCROLL are sent from the modal message loop
		 * so that they are delivered during inside the loop.
		 */
		event.event.type = W_EVENT_SELECTION;
		event.event.widget = widget;
		event.event.time = 0;
		event.event.platform_event = _EVENT_PLATFORM(e);
		event.event.data = 0;
		event.detail = 0;
		_w_widget_send_event(widget, (w_event*) &event);
		// the widget could be destroyed at this point
		return W_FALSE;
	}
}
void _w_slider_class_init(struct _w_slider_class *clazz) {
	_w_control_class_init(W_CONTROL_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_SLIDER;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_slider_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_slider);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_slider);
	/*
	 * public function
	 */
	clazz->get_increment = _w_slider_get_increment;
	clazz->get_maximum = _w_slider_get_maximum;
	clazz->get_minimum = _w_slider_get_minimum;
	clazz->get_page_increment = _w_slider_get_page_increment;
	clazz->get_selection = _w_slider_get_selection;
	clazz->set_increment = _w_slider_set_increment;
	clazz->set_maximum = _w_slider_set_maximum;
	clazz->set_minimum = _w_slider_set_minimum;
	clazz->set_page_increment = _w_slider_set_page_increment;
	clazz->set_selection = _w_slider_set_selection;
	/*
	 * private
	 */
	_w_control_priv *priv = _W_CONTROL_PRIV(W_WIDGET_CLASS(clazz)->reserved[0]);
	priv->check_style = _w_slider_check_style;
	priv->compute_size = _w_slider_compute_size;
	priv->create_handle = _w_slider_create_handle;
	priv->widget_style = _w_slider_widget_style;
	priv->widget_extstyle = _w_slider_widget_extstyle;
	priv->window_class = _w_slider_window_class;
	priv->get_def_window_proc = _w_slider_get_def_window_proc;
	/*
	 * messages
	 */
	dispatch_message *msg = priv->messages;
	msg[_WM_SCROLLCHILD] = _SLIDER_WM_SCROLLCHILD;

}
