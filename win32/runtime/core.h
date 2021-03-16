/*
 * Name:        core.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef WIN32_CORE_WIN32_H_
#define WIN32_CORE_WIN32_H_
#include <swtc.h>
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <uxtheme.h>
#include "resource.h"
#ifdef __cplusplus
extern "C" {
#endif
#define VERSION(major,minor) (major << 16 | minor)
#define WMIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define WMAX(X, Y) (((X) > (Y)) ? (X) : (Y))
extern HINSTANCE hinst;
wresult _win_text_fix(const char *text, int text_length, int enc, WCHAR **str,
		int *newlength);
void _win_text_free(const char *text, WCHAR *str, int length);
wresult _win_text_set(WCHAR *text, int length, w_alloc alloc, void *user_data,
		int enc);
#ifdef __cplusplus
}
#endif
#endif /* WIN32_CORE_WIN32_H_ */
