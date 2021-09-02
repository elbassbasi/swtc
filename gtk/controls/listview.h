/*
 * Name:        listview.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef GTK_CONTROLS_LISTVIEW_H_
#define GTK_CONTROLS_LISTVIEW_H_
#include "../widgets/shell.h"
/*
 * state
 */
#define STATE_TABLE_END (STATE_COMPOSITE_END + 1)
enum {
	COLUMN_TEXT = 0, //
	COLUMN_USER_DATA, //
	COLUMN_INFO, //
	COLUMN_IMAGE, //
	COLUMN_ARRAY,
	COLUMN_COUNT
};
extern GType gtk_store_types[COLUMN_COUNT];
#define COLUMN_INFO_CHECK (1 << 1)
#define COLUMN_INFO_GRAYED (1 << 2)
#define COLUMN_INFO_HAS_CHILDREN (1 << 3)
#define COLUMN_INFO_IS_ARRAY (1 << 4)

typedef struct _w_item_list {
	w_color background;
	w_color foreground;
	int image;
	w_font *font;
	char *text;
} _w_item_list;
/*
 * treeitem
 */
typedef struct _w_columnitem {
	_w_item item;
	GtkTreeViewColumn *column;
} _w_columnitem;
#define _W_COLUMNITEM(x) ((_w_columnitem*)x)
/*
 * listview
 */
typedef struct _w_listviewbase {
	_w_composite composite;
	GtkCellRenderer *pixbufrenderer;
	GtkCellRenderer *checkrenderer;
	w_imagelist *imagelist;
	w_imagelist *headerimagelist;
	int drawState;
	int drawFlags;
	unsigned rowActivated :1;
	unsigned ignoreSize :1;
} _w_listviewbase;
#define _W_LISTVIEWBASE(x) ((_w_listviewbase*)x)
#define _W_LISTVIEWBASE_GET_ITEM_CLASS(list) (W_WIDGETDATA_CLASS(W_LISTVIEWBASE_GET_CLASS(list)->class_item))
#define _W_LISTVIEWBASE_GET_COLUMN_CLASS(list) (W_WIDGETDATA_CLASS(W_LISTVIEWBASE_GET_CLASS(list)->class_column))
wresult _w_listviewbase_get_header_imagelist(w_listviewbase *list,
		w_imagelist **imagelist);
typedef struct _w_listview {
	_w_listviewbase base;
} _w_listview;
#define _W_LISTVIEW(x) ((_w_listview*)x)
/*
 * private
 */
enum {
	_W_LISTVIEW_SIGNAL_TOGGLED,//
	_W_LISTVIEW_SIGNAL_CHANGED, //
	_W_LISTVIEW_SIGNAL_ROW_HAS_CHILD_TOGGLED, //
	_W_LISTVIEW_SIGNAL_ROW_INSERTED, //
	_W_LISTVIEW_SIGNAL_ROW_DELETED,
	_W_LISTVIEW_SIGNAL_ROW_ACTIVATED, //
	_W_LISTVIEW_SIGNAL_START_INTERACTIVE_SEARCH, //
	_W_LISTVIEW_SIGNAL_TEST_EXPAND_ROW, //
	_W_LISTVIEW_SIGNAL_TEST_COLLAPSE_ROW, //
	_W_LISTVIEW_SIGNAL_EXPAND_COLLAPSE_CURSOR_ROW,
	_W_LISTVIEW_LAST
};
typedef struct _w_listviewbase_priv {
	_w_composite_priv composite;
	void (*renderer_render)(w_widget *widget, _w_control_priv *priv,
			GtkCellRenderer *cell, cairo_t *cr, GtkWidget *gtkwidget,
			const GdkRectangle *background_area, const GdkRectangle *cell_area,
			GtkCellRendererState flags);
#if GTK3
	void (*renderer_get_preferred_width)(w_widget *widget,
			_w_control_priv *priv, GtkCellRenderer *cell, GtkWidget *gtkwidget,
			gint *minimum_size, gint *natural_size);
#endif
	_gtk_signal signals[_W_LISTVIEW_LAST];
} _w_listviewbase_priv;
#define _W_LISTVIEWBASE_PRIV(x) ((_w_listviewbase_priv*)x)
#define _W_LISTVIEWBASE_GET_PRIV(x) ((_w_listviewbase_priv*)_W_WIDGET_GET_PRIV(x))
void _w_listviewbase_class_init(w_toolkit *toolkit, wushort classId,
		struct _w_listviewbase_class *clazz);
typedef struct _w_listview_priv {
	_w_listviewbase_priv tablebase;
} _w_listview_priv;
#define _W_LISTVIEW_PRIV(x) ((_w_listview_priv*)x)
void _w_listitem_class_init(struct _w_listitem_class *listitem);
void _w_listview_class_init(w_toolkit *toolkit, wushort classId,
		struct _w_listview_class *clazz);
/*
 * cell renderer
 */
typedef struct _w_cell_renderer {
	union {
		GtkCellRenderer renderer;
		GtkCellRendererText text_renderer;
		GtkCellRendererToggle toggle_renderer;
		GtkCellRendererPixbuf pixbuf_renderer;
	};
	w_widget *widget;
	w_columnitem column;
	union {
		w_listitem listitem;
		w_treeitem treeitem;
	};
} _w_cell_renderer;
typedef struct _w_cell_renderer_class {
	union {
		GtkCellRendererClass cellClass;
		GtkCellRendererTextClass textClass;
		GtkCellRendererToggleClass toggleClass;
		GtkCellRendererPixbufClass pixbufClass;
	};
	GParamSpec *param;
#if GTK3
	void (*default_get_preferred_width)(GtkCellRenderer *cell,
			GtkWidget *widget, gint *minimum_size, gint *natural_size);
#else
	void (*default_get_size)(GtkCellRenderer *cell, GtkWidget *widget,
			const GdkRectangle *cell_area, gint *x_offset, gint *y_offset,
			gint *width, gint *height);
#endif
	void (*default_render)(GtkCellRenderer *cell, cairo_t *cr,
			GtkWidget *widget, const GdkRectangle *background_area,
			const GdkRectangle *cell_area, GtkCellRendererState flags);
} _w_cell_renderer_class;
/*
 * function
 */
GtkCellRenderer* _w_text_renderer_new(w_widget *widget);
GtkCellRenderer* _w_pixbuf_renderer_new(w_widget *widget);
GtkCellRenderer* _w_toggle_renderer_new(w_widget *widget);
void _w_listviewbase_renderer_render(w_widget *widget, _w_control_priv *priv,
		GtkCellRenderer *cell, cairo_t *cr, GtkWidget *gtkwidget,
		const GdkRectangle *background_area, const GdkRectangle *cell_area,
		GtkCellRendererState flags);
#if GTK3
void _w_listviewbase_renderer_get_preferred_width(w_widget *widget,
		_w_control_priv *priv, GtkCellRenderer *cell, GtkWidget *gtkwidget,
		gint *minimum_size, gint *natural_size);
#else
void _w_listviewbase_rendererGetSizeProc(w_widget* widget,_w_control_priv *priv, GtkCellRenderer *cell,
		GtkWidget *widget, const GdkRectangle *cell_area, gint *x_offset,
		gint *y_offset, gint *width, gint *height);
#endif
wuint64 _w_listviewbase_check_style(w_widget *widget, wuint64 style);
wresult _w_listviewbase_create_handle(w_widget *widget, _w_control_priv *priv);
wresult _w_listviewbase_insert_column_0(w_listviewbase *list, int index,
		w_columnitem *column, _w_control_priv *priv);
wresult _w_listviewbase_get_header_height(w_listviewbase *list);

#endif /* SRC_SWT_GTK_CONTROLS_TABLE_H_ */
