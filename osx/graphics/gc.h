/*
 * Name:        gc.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef OSX_GRAPHICS_GC_H_
#define OSX_GRAPHICS_GC_H_
#include "../objc/objc.h"
#ifdef __cplusplus
extern "C" {
#endif
/*
 * font
 */
typedef struct _w_fontdata {
	int style;
	float height;
	NSString *nsName;
	char name[0];
} _w_fontdata;
#define _W_FONTDATA(x) ((_w_fontdata*)x)
typedef struct _w_font_metrics {
	int ascent;
	int descent;
	int averageCharWidth;
	int leading;
	int height;
} _w_font_metrics;
#define _W_FONTMETRICS(x) ((_w_font_metrics*)x)
typedef struct _w_font {
	NSFont *handle;
	unsigned isItalic :1;
	unsigned isBold :1;
	unsigned isMetrics :1;
#if defined(__LP64__) && __LP64__
	unsigned height :20;
	unsigned ascent :20;
	unsigned avgWidth :20;
#else
	unsigned height :9;
	unsigned ascent :9;
	unsigned avgWidth :9;
#endif
} _w_font;
#define _W_FONT(x) ((_w_font*)x)
void _w_font_add_traits_1(w_font *font, NSMutableDictionary *dict);
/*
 * image
 */
typedef struct _w_image {
	NSImage *handle;
} _w_image;
#define _W_IMAGE(x) ((_w_image*)x)
typedef struct _w_surface {
	NSBitmapImageRep *handle;
} _w_surface;
#define _W_SURFACE(x) ((_w_surface*)x)
typedef struct _w_imagelist_images {
	int alloc;
	int count;
	int style;
	int width;
	int height;
	void *reserved;
	NSImage *images[0];
} _w_imagelist_images;
typedef struct _w_imagelist {
	_w_imagelist_images *images;
} _w_imagelist;
#define _W_IMAGELIST(x) ((_w_imagelist*)x)
/*
 * cursor
 */
typedef struct _w_cursor {
	NSCursor* handle;
} _w_cursor;
#define _W_CURSOR(x) ((_w_cursor*)x)
/*
 *	pattern
 */
typedef struct _w_pattern_gradient {
	NSGradient *gradient;
	NSPoint pt1;
	NSPoint pt2;
	w_color color1;
	w_color color2;
} _w_pattern_gradient;
typedef struct _w_pattern {
	union {
		NSColor *color;
		_w_pattern_gradient *gradient;
	};
	unsigned is_color :1;
	unsigned height :30;
} _w_pattern;
#define _W_PATTERN(x) ((_w_pattern*)x)
/*
 * transform
 */
typedef struct _w_transfrom {
	NSAffineTransform *handle;
} _w_transfrom;
#define _W_TRANSFORM(x) ((_w_transfrom*)x)
/*
 * path
 */
typedef struct _w_path {
	NSBezierPath *handle;
} _w_path;
#define _W_PATH(x) ((_w_path*)x)
/*
 * region
 */
typedef struct _w_region {
	NSBezierPath *handle;
} _w_region;
#define _W_REGION(x) ((_w_region*)x)
/*
 * graphics
 */
#define GRAPHICS_STATE_FOREGROUND  (1 << 0)
#define GRAPHICS_STATE_BACKGROUND  (1 << 1)
#define GRAPHICS_STATE_FONT  (1 << 2)
#define GRAPHICS_STATE_LINE_STYLE  (1 << 3)
#define GRAPHICS_STATE_LINE_CAP  (1 << 4)
#define GRAPHICS_STATE_LINE_JOIN  (1 << 5)
#define GRAPHICS_STATE_LINE_WIDTH  (1 << 6)
#define GRAPHICS_STATE_LINE_MITERLIMIT  (1 << 7)
#define GRAPHICS_STATE_BACKGROUND_BG  (1 << 8)
#define GRAPHICS_STATE_DRAW_OFFSET  (1 << 9)
#define GRAPHICS_STATE_FOREGROUND_FILL (1 << 10)
#define GRAPHICS_STATE_CLIPPING (1 << 11)
#define GRAPHICS_STATE_TRANSFORM (1 << 12)
#define GRAPHICS_STATE_VISIBLE_REGION (1 << 13)
#define GRAPHICS_STATE_DRAW  (GRAPHICS_STATE_FOREGROUND | GRAPHICS_STATE_LINE_WIDTH | \
GRAPHICS_STATE_LINE_STYLE | GRAPHICS_STATE_LINE_CAP | GRAPHICS_STATE_LINE_JOIN\
		| GRAPHICS_STATE_LINE_MITERLIMIT | GRAPHICS_STATE_DRAW_OFFSET)
#define GRAPHICS_STATE_FILL  GRAPHICS_STATE_BACKGROUND
typedef struct _w_graphics {
	NSGraphicsContext *handle;
	NSGraphicsContext *flippedContext;
	NSBezierPath *path;
	NSBezierPath *clipPath, *visiblePath;
	void *visibleRgn;
	NSView *view;
	NSBitmapImageRep *image;
	wuchar alpha;
	unsigned textAntialias :2; // textAntialias -1
	unsigned antialias :2; // antialias -1
	unsigned xorMode :1;
	unsigned fillRule :2;
	unsigned restoreContext :1;
	unsigned ispaintRect :1;
	unsigned issize :1;
	unsigned isFlipped :1;
	wushort state;
	wushort style;
	int heigth;
	CGFloat drawXOffset;
	CGFloat drawYOffset;
	NSRect paintRect;
	NSSize size;
	w_pattern *foregroundPattern;
	w_pattern *backgroundPattern;
	NSAffineTransform *transform;
	NSAffineTransform *inverseTransform;
	w_color foreground;
	w_color background;
	NSColor *fg;
	NSColor *bg;
	w_font *font;
	NSTextStorage *textStorage;
	NSLayoutManager *layoutManager;
	NSTextContainer *textContainer;
	w_line_attributes line;
} _w_graphics;
void _w_graphics_init(w_graphics *gc, NSGraphicsContext *context, int flags);
void _w_graphics_fill_pattern(w_graphics *gc, NSBezierPath *path,
		w_pattern *pattern);
void _w_graphics_set_pattern_phase(w_graphics *gc, w_pattern *pattern);
void _w_graphics_stroke_pattern(w_graphics *gc, NSBezierPath *path,
		w_pattern *pattern);
#define _W_GRAPHICS(x) ((_w_graphics*)x)
#ifdef __cplusplus
}
#endif
#endif /* OSX_GRAPHICS_GC_H_ */
