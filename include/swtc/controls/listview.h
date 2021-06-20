/*
 * Name:        table.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef SWTC_CONTROLS_LISTVIEW_H_
#define SWTC_CONTROLS_LISTVIEW_H_
#include "../widgets/composite.h"
#ifdef __cplusplus
extern "C" {
#endif
/*
 * w_columnitem
 */
typedef struct w_columnitem {
	w_item item;
	void *handle[6];
} w_columnitem;
#define W_COLUMNITEM(x) ((w_columnitem*)x)
struct _w_columnitem_class {
	struct _w_item_class item;
	wresult (*get_alignment)(w_columnitem *column);
	wresult (*get_image)(w_columnitem *column);
	wresult (*get_moveable)(w_columnitem *column);
	wresult (*get_resizable)(w_columnitem *column);
	wresult (*get_tooltip_text)(w_columnitem *column, w_alloc alloc,
			void *user_data, int enc);
	wresult (*get_width)(w_columnitem *column);
	wresult (*pack)(w_columnitem *column);
	wresult (*set_alignment)(w_columnitem *column, int alignment);
	wresult (*set_image)(w_columnitem *column, int image);
	wresult (*set_moveable)(w_columnitem *column, int moveable);
	wresult (*set_resizable)(w_columnitem *column, int resizable);
	wresult (*set_tooltip_text)(w_columnitem *column, const char *text,
			int length, int enc);
	wresult (*set_width)(w_columnitem *column, int width);
};
#define W_COLUMNITEM_CLASS(x) ((struct _w_columnitem_class*)x)
#define W_COLUMNITEM_GET_CLASS(x) ((struct _w_columnitem_class*)W_WIDGETDATA_GET_CLASS(x))
SWT_PUBLIC wresult w_columnitem_get_alignment(w_columnitem *column);
SWT_PUBLIC wresult w_columnitem_get_image(w_columnitem *column);
SWT_PUBLIC wresult w_columnitem_get_moveable(w_columnitem *column);
SWT_PUBLIC wresult w_columnitem_get_resizable(w_columnitem *column);
SWT_PUBLIC wresult w_columnitem_get_tooltip_text(w_columnitem *column,
		w_alloc alloc, void *user_data, int enc);
SWT_PUBLIC wresult w_columnitem_get_width(w_columnitem *column);
SWT_PUBLIC wresult w_columnitem_pack(w_columnitem *column);
SWT_PUBLIC wresult w_columnitem_set_alignment(w_columnitem *column,
		int alignment);
SWT_PUBLIC wresult w_columnitem_set_image(w_columnitem *column, int image);
SWT_PUBLIC wresult w_columnitem_set_moveable(w_columnitem *column,
		int moveable);
SWT_PUBLIC wresult w_columnitem_set_resizable(w_columnitem *column,
		int resizable);
SWT_PUBLIC wresult w_columnitem_set_tooltip_text(w_columnitem *column,
		const char *text, int length, int enc);
SWT_PUBLIC wresult w_columnitem_set_width(w_columnitem *column, int width);
/*
 * w_listitem
 */
#define W_LISTITEM_ATTR_MASK_TEXT 0x1
#define W_LISTITEM_ATTR_MASK_FONT 0x2
#define W_LISTITEM_ATTR_MASK_BACKGROUND 0x4
#define W_LISTITEM_ATTR_MASK_FORGROUND 0x8
#define W_LISTITEM_ATTR_MASK_ALL_NO_TEXT (W_LISTITEM_ATTR_MASK_FONT | W_LISTITEM_ATTR_MASK_BACKGROUND |W_LISTITEM_ATTR_MASK_FORGROUND)
#define W_LISTITEM_ATTR_MASK_ALL (W_LISTITEM_ATTR_MASK_TEXT | W_LISTITEM_ATTR_MASK_ALL_NO_TEXT)
typedef struct w_list_textattr {
	union {
		int mask;
		struct {
			unsigned mask_text :1;
			unsigned mask_font :1;
			unsigned mask_background :1;
			unsigned mask_foreground :1;
		};
	};
	int enc;
	union {
		w_alloc alloc;
		int length;
	};
	union {
		char *text;
		void *user_data;
	};
	w_font *font;
	w_color background;
	w_color foreground;
} w_list_textattr;
typedef struct w_listitem {
	w_item item;
	void *handle[6];
} w_listitem;
#define W_LISTITEM(x) ((w_listitem*)x)
struct _w_listitem_class {
	struct _w_item_class item;
	wresult (*get_attr)(w_listitem *item, int index, int mask,
			w_list_textattr *attr);
	wresult (*get_bounds)(w_listitem *item, w_rect *bounds);
	wresult (*get_bounds_index)(w_listitem *item, int index, w_rect *bounds);
	wresult (*get_checked)(w_listitem *item);
	wresult (*get_grayed)(w_listitem *item);
	wresult (*get_image)(w_listitem *item);
	wresult (*get_text)(w_listitem *item, int index, w_alloc alloc,
			void *user_data, int enc);
	wresult (*set_attr)(w_listitem *item, int index, int mask,
			w_list_textattr *attr);
	wresult (*set_checked)(w_listitem *item, int checked);
	wresult (*set_grayed)(w_listitem *item, int grayed);
	wresult (*set_image)(w_listitem *item, int image);
	wresult (*set_text)(w_listitem *item, int index, const char *text,
			int length, int enc);
};
#define W_LISTITEM_CLASS(x) ((struct _w_listitem_class*)x)
#define W_LISTITEM_GET_CLASS(x) ((struct _w_listitem_class*)W_WIDGETDATA_GET_CLASS(x))
SWT_PUBLIC wresult w_listitem_get_attr(w_listitem *item, int index, int mask,
		w_list_textattr *attr);
SWT_PUBLIC wresult w_listitem_get_bounds(w_listitem *item, w_rect *bounds);
SWT_PUBLIC wresult w_listitem_get_bounds_index(w_listitem *item, int index,
		w_rect *bounds);
SWT_PUBLIC wresult w_listitem_get_checked(w_listitem *item);
SWT_PUBLIC wresult w_listitem_get_grayed(w_listitem *item);
SWT_PUBLIC wresult w_listitem_get_image(w_listitem *item);
SWT_PUBLIC wresult w_listitem_get_text(w_listitem *item, int index,
		w_alloc alloc, void *user_data, int enc);
SWT_PUBLIC wresult w_listitem_set_attr(w_listitem *item, int index, int mask,
		w_list_textattr *attr);
SWT_PUBLIC wresult w_listitem_set_checked(w_listitem *item, int checked);
SWT_PUBLIC wresult w_listitem_set_grayed(w_listitem *item, int grayed);
SWT_PUBLIC wresult w_listitem_set_image(w_listitem *item, int image);
SWT_PUBLIC wresult w_listitem_set_text(w_listitem *item, int index,
		const char *text, int length, int enc);
/*
 * w_listviewbase
 */
typedef struct w_event_list {
	w_event event;
	union {
		int detail;
		struct {
			unsigned doit :1;
			unsigned selected :1;
			unsigned focused :1;
			unsigned background :1;
			unsigned forground :1;
			unsigned hot :1;
		};
	};
	w_rect *rect;
	w_columnitem *column;
	w_listitem *item;
	w_graphics *gc;
	w_list_textattr *textattr;
} w_event_list;

typedef struct w_listviewbase {
	w_composite composite;
	void *handle[0x10];
} w_listviewbase;
#define W_LISTVIEWBASE(x) ((w_listviewbase*)x)
struct _w_listviewbase_class {
	struct _w_composite_class composite;
	struct _w_item_class *class_item;
	struct _w_columnitem_class *class_column;
	wresult (*clear_all)(w_listviewbase *list);
	wresult (*deselect_all)(w_listviewbase *list);
	wresult (*for_all_column)(w_listviewbase *list, w_widget_callback callback,
			void *user_data);
	wresult (*for_all_item)(w_listviewbase *list, w_widget_callback callback,
			void *user_data);
	wresult (*get_column)(w_listviewbase *list, int index,
			w_columnitem *column);
	wresult (*get_column_count)(w_listviewbase *list);
	wresult (*get_columns)(w_listviewbase *list, w_iterator *columns);
	wresult (*get_gridline_width)(w_listviewbase *list);
	wresult (*get_header_height)(w_listviewbase *list);
	wresult (*get_header_imagelist)(w_listviewbase *list,
			w_imagelist **imagelist);
	wresult (*get_header_visible)(w_listviewbase *list);
	wresult (*get_imagelist)(w_listviewbase *list, w_imagelist **imagelist);
	wresult (*get_item_height)(w_listviewbase *list);
	wresult (*get_lines_visible)(w_listviewbase *list);
	wresult (*get_selection)(w_listviewbase *list, w_iterator *selection);
	wresult (*get_selection_count)(w_listviewbase *list);
	wresult (*get_sort_column)(w_listviewbase *list, w_columnitem *column);
	wresult (*get_sort_direction)(w_listviewbase *list);
	wresult (*insert_column)(w_listviewbase *list, w_columnitem *column,
			int index);
	wresult (*remove_all)(w_listviewbase *list);
	wresult (*select_all)(w_listviewbase *list);
	wresult (*set_header_imagelist)(w_listviewbase *list,
			w_imagelist *imagelist);
	wresult (*set_header_visible)(w_listviewbase *list, int show);
	wresult (*set_imagelist)(w_listviewbase *list, w_imagelist *imagelist);
	wresult (*set_item_height)(w_listviewbase *list, int itemHeight);
	wresult (*set_lines_visible)(w_listviewbase *list, int show);
	wresult (*set_sort_column)(w_listviewbase *list, w_columnitem *column);
	wresult (*set_sort_direction)(w_listviewbase *list, int direction);
	wresult (*show_column)(w_listviewbase *list, w_columnitem *column);
	wresult (*show_selection)(w_listviewbase *list);
	wresult (*sort)(w_listviewbase *list);
};
#define W_LISTVIEWBASE_CLASS(x) ((struct _w_listviewbase_class*)x)
#define W_LISTVIEWBASE_GET_CLASS(x) ((struct _w_listviewbase_class*)W_WIDGET_GET_CLASS(x))
SWT_PUBLIC wresult w_listviewbase_clear_all(w_listviewbase *list);
SWT_PUBLIC wresult w_listviewbase_deselect_all(w_listviewbase *list);
SWT_PUBLIC wresult w_listviewbase_for_all_column(w_listviewbase *list,
		w_widget_callback callback, void *user_data);
SWT_PUBLIC wresult w_listviewbase_for_all_item(w_listviewbase *list,
		w_widget_callback callback, void *user_data);
SWT_PUBLIC wresult w_listviewbase_get_column(w_listviewbase *list, int index,
		w_columnitem *column);
SWT_PUBLIC wresult w_listviewbase_get_column_count(w_listviewbase *list);
SWT_PUBLIC wresult w_listviewbase_get_columns(w_listviewbase *list,
		w_iterator *columns);
SWT_PUBLIC wresult w_listviewbase_get_gridline_width(w_listviewbase *list);
SWT_PUBLIC wresult w_listviewbase_get_header_height(w_listviewbase *list);
SWT_PUBLIC wresult w_listviewbase_get_header_imagelist(w_listviewbase *list,
		w_imagelist **imagelist);
SWT_PUBLIC wresult w_listviewbase_get_header_visible(w_listviewbase *list);
SWT_PUBLIC wresult w_listviewbase_get_imagelist(w_listviewbase *list,
		w_imagelist **imagelist);
SWT_PUBLIC wresult w_listviewbase_get_item_height(w_listviewbase *list);
SWT_PUBLIC wresult w_listviewbase_get_lines_visible(w_listviewbase *list);
SWT_PUBLIC wresult w_listviewbase_get_selection(w_listviewbase *list,
		w_iterator *selection);
SWT_PUBLIC wresult w_listviewbase_get_selection_count(w_listviewbase *list);
SWT_PUBLIC wresult w_listviewbase_get_sort_column(w_listviewbase *list,
		w_columnitem *column);
SWT_PUBLIC wresult w_listviewbase_get_sort_direction(w_listviewbase *list);
SWT_PUBLIC wresult w_listviewbase_insert_column(w_listviewbase *list,
		w_columnitem *column, int index);
SWT_PUBLIC wresult w_listviewbase_remove_all(w_listviewbase *list);
SWT_PUBLIC wresult w_listviewbase_select_all(w_listviewbase *list);
SWT_PUBLIC wresult w_listviewbase_set_header_imagelist(w_listviewbase *list,
		w_imagelist *imagelist);
SWT_PUBLIC wresult w_listviewbase_set_header_visible(w_listviewbase *list,
		int show);
SWT_PUBLIC wresult w_listviewbase_set_imagelist(w_listviewbase *list,
		w_imagelist *imagelist);
SWT_PUBLIC wresult w_listviewbase_set_item_height(w_listviewbase *list,
		int itemHeight);
SWT_PUBLIC wresult w_listviewbase_set_lines_visible(w_listviewbase *list,
		int show);
SWT_PUBLIC wresult w_listviewbase_set_sort_column(w_listviewbase *list,
		w_columnitem *column);
SWT_PUBLIC wresult w_listviewbase_set_sort_direction(w_listviewbase *list,
		int direction);
SWT_PUBLIC wresult w_listviewbase_show_column(w_listviewbase *list,
		w_columnitem *column);
SWT_PUBLIC wresult w_listviewbase_show_selection(w_listviewbase *list);
SWT_PUBLIC wresult w_listviewbase_sort(w_listviewbase *list);
/*
 * w_listview
 */
typedef struct w_listview {
	w_listviewbase base;
} w_listview;
#define W_LISTVIEW(x) ((w_listview*)x)
struct _w_listview_class {
	struct _w_listviewbase_class base;
	wresult (*clear_index)(w_listview *list, int index);
	wresult (*clear_indices)(w_listview *list, int *indices, int length);
	wresult (*clear_item)(w_listview *list, w_listitem *item);
	wresult (*clear_range)(w_listview *list, int start, int end);
	wresult (*deselect_index)(w_listview *list, int index);
	wresult (*deselect_indices)(w_listview *table, int *indices, int length);
	wresult (*deselect_item)(w_listview *list, w_listitem *item);
	wresult (*deselect_range)(w_listview *list, int start, int end);
	wresult (*get_item)(w_listview *list, int index, w_listitem *item);
	wresult (*get_item_count)(w_listview *list);
	wresult (*get_item_from_point)(w_listview *list, w_point *point,
			w_listitem *item);
	wresult (*get_items)(w_listview *list, w_iterator *items);
	wresult (*get_selection_index)(w_listview *list);
	wresult (*get_top_index)(w_listview *list);
	wresult (*get_top_item)(w_listview *list, w_listitem *topitem);
	wresult (*is_selected)(w_listview *list, int index);
	wresult (*insert_item)(w_listview *list, w_listitem *item, int index);
	wresult (*remove_index)(w_listview *list, int index);
	wresult (*remove_indices)(w_listview *list, int *indices, int length);
	wresult (*remove_item)(w_listview *list, w_listitem *item);
	wresult (*remove_range)(w_listview *list, int start, int end);
	wresult (*select_index)(w_listview *list, int index);
	wresult (*select_indices)(w_listview *list, int *indices, int length);
	wresult (*select_item)(w_listview *list, w_listitem *item);
	wresult (*select_range)(w_listview *list, int start, int end);
	wresult (*set_insert_mark)(w_listview *list, w_listitem *item, int before);
	wresult (*set_selection_index)(w_listview *list, int index);
	wresult (*set_selection_indices)(w_listview *list, int *indices,
			int length);
	wresult (*set_selection_item)(w_listview *list, w_listitem *item);
	wresult (*set_selection_range)(w_listview *list, int start, int end);
	wresult (*set_top_index)(w_listview *list, int index);
	wresult (*set_top_item)(w_listview *list, w_listitem *item);
	wresult (*show_item)(w_listview *list, w_listitem *item);
};
#define W_LISTVIEW_CLASS(x) ((struct _w_listview_class*)x)
#define W_LISTVIEW_GET_CLASS(x) ((struct _w_listview_class*)W_WIDGET_GET_CLASS(x))
SWT_PUBLIC wresult w_listview_clear_index(w_listview *list, int index);
SWT_PUBLIC wresult w_listview_clear_indices(w_listview *list, int *indices,
		int length);
SWT_PUBLIC wresult w_listview_clear_item(w_listview *list, w_listitem *item);
SWT_PUBLIC wresult w_listview_clear_range(w_listview *list, int start, int end);
SWT_PUBLIC wresult w_listview_deselect_index(w_listview *list, int index);
SWT_PUBLIC wresult w_listview_deselect_indices(w_listview *table, int *indices,
		int length);
SWT_PUBLIC wresult w_listview_deselect_item(w_listview *list, w_listitem *item);
SWT_PUBLIC wresult w_listview_deselect_range(w_listview *list, int start,
		int end);
SWT_PUBLIC wresult w_listview_get_item(w_listview *list, int index,
		w_listitem *item);
SWT_PUBLIC wresult w_listview_get_item_count(w_listview *list);
SWT_PUBLIC wresult w_listview_get_item_from_point(w_listview *list,
		w_point *point, w_listitem *item);
SWT_PUBLIC wresult w_listview_get_items(w_listview *list, w_iterator *items);
SWT_PUBLIC wresult w_listview_get_selection_index(w_listview *list);
SWT_PUBLIC wresult w_listview_get_top_index(w_listview *list);
SWT_PUBLIC wresult w_listview_get_top_item(w_listview *list,
		w_listitem *topitem);
SWT_PUBLIC wresult w_listview_is_selected(w_listview *list, int index);
SWT_PUBLIC wresult w_listview_insert_item(w_listview *list, w_listitem *item,
		int index);
SWT_PUBLIC wresult w_listview_remove_index(w_listview *list, int index);
SWT_PUBLIC wresult w_listview_remove_indices(w_listview *list, int *indices,
		int length);
SWT_PUBLIC wresult w_listview_remove_item(w_listview *list, w_listitem *item);
SWT_PUBLIC wresult w_listview_remove_range(w_listview *list, int start,
		int end);
SWT_PUBLIC wresult w_listview_select_index(w_listview *list, int index);
SWT_PUBLIC wresult w_listview_select_indices(w_listview *list, int *indices,
		int length);
SWT_PUBLIC wresult w_listview_select_item(w_listview *list, w_listitem *item);
SWT_PUBLIC wresult w_listview_select_range(w_listview *list, int start,
		int end);
SWT_PUBLIC wresult w_listview_set_insert_mark(w_listview *list,
		w_listitem *item, int before);
SWT_PUBLIC wresult w_listview_set_selection_index(w_listview *list, int index);
SWT_PUBLIC wresult w_listview_set_selection_indices(w_listview *list,
		int *indices, int length);
SWT_PUBLIC wresult w_listview_set_selection_item(w_listview *list,
		w_listitem *item);
SWT_PUBLIC wresult w_listview_set_selection_range(w_listview *list, int start,
		int end);
SWT_PUBLIC wresult w_listview_set_top_index(w_listview *list, int index);
SWT_PUBLIC wresult w_listview_set_top_item(w_listview *list, w_listitem *item);
SWT_PUBLIC wresult w_listview_show_item(w_listview *list, w_listitem *item);
#ifdef __cplusplus
}
#endif
#endif /* SWTC_CONTROLS_LISTVIEW_H_ */
