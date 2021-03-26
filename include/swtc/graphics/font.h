/*
 * Name:        font.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef SWT_GRAPHICS_FONT_H_
#define SWT_GRAPHICS_FONT_H_
#include "rect.h"
#ifdef __cplusplus
extern "C" {
#endif
/*
 * fontdata
 */
typedef struct w_fontdata {
	void *handle[0x50];
} w_fontdata;
#define W_FONTDATA(x) ((w_fontdata*)x)
SWT_PUBLIC void w_fontdata_init(w_fontdata *fontdata);
SWT_PUBLIC wresult w_fontdata_create(w_fontdata *fontdata, const char *name,
		int length, int enc, int style, int size);
SWT_PUBLIC void w_fontdata_dispose(w_fontdata *fontdata);
SWT_PUBLIC int w_fontdata_get_height(w_fontdata *fontdata);
SWT_PUBLIC wresult w_fontdata_get_name(w_fontdata *fontdata, w_alloc name,
		void *user_data, int enc);
SWT_PUBLIC int w_fontdata_get_style(w_fontdata *fontdata);
SWT_PUBLIC wresult w_fontdata_set(w_fontdata *fontdata, const char *name,
		int length, int enc, int style, int size);
SWT_PUBLIC wresult w_fontdata_set_height(w_fontdata *fontdata, int height);
SWT_PUBLIC wresult w_fontdata_set_name(w_fontdata *fontdata, const char *name,
		int length, int enc);
SWT_PUBLIC wresult w_fontdata_set_style(w_fontdata *fontdata, int style);

/*
 * font
 */
typedef struct w_font {
	void *handle[2];
} w_font;
#define W_FONT(x) ((w_font*)x)
SWT_PUBLIC void w_font_init(w_font *font);
SWT_PUBLIC void w_font_dispose(w_font *font);
SWT_PUBLIC wresult w_font_is_ok(w_font *font);
SWT_PUBLIC wresult w_font_create(w_font *font, const char *name, int length,
		int enc, int style, int size);
SWT_PUBLIC wresult w_font_create_from_fontdata(w_font *font,
		w_fontdata *fontdata);
SWT_PUBLIC wresult w_font_get_fontdata(w_font *font, w_fontdata *fontdata);
/*
 * fontlist
 */
typedef struct w_fontlist {
	void *handle[2];
} w_fontlist;
#define W_FONTLIST(x) ((w_fontlist*)x)
SWT_PUBLIC void w_fontlist_init(w_fontlist *list);
SWT_PUBLIC void w_fontlist_dispose(w_fontlist *list);
SWT_PUBLIC wresult w_fontlist_is_ok(w_fontlist *list);
SWT_PUBLIC wresult w_fontlist_create(w_fontlist *list, int initialCount);
SWT_PUBLIC wresult w_fontlist_add(w_fontlist *list, w_fontdata *fontdata);
SWT_PUBLIC wresult w_fontlist_replace(w_fontlist *list, int index,
		w_fontdata *fontdata);
SWT_PUBLIC wresult w_fontlist_remove(w_fontlist *list, int index);
SWT_PUBLIC wresult w_fontlist_remove_all(w_fontlist *list);
SWT_PUBLIC wresult w_fontlist_get_font(w_fontlist *list, int index,
		w_font **font);
SWT_PUBLIC wresult w_fontlist_get_count(w_fontlist *list);
SWT_PUBLIC wresult w_fontlist_find_font(w_fontlist *list, w_fontdata *fontdata,
		w_font **font);
/*
 * font metrics
 */
typedef struct w_fontmetrics {
	int handle[20];
} w_fontmetrics;
#define W_FONTMETRICS(x) ((w_fontmetrics*)x)
SWT_PUBLIC void w_fontmetrics_init(w_fontmetrics *metrics);
SWT_PUBLIC void w_fontmetrics_dispose(w_fontmetrics *metrics);
SWT_PUBLIC int w_fontmetrics_get_ascent(w_fontmetrics *metrics);
SWT_PUBLIC int w_fontmetrics_get_average_charwidth(w_fontmetrics *metrics);
SWT_PUBLIC int w_fontmetrics_get_descent(w_fontmetrics *metrics);
SWT_PUBLIC int w_fontmetrics_get_height(w_fontmetrics *metrics);
SWT_PUBLIC int w_fontmetrics_get_leading(w_fontmetrics *metrics);

#ifdef __cplusplus
}
#endif
#endif /* SWT_GRAPHICS_FONT_H_ */
