/*
 * Name:        utf8.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef SWTC_CORE_UTF8_H_
#define SWTC_CORE_UTF8_H_
#include "alloc.h"
#ifdef __cplusplus
extern "C" {
#endif
enum {
	W_ENCODING_UTF8 = 0, W_ENCODING_UNICODE, W_ENCODING_PLATFORM,
};
typedef struct w_utf8_char {
	wuint char32;
	wchar char16[2];
	int char16_length;
} w_utf8_char;
/*
 * utf8 string
 */
#define W_UTF8_ATTNDS
typedef struct w_utf8_iter {
	const char *p;
	wuint utf32;
	wchar utf16;
	short flags;
} w_utf8_iter;
SWT_PUBLIC extern const char w_utf8_skip_data[256];
SWT_PUBLIC void w_utf8_iter_init(w_utf8_iter *iter, const char *p);
SWT_PUBLIC wunichar w_utf8_iter_next_utf32(w_utf8_iter *iter);
SWT_PUBLIC wchar w_utf8_iter_next_utf16(w_utf8_iter *iter);
SWT_PUBLIC wunichar w_utf8_get_unichar(const char *p);
SWT_PUBLIC int w_utf8_get_wchars(const char *p, wchar *wchars);
SWT_PUBLIC wunichar w_utf8_get_char_extended(const char *p, int max_len);
SWT_PUBLIC wunichar w_utf8_get_next_char(const char *p, const char **next);
SWT_PUBLIC const char* w_utf8_find_prev(const char *str, const char *p);
SWT_PUBLIC const char* w_utf8_find_next(const char *p, const char *end);
SWT_PUBLIC const char* w_utf8_prev(const char *p);
SWT_PUBLIC const char* w_utf8_next(const char *p);
SWT_PUBLIC const char* w_utf8_next_unichar(const char *p, wunichar *_unichar);
SWT_PUBLIC const char* w_utf8_next_wchars(const char *p, wchar *wchars);
SWT_PUBLIC size_t w_utf8_strlen(const char *p, size_t max);
SWT_PUBLIC char* w_utf8_offset_to_pointer(const char *str, int offset);
SWT_PUBLIC int w_utf8_pointer_to_offset(const char *str, const char *pos);
SWT_PUBLIC char* w_utf8_strncpy(char *dest, const char *src, int n);
SWT_PUBLIC int w_utf8_from_unichar(int c, char *outbuf);
SWT_PUBLIC char* w_utf8_strchr(const char *p, int c);
SWT_PUBLIC int w_utf8_from_utf16(const wchar *str, int len, char *result,
		int n);
SWT_PUBLIC int w_utf8_to_utf16(const char *str, int len, wchar *result, int n);
SWT_PUBLIC int w_utf8_from_ucs4(const wuint *str, int len, char *result, int n);
SWT_PUBLIC int w_utf8_to_ucs4(const char *str, int len, wunichar *result,
		int n);
SWT_PUBLIC char* w_strndup(const char *s, size_t n);
#ifdef __cplusplus
}
#endif
#endif /* SWTC_CORE_UTF8_H_ */
