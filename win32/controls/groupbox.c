/*
 * groupbox.c
 *
 *  Created on: Mar 16, 2021
 *      Author: azeddine
 */
#include "groupbox.h"
#include "../widgets/toolkit.h"
#define CLIENT_INSET 3
wuint64 _w_groupbox_check_style(w_widget *widget, wuint64 style) {
	style |= W_NO_FOCUS;
	/*
	 * Even though it is legal to create this widget
	 * with scroll bars, they serve no useful purpose
	 * because they do not automatically scroll the
	 * widget's client area.  The fix is to clear
	 * the SWT style.
	 */
	return style & ~(W_HSCROLL | W_VSCROLL);
}
wresult _w_groupbox_get_text(w_groupbox *group, w_alloc alloc, void *user_data,
		int enc) {
	HWND handle = _W_WIDGET(group)->handle;
	wresult result = W_FALSE;
	int l = GetWindowTextLengthW(handle);
	WCHAR *str = _w_toolkit_malloc((l + 1) * sizeof(WCHAR));
	if (str != 0) {
		GetWindowTextW(handle, str, (l + 1));
		_win_text_set(str, (l + 1), alloc, user_data, enc);
		result = W_TRUE;
	}
	_w_toolkit_free(str, (l + 1) * sizeof(WCHAR));
	return result;
}
wresult _w_groupbox_set_text(w_groupbox *group, const char *string, int length,
		int enc) {
	WCHAR *str;
	int newlength;
	wresult result = _win_text_fix(string, length, enc, &str, &newlength);
	if (result > 0) {
		SetWindowTextW(_W_WIDGET(group)->handle, str);
	}
	_win_text_free(string, str, newlength);
	return result;
}
wresult _w_groupbox_get_client_area(w_widget *widget, w_event_client_area *e,
		_w_control_priv *priv) {
	RECT rect;
	GetClientRect(_W_WIDGET(widget)->handle, &rect);
	HFONT newFont, oldFont = 0;
	HDC hDC = GetDC(_W_WIDGET(widget)->handle);
	newFont = (HFONT) SendMessageW(_W_WIDGET(widget)->handle, WM_GETFONT, 0, 0);
	if (newFont != 0)
		oldFont = SelectObject(hDC, newFont);
	TEXTMETRICW tm;
	GetTextMetricsW(hDC, &tm);
	if (newFont != 0)
		SelectObject(hDC, oldFont);
	ReleaseDC(_W_WIDGET(widget)->handle, hDC);
	int offsetY = _COMCTL32_VERSION >= VERSION(6, 0) && IsAppThemed() ? 0 : 1;
	e->rect->x = CLIENT_INSET;
	e->rect->y = tm.tmHeight + offsetY;
	e->rect->width = WMAX(0, rect.right - CLIENT_INSET * 2);
	e->rect->height = WMAX(0, rect.bottom - e->rect->y - CLIENT_INSET);
	return W_TRUE;
}
wresult _w_groupbox_compute_size(w_widget *widget, w_event_compute_size *e,
		_w_control_priv *priv) {
	wresult ret = _w_composite_compute_size(widget, e, priv);
	HWND handle = _W_WIDGET(widget)->handle;
	int length = GetWindowTextLengthW(handle);
	if (length != 0) {
		WCHAR *buffer;
		buffer = _w_toolkit_malloc((length + 3) * sizeof(WCHAR));
		if (buffer != 0) {
			GetWindowTextW(handle, buffer, length + 3);
			//String string = fixText(false);

			/*
			 * If the group has text, and the text is wider than the
			 * client area, pad the width so the text is not clipped.
			 */
			HFONT newFont, oldFont = 0;
			HDC hDC = GetDC(handle);
			newFont = (HFONT) SendMessageW(handle, WM_GETFONT, 0, 0);
			if (newFont != 0)
				oldFont = SelectObject(hDC, newFont);
			RECT rect;
			int flags = DT_CALCRECT | DT_SINGLELINE;
			DrawTextW(hDC, buffer, -1, &rect, flags);
			if (newFont != 0)
				SelectObject(hDC, oldFont);
			ReleaseDC(handle, hDC);
			int offsetY =
			_COMCTL32_VERSION >= VERSION(6, 0) && IsAppThemed() ? 0 : 1;
			e->size->width = WMAX(e->size->width,
					rect.right - rect.left + CLIENT_INSET * 6 + offsetY);
		}
		_w_toolkit_free(buffer, (length + 3) * sizeof(WCHAR));
	}
	return ret;
}
wresult _w_groupbox_compute_trim(w_widget *widget, w_event_compute_trim *e,
		_w_control_priv *priv) {
	int ret = _w_scrollable_compute_trim(widget, e, priv);
	HWND handle = _W_WIDGET(widget)->handle;
	HFONT newFont, oldFont = 0;
	HDC hDC = GetDC(handle);
	newFont = (HFONT) SendMessageW(handle, WM_GETFONT, 0, 0);
	if (newFont != 0)
		oldFont = SelectObject(hDC, newFont);
	TEXTMETRICW tm;
	GetTextMetricsW(hDC, &tm);
	if (newFont != 0)
		SelectObject(hDC, oldFont);
	ReleaseDC(handle, hDC);
	int offsetY = _COMCTL32_VERSION >= VERSION(6, 0) && IsAppThemed() ? 0 : 1;
	e->result->x -= CLIENT_INSET;
	e->result->y -= tm.tmHeight + offsetY;
	e->result->width += CLIENT_INSET * 2;
	e->result->height += tm.tmHeight + CLIENT_INSET + offsetY;
	return ret;
}
DWORD _w_groupbox_widget_style(w_control *control, _w_control_priv *priv) {
	DWORD bits = _w_control_widget_style(control, priv);
	/*
	 * Bug in Windows.  When GetDCEx() is called with DCX_INTERSECTUPDATE,
	 * the HDC that is returned does not include the current update region.
	 * This was confirmed under DEBUG Windows when GetDCEx() complained about
	 * invalid flags.  Therefore, it is not easily possible to get an HDC from
	 * outside of WM_PAINT that includes the current damage and clips children.
	 * Because the receiver has children and draws a frame and label, it is
	 * necessary that the receiver always draw clipped, in the current damaged
	 * area.  The fix is to force the receiver to be fully clipped by including
	 * WS_CLIPCHILDREN and WS_CLIPSIBLINGS in the default style bits.
	 */
	return bits | BS_GROUPBOX | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
}

const char* _w_groupbox_window_class(w_control *control,
		_w_control_priv *priv) {
	return WC_BUTTONA;
}
/*
 * messages
 */
wresult _GROUP_WM_ERASEBKGND(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	_COMPOSITE_WM_ERASEBKGND(widget, e, priv);
	RECT rect;
	GetClientRect(_W_WIDGET(widget)->handle, &rect);
	DrawThemeBackground(NULL, (HDC) e->wparam, 0, 0, &rect, NULL);
	e->result = 1;
	return W_TRUE;
}
void _w_groupbox_class_init(struct _w_groupbox_class *clazz) {
	_w_composite_class_init(W_COMPOSITE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_GROUPBOX;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_groupbox_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_groupbox);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_groupbox);
	/*
	 * public function
	 */
	clazz->get_text = _w_groupbox_get_text;
	clazz->set_text = _w_groupbox_set_text;
	/*
	 * private
	 */
	_w_control_priv *priv = _W_CONTROL_PRIV(W_WIDGET_CLASS(clazz)->reserved[0]);
	priv->check_style = _w_groupbox_check_style;
	priv->get_client_area = _w_groupbox_get_client_area;
	priv->compute_trim = _w_groupbox_compute_trim;
	priv->compute_size = _w_groupbox_compute_size;
	priv->widget_style = _w_groupbox_widget_style;
	priv->window_class = _w_groupbox_window_class;
	/*
	 * messages
	 */
	dispatch_message *msg = priv->messages;
	//msg[_WM_ERASEBKGND] = _GROUP_WM_ERASEBKGND;
}
