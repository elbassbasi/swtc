/*
 * Name:        core.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "core.h"
#include "../widgets/toolkit.h"
wresult _win_text_fix(const char *text, int text_length, int enc, WCHAR **str,
		int *newlength) {
	*str = 0;
	*newlength = 0;
	if (text == 0)
		return 0;
	WCHAR *s;
	if ((enc & 0xFF) == W_ENCODING_UNICODE
			|| (enc & 0xFF) == W_ENCODING_PLATFORM) {
		if (text_length == -1) {
			s = (WCHAR*) text;
			*newlength = lstrlenW(s);
		} else {
			size_t total;
			s = _w_toolkit_malloc((text_length + 1) * sizeof(WCHAR));
			if (s != 0) {
				memcpy(s, text, text_length * sizeof(WCHAR));
				s[text_length] = 0;
				*newlength = text_length + 1;
			} else
				*newlength = 0;
		}
	} else {
		size_t total;
		s = (WCHAR*) _w_toolkit_malloc_all(&total);
		size_t l = w_utf8_to_utf16(text, text_length, s, total / sizeof(WCHAR));
		if (((l + 1) * sizeof(WCHAR)) > total) {
			win_toolkit->tmp_length -= total;
			s = _w_toolkit_malloc((l + 1) * sizeof(WCHAR));
			if (s != 0) {
				w_utf8_to_utf16(text, text_length, s, l + 1);
				*newlength = l + 1;
			} else
				*newlength = 0;
		} else {
			win_toolkit->tmp_length += (l + 1) * sizeof(WCHAR) - total;
			*newlength = l + 1;
		}
	}
	*str = s;
	return W_TRUE;
}
void _win_text_free(const char *text, WCHAR *alloc, int length) {
	if ((WCHAR*) text == alloc || alloc == 0)
		return;
	_w_toolkit_free(alloc, length * sizeof(WCHAR));
}
wresult _win_text_set(WCHAR *text, int length, w_alloc alloc, void *user_data,
		int enc) {
	if ((enc & 0xFF) == W_ENCODING_UNICODE
			|| (enc & 0xFF) == W_ENCODING_PLATFORM) {
		if (length < 0)
			length = lstrlenW(text);
		void *buf = 0;
		int l2 = alloc(user_data, length + 1, &buf);
		if (buf == 0)
			return W_ERROR_NO_MEMORY;
		int l = WMIN(length, l2);
		memcpy(buf, text, l * sizeof(WCHAR));
		return W_TRUE;
	} else {
		int l = w_utf8_from_utf16(text, length, 0, 0);
		void *buf = 0;
		int l2 = alloc(user_data, l + 1, &buf);
		if (buf == 0)
			return W_ERROR_NO_MEMORY;
		int ll = WMIN(l, l2);
		w_utf8_from_utf16(text, length, (char*) buf, ll);
		return W_TRUE;
	}
}
HINSTANCE hinst;
int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpvReserved) {
	switch (dwReason) {
	case DLL_PROCESS_ATTACH:
		hinst = hInstance;
		break;
	case DLL_PROCESS_DETACH:
		hinst = 0;
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}
