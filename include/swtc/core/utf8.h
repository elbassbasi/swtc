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
	W_ENCODING_MASK = 0xFF << 24, //
	W_ENCODING_UTF8 = 0 << 24, //
	W_ENCODING_UNICODE = 1 << 24, //
	W_ENCODING_PLATFORM = 2 << 24, //
};
#define _W_ENCODING(x) (x >> 24)
typedef struct w_utf8_char {
	wuint char32;
	wchar char16[2];
	int char16_length;
} w_utf8_char;
/*
 * utf8 string
 */
SWT_PUBLIC extern wuchar w_utf8_skip_data[256];
SWT_PUBLIC wunichar w_utf16_get_unichar(const wchar_t *p, wintptr max_len);
SWT_PUBLIC const wchar_t* w_utf16_next(const wchar_t *p);
SWT_PUBLIC wunichar w_utf8_get_unichar(const char *p, wintptr max_len);
SWT_PUBLIC int w_utf8_get_wchars(const char *p, wchar_t *wchars);
SWT_PUBLIC const char* w_utf8_find_prev(const char *str, const char *p);
SWT_PUBLIC const char* w_utf8_find_next(const char *p, const char *end);
SWT_PUBLIC const char* w_utf8_prev(const char *p);
SWT_PUBLIC const char* w_utf8_next(const char *p);
SWT_PUBLIC size_t w_utf8_strlen(const char *p, size_t max);
SWT_PUBLIC char* w_utf8_offset_to_pointer(const char *str, int offset);
SWT_PUBLIC int w_utf8_pointer_to_offset(const char *str, const char *pos);
SWT_PUBLIC char* w_utf8_strncpy(char *dest, const char *src, size_t n);
SWT_PUBLIC size_t w_utf8_add_unichar(char *outbuf, size_t sz, wunichar c);
SWT_PUBLIC char* w_utf8_strchr(const char *p, int c);
SWT_PUBLIC char* w_strndup(const char *s, size_t n);
/*
 *
 */
SWT_PUBLIC int w_utf8_from_utf16(const wchar_t *str, size_t len, char *result,
		size_t n);
SWT_PUBLIC int w_utf8_to_utf16(const char *str, size_t len, wchar_t *result,
		size_t n);
SWT_PUBLIC int w_utf8_from_ucs4(const wunichar *str, size_t len, char *result,
		size_t n);
SWT_PUBLIC int w_utf8_to_ucs4(const char *str, size_t len, wunichar *result,
		size_t n);
#ifdef __cplusplus
}
#endif
#endif /* SWTC_CORE_UTF8_H_ */
