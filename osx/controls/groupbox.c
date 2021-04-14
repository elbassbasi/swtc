/*
 * groupbox.c
 *
 *  Created on: Mar 16, 2021
 *      Author: azeddine
 */
#include "groupbox.h"
#include "../widgets/toolkit.h"
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
NSView* _w_groupbox_content_view(w_widget *widget, _w_control_priv *priv) {
	return NSBox_contentView((NSBox*) _W_WIDGET(widget)->handle);
}
wresult _w_groupbox_create_handle(w_widget *widget, _w_control_priv *priv) {
	_W_WIDGET(widget)->state |= STATE_THEME_BACKGROUND;
	NSBox *handle = SWTBox_new(widget);
	NSBox_setTitlePosition(handle, NSNoTitle);
	NSSize margins, size;
	NSBox_contentViewMargins(handle, &margins);
	_W_GROUPBOX(widget)->hMargin = (int) margins.width;
	_W_GROUPBOX(widget)->vMargin = (int) margins.height;
	size.width = 0;
	size.height = 0;
	NSBox_setContentViewMargins(handle, &size);

	NSView *contentWidget = SWTView_new(widget);
//	contentWidget.setDrawsBackground(false);
	NSBox_setContentView(handle, contentWidget);
	_W_WIDGET(widget)->handle = handle;
	return W_TRUE;
}
wresult _w_groupbox_get_text(w_groupbox *group, w_alloc alloc, void *user_data,
		int enc) {
	return W_TRUE;
}
wresult _w_groupbox_set_text(w_groupbox *group, const char *string, int length,
		int enc) {
	NSBox *box = _W_WIDGET(group)->handle;
	NSString *str = NSString_new(string, length, enc);
	NSBox_setTitlePosition(box, str == 0 ? NSNoTitle : NSAtTop);
	if (str != 0) {
		NSBox_setTitle(box, str);
		return W_TRUE;
	}
	return W_FALSE;
}
wresult _w_groupbox_get_client_area(w_widget *widget, w_event_client_area *e,
		_w_control_priv *priv) {
	NSView *contentView = NSBox_contentView((NSBox*) _W_WIDGET(widget)->handle);
	NSRect rect;
	NSView_bounds(contentView, &rect);
	int hMargin = _W_GROUPBOX(widget)->hMargin;
	int vMargin = _W_GROUPBOX(widget)->vMargin;
	w_rect *result = e->rect;
	result->x = (int) (rect.x) + hMargin;
	result->y = (int) (rect.y) + vMargin;
	result->width = WMAX(0, (int ) rect.width - hMargin * 2);
	result->height = WMAX(0, (int ) rect.height - vMargin * 2);
	int width = WMAX(0, (int ) rect.width - hMargin * 2);
	int height = WMAX(0, (int ) rect.height - vMargin * 2);
	return W_TRUE;
}
wresult _w_groupbox_compute_size(w_widget *widget, w_event_compute_size *e,
		_w_control_priv *priv) {
	return _w_composite_compute_size(widget, e, priv);
}
wresult _w_groupbox_compute_trim(w_widget *widget, w_event_compute_trim *e,
		_w_control_priv *priv) {
	NSView *box = _W_WIDGET(widget)->handle;
	w_rect *rect = e->rect;
	w_rect *result = e->result;
	w_rect tmp;
	NSRect newRect, oldRect;
	newRect.x = rect->x;
	newRect.y = rect->y;
	newRect.width = rect->width;
	newRect.height = rect->height;
	NSView_frame(box, &oldRect);
	_W_GROUPBOX(widget)->ignoreResize = W_TRUE;
	NSBox_setFrameFromContentFrame((NSBox*) box, &newRect);
	NSView_frame(box, &newRect);
	NSView_setFrame(box, &oldRect);
	_W_GROUPBOX(widget)->ignoreResize = W_FALSE;
	int hMargin = _W_GROUPBOX(widget)->hMargin;
	int vMargin = _W_GROUPBOX(widget)->vMargin;
	tmp.x = (int) ceil(newRect.x) - hMargin;
	tmp.y = (int) ceil(newRect.y) - vMargin;
	tmp.width = (int) ceil(newRect.width) + (hMargin * 2);
	tmp.height = (int) ceil(newRect.height) + (vMargin * 2);
	e->rect = &tmp;
	wresult ret = _w_scrollable_compute_trim(widget, e, priv);
	e->rect = rect;
	return ret;
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
	 * priv
	 */
	_w_control_priv *priv = _W_CONTROL_PRIV(W_WIDGET_CLASS(clazz)->reserved[0]);
	_W_WIDGET_PRIV(priv)->check_style = _w_groupbox_check_style;
	_W_WIDGET_PRIV(priv)->create_handle = _w_groupbox_create_handle;
	_W_WIDGET_PRIV(priv)->get_client_area = _w_groupbox_get_client_area;
	_W_WIDGET_PRIV(priv)->compute_trim = _w_groupbox_compute_trim;
	_W_WIDGET_PRIV(priv)->compute_size = _w_groupbox_compute_size;
	priv->content_view = _w_groupbox_content_view;
}
