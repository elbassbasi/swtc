/*
 * Name:        listview.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef WIN32_CONTROLS_LISTVIEW_H_
#define WIN32_CONTROLS_LISTVIEW_H_
#include "../widgets/shell.h"
/*
 * state
 */
#define STATE_TABLE_END (STATE_COMPOSITE_END + 1)
#define _W_TREECOLUMN_NOT_MOVEABLE 0x1
#define _W_TREECOLUMN_NOT_RESIZABLE 0x2
typedef struct _w_column_list {
	int flags;
	int id;
	char *tooltip;
} _w_column_list;
typedef struct _w_item_list {
	w_color background;
	w_color foreground;
	int image;
	w_font *font;
	char *text;
} _w_item_list;

typedef union LISTITEM {
	TVITEMW tvItem;
	LVITEMW lvItem;
} LISTITEM;
#define LISTITEM_GET_PARAM(item,clazz_id) \
	(class_id == _W_CLASS_TREEVIEW ? item.tvItem.lParam : item.lvItem.lParam)
#define LISTITEM_SET_PARAM(item,clazz_id,lParam) do {\
	if(class_id == _W_CLASS_TREEVIEW) item.tvItem.lParam=lParam;\
	else item.lvItem.lParam=lParam;} while (0)

/*
 * listitem
 */
#define ITEM_STATE_IMAGEMASK 0x3000
#define ITEM_STATE_PARAMMASK 0x4000
#define ITEM_STATE_UNCHECKEDNORMAL 0x0000
#define ITEM_STATE_CHECKEDNORMAL 0x1000
#define ITEM_STATE_MIXEDNORMAL 0x3000
#define ITEM_STATE_SET_CHECK(x) (x)
#define ITEM_STATE_SELECTED(x)  x ^= ITEM_STATE_CHECKEDNORMAL;
typedef struct _w_listitem {
	_w_item item;
} _w_listitem;
#define _W_LISTITEM(x) ((_w_listitem*)x)
/*
 * listview
 */
typedef struct _w_listviewbase {
	_w_composite composite;
	unsigned explorerTheme :1;
	unsigned createdAsRTL :1;
	unsigned sortDirection :2;
	int sortColumn;
	w_imagelist *imagelist;
	w_imagelist *headerimagelist;
	w_array *columns;
	HWND headerToolTipHandle;
} _w_listviewbase;
#define _W_LISTVIEWBASE(x) ((_w_listviewbase*)x)
#define _W_LISTVIEWBASE_GET_ITEM_CLASS(list) (W_WIDGETDATA_CLASS(W_LISTVIEWBASE_GET_CLASS(list)->class_item))
#define _W_LISTVIEWBASE_GET_COLUMN_CLASS(list) (W_WIDGETDATA_CLASS(W_LISTVIEWBASE_GET_CLASS(list)->class_column))
typedef struct _w_listview {
	_w_listviewbase base;
} _w_listview;
#define _W_LISTVIEW(x) ((_w_listview*)x)
/*
 * private
 */
typedef struct _w_listviewbase_priv {
	_w_composite_priv composite;
} _w_listviewbase_priv;
#define _W_LISTVIEWBASE_PRIV(x) ((_w_listviewbase_priv*)x)
#define _W_LISTVIEWBASE_GET_PRIV(x) ((_w_listviewbase_priv*)_w_widget_get_priv(W_WIDGET(x)))
void _w_listviewbase_class_init(w_toolkit *toolkit, wushort classId,
		struct _w_listviewbase_class *clazz);
typedef struct _w_listview_priv {
	_w_listviewbase_priv base;
} _w_listview_priv;
#define _W_LISTVIEW_PRIV(x) ((_w_listview_priv*)x)

wresult _w_listitem_copy(w_widgetdata *from, w_widgetdata *to);
wresult _w_listitem_equals(w_widgetdata *obj1, w_widgetdata *obj2);
wresult _w_listitem_get_data(w_item *item, void **data);
wresult _w_listitem_get_text(w_item *item, w_alloc alloc, void *user_data,
		int enc);
wresult _w_listitem_get_text_0(w_listitem *item, int index, w_alloc alloc,
		void *user_data, int enc);
wresult _w_listitem_set_data(w_item *item, void *data);
wresult _w_listitem_set_text(w_item *item, const char *text, int length,
		int enc);
wresult _w_listitem_set_text_0(w_listitem *item, int index, const char *text,
		int length, int enc);
wresult _w_listitem_get_attr(w_listitem *item, int index, int mask,
		w_item_attr *attr);
wresult _w_listitem_get_attr_0(w_listitem *item, int index, int mask,
		w_item_attr *attr, int flags);
wresult _w_listitem_get_bounds(w_listitem *item, w_rect *bounds);
wresult _w_listitem_get_bounds_index(w_listitem *item, int index,
		w_rect *bounds);
wresult _w_listitem_get_checked(w_listitem *item);
wresult _w_listitem_get_grayed(w_listitem *item);
wresult _w_listitem_get_image(w_listitem *item);
wresult _w_listitem_set_attr(w_listitem *item, int index, int mask,
		w_item_attr *attr);
wresult _w_listitem_set_checked(w_listitem *item, int checked);
wresult _w_listitem_set_grayed(w_listitem *item, int grayed);
wresult _w_listitem_set_image(w_listitem *item, int image);
wresult _w_listviewbase_insert_column(w_listviewbase *list,
		w_columnitem *column, int index);
void _w_listview_class_init(w_toolkit *toolkit, wushort classId,
		struct _w_listview_class *clazz);
#endif /* WIN32_CONTROLS_TABLE_H_ */
