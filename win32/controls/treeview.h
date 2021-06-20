/*
 * Name:        treeview.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef WIN32_CONTROLS_TREEVIEW_H_
#define WIN32_CONTROLS_TREEVIEW_H_
#include "listview.h"
#if NTDDI_VERSION < 0x06000000
#define TVS_EX_MULTISELECT 0x2
#define TVS_EX_DOUBLEBUFFER 0x4
#define TVS_EX_NOINDENTSTATE 0x8
#define TVS_EX_RICHTOOLTIP 0x10
#define TVS_EX_AUTOHSCROLL 0x20
#define TVS_EX_FADEINOUTEXPANDOS 0x40
#define TVS_EX_PARTIALCHECKBOXES 0x80
#define TVS_EX_EXCLUSIONCHECKBOXES 0x100
#define TVS_EX_DIMMEDCHECKBOXES 0x200
#define TVS_EX_DRAWIMAGEASYNC 0x400
#endif

/*
 * treeitem
 */
typedef struct _w_treeitem {
	_w_item item;
	HTREEITEM htreeitem;
} _w_treeitem;
#define _W_TREEITEM(x) ((_w_treeitem*)x)
/*
 * treeview
 */
typedef struct _w_treeview {
	_w_listviewbase base;
	HWND hwndParent;
	HWND hwndHeader;
	unsigned explorerTheme :1;
	unsigned customDraw :1;
	unsigned printClient :1;
	unsigned linesVisible :1;
	unsigned ignoreDrawForeground :1;
	unsigned ignoreDrawBackground :1;
	unsigned ignoreDrawSelection :1;
	unsigned ignoreDrawFocus :1;
	unsigned ignoreDrawHot :1;
	unsigned ignoreFullSelection :1;
	unsigned ignoreCustomDraw :1;
	unsigned enableEraseItem :1;
	unsigned enableMeasureItem :1;
	unsigned enableSelectionForeground :1;
	unsigned lockSelection :1;
	unsigned oldSelected :1;
	unsigned newSelected :1;
	unsigned ignoreSelect :1;
	unsigned ignoreDeselect :1;
	unsigned ignoreExpand :1;
	unsigned ignoreColumnMove :1;
	unsigned ignoreResize :1;
	unsigned ignoreGetDisp :1;
	unsigned hooksEraseItem :1;
	unsigned painted :1;
	int scrollWidth;
	w_color selectionForeground;
	HTREEITEM hSelect;
	HTREEITEM hAnchor;
} _w_treeview;
#define _W_TREEVIEW(x) ((_w_treeview*)x)
/*
 * private
 */
typedef struct _w_treeview_priv {
	_w_listviewbase_priv base;
	WNDPROC HeaderProc;
} _w_treeview_priv;
#define _W_TREEVIEW_PRIV(x) ((_w_treeview_priv*)x)

#define TREEVIEW_INCREMENT 5
#define TREEVIEW_INSET 3
#define TREEVIEW_GRID_WIDTH 1
#define TREEVIEW_EXPLORER_EXTRA 2
#define TREEVIEW_INSET  3
#define TREEVIEW_HEADER_MARGIN 12
#define TREEVIEW_HEADER_EXTRA 3
#define TREEVIEW_SORT_WIDTH 10

#define TREEVIEW_BOUNDS_GET_TEXT (1 << 0)
#define TREEVIEW_BOUNDS_GET_IMAGE (1 << 1)
#define TREEVIEW_BOUNDS_FULLTEXT (1 << 2)
#define TREEVIEW_BOUNDS_FULLIMAGE (1 << 3)
#define TREEVIEW_BOUNDS_CLIP (1 << 4)
/*
 * functions
 */
void _w_treeitem_get_bounds_0(w_treeitem *item, RECT *rect, HDC hDC, int index,
		int flags);
wresult _w_treeview_call_window_proc(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _w_treeview_create_parent(w_treeview *tree, _w_control_priv *priv);
wresult _w_treeview_insert_column(w_listviewbase *tree, w_columnitem *column,
		int index);
HTREEITEM _w_treeview_get_bottom_item(HWND handle);
int _w_treeview_get_header_width(w_treeview *tree);
wresult _w_treeview_post_event(w_widget *widget, w_event *ee);
void _w_treeview_set_scroll_width_0(w_treeview *tree, int width);
void _w_treeview_set_scroll_width(w_treeview *tree);
void _w_treeview_update_scrollbar(w_treeview *tree);
void _w_treeview_set_checkbox_imagelist (w_treeview *tree,_w_control_priv* priv);
void _w_treeview_class_init(struct _w_treeview_class *clazz);
/*
 * messages
 */
wresult _TREEVIEW_WM_CHAR(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _TREEVIEW_WM_ERASEBKGND(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _TREEVIEW_WM_GETOBJECT(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _TREEVIEW_WM_HSCROLL(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _TREEVIEW_WM_KEYDOWN(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _TREEVIEW_WM_KILLFOCUS(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _TREEVIEW_WM_LBUTTONDBLCLK(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _TREEVIEW_WM_LBUTTONDOWN(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _TREEVIEW_WM_MOUSEMOVE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _TREEVIEW_WM_MOUSEWHEEL(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _TREEVIEW_WM_MOVE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _TREEVIEW_WM_RBUTTONDOWN(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _TREEVIEW_WM_PAINT(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _TREEVIEW_WM_SETCURSOR(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _TREEVIEW_WM_SETFOCUS(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _TREEVIEW_WM_SETFONT(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _TREEVIEW_WM_SETREDRAW(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _TREEVIEW_WM_SIZE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _TREEVIEW_WM_SYSCOLORCHANGE(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _TREEVIEW_WM_VSCROLL(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _TREEVIEW_WM_TIMER(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _TREEVIEW_WM_CTLCOLORCHILD(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _TREEVIEW_WM_NOTIFY(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
/*
 * tree notification
 */
wresult _TREEVIEW_WM_NOTIFY_GETDISPINFO(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _TREEVIEW_WM_NOTIFY_CDDS_PREPAINT(w_widget *widget,
		_w_event_platform *e, _w_control_priv *priv);
wresult _TREEVIEW_WM_NOTIFY_CDDS_ITEMPREPAINT(w_widget *widget,
		_w_event_platform *e, _w_control_priv *priv);
wresult _TREEVIEW_WM_NOTIFY_CDDS_ITEMPOSTPAINT(w_widget *widget,
		_w_event_platform *e, _w_control_priv *priv);
wresult _TREEVIEW_WM_NOTIFY_CDDS_POSTPAINT(w_widget *widget,
		_w_event_platform *e, _w_control_priv *priv);
wresult _TREEVIEW_WM_NOTIFY_CUSTOMDRAW(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _TREEVIEW_WM_NOTIFY_DBLCLK(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _TREEVIEW_WM_NOTIFY_ITEMCHANGING(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _TREEVIEW_WM_NOTIFY_SELCHANGING(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _TREEVIEW_WM_NOTIFY_SELCHANGED(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _TREEVIEW_WM_NOTIFY_ITEMEXPANDED(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _TREEVIEW_WM_NOTIFY_ITEMEXPANDING(w_widget *widget,
		_w_event_platform *e, _w_control_priv *priv);
wresult _TREEVIEW_WM_NOTIFY_BEGINDRAG(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
wresult _TREEVIEW_WM_NOTIFYCHILD(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
/*
 *
 */
wresult _TREEVIEW_WM_NOTIFY_HEADER(w_widget *widget, _w_event_platform *e,
		_w_control_priv *priv);
#endif /* WIN32_CONTROLS_TREEVIEW_H_ */
