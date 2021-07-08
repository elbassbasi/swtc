/*
 * Name:        core.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "core.h"
#include "../widgets/toolkit.h"
#include <wchar.h>
wresult w_alloc_set_text(w_alloc alloc, void *user_data, int toenc,
		const char *text, int length, int enc) {
	if (alloc == 0)
		return W_FALSE;
	if ((enc & W_ENCODING_MASK) == W_ENCODING_PLATFORM
			|| (enc & W_ENCODING_MASK) == W_ENCODING_UNICODE) {
		if ((toenc & W_ENCODING_MASK) == W_ENCODING_PLATFORM
				|| (toenc & W_ENCODING_MASK) == W_ENCODING_UNICODE) {
			if (length < 0)
				length = wcslen((wchar_t*) text);
			void *buf = 0;
			int l2 = alloc(user_data, (length + 1) * sizeof(wchar_t), &buf);
			if (buf == 0)
				return W_ERROR_NO_MEMORY;
			int l = WMIN(length, l2 / 2);
			memcpy(buf, text, l * sizeof(wchar_t));
			return W_TRUE;
		} else {
			int l = w_utf8_from_utf16((wchar_t*) text, length, 0, 0);
			void *buf = 0;
			int l2 = alloc(user_data, l + 1, &buf);
			if (buf == 0)
				return W_ERROR_NO_MEMORY;
			int ll = WMIN(l, l2);
			w_utf8_from_utf16((wchar_t*) text, length, (char*) buf, ll);
			return W_TRUE;
		}
	} else {
		if ((toenc & W_ENCODING_MASK) == W_ENCODING_PLATFORM
				|| (toenc & W_ENCODING_MASK) == W_ENCODING_UNICODE) {
			int l = w_utf8_to_utf16(text, length, 0, 0);
			void *buf = 0;
			int l2 = alloc(user_data, (l + 1) * sizeof(wchar_t), &buf);
			if (buf == 0)
				return W_ERROR_NO_MEMORY;
			int ll = WMIN(l, l2 / 2);
			w_utf8_to_utf16(text, length, (wchar_t*) buf, ll);
			return W_TRUE;
		} else {
			if (length < 0)
				length = strlen(text);
			char *buf = 0;
			int l2 = alloc(user_data, length + 1, (void**) &buf);
			if (buf == 0)
				return W_ERROR_NO_MEMORY;
			int l = WMIN(length, l2);
			memcpy(buf, text, l);
			buf[length] = 0;
			return W_TRUE;
		}
	}
}
wresult w_alloc_printf(w_alloc alloc, void *user_data, int toenc, int enc,
		const char *format, va_list args) {
	va_list args2;
	va_copy(args2, args);
	wchar_t *_format;
	if ((enc & W_ENCODING_MASK) == W_ENCODING_PLATFORM
			|| (enc & W_ENCODING_MASK) == W_ENCODING_UNICODE) {
		if ((toenc & W_ENCODING_MASK) == W_ENCODING_PLATFORM
				|| (toenc & W_ENCODING_MASK) == W_ENCODING_UNICODE) {
			size_t sz = vswprintf(0, 0, (wchar_t*) format, args2);
			va_end(args2);
			void *buf = 0;
			int l2 = alloc(user_data, (sz + 1) * sizeof(wchar_t), &buf);
			if (buf == 0)
				return W_ERROR_NO_MEMORY;
			vswprintf((wchar_t*) buf, l2 / 2, (wchar_t*) format, args);
			return W_TRUE;
		} else {
			int l = w_utf8_from_utf16((wchar_t*) format, -1, 0, 0);
			int size = l + 1;
			char *_format = _w_toolkit_malloc(size);
			if (_format == 0)
				return W_ERROR_NO_MEMORY;
			w_utf8_from_utf16((wchar_t*) format, -1, _format, l + 1);
			size_t sz = vsnprintf(0, 0, _format, args2);
			va_end(args2);
			_w_toolkit_free(_format, size);
			void *buf = 0;
			int l2 = alloc(user_data, sz + 1, &buf);
			if (buf == 0)
				return W_ERROR_NO_MEMORY;
			_format = _w_toolkit_malloc(size);
			if (_format == 0)
				return W_ERROR_NO_MEMORY;
			w_utf8_from_utf16((wchar_t*) format, -1, _format, l + 1);
			vsnprintf((char*) buf, l2, _format, args);
			_w_toolkit_free(_format, size);
			return W_TRUE;
		}
	} else {
		if ((toenc & W_ENCODING_MASK) == W_ENCODING_PLATFORM
				|| (toenc & W_ENCODING_MASK) == W_ENCODING_UNICODE) {
			size_t size;
			char *s = (char*) _w_toolkit_malloc_all(&size);
			size_t sz = vsnprintf(s, size, format, args2);
			va_end(args2);
			if (sz > size) {
				_w_toolkit_free(s, size);
				s = malloc(sz);
			}
			int utf16_size = w_utf8_to_utf16(s, sz, 0, 0);
			if (sz <= size) {
				_w_toolkit_free(s, size);
			}
			void *buf = 0;
			int l2 = alloc(user_data, (utf16_size + 1) * sizeof(wchar_t), &buf);
			if (buf == 0)
				return W_ERROR_NO_MEMORY;
			if (sz <= size) {
				s = (char*) _w_toolkit_malloc(sz);
				vsnprintf(s, sz, format, args);
			}
			w_utf8_to_utf16(s, sz, (wchar_t*) buf,
					(utf16_size + 1) * sizeof(wchar_t));
			if (sz <= size) {
				_w_toolkit_free(s, sz);
			} else {
				free(s);
			}
			return W_TRUE;
		} else {
			size_t sz = vsnprintf(0, 0, format, args2);
			va_end(args2);
			void *buf = 0;
			int l2 = alloc(user_data, sz + 1, &buf);
			if (buf == 0)
				return W_ERROR_NO_MEMORY;
			vsnprintf((char*) buf, l2, format, args);
			return W_TRUE;
		}
	}
}

int _mnemonics_from_gtk_0(const char *text, size_t text_length, char *out,
		size_t out_length, int *process) {
	int length = 0;
	*process = 0;
	char ch, _t[2], _l;
	for (wuint i = 0; (i < text_length && (ch = text[i]) != 0); i++) {
		_l = 1;
		if (ch == '_') {
			if ((i + 1) < text_length && text[i + 1] == '_') {
				_t[0] = '_';
				i++;
			} else {
				_t[0] = '&';
			}
			*process = 1;
		} else if (ch == '&') {
			_t[0] = '&';
			_t[1] = '&';
			_l = 2;
			*process = 1;
		} else {
			_t[0] = ch;
		}
		if (length < out_length) {
			memcpy(&out[length], _t, _l);
		}
		length += _l;
	}
	return length;
}
char* _gtk_text_fix(const char *text, int text_length, int enc, int *newlength,
		int *mnemonic) {
	if (text == 0)
		return 0;
	*mnemonic = -1;
	if ((enc & 0xFF) == W_ENCODING_UNICODE) {
		size_t new_size;
		char *s = _w_toolkit_malloc_all(&new_size);
		size_t l = w_utf8_from_utf16((const wchar_t*) text, text_length, s,
				new_size);
		if ((enc & _GTK_TEXT_FIX_COPY) != 0 || l > new_size) {
			_w_toolkit_free(s, new_size);
			s = _w_toolkit_malloc(l + 1);
			if (s == 0)
				return 0;
			w_utf8_from_utf16((const wchar_t*) text, text_length, s, l + 1);
		}
		if (newlength != 0) {
			*newlength = l + 1;
		}
		return s;
	} else {
		// W_ENCODING_UTF8
		if ((enc & _GTK_TEXT_FIX_COPY) == 0
				&& (text_length == -1
						|| (enc & _GTK_TEXT_FIX_REQUIRED_NULL) == 0)) {
			if (newlength != 0) {
				if ((enc & _GTK_TEXT_FIX_CALCUL_LENGTH) != 0
						&& text_length == -1) {
					*newlength = strlen(text);
				} else {
					*newlength = text_length;
				}
			}
			return (char*) text;
		} else {
			size_t new_size;
			char *s = _w_toolkit_malloc_all(&new_size);
			if ((enc & _GTK_TEXT_FIX_COPY) != 0
					|| (text_length + 1) > new_size) {
				_w_toolkit_free(s, new_size);
				s = malloc(text_length + 1);
				if (s == 0)
					return 0;
			}
			if (newlength != 0) {
				*newlength = text_length + 1;
			}
			memcpy(s, text, text_length);
			s[text_length] = 0;
			return s;
		}
	}
}
void _gtk_text_free(const char *text, char *alloc, int length) {
	if (text == alloc)
		return;
	_w_toolkit_free(alloc, length);
}
wresult _gtk_alloc_set_text(w_alloc string, void *user_data, const char *text,
		size_t text_length, int enc) {
	if (text == 0)
		return W_FALSE;
	int process;
	int length = _mnemonics_from_gtk_0(text, text_length, 0, 0, &process);
	char *t = 0;
	int ll = string(user_data, length + 1, (void**) &t);
	if (t != 0 && ll > 0) {
		_mnemonics_from_gtk_0(text, text_length, t, ll, &process);
		t[ll - 1] = 0;
		return W_TRUE;
	}
	return W_FALSE;
}
