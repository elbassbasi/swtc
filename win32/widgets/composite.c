/*
 * Name:        composite.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "composite.h"
#include "toolkit.h"
wresult _w_composite_create_handle(w_control *control, _w_control_priv *priv) {
	wresult result = _w_control_create_handle(control, priv);
	if (result > 0) {
		_W_WIDGET(control)->state |= STATE_CANVAS;
		wuint64 style = _W_WIDGET(control)->style;
		if ((style & (W_HSCROLL | W_VSCROLL))
				== 0 /*|| _w_control_find_theme_control (control) == _W_CONTROL(control)->parent*/) {
			_W_WIDGET(control)->state |= STATE_THEME_BACKGROUND;
		}
		if ((style & W_TRANSPARENT) != 0) {
			DWORD bits = GetWindowLongW(_W_WIDGET(control)->handle,
			GWL_EXSTYLE);
			bits |= WS_EX_TRANSPARENT;
			SetWindowLongW(_W_WIDGET(control)->handle, GWL_EXSTYLE, bits);
		}
	}
	return result;
}
wresult _w_composite_compute_size(w_widget *widget, w_event_compute_size *e,
		_w_control_priv *priv) {
	w_size size;
	w_layout *layout;
	w_composite_get_layout(W_COMPOSITE(widget), &layout);
	if (layout != 0) {
		if (e->wHint == W_DEFAULT || e->hHint == W_DEFAULT) {
			//changed |= (state & LAYOUT_CHANGED) != 0;
			//state &= ~LAYOUT_CHANGED;
			w_layout_compute_size(layout, W_COMPOSITE(widget), &size, e->wHint,
					e->hHint, W_TRUE);
		} else {
			size.width = e->wHint;
			size.height = e->hHint;
		}
	} else {
		_w_composite_minimum_size(W_COMPOSITE(widget), &size, e->wHint,
				e->hHint, TRUE);
		if (size.width == 0)
			size.width = DEFAULT_WIDTH;
		if (size.height == 0)
			size.height = DEFAULT_HEIGHT;
	}
	if (e->wHint != W_DEFAULT)
		size.width = e->wHint;
	if (e->hHint != W_DEFAULT)
		size.height = e->hHint;
	/*
	 * Since computeTrim can be overridden by subclasses, we cannot
	 * call computeTrimInPixels directly.
	 */
	w_rect rect, trim;
	rect.x = 0;
	rect.y = 0;
	rect.width = size.width;
	rect.height = size.height;
	w_scrollable_compute_trim(W_SCROLLABLE(widget), &trim, &rect);
	e->size->width = trim.width;
	e->size->height = trim.height;
	return TRUE;
}
DWORD _w_composite_widget_style(w_control *control, _w_control_priv *priv) {
	return _w_scrollable_widget_style(control, priv) | WS_CLIPCHILDREN;
}
/*
 * iterator of children control
 */
typedef struct _w_composite_children {
	w_basic_iterator iter;
	w_composite *composite;
	HWND hwndChild;
	wresult init;
	int count;
} _w_composite_children;
wresult _w_composite_children_close(w_iterator *it) {
	return W_TRUE;
}
wresult _w_composite_children_next(w_iterator *it, void *obj) {
	_w_composite_children *iter = (_w_composite_children*) it;
	if (iter->init == W_TRUE) {
		iter->hwndChild = GetWindow(_W_WIDGET(iter->composite)->handle,
		GW_CHILD);
		iter->init = W_FALSE;
	} else {
		if (iter->hwndChild != 0) {
			iter->hwndChild = GetWindow(iter->hwndChild, GW_HWNDNEXT);
		}
	}
	if (iter->hwndChild != 0) {
		*((w_widget**) obj) = _w_widget_find_control(iter->hwndChild);
		return W_TRUE;
	} else {
		return W_FALSE;
	}

}
wresult _w_composite_children_reset(w_iterator *it) {
	_w_composite_children *iter = (_w_composite_children*) it;
	iter->init = W_TRUE;
	return W_TRUE;

}
wresult _w_composite_children_remove(w_iterator *it) {
	return W_ERROR_NOT_IMPLEMENTED;

}
size_t _w_composite_children_get_count(w_iterator *it) {
	_w_composite_children *iter = (_w_composite_children*) it;
	if (iter->count < 0) {
		int count = 0;
		HWND hwnd = GetWindow(_W_WIDGET(iter->composite)->handle, GW_CHILD);
		if (hwnd != 0) {
			count++;
			while ((hwnd = GetWindow(hwnd, GW_HWNDNEXT)) != 0) {
				if (_w_widget_find_control(hwnd) != 0)
					count++;
			}
		}
		iter->count = count;
	}
	return iter->count;
}
_w_iterator_class _w_composite_children_class = { //
		_w_composite_children_close, //
				_w_composite_children_next, //
				_w_composite_children_reset, //
				_w_composite_children_remove, //
				_w_composite_children_get_count //
		};
wresult _w_composite_get_children(w_composite *composite, w_iterator *it) {
	_w_composite_children *iter = (_w_composite_children*) it;
	it->base.clazz = &_w_composite_children_class;
	iter->composite = composite;
	iter->init = W_TRUE;
	iter->hwndChild = 0;
	iter->count = -1;
	return W_TRUE;
}
wresult _w_composite_get_layout(w_composite *composite, w_layout **layout) {
	*layout = _W_COMPOSITE(composite)->layout;
	return W_TRUE;
}
wresult _w_composite_get_layout_deferred(w_composite *composite) {
	return W_FALSE;
}
wresult _w_composite_get_tab_list(w_composite *composite, w_iterator *it) {
	return W_FALSE;
}
wresult _w_composite_is_layout_deferred(w_composite *composite) {
	return W_FALSE;
}
wresult _w_composite_do_layout(w_composite *composite, int changed, int all) {
	return W_FALSE;
}
wresult _w_composite_layout_changed(w_composite *_this, w_control **changed,
		size_t length, int flags) {
	return W_FALSE;
}
void _w_composite_minimum_size(w_composite *composite, w_size *result,
		int wHint, int hHint, wresult changed) {
	w_iterator children;
	w_iterator_init(&children);
	w_composite_get_children(composite, &children);
	w_rect clientArea, rect;
	w_scrollable_get_client_area(W_SCROLLABLE(composite), &clientArea);
	int width = 0, height = 0;
	w_control *child = 0;
	while (w_iterator_next(&children, (void*) &child)) {
		if (child != 0) {
			w_control_get_bounds(child, &rect.pt, &rect.sz);
			width = WMAX(width, rect.x - clientArea.x + rect.width);
			height = WMAX(height, rect.y - clientArea.y + rect.height);
		}
	}
	result->width = width;
	result->height = height;
}
wresult _w_composite_set_layout(w_composite *composite, w_layout *layout) {
	_W_COMPOSITE(composite)->layout = layout;
	return W_FALSE;
}
wresult _w_composite_set_layout_deferred(w_composite *composite, int defer) {
	return W_FALSE;
}
/*
 * messages
 */
wresult _COMPOSITE_WM_ERASEBKGND(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _COMPOSITE_WM_GETDLGCODE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _COMPOSITE_WM_GETFONT(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _COMPOSITE_WM_LBUTTONDOWN(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _COMPOSITE_WM_NCHITTEST(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _COMPOSITE_WM_PARENTNOTIFY(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
/*
 * BufferedPaint init function
 */
#if (NTDDI_VERSION < NTDDI_VISTA)
typedef enum _BP_BUFFERFORMAT {
	BPBF_COMPATIBLEBITMAP, BPBF_DIB, BPBF_TOPDOWNDIB, BPBF_TOPDOWNMONODIB
} BP_BUFFERFORMAT;
typedef struct _BP_PAINTPARAMS {
	DWORD cbSize;
	DWORD dwFlags;
	const RECT *prcExclude;
	const BLENDFUNCTION *pBlendFunction;
} BP_PAINTPARAMS, *PBP_PAINTPARAMS;
DECLARE_HANDLE(HPAINTBUFFER);
#endif
typedef HPAINTBUFFER (WINAPI *_BeginBufferedPaint)(HDC hdcTarget,
		const RECT *prcTarget, BP_BUFFERFORMAT dwFormat,
		BP_PAINTPARAMS *pPaintParams, HDC *phdc);
typedef HRESULT (WINAPI *_EndBufferedPaint)(HPAINTBUFFER hBufferedPaint,
BOOL fUpdateTarget);
int _w_composite_init_bufferedpaint() {
	if (win_toolkit->_BeginBufferedPaint == 0) {
		HMODULE module = _w_toolkit_load_library("UxTheme.dll");
		if (module != 0) {
			win_toolkit->_BeginBufferedPaint = GetProcAddress(module,
					"BeginBufferedPaint");
			win_toolkit->_EndBufferedPaint = GetProcAddress(module,
					"EndBufferedPaint");
			FreeLibrary(module);
		}
		if (win_toolkit->_BeginBufferedPaint == 0
				|| win_toolkit->_EndBufferedPaint == 0) {
			win_toolkit->_BeginBufferedPaint = 0;
			win_toolkit->_EndBufferedPaint = 0;
			return W_FALSE;
		}
	}
	return W_TRUE;
}
wresult _COMPOSITE_WM_PAINT(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	w_event_paint event;
	_w_graphics gc;
	PAINTSTRUCT ps;
	w_color forground, background;
	w_font *font = 0;
	int uiState, ret;
	if ((_W_WIDGET(widget)->state & STATE_DISPOSE_SENT) != 0) {
		e->result = 0;
		return TRUE;
	}
	if ((_W_WIDGET(widget)->state & STATE_CANVAS) == 0
			|| (_W_WIDGET(widget)->state & STATE_FOREIGN_HANDLE) != 0) {
		return _CONTROL_WM_PAINT(widget, e, priv);
	}
	/* Set the clipping bits */
	int oldBits = GetWindowLongW(_W_WIDGET(widget)->handle, GWL_STYLE);
	int newBits = oldBits | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	if (newBits != oldBits)
		SetWindowLongW(_W_WIDGET(widget)->handle, GWL_STYLE, newBits);

	HDC hDC = BeginPaint(_W_WIDGET(widget)->handle, &ps);

	/* Use the buffered paint when possible */
	wresult bufferedPaint = FALSE;
	if ((_W_WIDGET(widget)->style & W_DOUBLE_BUFFERED) != 0) {
		if ((_W_WIDGET(widget)->style
				& (W_NO_MERGE_PAINTS | W_RIGHT_TO_LEFT | W_TRANSPARENT)) == 0) {
			bufferedPaint = _w_composite_init_bufferedpaint();
		}
	}
	if (bufferedPaint) {
		int width = ps.rcPaint.right - ps.rcPaint.left;
		int height = ps.rcPaint.bottom - ps.rcPaint.top;
		if (width != 0 && height != 0) {
			HDC phdc;
			BP_BUFFERFORMAT flags = BPBF_COMPATIBLEBITMAP;
			_BeginBufferedPaint __BeginBufferedPaint =
					(_BeginBufferedPaint) win_toolkit->_BeginBufferedPaint;
			HPAINTBUFFER hBufferedPaint = __BeginBufferedPaint(hDC, &ps.rcPaint,
					flags, NULL, &phdc);
			_w_control_init_graphics(W_CONTROL(widget), W_GRAPHICS(&gc), phdc,
					0);
			if ((_W_WIDGET(widget)->style & W_NO_BACKGROUND) != 0) {
				/* This code is intentionally commented because it may be slow to copy bits from the screen */
				//paintGC.copyArea (image, ps.left, ps.top);
			} else {
				//drawBackground(phdc, &ps.rcPaint);
			}
			event.event.type = W_EVENT_PAINT;
			event.event.platform_event = (w_event_platform*) e;
			event.event.widget = widget;
			event.event.data = 0;
			event.bounds.x = ps.rcPaint.left;
			event.bounds.y = ps.rcPaint.top;
			event.bounds.width = width;
			event.bounds.height = height;
			event.gc = W_GRAPHICS(&gc);
			_w_widget_send_event(widget, (w_event*) &event);
			/* if (data.focusDrawn && !isDisposed())
			 updateUIState();*/
			w_graphics_dispose(W_GRAPHICS(&gc));
			_EndBufferedPaint __EndBufferedPaint =
					(_EndBufferedPaint) win_toolkit->_EndBufferedPaint;
			__EndBufferedPaint(hBufferedPaint, TRUE);
		}
	} else {
		/* Get the system region for the paint HDC */
		HRGN sysRgn = 0;
		if ((_W_WIDGET(widget)->style & (W_DOUBLE_BUFFERED | W_TRANSPARENT))
				!= 0 || (_W_WIDGET(widget)->style & W_NO_MERGE_PAINTS) != 0) {
			sysRgn = CreateRectRgn(0, 0, 0, 0);
			if (GetRandomRgn(hDC, sysRgn, SYSRGN) == 1) {
				if ((GetLayout(hDC) & LAYOUT_RTL) != 0) {
					int nBytes = GetRegionData(sysRgn, 0, NULL);
					LPRGNDATA lpRgnData = (LPRGNDATA) _w_toolkit_malloc(nBytes);
					if (lpRgnData != 0) {
						GetRegionData(sysRgn, nBytes, lpRgnData);
						static XFORM _rgnXForm[] = { -1, 0, 0, 1, 0, 0 };
						HRGN newSysRgn = ExtCreateRegion(_rgnXForm, nBytes,
								lpRgnData);
						DeleteObject(sysRgn);
						_w_toolkit_free(lpRgnData, nBytes);
						sysRgn = newSysRgn;
					}
				}
				POINT pt;
				MapWindowPoints(0, _W_WIDGET(widget)->handle, &pt, 1);
				OffsetRgn(sysRgn, pt.x, pt.y);
			}
		}

		/* Send the paint event */
		int width = ps.rcPaint.right - ps.rcPaint.left;
		int height = ps.rcPaint.bottom - ps.rcPaint.top;
		if (width != 0 && height != 0) {
			w_surface image;
			w_surface_init(&image);
			w_graphics_init(W_GRAPHICS(&gc));
			if ((_W_WIDGET(widget)->style & (W_DOUBLE_BUFFERED | W_TRANSPARENT))
					!= 0) {
				w_size size = { width, height };
				w_surface_create(&image, &size);
				w_surface_get_graphics(&image, W_GRAPHICS(&gc));
				/*gc = new GC(image, paintGC.getStyle() & W_RIGHT_TO_LEFT);
				 GCData gcData = gc.getGCData();
				 gcData.uiState = data.uiState;
				 gc.setForeground(getForeground());
				 gc.setBackground(getBackground());*/
				w_control_get_font(W_CONTROL(widget), &font);
				w_graphics_set_font(W_GRAPHICS(&gc), font);
				if ((_W_WIDGET(widget)->style & W_TRANSPARENT) != 0) {
					BitBlt(hDC, 0, 0, width, height,
					_W_GRAPHICS(&gc)->handle, ps.rcPaint.left, ps.rcPaint.top,
					SRCCOPY);
				}
				OffsetRgn(sysRgn, -ps.rcPaint.left, -ps.rcPaint.top);
				SelectClipRgn(hDC, sysRgn);
				OffsetRgn(sysRgn, ps.rcPaint.left, ps.rcPaint.top);
				SetMetaRgn(hDC);
				SetWindowOrgEx(hDC, ps.rcPaint.left, ps.rcPaint.top, NULL);
				SetBrushOrgEx(hDC, ps.rcPaint.left, ps.rcPaint.top, NULL);
				if ((_W_WIDGET(widget)->style
						& (W_NO_BACKGROUND | W_TRANSPARENT)) != 0) {
					/* This code is intentionally commented because it may be slow to copy bits from the screen */
					//paintGC.copyArea (image, ps.left, ps.top);
				} else {
					//drawBackground(gc.handle, &ps.rcPaint);
				}
			} else {
				_w_control_init_graphics(W_CONTROL(widget), W_GRAPHICS(&gc),
						hDC, 0);
			}
			RECT rect;
			if ((_W_WIDGET(widget)->style & W_NO_MERGE_PAINTS)
					!= 0&& GetRgnBox (sysRgn, &rect) == COMPLEXREGION) {
				int nBytes = GetRegionData(sysRgn, 0, NULL);
				LPRGNDATA lpRgnData = (LPRGNDATA) _w_toolkit_malloc(nBytes);
				if (lpRgnData != 0) {
					GetRegionData(sysRgn, nBytes, lpRgnData);
					int count = lpRgnData->rdh.nCount;
					for (int i = 0; i < count; i++) {
						int offset = 8 + (i << 2);
						SetRect(&rect, ((int*) lpRgnData)[offset],
								((int*) lpRgnData)[offset + 1],
								((int*) lpRgnData)[offset + 2],
								((int*) lpRgnData)[offset + 3]);
						if ((_W_WIDGET(widget)->style
								& (W_DOUBLE_BUFFERED | W_NO_BACKGROUND
										| W_TRANSPARENT)) == 0) {
							//drawBackground(gc.handle, &rect);
						}
						//event.count = count - 1 - i;
						event.event.type = W_EVENT_PAINT;
						event.event.platform_event = (w_event_platform*) e;
						event.event.widget = widget;
						event.event.data = 0;
						event.bounds.x = rect.left;
						event.bounds.y = rect.top;
						event.bounds.width = rect.right - rect.left;
						event.bounds.height = rect.bottom - rect.top;
						event.gc = W_GRAPHICS(&gc);
						_w_widget_send_event(widget, (w_event*) &event);
					}
					_w_toolkit_free(lpRgnData, nBytes);
				}
			} else {
				if ((_W_WIDGET(widget)->style
						& (W_DOUBLE_BUFFERED | W_NO_BACKGROUND | W_TRANSPARENT))
						== 0) {
					//drawBackground(hDC, &ps.rcPaint);
				}
				event.event.type = W_EVENT_PAINT;
				event.event.platform_event = (w_event_platform*) e;
				event.event.widget = widget;
				event.event.data = 0;
				event.bounds.x = ps.rcPaint.left;
				event.bounds.y = ps.rcPaint.top;
				event.bounds.width = width;
				event.bounds.height = height;
				event.gc = W_GRAPHICS(&gc);
				_w_widget_send_event(widget, (w_event*) &event);
			}
			// widget could be disposed at this point
			if ((_W_WIDGET(widget)->style & (W_DOUBLE_BUFFERED | W_TRANSPARENT))
					!= 0) {
				/*if (w_widget_is_ok(widget)) {
				 GCData gcData = gc.getGCData();
				 if (gcData.focusDrawn && !isDisposed())
				 updateUIState();
				 }
				 gc.dispose();
				 if (w_widget_is_ok(widget))
				 paintGC.drawImage(image, DPIUtil.autoScaleDown(ps.left),
				 DPIUtil.autoScaleDown(ps.top));
				 image.dispose();
				 gc = paintGC;*/
			}
			/* Dispose the paint GC */
			w_graphics_dispose(W_GRAPHICS(&gc));
			w_surface_dispose(&image);
		}
		if (sysRgn != 0)
			DeleteObject(sysRgn);
		/*if (data.focusDrawn && !isDisposed())
		 updateUIState();*/
	}
	EndPaint(_W_WIDGET(widget)->handle, &ps);

	/* Restore the clipping bits */
	if (w_widget_is_ok(widget)) {
		if (newBits != oldBits) {
			/*
			 * It is possible (but unlikely), that application
			 * code could have disposed the widget in the paint
			 * event.  If this happens, don't attempt to restore
			 * the style.
			 */
			SetWindowLong(_W_WIDGET(widget)->handle, GWL_STYLE, oldBits);
		}
	}
	e->result = 0;
	return TRUE;
}
wresult _COMPOSITE_WM_PRINTCLIENT(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _COMPOSITE_WM_SETFONT(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _COMPOSITE_WM_SIZE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	int result = W_FALSE;
	w_event _e;
	if ((_W_WIDGET(widget)->state & STATE_RESIZE_DEFERRED) != 0) {
		result = _SCROLLABLE_WM_SIZE(widget, e, priv);
	} else {
		result = _SCROLLABLE_WM_SIZE(widget, e, priv);
		_e.type = W_EVENT_LAYOUTDETECT;
		_e.platform_event = _EVENT_PLATFORM(e);
		_e.widget = widget;
		_e.data = 0;
		_w_widget_send_event(widget, &_e);
	}
	/* Damage the widget to cause a repaint */
	if (IsWindowVisible(_W_WIDGET(widget)->handle)) {
		if ((_W_WIDGET(widget)->state & STATE_CANVAS) != 0) {
			if ((_W_WIDGET(widget)->style & W_NO_REDRAW_RESIZE) == 0) {
				InvalidateRect(_W_WIDGET(widget)->handle, NULL, TRUE);
			}
		}
	}
	return result;
}
wresult _COMPOSITE_WM_SYSCOLORCHANGE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _COMPOSITE_WM_SYSCOMMAND(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _COMPOSITE_WM_UPDATEUISTATE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _COMPOSITE_WM_NCPAINT(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return W_FALSE;
}
wresult _COMPOSITE_WM_NOTIFY(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv) {
	return _CONTROL_WM_NOTIFY(widget, e, priv);
}
void _w_composite_class_init(struct _w_composite_class *clazz) {
	_w_scrollable_class_init(W_SCROLLABLE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_COMPOSITE;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_composite_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_composite);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_composite);
	/*
	 * functions
	 */
	clazz->get_children = _w_composite_get_children;
	clazz->get_layout = _w_composite_get_layout;
	clazz->get_layout_deferred = _w_composite_get_layout_deferred;
	clazz->get_tab_list = _w_composite_get_tab_list;
	clazz->is_layout_deferred = _w_composite_is_layout_deferred;
	clazz->do_layout = _w_composite_do_layout;
	clazz->layout_changed = _w_composite_layout_changed;
	clazz->set_layout = _w_composite_set_layout;
	clazz->set_layout_deferred = _w_composite_set_layout_deferred;
	/*
	 * private
	 */
	_w_control_priv *priv = _W_CONTROL_PRIV(W_WIDGET_CLASS(clazz)->reserved[0]);
	priv->create_handle = _w_composite_create_handle;
	/*
	 * messages
	 */
	dispatch_message *msg = priv->messages;
	msg[_WM_ERASEBKGND] = _COMPOSITE_WM_ERASEBKGND;
	msg[_WM_GETDLGCODE] = _COMPOSITE_WM_GETDLGCODE;
	msg[_WM_GETFONT] = _COMPOSITE_WM_GETFONT;
	msg[_WM_LBUTTONDOWN] = _COMPOSITE_WM_LBUTTONDOWN;
	msg[_WM_NCHITTEST] = _COMPOSITE_WM_NCHITTEST;
	msg[_WM_PARENTNOTIFY] = _COMPOSITE_WM_PARENTNOTIFY;
	msg[_WM_PAINT] = _COMPOSITE_WM_PAINT;
	msg[_WM_PRINTCLIENT] = _COMPOSITE_WM_PRINTCLIENT;
	msg[_WM_SETFONT] = _COMPOSITE_WM_SETFONT;
	msg[_WM_SIZE] = _COMPOSITE_WM_SIZE;
	msg[_WM_SYSCOLORCHANGE] = _COMPOSITE_WM_SYSCOLORCHANGE;
	msg[_WM_SYSCOMMAND] = _COMPOSITE_WM_SYSCOMMAND;
	msg[_WM_UPDATEUISTATE] = _COMPOSITE_WM_UPDATEUISTATE;
	msg[_WM_NCPAINT] = _COMPOSITE_WM_NCPAINT;
	msg[_WM_NOTIFY] = _COMPOSITE_WM_NOTIFY;
}
