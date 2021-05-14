/*
 * Name:        core.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "core.h"
#include "../widgets/toolkit.h"
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
