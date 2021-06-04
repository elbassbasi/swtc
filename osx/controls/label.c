/*
 * label.c
 *
 *  Created on: Mar 16, 2021
 *      Author: azeddine
 */
#include "label.h"
#include "../widgets/toolkit.h"
#include <float.h>

wresult _w_label_get_image(w_label *label, w_image *image) {
	if (image == 0)
		return W_ERROR_NULL_ARGUMENT;
	if ((_W_WIDGET(label)->style & (W_SEPARATOR | W_HYPERLINK)) != 0)
		return W_FALSE;
	return W_TRUE;
}
wresult _w_label_get_text(w_label *label, w_alloc string, void *user_data,
		int enc) {
	wuint64 style = _W_WIDGET(label)->style;
	NSView *handle = _W_WIDGET(label)->handle;
	wresult result;
	if (style & W_HYPERLINK) {
		result = W_TRUE;
	} else if ((style & W_SEPARATOR) != 0) {
		result = W_FALSE;
	} else {
		NSView *textView = NSBox_contentView((NSBox*) handle);
		NSCell *cell = NSControl_cell((NSControl*) textView);
		NSAttributedString* attributedTitle = NSCell_attributedStringValue(cell);
		NSString* str = NSAttributedString_string(attributedTitle);
		_w_toolkit_set_text(str, string, user_data, enc);
	}
	return result;
}
wresult _w_label_set_image(w_label *label, w_image *image) {
	if ((_W_WIDGET(label)->style & (W_SEPARATOR | W_HYPERLINK)) != 0)
		return W_FALSE;
	return W_TRUE;
}
wresult _w_label_set_text(w_label *label, const char *text, int length,
		int enc) {
	wuint64 style = _W_WIDGET(label)->style;
	NSView *handle = _W_WIDGET(label)->handle;
	if (style & W_HYPERLINK) {

	} else if ((style & W_SEPARATOR) != 0) {
	} else {
		NSView *textView = NSBox_contentView((NSBox*) handle);
		NSCell *cell = NSControl_cell((NSControl*) textView);
		NSString *str = NSString_new(text, length, enc);
		NSAttributedString *attributedTitle = _w_control_create_string(
				W_CONTROL(label), str, 0, _W_CONTROL(label)->foreground,
				_W_WIDGET(label)->style);
		NSCell_setAttributedStringValue(cell, attributedTitle);
		NSBox_setContentView((NSBox*) handle, textView);
	}
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
wresult _w_label_create_handle(w_widget *widget, _w_control_priv *priv) {
	wuint64 style = _W_WIDGET(widget)->style;
	if (style & W_HYPERLINK) {

	} else if ((style & W_SEPARATOR) != 0) {
		_W_WIDGET(widget)->state |= STATE_THEME_BACKGROUND;
		NSView *handle = SWTBox_new(widget);
		/*
		 * Feature in Cocoa: Separator control decides how to orient itself
		 * based on the width and height. If height > width it orients
		 * vertically, else it orients horizontally.
		 * Fix is to have two native controls to implement the separator label.
		 * The top control (Custom NSBox) honors the bounds set by the
		 * user and the inner one (Separator NSBox) creates the separator
		 * with the correct orientation.
		 */
		NSRect rect;
		rect.x = 0;
		rect.y = 0;
		rect.width = DEFAULT_WIDTH;
		rect.height = DEFAULT_HEIGHT;

		NSView_setFrame(handle, &rect);
		NSString *str = mac_toolkit->emptyString;
		NSBox_setTitle((NSBox*) handle, str);
		NSBox_setBorderType((NSBox*) handle, NSNoBorder);
		NSBox_setBoxType((NSBox*) handle, NSBoxCustom);
		rect.size.width = 0;
		rect.size.height = 0;
		NSBox_setContentViewMargins((NSBox*) handle, &rect.size);

		CGFloat lineWidth = NSBox_borderWidth((NSBox*) handle);
		if ((style & W_HORIZONTAL) != 0) {
			rect.height = ceil(lineWidth * 2);
			rect.y = (DEFAULT_HEIGHT / 2) - (rect.height / 2);
		} else {
			rect.width = ceil(lineWidth * 2);
			rect.x = (DEFAULT_WIDTH / 2) - (rect.width / 2);
		}

		NSView *separator = SWTBox_new(widget);
		NSView_setFrame(separator, &rect);
		NSBox_setBoxType((NSBox*) separator, NSBoxSeparator);
		int resize;
		if ((style & W_HORIZONTAL) != 0) {
			resize = NSViewWidthSizable | NSViewMinYMargin | NSViewMaxYMargin;
		} else {
			resize = NSViewHeightSizable | NSViewMinXMargin | NSViewMaxXMargin;
		}
		NSView_setAutoresizingMask(separator, resize);
		NSView *child = SWTView_new(widget);
		NSBox_setContentView((NSBox*) separator, child);
		NSObject_release(child);

		NSView_addSubview(handle, separator);
		_W_WIDGET(widget)->handle = handle;
	} else {
		_W_WIDGET(widget)->state |= STATE_THEME_BACKGROUND;
		NSView *handle = SWTBox_new(widget);
		NSString *str = mac_toolkit->emptyString;
		NSBox_setTitle((NSBox*) handle, str);
		NSBox_setBorderType((NSBox*) handle, NSNoBorder);
		NSBox_setBorderWidth((NSBox*) handle, 0);
		NSBox_setBoxType((NSBox*) handle, NSBoxCustom);
		NSSize offsetSize;
		offsetSize.width = 0;
		offsetSize.height = 0;
		NSBox_setContentViewMargins((NSBox*) handle, &offsetSize);

		NSImageView *imageWidget = SWTImageView_new(widget);
		NSImageView_setImageScaling(imageWidget, NSScaleNone);

		NSTextField *textWidget = SWTTextField_new(widget);
		NSTextField_setBordered(textWidget, W_FALSE);
		NSTextField_setEditable(textWidget, W_FALSE);
		NSTextField_setDrawsBackground(textWidget, W_FALSE);
		NSCell *cell = NSControl_cell((NSControl*) textWidget);
		NSCell_setWraps(cell, (style & W_WRAP) != 0);

		NSView_addSubview(handle, (NSView*) imageWidget);
		NSView_addSubview(handle, (NSView*) textWidget);
		NSBox_setContentView((NSBox*) handle, (NSView*) textWidget);

		//_setAlignment();
		_W_WIDGET(widget)->handle = handle;
	}
	return W_TRUE;
	_err: return W_ERROR_NO_HANDLES;
}
wresult _w_label_compute_size(w_widget *widget, w_event_compute_size *e,
		_w_control_priv *priv) {
	wuint64 style = _W_WIDGET(widget)->style;
	int width = DEFAULT_WIDTH;
	int height = DEFAULT_HEIGHT;
	int wHint = e->wHint;
	int hHint = e->hHint;
	NSView *handle = _W_WIDGET(widget)->handle;
	if (style & W_HYPERLINK) {

	} else if ((style & W_SEPARATOR) != 0) {
		double /*float*/lineWidth = NSBox_borderWidth((NSBox*) handle);
		if ((style & W_HORIZONTAL) != 0) {
			height = ceil(lineWidth * 2);
		} else {
			width = ceil(lineWidth * 2);
		}
		if (wHint != W_DEFAULT)
			width = wHint;
		if (hHint != W_DEFAULT)
			height = hHint;
		int border = w_control_get_border_width(W_CONTROL(widget));
		width += border * 2;
		height += border * 2;
	} else {
		int isImage = 0; // _W_WIDGET(widget)->state;
		if (isImage) {
			/*if (image != 0) {
			 NSImage nsimage = image.handle;
			 NSSize size = nsimage.size();
			 width = size.width;
			 height = size.height;
			 } else {
			 width = height = 0;
			 }*/
		} else {
			NSSize size;
			NSView *textView = NSBox_contentView((NSBox*) handle);
			NSCell *cell = NSControl_cell((NSControl*) textView);
			if ((style & W_WRAP) != 0 && wHint != W_DEFAULT) {
				NSRect rect;
				rect.width = wHint;
				rect.height = hHint != W_DEFAULT ? hHint : CGFLOAT_MAX;
				NSCell_cellSizeForBounds(cell, &size, &rect);
			} else {
				NSCell_cellSize(cell, &size);
			}
			width = ceil(size.width);
			height = ceil(size.height);
		}
		if (wHint != W_DEFAULT)
			width = wHint;
		if (hHint != W_DEFAULT)
			height = hHint;
	}
	e->size->width = width;
	e->size->height = height;
	return W_TRUE;
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
	priv->widget.check_style = _w_label_check_style;
	priv->widget.create_handle = _w_label_create_handle;
	priv->widget.compute_size = _w_label_compute_size;
}
