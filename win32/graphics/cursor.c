/*
 * Name:        cursor.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "gc.h"
void w_cursor_init(w_cursor *cursor) {
	_W_CURSOR(cursor)->handle = 0;
}
void w_cursor_dispose(w_cursor *cursor) {
	if (_W_CURSOR(cursor)->handle != 0) {
		if (_W_CURSOR(cursor)->flags & W_CURSOR_IS_ICON) {
			DestroyIcon((HICON) _W_CURSOR(cursor)->handle);
		} else {
			/*
			 * The MSDN states that one should not destroy a shared
			 * handle, that is, one obtained from LoadCursor.
			 * However, it does not appear to do any harm, so rather
			 * than keep track of how a handle was created, we just
			 * destroy them all. If this causes problems in the future,
			 * put the flag back in.
			 */
			DestroyCursor((HICON) _W_CURSOR(cursor)->handle);
		}
		_W_CURSOR(cursor)->handle = 0;
	}
}
wresult w_cursor_is_ok(w_cursor *cursor) {
	return cursor != 0 && _W_CURSOR(cursor)->handle != 0;
}
wresult w_cursor_create(w_cursor *cursor, w_image *source, int hotspotX,
		int hotspotY) {
	if (cursor == 0)
		return W_ERROR_NULL_ARGUMENT;
	w_cursor_dispose(cursor);
	if (source == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (w_image_is_ok(source) <= 0)
		return W_ERROR_INVALID_ARGUMENT;
	/* Check the hotspots */
	w_size sz;
	w_image_get_size(source, &sz);
	if (hotspotX >= sz.width || hotspotX < 0 || hotspotY >= sz.height
			|| hotspotY < 0) {
		return W_ERROR_INVALID_ARGUMENT;
	}
	_w_image_hbitmap bitmap;
	_w_image_get_hbitmap(source, &bitmap);
	/* Create the icon */
	ICONINFO info;
	info.fIcon = FALSE;
	info.hbmColor = bitmap.hbmColor;
	info.hbmMask = bitmap.hbmMask;
	info.xHotspot = hotspotX;
	info.yHotspot = hotspotY;
	_W_CURSOR(cursor)->handle = (HCURSOR) CreateIconIndirect(&info);
	_w_image_dispose_hbitmap(source, &bitmap);
	if (_W_CURSOR(cursor)->handle == 0)
		return W_ERROR_NO_HANDLES;
	_W_CURSOR(cursor)->flags = W_CURSOR_IS_ICON;
	return W_TRUE;
}
