/*
 * label.c
 *
 *  Created on: 12 mars 2021
 *      Author: Azeddine
 */
#include "label.h"
#include "../widgets/toolkit.h"
#define MARGIN 4
WNDPROC* _w_label_get_def_window_proc(w_control *control,
		_w_control_priv *priv) {
	if (_W_WIDGET(control)->style & W_HYPERLINK) {
		return &_W_LABEL_PRIV(priv)->def_link_proc;
	} else {
		return &priv->def_window_proc;
	}
}
wresult _w_label_get_image(w_label *label, w_image *image) {
	if (image == 0)
		return W_ERROR_NULL_ARGUMENT;
	if ((_W_WIDGET(label)->style & W_SEPARATOR) != 0)
		return W_FALSE;
	return W_TRUE;
}
wresult _w_label_get_text(w_label *label, w_alloc string, void *user_data,
		int enc) {
	if (string == 0)
		return W_ERROR_NULL_ARGUMENT;
	if ((_W_WIDGET(label)->style & W_SEPARATOR) != 0)
		return W_TRUE;
	return W_TRUE;
}
wresult _w_label_set_image(w_label *label, w_image *image) {
	if ((_W_WIDGET(label)->style & W_SEPARATOR) != 0)
		return W_FALSE;
	return W_TRUE;
}
wresult _w_label_set_text(w_label *label, const char *text, int length,
		int enc) {
	if (text == 0)
		return W_ERROR_NULL_ARGUMENT;
	if ((_W_WIDGET(label)->style & W_SEPARATOR) != 0)
		return W_TRUE;
	if (text == 0)
		text = "";
	WCHAR *str;
	int newlength;
	BOOL result = FALSE;
	_win_text_fix(text, length, enc, &str, &newlength);
	if (str != 0) {
		result = SetWindowTextW(_W_WIDGET(label)->handle, str);
	}
	_win_text_free(text, str, newlength);
	if (result)
		return W_TRUE;
	else
		return W_ERROR_CANNOT_SET_TEXT;
	return W_TRUE;
}
wuint64 _w_label_check_style(w_widget *widget, wuint64 style) {
	style |= W_NO_FOCUS;
	if ((style & W_SEPARATOR) != 0) {
		style = _w_widget_check_bits(style, W_VERTICAL, W_HORIZONTAL, 0, 0, 0,
				0);
		return _w_widget_check_bits(style, W_SHADOW_OUT, W_SHADOW_IN,
				W_SHADOW_NONE, 0, 0, 0);
	}
	return _w_widget_check_bits(style, W_LEFT, W_CENTER, W_RIGHT, 0, 0, 0);
}
wresult _w_label_compute_size(w_widget *widget, w_event_compute_size *e,
		_w_control_priv *priv) {
	int wHint = e->wHint;
	int hHint = e->hHint;
	int width = 0, height = 0;
	int border = _w_control_get_border_width(W_CONTROL(widget));
	if ((_W_WIDGET(widget)->style & W_SEPARATOR) != 0) {
		int lineWidth = GetSystemMetrics(SM_CXBORDER);
		if ((_W_WIDGET(widget)->style & W_HORIZONTAL) != 0) {
			width = DEFAULT_WIDTH;
			height = lineWidth * 2;
		} else {
			width = lineWidth * 2;
			height = DEFAULT_HEIGHT;
		}
		if (wHint != W_DEFAULT)
			width = wHint;
		if (hHint != W_DEFAULT)
			height = hHint;
		width += border * 2;
		height += border * 2;
		e->size->width = width;
		e->size->height = height;
		return W_TRUE;
	}
	DWORD bits = GetWindowLong(_W_WIDGET(widget)->handle, GWL_STYLE);
	int drawText = TRUE;
	int drawImage = (bits & SS_OWNERDRAW) == SS_OWNERDRAW;
	if (drawImage) {
		if (_W_LABEL(widget)->image != 0) {
			BITMAP bm;
			GetObjectW(_W_LABEL(widget)->image, sizeof(bm), &bm);
			width += bm.bmWidth;
			height += bm.bmHeight;
			if (_W_WIDGET(widget)->state & STATE_LABEL_IMAGE_AND_TEXT) {
				if (GetWindowTextLengthW(_W_WIDGET(widget)->handle) != 0)
					width += MARGIN;
			} else {
				drawText = FALSE;
			}
		}
	}
	if (drawText) {
		HDC hDC = GetDC(_W_WIDGET(widget)->handle);
		HFONT newFont = (HFONT) SendMessageW(_W_WIDGET(widget)->handle,
		WM_GETFONT, 0, 0);
		HFONT oldFont = SelectObject(hDC, newFont);
		int length = GetWindowTextLengthW(_W_WIDGET(widget)->handle);
		if (length == 0) {
			TEXTMETRICW tm;
			GetTextMetricsW(hDC, &tm);
			height = WMAX(height, tm.tmHeight);
		} else {
			RECT rect;
			memset(&rect, 0, sizeof(RECT));
			int flags = DT_CALCRECT | DT_EDITCONTROL | DT_EXPANDTABS;
			if ((_W_WIDGET(widget)->style & W_WRAP) != 0
					&& wHint != W_DEFAULT) {
				flags |= DT_WORDBREAK;
				rect.right = WMAX(0, wHint - width);
			}
			WCHAR *buffer = _w_toolkit_malloc((length + 1) * sizeof(WCHAR));
			if (buffer != 0) {
				GetWindowTextW(_W_WIDGET(widget)->handle, buffer, length + 1);
				DrawTextW(hDC, buffer, length, &rect, flags);
			}
			_w_toolkit_free(buffer, (length + 1) * sizeof(WCHAR));
			width += rect.right - rect.left;
			height = WMAX(height, rect.bottom - rect.top);
		}
		if (newFont != 0)
			SelectObject(hDC, oldFont);
		ReleaseDC(_W_WIDGET(widget)->handle, hDC);
	}
	if (wHint != W_DEFAULT)
		width = wHint;
	if (hHint != W_DEFAULT)
		height = hHint;
	width += border * 2;
	height += border * 2;
	e->size->width = width;
	e->size->height = height;
	return W_TRUE;
}
DWORD _w_label_widget_extstyle(w_control *control, _w_control_priv *priv) {
	DWORD bits = _w_control_widget_extstyle(control, priv) & ~WS_EX_CLIENTEDGE;
	if ((_W_WIDGET(control)->style & W_BORDER) != 0)
		return bits | WS_EX_STATICEDGE;
	return bits;
}
DWORD _w_label_widget_style(w_control *control, _w_control_priv *priv) {
	DWORD bits = _w_control_widget_style(control, priv);
	wuint64 style = _W_WIDGET(control)->style;
	if (style & W_HYPERLINK) {
		bits |= WS_TABSTOP;
	} else {
		bits |= SS_NOTIFY;
		if ((style & W_SEPARATOR) != 0)
			return bits | SS_OWNERDRAW;
		if (WIN32_VERSION >= VERSION(5, 0)) {
			if ((style & W_WRAP) != 0)
				bits |= SS_EDITCONTROL;
		}
		if ((style & W_CENTER) != 0)
			return bits | SS_CENTER;
		if ((style & W_RIGHT) != 0)
			return bits | SS_RIGHT;
		if ((style & W_WRAP) != 0)
			return bits | SS_LEFT;
		bits |= SS_LEFTNOWORDWRAP;
	}
	return bits;
}
const char* _w_label_window_class(w_control *control, _w_control_priv *priv) {
	wuint64 style = _W_WIDGET(control)->style;
	if (style & W_HYPERLINK) {
		return "SysLink";
	} else {
		return WC_STATICA;
	}
}
void _w_label_class_init(struct _w_label_class *clazz) {
	_w_control_class_init(W_CONTROL_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_LABEL;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_label_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_label);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_label);
	/*
	 * public function
	 */
	clazz->get_image = _w_label_get_image;
	clazz->get_text = _w_label_get_text;
	clazz->set_image = _w_label_set_image;
	clazz->set_text = _w_label_set_text;
	/*
	 * private
	 */
	_w_control_priv *priv = _W_CONTROL_PRIV(W_WIDGET_CLASS(clazz)->reserved[0]);
	priv->compute_size = _w_label_compute_size;
	priv->widget_extstyle = _w_label_widget_extstyle;
	priv->widget_style = _w_label_widget_style;
	priv->window_class = _w_label_window_class;
	priv->get_def_window_proc = _w_label_get_def_window_proc;
}

