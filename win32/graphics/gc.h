/*
 * Name:        gc.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef WIN32_GRAPHICS_GC_H_
#define WIN32_GRAPHICS_GC_H_
#include "../runtime/core.h"
#include "gdip.h"
#ifdef __cplusplus
extern "C" {
#endif
/*
 * image
 */
enum {
	_IMAGE_HBITMAP = 1, _IMAGE_GPBITMAP = 2, _IMAGE_ICON = 3,
};
typedef struct _w_image {
	void *handle;
	union {
		struct {
			unsigned type :2;
			unsigned nodispose :1;
		};
		int flags;
	};
} _w_image;
#define _W_IMAGE(x) ((_w_image*)x)

typedef struct _w_image_hbitmap {
	HICON hicon;
	HBITMAP hbmColor;
	HBITMAP hbmMask;
} _w_image_hbitmap;

void _w_image_get_hbitmap(w_image *image, _w_image_hbitmap *hbitmap);
void _w_image_dispose_hbitmap(w_image *image, _w_image_hbitmap *hbitmap);
void _w_image_get_gpimage(w_image *image, GpImage **gpimage);
void _w_image_dispose_gpimage(w_image *image, GpImage *gpimage);
wresult w_image_resize_0(w_image *image, w_size *size, GpImage **dstimg);
HBITMAP _w_image_create_32bit_dib(w_image *image);
typedef struct swt_stream {
	void *lpVtbl;
	w_stream *s;
} swt_stream;

void swt_stream_init(swt_stream *stream, w_stream *s);
/*
 * imagelist
 */
typedef struct _w_imagelist {
	HIMAGELIST imagelist;
} _w_imagelist;
#define _W_IMAGELIST(x) ((_w_imagelist*)x)
/*
 * cursor
 */
typedef struct _w_cursor {
	HCURSOR handle;
	int flags;
} _w_cursor;
#define W_CURSOR_IS_ICON (1 << 0)
#define _W_CURSOR(x) ((struct _w_cursor*)x)
/*
 * pattern
 */
typedef struct _w_pattern {
	GpBrush *handle;
} _w_pattern;
#define _W_PATTERN(x) ((_w_pattern*)x)
/*
 * pattern
 */
typedef struct _w_region {
	HRGN handle;
} _w_region;
#define _W_REGION(x) ((_w_region*)x)
/*
 * path
 */
typedef struct _w_path {
	GpPath *handle;
	GpPointF currentPoint;
	GpPointF startPoint;
} _w_path;
#define _W_PATH(x) ((_w_path*)x)
/*
 * transform
 */
typedef struct _w_transform {
	GpMatrix *handle;
} _w_transform;
#define _W_TRANSFORM(x) ((_w_transform*)x)
/*
 * graphics
 */
enum {
	GRAPHICS_STATE_DELETEDC = 1 << 0,
	GRAPHICS_STATE_RELEASEDC = 1 << 1,
	GRAPHICS_STATE_HIDEFOCUS = 1 << 2,
	GRAPHICS_STATE_GDIPRELEASEDC = 1 << 3,
	GRAPHICS_STATE_FOREGROUND = 1 << 5,
	GRAPHICS_STATE_BACKGROUND = 1 << 6,
	GRAPHICS_STATE_FONT = 1 << 7,
	GRAPHICS_STATE_LINE_STYLE = 1 << 8,
	GRAPHICS_STATE_LINE_WIDTH = 1 << 9,
	GRAPHICS_STATE_LINE_CAP = 1 << 10,
	GRAPHICS_STATE_LINE_JOIN = 1 << 11,
	GRAPHICS_STATE_LINE_MITERLIMIT = 1 << 12,
	GRAPHICS_STATE_FOREGROUND_TEXT = 1 << 13,
	GRAPHICS_STATE_BACKGROUND_TEXT = 1 << 14,
	GRAPHICS_STATE_BRUSH = 1 << 15,
	GRAPHICS_STATE_PEN = 1 << 16,
	GRAPHICS_STATE_NULL_BRUSH = 1 << 17,
	GRAPHICS_STATE_NULL_PEN = 1 << 18,
	GRAPHICS_STATE_DRAW_OFFSET = 1 << 19,
	GRAPHICS_STATE_MIRRORED = 1 << 20,
	GRAPHICS_STATE_UISF_HIDEACCEL = 1 << 21,
	GRAPHICS_STATE_UISF_HIDEFOCUS = 1 << 22,
	GRAPHICS_STATE_RIGHT_TO_LEFT = 1 << 23,
	GRAPHICS_STATE_LEFT_TO_RIGHT = 1 << 24,

	GRAPHICS_STATE_DRAW = GRAPHICS_STATE_FOREGROUND | GRAPHICS_STATE_LINE_STYLE
			| GRAPHICS_STATE_LINE_WIDTH | GRAPHICS_STATE_LINE_CAP
			| GRAPHICS_STATE_LINE_JOIN | GRAPHICS_STATE_LINE_MITERLIMIT
			| GRAPHICS_STATE_PEN | GRAPHICS_STATE_NULL_BRUSH
			| GRAPHICS_STATE_DRAW_OFFSET,
	GRAPHICS_STATE_FILL = GRAPHICS_STATE_BACKGROUND | GRAPHICS_STATE_BRUSH
			| GRAPHICS_STATE_NULL_PEN
};
typedef struct _w_graphics {
	HDC handle;
	HWND hwnd;
	w_font *font;
	int state;
	w_color foreground;
	w_color background;
	char alpha;
	w_line_attributes line;
	HPEN hPen;
	HPEN hOldPen;
	HBRUSH hBrush;
	HBRUSH hOldBrush;
	HFONT hGDIFont;
	GpGraphics *gdipGraphics;
	GpPen *gdipPen;
	GpBrush *gdipFgBrush;
	GpBrush *gdipBgBrush;
	GpBrush *gdipBrush;
	GpFont *gdipFont;
	float gdipXOffset, gdipYOffset;
	w_pattern *foregroundPattern;
	w_pattern *backgroundPattern;
	w_surface *image;
	HBITMAP oldbitmap;
	PAINTSTRUCT *ps;
} _w_graphics;

void _w_graphics_init(w_graphics *gc, HDC handle);
wresult w_graphics_check(w_graphics *gc, int mask);
GpBrush* _w_graphics_get_fg_brush(w_graphics *gc);
wresult _w_graphics_init_gdip(w_graphics *gc);
void _w_graphics_set_clipping_hrgn(w_graphics *gc, HRGN clipRgn);
GpFont* w_graphics_create_gdip_font(HDC hDC, HFONT hFont, GpGraphics *graphics,
		GpFontCollection *fontCollection, GpFontFamily **outFamily,
		HFONT *outFont);
#define _W_GRAPHICS(x) ((_w_graphics*)x)
/*
 * textlayout
 */

typedef struct _w_textlayout {
	w_font *font;
	struct _w_textlayout_text *text;
	int textDirection;
	int *tabs;
	int tabs_length;
	int lineSpacingInPoints;
	int ascentInPixels;
	int descentInPixels;
	int alignment;
	int wrapWidth;
	int orientation;
	int indent;
	int wrapIndent;
	unsigned justify :1;
	unsigned created :1;
	unsigned is_complex :1;
} _w_textlayout;
#define _W_TEXTLAYOUT(x) ((_w_textlayout*)x)
#ifdef __cplusplus
}
#endif
#endif /* WIN32_GRAPHICS_GC_H_ */
