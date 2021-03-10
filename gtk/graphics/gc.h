/*
 * Name:        gc.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef GTK_GRAPHICS_GC_H_
#define GTK_GRAPHICS_GC_H_
#include "../runtime/core.h"
#ifdef __cplusplus
extern "C" {
#endif
/*
 * font
 */
typedef struct _w_font {
	PangoFontDescription *handle;
} _w_font;
#define _W_FONT(x) ((struct _w_font*)x)
typedef struct _w_fontdata {
	PangoStyle style :2;
	PangoStretch stretch :4;
	PangoWeight weight :10;
	wushort size;
	char family[0];
} _w_fontdata;
#define _W_FONTMETRICS(x) ((struct _w_fontmetrics*)x)
typedef struct _w_fontmetrics {
	int ascentInPoints;
	int descentInPoints;
	int averageCharWidthInPoints;
} _w_fontmetrics;
#define _W_FONTDATA(x) ((struct _w_fontdata*)x)
/*
 * image
 */
typedef struct _w_image {
	GdkPixbuf *pixbuf;
	int flags;
} _w_image;
#define _W_IMAGE(x) ((_w_image*)x)
cairo_surface_t* _w_image_create_surface(GdkPixbuf *pixbuf);
cairo_pattern_t* _w_image_create_pattern(GdkPixbuf *pixbuf,
		cairo_surface_t **surface);
typedef struct _w_surface {
	cairo_surface_t *handle;
} _w_surface;
#define _W_SURFACE(x) ((_w_surface*)x)
typedef struct _w_imagelist_images {
	int alloc;
	int count;
	int style;
	int width;
	int height;
	void *reserved;
	GdkPixbuf *images[0];
} _w_imagelist_images;
typedef struct _w_imagelist {
	_w_imagelist_images *images;
} _w_imagelist;
#define _W_IMAGELIST(x) ((_w_imagelist*)x)
GdkPixbuf* w_imagelist_get_pixbuf(w_imagelist *imagelist, int index);
/*
 * cursor
 */
typedef struct _w_cursor {
	GdkCursor *handle;
} _w_cursor;
#define _W_CURSOR(x) ((_w_cursor*)x)
/*
 * pattern
 */
typedef struct _w_pattern {
	cairo_pattern_t *handle;
} _w_pattern;
#define _W_PATTERN(x) ((_w_pattern*)x)
/*
 * transform
 */
typedef struct _w_transform {
	cairo_matrix_t handle;
	unsigned is_created :1;
} _w_transform;
#define _W_TRANSFORM(x) ((_w_transform*)x)
/*
 * path
 */
typedef struct _w_path {
	cairo_t *handle;
	unsigned moved :1;
	unsigned closed :1;
} _w_path;
#define _W_PATH(x) ((_w_path*)x)
/*
 * region
 */
typedef struct _w_region {
	cairo_region_t *handle;
} _w_region;
#define _W_REGION(x) ((_w_region*)x)
void _gdk_region_get_rectangles(cairo_region_t *region,
		GdkRectangle **rectangles, int *n_rectangles);
cairo_region_t* _gdk_region_polygon(w_point *points, size_t count,
		int fill_rule);
#define GDK_EVEN_ODD_RULE 0
/*
 * graphics
 */
#define GRAPHICS_STATE_CAIRO  (1 << 0)
#define GRAPHICS_STATE_FOREGROUND  (1 << 1)
#define GRAPHICS_STATE_BACKGROUND  (1 << 2)
#define GRAPHICS_STATE_FONT  (1 << 3)
#define GRAPHICS_STATE_LINE_STYLE  (1 << 4)
#define GRAPHICS_STATE_LINE_CAP  (1 << 5)
#define GRAPHICS_STATE_LINE_JOIN  (1 << 6)
#define GRAPHICS_STATE_LINE_WIDTH  (1 << 7)
#define GRAPHICS_STATE_LINE_MITERLIMIT  (1 << 8)
#define GRAPHICS_STATE_BACKGROUND_BG  (1 << 9)
#define GRAPHICS_STATE_DRAW_OFFSET  (1 << 10)
#define GRAPHICS_STATE_DRAW  (GRAPHICS_STATE_FOREGROUND | GRAPHICS_STATE_LINE_WIDTH | \
GRAPHICS_STATE_LINE_STYLE | GRAPHICS_STATE_LINE_CAP | GRAPHICS_STATE_LINE_JOIN\
		| GRAPHICS_STATE_LINE_MITERLIMIT | GRAPHICS_STATE_DRAW_OFFSET)
#define GRAPHICS_STATE_FILL  GRAPHICS_STATE_BACKGROUND
#define GRAPHICS_STATE_MIRRORED  (1 << 11)
#define GRAPHICS_STATE_DISPOSE_CAIRO  (1 << 12)
#define GRAPHICS_STATE_HAS_IDENTITY  (1 << 12)
typedef struct _w_graphics {
	union {
#if USE_CAIRO
		struct {
			cairo_t *cairo;
			PangoContext *context;
			PangoLayout *layout;
			double cairoXoffset, cairoYoffset;
		};
#else
#endif
	};
	wushort state;
	wuchar alpha;
	unsigned interpolation :2;
	unsigned xorMode :1;
	unsigned isclippingTransform;
	int stringWidth;
	int stringHeight;
	int width;
	int height;
	GdkRectangle clipping;
	w_color foreground;
	w_color background;
	w_pattern *foregroundPattern;
	w_pattern *backgroundPattern;
	w_font *font;
	w_line_attributes line;
	cairo_region_t *clipRgn;
	cairo_region_t *damageRgn;
	cairo_matrix_t identity;
	cairo_matrix_t clippingTransform;
} _w_graphics;
#define _W_GRAPHICS(x) ((_w_graphics*)x)
#if USE_CAIRO
void _w_graphics_init(w_graphics *gc, cairo_t *drawable);
#else
void _w_graphics_init(w_graphics* gc,GdkWindow* drawable);
#endif
wresult _w_graphics_check(w_graphics *gc, int mask);
#ifdef __cplusplus
}
#endif
#endif /* GTK_GRAPHICS_GC_H_ */
