/*
 * button.c
 *
 *  Created on: Mar 5, 2021
 *      Author: azeddine
 */
#include "button.h"
#include "../widgets/toolkit.h"
#define INNER_BORDER  1
#define DEFAULT_BORDER  1
#define MAX_SIZE 40000
/*
 * private function
 */
int _w_button_check_style(int style) {
	style = _w_widget_check_bits(style, W_PUSH, W_ARROW, W_CHECK, W_RADIO,
			W_TOGGLE, 0);
	if ((style & (W_PUSH | W_TOGGLE)) != 0) {
		return _w_widget_check_bits(style, W_CENTER, W_LEFT, W_RIGHT, 0, 0, 0);
	}
	if ((style & (W_CHECK | W_RADIO)) != 0) {
		return _w_widget_check_bits(style, W_LEFT, W_RIGHT, W_CENTER, 0, 0, 0);
	}
	if ((style & W_ARROW) != 0) {
		style |= W_NO_FOCUS;
		return _w_widget_check_bits(style, W_UP, W_DOWN, W_LEFT, W_RIGHT, 0, 0);
	}
	return style;
}
/*
 * public function
 */
int _w_button_get_alignment(w_button *button) {
	if ((_W_WIDGET(button)->style & W_ARROW) != 0) {
		if ((_W_WIDGET(button)->style & W_UP) != 0)
			return W_UP;
		if ((_W_WIDGET(button)->style & W_DOWN) != 0)
			return W_DOWN;
		if ((_W_WIDGET(button)->style & W_LEFT) != 0)
			return W_LEFT;
		if ((_W_WIDGET(button)->style & W_RIGHT) != 0)
			return W_RIGHT;
		return W_UP;
	}
	if ((_W_WIDGET(button)->style & W_LEFT) != 0)
		return W_LEFT;
	if ((_W_WIDGET(button)->style & W_CENTER) != 0)
		return W_CENTER;
	if ((_W_WIDGET(button)->style & W_RIGHT) != 0)
		return W_RIGHT;
	return W_LEFT;
}
wresult _w_button_get_grayed(w_button *button) {
	return _W_WIDGET(button)->state & STATE_BUTTON_GRAYED;
}
wresult _w_button_get_image(w_button *button, w_image *image) {
	return W_FALSE;
}
wresult _w_button_get_selection(w_button *button) {
	return W_FALSE;
}
wresult _w_button_get_text(w_button *button, w_alloc alloc, void *user_data) {
	return W_FALSE;
}
wresult _w_button_set_alignment(w_button *button, int alignment) {
	return W_TRUE;
}
wresult _w_button_set_grayed(w_button *button, int grayed) {
	if ((_W_WIDGET(button)->style & W_CHECK) == 0)
		return W_FALSE;
	return W_TRUE;
}
wresult _w_button_set_image(w_button *button, w_image *image) {
	if ((_W_WIDGET(button)->style & W_ARROW) != 0)
		return W_FALSE;
	return W_TRUE;
}
wresult _w_button_set_selection(w_button *button, int selected) {
	if ((_W_WIDGET(button)->style & (W_CHECK | W_RADIO | W_TOGGLE)) == 0)
		return W_FALSE;
	return W_TRUE;
}
wresult _w_button_set_text(w_button *button, const char *text, int length,
		int enc) {
	if (text == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_W_WIDGET(button)->style & W_ARROW)
		return W_TRUE;
	NSString *str = NSString_new(text, length, enc);
	NSMutableAttributedString *attributedTitle = _w_control_create_string(W_CONTROL(button),
			str, 0, _W_CONTROL(button)->foreground, _W_WIDGET(button)->style);
	NSButton* btn =(NSButton*) _W_WIDGET(button)->handle;
	NSButton_setAttributedTitle(btn, attributedTitle);
	NSObject_release(str);
	return W_TRUE;
}
/*
 *
 */
wresult _w_button_create_handle(w_widget *widget, _w_widget_priv *priv) {
	wuint64 style = _W_WIDGET(widget)->style;
	if ((style & W_PUSH) == 0)
		_W_WIDGET(widget)->state |= STATE_THEME_BACKGROUND;
	NSButton *button = SWTButton_new(widget);
	NSButtonCell *cell = SWTButtonCell_new();
	NSControl_setCell((NSControl*) button, (NSCell*) cell);
	NSObject_release(cell);
	if ((style & (W_PUSH | W_TOGGLE)) != 0 && (style & W_FLAT) == 0) {
		/*NSView superview = parent.view;
		 while (superview != null) {
		 if (superview.isKindOfClass(OS.class_NSTableView)) {
		 style |= SWT.FLAT;
		 break;
		 }
		 superview = superview.superview();
		 }*/
	}
	int type = NSMomentaryLightButton;
	if ((style & W_PUSH) != 0) {
		int BezelStyle;
		if ((style & W_FLAT) != 0) {
			BezelStyle = NSShadowlessSquareBezelStyle;
		} else {
			BezelStyle =
					((style & W_WRAP) != 0) ?
							NSRegularSquareBezelStyle : NSRoundedBezelStyle;
		}
		NSButton_setBezelStyle(button, BezelStyle);
	} else if ((style & W_CHECK) != 0) {
		type = NSSwitchButton;
	} else if ((style & W_RADIO) != 0) {
		type = NSRadioButton;
		//NSView* radioParent = SWTView_new(widget);
	} else if ((style & W_TOGGLE) != 0) {
		type = NSPushOnPushOffButton;
		int BezelStyle;
		if ((style & W_FLAT) != 0) {
			BezelStyle = NSShadowlessSquareBezelStyle;
		} else {
			BezelStyle =
					((style & W_WRAP) != 0) ?
							NSRegularSquareBezelStyle : NSRoundedBezelStyle;
		}
		NSButton_setBezelStyle(button, BezelStyle);
	} else if ((style & W_ARROW) != 0) {
		NSButton_setBezelStyle(button, NSShadowlessSquareBezelStyle);
	}
	NSButton_setButtonType(button, type);
	NSButton_setTitle(button, mac_toolkit->emptyString);
	NSButton_setImagePosition(button, NSImageLeft);
	// _setAlignment(style);
	_W_WIDGET(widget)->handle = (NSView*) button;
	return W_TRUE;
}
int _w_button_compute_size(w_widget *widget, w_event_compute_size *e,
		_w_control_priv *priv) {
	int wHint = e->wHint;
	int hHint = e->hHint;
	if (wHint != W_DEFAULT && wHint < 0)
		wHint = 0;
	if (hHint != W_DEFAULT && hHint < 0)
		hHint = 0;
	e->size->width = 70;
	e->size->height = 40;
	if ((_W_WIDGET(widget)->style & W_ARROW) != 0) {
		// TODO use some OS metric instead of hardcoded values
		e->size->width = wHint != W_DEFAULT ? wHint : 14;
		e->size->height = hHint != W_DEFAULT ? hHint : 14;
	} else {
		NSSize size;
		size.width = 70;
		size.height = 40;
		NSView *view = priv->get_view(widget);
		NSCell *cell = NSControl_cell((NSControl*) view);
		if ((_W_WIDGET(widget)->style & W_WRAP) != 0 && wHint != W_DEFAULT) {
			NSRect rect;
			rect.width = wHint;
			rect.height = hHint != W_DEFAULT ? hHint : MAX_SIZE;
			//size = cell.cellSizeForBounds(rect);
		} else {
			//size = cell.cellSize();
		}
		int width = ceil(size.width);
		int height = ceil(size.height);
		if (wHint != W_DEFAULT)
			width = wHint;
		if (hHint != W_DEFAULT)
			height = hHint;
		e->size->width = width;
		e->size->height = height;
	}
	return W_TRUE;
}
void _w_button_class_init(struct _w_button_class *clazz) {
	_w_control_class_init(W_CONTROL_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_BUTTON;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_button_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_button);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_button);
	/*
	 * public function
	 */
	clazz->get_alignment = _w_button_get_alignment;
	clazz->get_grayed = _w_button_get_grayed;
	clazz->get_image = _w_button_get_image;
	clazz->get_selection = _w_button_get_selection;
	clazz->get_text = _w_button_get_text;
	clazz->set_alignment = _w_button_set_alignment;
	clazz->set_grayed = _w_button_set_grayed;
	clazz->set_image = _w_button_set_image;
	clazz->set_selection = _w_button_set_selection;
	clazz->set_text = _w_button_set_text;
	/*
	 * priv
	 */
	_w_control_priv *priv = _W_CONTROL_PRIV(W_WIDGET_CLASS(clazz)->reserved[0]);
	priv->widget.compute_size = _w_button_compute_size;
	_W_WIDGET_PRIV(priv)->create_handle = _w_button_create_handle;
	/*
	 * signals
	 */

}

