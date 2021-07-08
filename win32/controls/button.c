/*
 * button.c
 *
 *  Created on: 7 mars 2021
 *      Author: Azeddine
 */
#include "button.h"
#include "../widgets/toolkit.h"
#include "vsstyle.h"
#define  MARGIN  4
wresult _w_button_get_selection(w_button *button);
wresult _w_button_set_selection(w_button *button, int selected);
wuint64 _w_button_check_style(w_widget *widget, wuint64 style) {
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
wresult _w_button_create_handle(w_control *control, _w_control_priv *priv) {
	wresult result;
	/*
	 * Feature in Windows.  When a button is created,
	 * it clears the UI state for all controls in the
	 * shell by sending WM_CHANGEUISTATE with UIS_SET,
	 * UISF_HIDEACCEL and UISF_HIDEFOCUS to the parent.
	 * This is undocumented and unexpected.  The fix
	 * is to ignore the WM_CHANGEUISTATE, when sent
	 * from CreateWindowEx().
	 */
	w_composite *parent = _W_WIDGET(control)->parent;
	_W_WIDGET(parent)->state |= STATE_IGNORE_WM_CHANGEUISTATE;
	result = _w_control_create_handle(control, priv);
	_W_WIDGET(parent)->state &= ~ STATE_IGNORE_WM_CHANGEUISTATE;

	/* Set the theme background */
	if (_COMCTL32_VERSION >= VERSION(6, 0) && IsAppThemed()) {
		/*
		 * NOTE: On Vista this causes problems when the tab
		 * key is pressed for push buttons so disable the
		 * theme background drawing for these widgets for
		 * now.
		 */
		if (WIN32_VERSION < VERSION(6, 0)) {
			_W_WIDGET(control)->state |= STATE_THEME_BACKGROUND;
		} else {
			if ((_W_WIDGET(control)->style & (W_PUSH | W_TOGGLE)) == 0) {
				_W_WIDGET(control)->state |= STATE_THEME_BACKGROUND;
			}
		}
	}

	/*
	 * Bug in Windows.  For some reason, the HBRUSH that
	 * is returned from WM_CTRLCOLOR is misaligned when
	 * the button uses it to draw.  If the brush is a solid
	 * color, this does not matter.  However, if the brush
	 * contains an image, the image is misaligned.  The
	 * fix is to draw the background in WM_CTRLCOLOR.
	 *
	 * NOTE: For comctl32.dll 6.0 with themes disabled,
	 * drawing in WM_ERASEBKGND will draw on top of the
	 * text of the control.
	 */
	if (_COMCTL32_VERSION >= VERSION(6, 0) && IsAppThemed()) {
		if ((_W_WIDGET(control)->style & W_RADIO) != 0)
			_W_WIDGET(control)->state |= STATE_DRAW_BACKGROUND;
	}

	/*
	 * Feature in Windows.  Push buttons draw border around
	 * the button using the default background color instead
	 * of using the color provided by WM_CTRLCOLOR.  The fix
	 * is to draw the background in WM_CTRLCOLOR.
	 *
	 * NOTE: On Vista this causes problems when the tab key
	 * is pressed for push buttons so disable the fix for now.
	 */
	if (_COMCTL32_VERSION >= VERSION(6, 0) && IsAppThemed()) {
		if (WIN32_VERSION < VERSION(6, 0)) {
			if ((_W_WIDGET(control)->style & (W_PUSH | W_TOGGLE)) != 0) {
				_W_WIDGET(control)->state |= STATE_DRAW_BACKGROUND;
			}
		}
	}
	return result;
}
wresult _w_button_is_checked(w_button *button) {
	LRESULT flags = SendMessageW(_W_WIDGET(button)->handle, BM_GETCHECK, 0, 0);
	return flags != BST_UNCHECKED;
}
wresult _w_button_set_radio_selection(w_button *button, wresult value) {
	if ((_W_WIDGET(button)->style & W_RADIO) == 0)
		return W_FALSE;
	if (_w_button_get_selection(button) != value) {
		_w_button_set_selection(button, value);
		w_event event;
		memset(&event, 0, sizeof(event));
		event.type = W_EVENT_SELECTION;
		//event.platform_event = (struct w_event_platform*) 0;
		event.widget = W_WIDGET(button);
		_w_widget_send_event(W_WIDGET(button), &event, W_EVENT_SEND);
	}
	return W_TRUE;
}
void _w_button_select_radio_0(w_button *button, UINT uCmd) {
	HWND h = _W_WIDGET(button)->handle;
	while ((h = GetWindow(h, uCmd)) != 0) {
		w_control *c = (w_control*) GetWindowLongPtrW(h, GWLP_USERDATA);
		if (c != 0) {
			if (w_widget_get_class(W_WIDGET(c))->class_id == _W_CLASS_BUTTON) {
				if (!_w_button_set_radio_selection(W_BUTTON(c), W_FALSE))
					break;
			} else
				break;
		}
	}
}

void _w_button_select_radio(w_button *button) {
	/*
	 * This code is intentionally commented.  When two groups
	 * of radio buttons with the same parent are separated by
	 * another control, the correct behavior should be that
	 * the two groups act independently.  This is consistent
	 * with radio tool and menu items.  The commented code
	 * implements this behavior.
	 */
	_w_button_select_radio_0(button, GW_HWNDNEXT);
	_w_button_select_radio_0(button, GW_HWNDPREV);
	_w_button_set_selection(button, W_TRUE);
}
/*
 *
 */
wresult _w_button_get_alignment(w_button *button) {
	return 0;
}
wresult _w_button_get_grayed(w_button *button) {
	if ((_W_WIDGET(button)->style & W_CHECK) == 0)
		return W_FALSE;
	return _W_WIDGET(button)->state & STATE_BUTTON_GRAYED;
}
wresult _w_button_get_image_0(w_button *button, w_image *image, int flags) {
	if (_COMCTL32_VERSION >= VERSION(6, 0)) {
		BUTTON_IMAGELIST buttonImageList;
		memset(&buttonImageList, 0, sizeof(buttonImageList));
		SendMessageW(_W_WIDGET(button)->handle, BCM_GETIMAGELIST, 0,
				(LPARAM) &buttonImageList);
		w_imagelist imagelist;
		_W_IMAGELIST(&imagelist)->imagelist = buttonImageList.himl;
		w_imagelist_get_image(&imagelist, 0, TRUE, image);
	} else {

	}
	return W_TRUE;
}
wresult _w_button_get_image(w_button *button, w_image *image) {
	return 0;
}
wresult _w_button_get_selection(w_button *button) {
	if ((_W_WIDGET(button)->style & (W_CHECK | W_RADIO | W_TOGGLE)) == 0)
		return W_FALSE;
	return _w_button_is_checked(button);
}
wresult _w_button_get_text(w_button *button, w_alloc alloc, void *user_data,
		int enc) {
	size_t length;
	WCHAR *s = _w_toolkit_malloc_all(&length);
	size_t l = GetWindowTextW(_W_WIDGET(button)->handle, s, length);
	if (l > length) {
		_w_toolkit_free(s, length);
		length = (l + 1) * sizeof(WCHAR);
		s = (WCHAR*) _w_toolkit_malloc(length);
		if (s == 0)
			return W_ERROR_NO_MEMORY;
		GetWindowTextW(_W_WIDGET(button)->handle, s, l);
	}
	s[l] = 0;
	_win_text_set(s, l, alloc, user_data, enc);
	_w_toolkit_free(s, length);
	return W_TRUE;
}
wresult _w_button_mnemonic_hit(w_control *control, int key,
		_w_control_priv *priv) {
	if (_w_control_set_focus(control) <= 0)
		return W_FALSE;
	/*
	 * Feature in Windows.  When a radio button gets focus,
	 * it selects the button in WM_SETFOCUS.  Therefore, it
	 * is not necessary to click the button or send events
	 * because this has already happened in WM_SETFOCUS.
	 */
	if ((_W_WIDGET(control)->style & W_RADIO) == 0) {
		/*
		 * Feature in Windows.  BM_CLICK sends a fake WM_LBUTTONDOWN and
		 * WM_LBUTTONUP in order to click the button.  This causes the
		 * application to get unexpected mouse events.  The fix is to
		 * ignore mouse events when they are caused by BM_CLICK.
		 */
		_W_WIDGET(control)->state |= STATE_BUTTON_IGNORE_MOUSE;
		SendMessageW(_W_WIDGET(control)->handle, BM_CLICK, 0, 0);
		_W_WIDGET(control)->state &= ~ STATE_BUTTON_IGNORE_MOUSE;
	}
	return W_TRUE;
}
wresult _w_button_set_alignment(w_button *button, int alignment) {
	return 0;
}
wresult _w_button_set_grayed(w_button *button, int grayed) {
	if ((_W_WIDGET(button)->style & W_CHECK) == 0)
		return W_FALSE;
	LRESULT flags = SendMessageW(_W_WIDGET(button)->handle, BM_GETCHECK, 0, 0);
	if (grayed) {
		_W_WIDGET(button)->state |= STATE_BUTTON_GRAYED;
		if (flags == BST_CHECKED)
			_w_button_update_selection(button, BST_INDETERMINATE);
	} else {
		_W_WIDGET(button)->state &= ~STATE_BUTTON_GRAYED;
		if (flags == BST_INDETERMINATE)
			_w_button_update_selection(button, BST_CHECKED);
	}
	return W_TRUE;
}
wresult _w_button_set_image(w_button *button, w_image *image) {
	if ((_W_WIDGET(button)->style & W_COMMAND) != 0)
		return W_FALSE;
	HWND handle = _W_WIDGET(button)->handle;
	if (_COMCTL32_VERSION >= VERSION(6, 0)) {
		if (image != 0) {
			w_size size;
			w_image_get_size(image, &size);
			w_imagelist imagelist;
			w_imagelist_init(&imagelist);
			w_imagelist_create(&imagelist, &size, 2);
			if (IsWindowEnabled(handle)) {
				w_imagelist_add(&imagelist, image);
			} else {
			}
			BUTTON_IMAGELIST buttonImageList;
			memset(&buttonImageList, 0, sizeof(buttonImageList));
			buttonImageList.himl = _W_IMAGELIST(&imagelist)->imagelist;
			DWORD oldBits = GetWindowLong(handle, GWL_STYLE), newBits = oldBits;
			newBits &= ~(BS_LEFT | BS_CENTER | BS_RIGHT);
			wuint64 style = _W_WIDGET(button)->style;
			if ((style & W_LEFT) != 0)
				newBits |= BS_LEFT;
			if ((style & W_CENTER) != 0)
				newBits |= BS_CENTER;
			if ((style & W_RIGHT) != 0)
				newBits |= BS_RIGHT;
			if (GetWindowTextLengthW(handle) == 0) {
				if ((style & W_LEFT) != 0)
					buttonImageList.uAlign = BUTTON_IMAGELIST_ALIGN_LEFT;
				if ((style & W_CENTER) != 0)
					buttonImageList.uAlign = BUTTON_IMAGELIST_ALIGN_CENTER;
				if ((style & W_RIGHT) != 0)
					buttonImageList.uAlign = BUTTON_IMAGELIST_ALIGN_RIGHT;
			} else {
				buttonImageList.uAlign = BUTTON_IMAGELIST_ALIGN_LEFT;
				//buttonImageList.margin.left = computeLeftMargin();
				buttonImageList.margin.right = MARGIN;
				newBits &= ~(BS_CENTER | BS_RIGHT);
				newBits |= BS_LEFT;
			}
			if (newBits != oldBits) {
				SetWindowLong(handle, GWL_STYLE, newBits);
				InvalidateRect(handle, NULL, TRUE);
			}
			SendMessageW(handle, BCM_SETIMAGELIST, 0,
					(LPARAM) &buttonImageList);
		} else {
			SendMessageW(handle, BCM_SETIMAGELIST, 0, 0);
		}
		/*
		 * Bug in Windows.  Under certain cirumstances yet to be
		 * isolated, BCM_SETIMAGELIST does not redraw the control
		 * when a new image is set.  The fix is to force a redraw.
		 */
		InvalidateRect(handle, NULL, TRUE);
	} else {
		void *hImage = 0;
		int imageBits = 0, fImageType = 0;
		if (image != 0) {
			w_size size;
			w_image_get_size(image, &size);
			switch (_W_IMAGE(image)->type) {
			case _IMAGE_HBITMAP: {
				BITMAP bm;
				GetObjectW(_W_IMAGE(image)->handle, sizeof(bm), &bm);
				switch (bm.bmBitsPixel) {
				case 32: {
					w_surface surface;
					w_graphics gc;
					w_surface_init(&surface);
					w_surface_create(&surface, &size);
					w_graphics_init(&gc);
					w_surface_get_graphics(&surface, &gc);
					w_color background;
					w_control_get_background(W_CONTROL(button), &background);
					w_graphics_set_background(&gc, background);
					w_rect r;
					r.x = 0;
					r.y = 0;
					r.width = size.width;
					r.height = size.height;
					w_graphics_fill_rectangle(&gc, &r);
					w_graphics_draw_image(&gc, image, &r, &r, 0);
					w_graphics_dispose(&gc);
					hImage = _W_IMAGE(&surface)->handle;
					imageBits = BS_BITMAP;
					fImageType = IMAGE_BITMAP;
				}
					break;
				default:
					hImage = _W_IMAGE(image)->handle;
					imageBits = BS_BITMAP;
					fImageType = IMAGE_BITMAP;
					break;
				}
				break;
			}
			case _IMAGE_ICON: {
				hImage = _W_IMAGE(image)->handle;
				imageBits = BS_ICON;
				fImageType = IMAGE_ICON;
				break;
			}
			}
			/*
			 * Feature in Windows.  The button control mirrors its image when the
			 * flag WS_EX_LAYOUTRTL is set. This behaviour is not desirable in SWT.
			 * The fix is to set a mirrored version of real image in the button.
			 */
			if ((_W_WIDGET(button)->style & W_RIGHT_TO_LEFT) != 0) {
				if (WIN32_VERSION >= VERSION(4, 10)) {
					HDC hDC = GetDC(handle);
					HDC dstHdc = CreateCompatibleDC(hDC);
					HBITMAP hBitmap = CreateCompatibleBitmap(hDC, size.width,
							size.height);
					HDC oldBitmap = SelectObject(dstHdc, hBitmap);
					SetLayout(dstHdc, LAYOUT_RTL);
					if (fImageType == IMAGE_BITMAP) {
						HDC srcHdc = CreateCompatibleDC(hDC);
						HBITMAP oldSrcBitmap = SelectObject(srcHdc, hImage);
						SetLayout(dstHdc, 0);
						BitBlt(dstHdc, 0, 0, size.width, size.height, srcHdc, 0,
								0, SRCCOPY);
						SelectObject(srcHdc, oldSrcBitmap);
						DeleteDC(srcHdc);
					} else {
						w_control *control = 0; // findBackgroundControl();
						if (control == 0)
							control = W_CONTROL(button);
						w_color background = GetSysColor(COLOR_BTNFACE);
						//control.getBackgroundPixel();
						HBRUSH newBrush = CreateSolidBrush(background);
						HBRUSH oldBrush = SelectObject(dstHdc, newBrush);
						PatBlt(dstHdc, 0, 0, size.width, size.height, PATCOPY);
						DrawIconEx(dstHdc, 0, 0, (HICON) hImage, 0, 0, 0, 0,
						DI_NORMAL);
						SelectObject(dstHdc, oldBrush);
						DeleteObject(newBrush);
					}
					SelectObject(dstHdc, oldBitmap);
					DeleteDC(dstHdc);
					ReleaseDC(handle, hDC);
					imageBits = BS_BITMAP;
					fImageType = IMAGE_BITMAP;
					hImage = hBitmap;
				}
			}
		}
		int newBits = GetWindowLong(handle, GWL_STYLE), oldBits = newBits;
		newBits &= ~(BS_BITMAP | BS_ICON);
		newBits |= imageBits;
		if (newBits != oldBits)
			SetWindowLong(handle, GWL_STYLE, newBits);
		SendMessageW(handle, BM_SETIMAGE, fImageType, (LPARAM) hImage);
		if (hImage != 0 && hImage != _W_IMAGE(image)->handle) {
			DeleteObject(hImage);
		}
	}
	return W_TRUE;
}
wresult _w_button_set_selection(w_button *button, int selected) {
	if ((_W_WIDGET(button)->style & (W_CHECK | W_RADIO | W_TOGGLE)) == 0)
		return W_FALSE;
	int flags = selected ? BST_CHECKED : BST_UNCHECKED;
	if ((_W_WIDGET(button)->style & W_CHECK) != 0) {
		if (selected && (_W_WIDGET(button)->state & STATE_BUTTON_GRAYED))
			flags = BST_INDETERMINATE;
	}
	_w_button_update_selection(button, flags);
	return W_TRUE;
}
wresult _w_button_set_text(w_button *button, const char *text, int length,
		int enc) {
	if (text == 0)
		text = "";
	if ((_W_WIDGET(button)->style & W_ARROW) != 0)
		return W_TRUE;
	WCHAR *str;
	int newlength;
	wresult result = _win_text_fix(text, length, enc, &str, &newlength);
	if (result > 0) {
		BOOL rest = SetWindowTextW(_W_WIDGET(button)->handle, str);
	}
	_win_text_free(text, str, newlength);
	return result;
}
void _w_button_update_selection(w_button *button, int flags) {
	if (flags != SendMessageW(_W_WIDGET(button)->handle, BM_GETCHECK, 0, 0)) {
		/*
		 * Feature in Windows. When BM_SETCHECK is used
		 * to set the checked state of a radio or check
		 * button, it sets the WS_TABSTOP style.  This
		 * is undocumented and unwanted.  The fix is
		 * to save and restore the window style bits.
		 */
		int bits = GetWindowLongW(_W_WIDGET(button)->handle, GWL_STYLE);
		if ((_W_WIDGET(button)->style & W_CHECK) != 0) {
			if (flags == BST_INDETERMINATE) {
				bits &= ~BS_CHECKBOX;
				bits |= BS_3STATE;
			} else {
				bits |= BS_CHECKBOX;
				bits &= ~BS_3STATE;
			}
			if (bits != GetWindowLongW(_W_WIDGET(button)->handle, GWL_STYLE)) {
				SetWindowLongW(_W_WIDGET(button)->handle, GWL_STYLE, bits);
			}
		}
		SendMessageW(_W_WIDGET(button)->handle, BM_SETCHECK, flags, 0);
		if (bits != GetWindowLong(_W_WIDGET(button)->handle, GWL_STYLE)) {
			SetWindowLongW(_W_WIDGET(button)->handle, GWL_STYLE, bits);
		}
	}
}
DWORD _w_button_widget_style(w_control *control, _w_control_priv *priv) {
	DWORD bits = _w_control_widget_style(control, priv);
	wuint64 style = _W_WIDGET(control)->style;
	if ((style & W_FLAT) != 0)
		bits |= BS_FLAT;
	if ((style & W_ARROW) != 0)
		return bits | BS_OWNERDRAW;
	if ((style & W_LEFT) != 0)
		bits |= BS_LEFT;
	if ((style & W_CENTER) != 0)
		bits |= BS_CENTER;
	if ((style & W_RIGHT) != 0)
		bits |= BS_RIGHT;
	if ((style & W_WRAP) != 0)
		bits |= BS_MULTILINE;
	if ((style & W_PUSH) != 0)
		return bits | BS_PUSHBUTTON | WS_TABSTOP;
	if ((style & W_CHECK) != 0)
		return bits | BS_CHECKBOX | WS_TABSTOP;
	if ((style & W_RADIO) != 0)
		return bits | BS_RADIOBUTTON;
	if ((style & W_TOGGLE) != 0)
		return bits | BS_PUSHLIKE | BS_CHECKBOX | WS_TABSTOP;
	if ((style & W_COMMAND) != 0)
		return bits | BS_COMMANDLINK | WS_TABSTOP;
	return bits | BS_PUSHBUTTON | WS_TABSTOP;
}
WCHAR* _w_button_window_class(w_control *control, _w_control_priv *priv) {
	return WC_BUTTONW;
}
/*
 *	messages
 */
wresult _BUTTON_WM_GETDLGCODE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	int result = _CONTROL_WM_GETDLGCODE(widget, e, priv);
	if (result != W_FALSE)
		return result;
	if ((_W_WIDGET(widget)->style & W_ARROW) != 0) {
		e->result = DLGC_STATIC;
	}
	return result;
}

wresult _BUTTON_WM_GETOBJECT(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	/*
	 * Ensure that there is an accessible object created for this
	 * control because support for radio button position in group
	 * accessibility is implemented in the accessibility package.
	 */
	/*if ((style & SWT.RADIO) != 0) {
	 if (accessible == null) accessible = new_Accessible (this);
	 }*/
	return _CONTROL_WM_GETOBJECT(widget, e, priv);
}

wresult _BUTTON_WM_KILLFOCUS(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	wresult result = _WIDGET_WM_KILLFOCUS(widget, e, priv);
	return result;
}

wresult _BUTTON_WM_LBUTTONDOWN(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	if (_W_WIDGET(widget)->state & STATE_BUTTON_IGNORE_MOUSE)
		return W_FALSE;
	return _WIDGET_WM_LBUTTONDOWN(widget, e, priv);
}

wresult _BUTTON_WM_LBUTTONUP(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	if (_W_WIDGET(widget)->state & STATE_BUTTON_IGNORE_MOUSE)
		return W_FALSE;
	return _WIDGET_WM_LBUTTONUP(widget, e, priv);
}

wresult _BUTTON_WM_SETFOCUS(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	/*
	 * Feature in Windows. When Windows sets focus to
	 * a radio button, it sets the WS_TABSTOP style.
	 * This is undocumented and unwanted.  The fix is
	 * to save and restore the window style bits.
	 */
	int bits = 0;
	if ((_W_WIDGET(widget)->style & W_RADIO) != 0) {
		bits = GetWindowLong(_W_WIDGET(widget)->handle, GWL_STYLE);
	}
	int result = _WIDGET_WM_SETFOCUS(widget, e, priv);
	if ((_W_WIDGET(widget)->style & W_RADIO) != 0) {
		SetWindowLong(_W_WIDGET(widget)->handle, GWL_STYLE, bits);
	}
	if ((_W_WIDGET(widget)->style & W_PUSH) != 0) {
		w_shell *shell;
		w_widget_get_shell(widget, &shell);
		w_shell_set_default_button(shell, W_BUTTON(widget));
	}
	return result;
}

wresult _BUTTON_WM_SIZE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	int result = _CONTROL_WM_SIZE(widget, e, priv);
	if (result != W_FALSE)
		return result;
	return result;
}

wresult _BUTTON_WM_SYSCOLORCHANGE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}

wresult _BUTTON_WM_UPDATEUISTATE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}

wresult _BUTTON_WM_COMMANDCHILD(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	int code = HIWORD(e->wparam);
	switch (code) {
	case BN_CLICKED:
	case BN_DOUBLECLICKED:
		if ((_W_WIDGET(widget)->style & (W_CHECK | W_TOGGLE)) != 0) {
			_w_button_set_selection(W_BUTTON(widget),
					!_w_button_get_selection(W_BUTTON(widget)));
		} else {
			if ((_W_WIDGET(widget)->style & W_RADIO) != 0) {
				w_composite *parent = _W_WIDGET(widget)->parent;
				if ((_W_WIDGET(parent)->style & W_NO_RADIO_GROUP) != 0) {
					_w_button_set_selection(W_BUTTON(widget),
							!_w_button_get_selection(W_BUTTON(widget)));
				} else {
					_w_button_select_radio(W_BUTTON(widget));
				}
			}
		}
		w_event event;
		memset(&event, 0, sizeof(event));
		event.type = W_EVENT_SELECTION;
		event.platform_event = (struct w_event_platform*) e;
		event.widget = widget;
		_w_widget_send_event(widget, &event, W_EVENT_SEND);
	}
	return W_FALSE;
}

wresult _BUTTON_WM_NOTIFYCHILD(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}

wresult _BUTTON_WM_DRAWCHILD(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	if ((_W_WIDGET(widget)->style & W_ARROW) == 0)
		return W_FALSE;
	DRAWITEMSTRUCT *item = (DRAWITEMSTRUCT*) e->lparam;
	if (IsAppThemed()) {
		int iStateId = ABS_LEFTNORMAL;
		switch (_W_WIDGET(widget)->style & (W_UP | W_DOWN | W_LEFT | W_RIGHT)) {
		case W_UP:
			iStateId = ABS_UPNORMAL;
			break;
		case W_DOWN:
			iStateId = ABS_DOWNNORMAL;
			break;
		case W_LEFT:
			iStateId = ABS_LEFTNORMAL;
			break;
		case W_RIGHT:
			iStateId = ABS_RIGHTNORMAL;
			break;
		}
		/*
		 * Feature in Windows.  DrawThemeBackground() does not mirror the drawing.
		 * The fix is switch left to right and right to left.
		 */
		if ((_W_WIDGET(widget)->style & W_MIRRORED) != 0) {
			if ((_W_WIDGET(widget)->style & (W_LEFT | W_RIGHT)) != 0) {
				iStateId =
						iStateId == ABS_RIGHTNORMAL ?
								ABS_LEFTNORMAL : ABS_RIGHTNORMAL;
			}
		}
		/*
		 * NOTE: The normal, hot, pressed and disabled state is
		 * computed relying on the fact that the increment between
		 * the direction states is invariant (always separated by 4).
		 */
		if (!IsWindowEnabled(_W_WIDGET(widget)->handle))
			iStateId += ABS_UPDISABLED - ABS_UPNORMAL;
		if ((item->itemState & ODS_SELECTED) != 0)
			iStateId += ABS_UPPRESSED - ABS_UPNORMAL;
		HTHEME hTheme = OpenThemeData(NULL, L"SCROLLBAR");
		DrawThemeBackground(hTheme, item->hDC, SBP_ARROWBTN, iStateId,
				&item->rcItem, NULL);
		CloseThemeData(hTheme);
	} else {
		int uState = DFCS_SCROLLLEFT;
		switch (_W_WIDGET(widget)->style & (W_UP | W_DOWN | W_LEFT | W_RIGHT)) {
		case W_UP:
			uState = DFCS_SCROLLUP;
			break;
		case W_DOWN:
			uState = DFCS_SCROLLDOWN;
			break;
		case W_LEFT:
			uState = DFCS_SCROLLLEFT;
			break;
		case W_RIGHT:
			uState = DFCS_SCROLLRIGHT;
			break;
		}
		if (!IsWindowEnabled(_W_WIDGET(widget)->handle))
			uState |= DFCS_INACTIVE;
		if ((_W_WIDGET(widget)->style & W_FLAT) == W_FLAT)
			uState |= DFCS_FLAT;
		if ((item->itemState & ODS_SELECTED) != 0)
			uState |= DFCS_PUSHED;
		DrawFrameControl(item->hDC, &item->rcItem, DFC_SCROLL, uState);
	}
	return W_FALSE;
}
#define OBM_CHECKBOXES 0x7ff7
void _w_button_init_check_widths(_w_button_priv *priv) {
	if (priv->CHECK_WIDTH != 0 || priv->CHECK_HEIGHT != 0)
		return;
	HBITMAP hBitmap = LoadBitmapW(NULL, (LPCWSTR) OBM_CHECKBOXES);
	if (hBitmap == 0) {
		priv->CHECK_WIDTH = GetSystemMetrics(SM_CXVSCROLL);
		priv->CHECK_HEIGHT = GetSystemMetrics(SM_CYVSCROLL);
	} else {
		BITMAP bitmap;
		GetObjectW(hBitmap, sizeof(BITMAP), &bitmap);
		DeleteObject(hBitmap);
		priv->CHECK_WIDTH = bitmap.bmWidth / 4;
		priv->CHECK_HEIGHT = bitmap.bmHeight / 3;
	}
}
wresult _w_button_compute_size(w_widget *widget, w_event_compute_size *e,
		_w_control_priv *priv) {
	int wHint = e->wHint;
	int hHint = e->hHint;
	int width = 0, height = 0;
	int border = _w_control_get_border_width(W_CONTROL(widget));
	if ((_W_WIDGET(widget)->style & W_ARROW) != 0) {
		if ((_W_WIDGET(widget)->style & (W_UP | W_DOWN)) != 0) {
			width += GetSystemMetrics(SM_CXVSCROLL);
			height += GetSystemMetrics(SM_CYVSCROLL);
		} else {
			width += GetSystemMetrics(SM_CXHSCROLL);
			height += GetSystemMetrics(SM_CYHSCROLL);
		}
	} else {
		if ((_W_WIDGET(widget)->style & W_COMMAND) != 0) {
			SIZE size;
			if (wHint != W_DEFAULT) {
				size.cx = wHint;
				SendMessageW(_W_WIDGET(widget)->handle, BCM_GETIDEALSIZE, 0,
						(LPARAM) &size);
				width = size.cx;
				height = size.cy;
			} else {
				SendMessageW(_W_WIDGET(widget)->handle, BCM_GETIDEALSIZE, 0,
						(LPARAM) &size);
				width = size.cy;
				height = size.cy;
				size.cy = 0;
				while (size.cy != height) {
					size.cx = width++;
					size.cy = 0;
					SendMessageW(_W_WIDGET(widget)->handle, BCM_GETIDEALSIZE, 0,
							(LPARAM) &size);
				}
			}
		} else {
			w_image img;
			w_image_init(&img);
			_w_button_get_image_0(W_BUTTON(widget), &img, 0);
			int extra = 0;
			int hasImage = w_image_is_ok(&img) > 0;
			int hasText = TRUE;
			if (_COMCTL32_VERSION < VERSION(6, 0)) {
				if ((_W_WIDGET(widget)->style & W_PUSH) == 0) {
					int bits = GetWindowLong(_W_WIDGET(widget)->handle,
					GWL_STYLE);
					hasImage = (bits & (BS_BITMAP | BS_ICON)) != 0;
					if (hasImage)
						hasText = FALSE;
				}
			}
			if (hasImage) {
				if (w_image_is_ok(&img) > 0) {
					w_size size;
					w_image_get_size(&img, &size);
					width = size.width;
					if (hasText
							&& GetWindowTextLengthW(_W_WIDGET(widget)->handle)
									!= 0) {
						width += MARGIN * 2;
					}
					height = size.height;
					extra = MARGIN * 2;
				}
			}
			if (hasText) {
				RECT rect;
				HFONT oldFont = 0;
				HDC hDC = GetDC(_W_WIDGET(widget)->handle);
				HFONT newFont = (HFONT) SendMessageW(_W_WIDGET(widget)->handle,
				WM_GETFONT, 0, 0);
				if (newFont != 0)
					oldFont = SelectObject(hDC, newFont);
				TEXTMETRICW lptm;
				GetTextMetricsW(hDC, &lptm);
				int length = GetWindowTextLengthW(_W_WIDGET(widget)->handle);
				if (length == 0) {
					height = WMAX(height, lptm.tmHeight);
				} else {
					WCHAR *buffer;
					buffer = _w_toolkit_malloc((length + 1) * sizeof(WCHAR));
					if (buffer != 0) {
						GetWindowTextW(_W_WIDGET(widget)->handle, buffer,
								length + 1);
						extra = WMAX(MARGIN * 2, lptm.tmAveCharWidth);
						int flags = DT_CALCRECT | DT_SINGLELINE;
						if ((_W_WIDGET(widget)->style & W_WRAP) != 0
								&& wHint != W_DEFAULT) {
							flags = DT_CALCRECT | DT_WORDBREAK;
							rect.right = wHint - width - 2 * border;
							if ((_W_WIDGET(widget)->style & (W_CHECK | W_RADIO))
									!= 0) {
								_w_button_init_check_widths(
										_W_BUTTON_PRIV(priv));
								rect.right -= _W_BUTTON_PRIV(priv)->CHECK_WIDTH
										+ 3;
							} else {
								rect.right -= 6;
							}
							if (_COMCTL32_VERSION < VERSION(6, 0)
									|| !IsAppThemed()) {
								rect.right -= 2;
								if ((_W_WIDGET(widget)->style
										& (W_CHECK | W_RADIO)) != 0) {
									rect.right -= 2;
								}
							}
						}
						DrawTextW(hDC, buffer, -1, &rect, flags);
						_w_toolkit_free(buffer, (length + 1) * sizeof(WCHAR));
					}
					width += rect.right - rect.left;
					height = WMAX(height, rect.bottom - rect.top);
				}
				if (newFont != 0)
					SelectObject(hDC, oldFont);
				ReleaseDC(_W_WIDGET(widget)->handle, hDC);
			}
			if ((_W_WIDGET(widget)->style & (W_CHECK | W_RADIO)) != 0) {
				_w_button_init_check_widths(_W_BUTTON_PRIV(priv));
				width += _W_BUTTON_PRIV(priv)->CHECK_WIDTH + extra;
				height = WMAX(height, _W_BUTTON_PRIV(priv)->CHECK_HEIGHT + 3);
			}
			if ((_W_WIDGET(widget)->style & (W_PUSH | W_TOGGLE)) != 0) {
				width += 12;
				height += 10;
			}
		}
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
void _w_button_class_init(w_toolkit *toolkit, wushort classId,
		struct _w_button_class *clazz) {
	if (classId == _W_CLASS_BUTTON) {
		W_WIDGET_CLASS(clazz)->platformPrivate =
				&win_toolkit->class_button_priv;
	}
	_w_control_class_init(toolkit, classId, W_CONTROL_CLASS(clazz));
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
	_w_control_priv *priv = _W_CONTROL_PRIV(
			W_WIDGET_CLASS(clazz)->platformPrivate);
	if (_W_WIDGET_PRIV(priv)->init == 0) {
		if (classId == _W_CLASS_BUTTON) {
			_W_WIDGET_PRIV(priv)->init = 1;
		}
		priv->check_style = _w_button_check_style;
		priv->compute_size = _w_button_compute_size;
		priv->create_handle = _w_button_create_handle;
		priv->widget_style = _w_button_widget_style;
		priv->window_class = _w_button_window_class;
		priv->mnemonic_hit = _w_button_mnemonic_hit;
		/*
		 * messages
		 */
		dispatch_message *msg = priv->messages;
		msg[_WM_GETDLGCODE] = _BUTTON_WM_GETDLGCODE;
		msg[_WM_GETOBJECT] = _BUTTON_WM_GETOBJECT;
		msg[_WM_KILLFOCUS] = _BUTTON_WM_KILLFOCUS;
		msg[_WM_LBUTTONDOWN] = _BUTTON_WM_LBUTTONDOWN;
		msg[_WM_LBUTTONUP] = _BUTTON_WM_LBUTTONUP;
		msg[_WM_SETFOCUS] = _BUTTON_WM_SETFOCUS;
		msg[_WM_SIZE] = _BUTTON_WM_SIZE;
		msg[_WM_SYSCOLORCHANGE] = _BUTTON_WM_SYSCOLORCHANGE;
		msg[_WM_UPDATEUISTATE] = _BUTTON_WM_UPDATEUISTATE;
		msg[_WM_COMMANDCHILD] = _BUTTON_WM_COMMANDCHILD;
		msg[_WM_NOTIFYCHILD] = _BUTTON_WM_NOTIFYCHILD;
		msg[_WM_DRAWCHILD] = _BUTTON_WM_DRAWCHILD;
	}
}

