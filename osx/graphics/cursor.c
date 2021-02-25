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
		NSObject_release(NSOBJECT(_W_CURSOR(cursor)->handle));
		_W_CURSOR(cursor)->handle = 0;
	}
}
wresult w_cursor_is_ok(w_cursor *cursor) {
	return cursor != 0 && _W_CURSOR(cursor)->handle != 0;
}
wresult w_cursor_create(w_cursor *cursor, w_image *source, int hotspotX,
		int hotspotY) {
	w_cursor_dispose(cursor);
	if (source == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_W_IMAGE(source)->handle == 0)
		return W_ERROR_NO_HANDLES;
	NSPoint pt;
	pt.x = hotspotX;
	pt.y = hotspotY;
	NSCursor *handle = NSCursor_initWithImage(_W_IMAGE(source)->handle, &pt);
	if (handle == 0)
		return W_ERROR_NO_HANDLES;
	_W_CURSOR(cursor)->handle = handle;
	return W_TRUE;
}
