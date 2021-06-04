/*
 * Name:        treeview.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "treeview.h"
#include "../widgets/toolkit.h"
/*
 * treeitem
 */
wresult _w_treeitem_get_data(w_item *item, void **data) {
	*data = SWTTreeItem_getUserData(_W_TREEITEM(item)->handle);
	return W_TRUE;
}
wresult _w_treeitem_get_text(w_item *item, w_alloc alloc, void *user_data,
		int enc) {
	return W_FALSE;
}
wresult _w_treeitem_set_data(w_item *item, void *data) {
	SWTTreeItem_setUserData(_W_TREEITEM(item)->handle, data);
	return W_TRUE;
}
wresult _w_treeitem_set_text(w_item *item, const char *text, int length,
		int enc) {
	NSString *str = NSString_new(text, length, enc);
	SWTTreeItem_setText(_W_TREEITEM(item)->handle, str);
	return W_TRUE;
}
wresult _w_treeitem_clear(w_treeitem *item, int index, int all) {
	return W_FALSE;
}
wresult _w_treeitem_clear_all(w_treeitem *item, int all) {
	return W_FALSE;
}
wresult _w_treeitem_get_bounds(w_treeitem *item, w_rect *bounds) {
	return W_FALSE;
}
wresult _w_treeitem_get_checked(w_treeitem *item) {
	return W_FALSE;
}
wresult _w_treeitem_get_expanded(w_treeitem *item) {
	return W_FALSE;
}
wresult _w_treeitem_get_first_child(w_treeitem *item, w_treeitem *child) {
	return W_FALSE;
}
wresult _w_treeitem_get_grayed(w_treeitem *item) {
	return W_FALSE;
}
wresult _w_treeitem_get_item(w_treeitem *item, int index, w_treeitem *subitem,
		int flags) {
	return W_FALSE;
}
wresult _w_treeitem_get_item_count(w_treeitem *item) {
	return W_FALSE;
}
wresult _w_treeitem_get_items(w_treeitem *item, w_iterator *items) {
	return W_FALSE;
}
wresult _w_treeitem_get_image(w_treeitem *item) {
	return W_FALSE;
}
wresult _w_treeitem_get_last_child(w_treeitem *item, w_treeitem *child) {
	return W_FALSE;
}
wresult _w_treeitem_get_next_sibling(w_treeitem *item, w_treeitem *next) {
	return W_FALSE;
}
wresult _w_treeitem_get_parent_item(w_treeitem *item, w_treeitem *parent) {
	return W_FALSE;
}
wresult _w_treeitem_get_prev_sibling(w_treeitem *item, w_treeitem *prev) {
	return W_FALSE;
}
wresult _w_treeitem_insert_item(w_treeitem *item, w_treeitem *subitem, int index,
		w_treeitem *after, int flags) {
	NSOutlineView *view = _W_TREEITEM(item)->view;
	SWTTreeItem *parent = _W_TREEITEM(item)->handle;
	SWTTreeItem *newitem = SWTTreeItem_create(_W_TREEITEM(item)->view, parent,
			index);
	if (subitem != 0) {
		W_WIDGETDATA(subitem)->clazz = W_WIDGETDATA(item)->clazz;
		_W_ITEM(subitem)->parent = _W_ITEM(item)->parent;
		_W_ITEM(subitem)->index = index;
		_W_TREEITEM(subitem)->handle = newitem;
		_W_TREEITEM(subitem)->view = _W_TREEITEM(item)->view;
	}
	if (parent != 0) {
		NSOutlineView_reloadItem(view, parent, W_TRUE);
	} else {
		NSOutlineView_reloadData(view);
	}
	return W_FALSE;
}
wresult _w_treeitem_insert_item_after(w_treeitem *item, w_treeitem *subitem,
		w_treeitem *after) {
	return W_FALSE;
}
wresult _w_treeitem_remove_all(w_treeitem *item) {
	return W_FALSE;
}
wresult _w_treeitem_set_checked(w_treeitem *item, int checked) {
	return W_FALSE;
}
wresult _w_treeitem_set_expanded(w_treeitem *item, int expanded) {
	return W_FALSE;
}
wresult _w_treeitem_set_grayed(w_treeitem *item, int grayed) {
	return W_FALSE;
}
wresult _w_treeitem_set_has_children(w_treeitem *item) {
	SWTTreeItem_setHasChildren(_W_TREEITEM(item)->handle);
	return W_TRUE;
}
wresult _w_treeitem_set_image(w_treeitem *item, int image) {
	return W_FALSE;
}
wresult _w_treeitem_set_item_count(w_treeitem *item, int count) {
	return W_FALSE;
}
/*
 * treeview
 */
wuint64 _w_treeview_check_style(w_widget *widget, wuint64 style) {
	return style;
}
wresult _w_treeview_create_handle(w_widget *widget, _w_control_priv *priv) {
	wuint64 style = _W_WIDGET(widget)->style;
	NSScrollView *scrollWidget = SWTScrollView_new(widget);
	NSScrollView_setHasHorizontalScroller(scrollWidget,
			(style & W_HSCROLL) != 0);
	NSScrollView_setHasVerticalScroller(scrollWidget, (style & W_VSCROLL) != 0);
	NSScrollView_setAutohidesScrollers(scrollWidget, W_TRUE);
	NSScrollView_setBorderType(scrollWidget,
			(style & W_BORDER) ? NSBezelBorder : NSNoBorder);

	NSOutlineView *outline = SWTOutlineView_new(widget);
	NSOutlineView_setAllowsMultipleSelection(outline, (style & W_MULTI) != 0);
	NSOutlineView_setAutoresizesOutlineColumn(outline, W_FALSE);
	NSOutlineView_setAutosaveExpandedItems(outline, W_TRUE);
	NSOutlineView_setColumnAutoresizingStyle(outline,
			NSTableViewNoColumnAutoresizing);
	NSOutlineView_setIntercellSpacing(outline, LISTVIEW_CELL_GAP, LISTVIEW_CELL_GAP);
	//outline.setDoubleAction (OS.sel_sendDoubleSelection);
	//if (!(style & W_BORDER)) outline.setFocusRingType (OS.NSFocusRingTypeNone);

	NSScrollView_setDocumentView(scrollWidget, (NSView*) outline);

	_W_LISTVIEWBASE(widget)->headerView = NSTableHeaderView_new(widget);
	NSTableView_setHeaderView((NSTableView*)outline,0);

	//NSString str = NSString.string();
	NSString *_id;
	char __id[10];
	if ((style & W_CHECK) != 0) {
	}
	sprintf(__id, "id_%d", _W_LISTVIEWBASE(widget)->NEXT_ID++);
	_id = NSString_stringWithUTF8String(__id);
	NSTableColumn *firstColumn = SWTTableColumn_new(_id);
	/*
	 * Feature in Cocoa.  If a column's width is too small to show any content
	 * then outlineView_objectValueForTableColumn_byItem is never invoked to
	 * query for item values, which is a problem for VIRTUAL Trees.  The
	 * workaround is to ensure that, for 0-column Trees, the internal first
	 * column always has a minimal width that makes this call come in.
	 */
	NSTableColumn_setMinWidth(firstColumn, LISTVIEW_FIRST_COLUMN_MINIMUM_WIDTH);
	//NSTableColumn_setWidth(firstColumn,0);
	NSTableColumn_setResizingMask(firstColumn, NSTableColumnNoResizing);
	NSCell_setTitle(NSTableColumn_headerCell(firstColumn), _id/*str*/);
	NSTableView_addTableColumn((NSTableView*) outline, firstColumn);
	NSOutlineView_setOutlineTableColumn(outline, firstColumn);
	NSCell *dataCell = SWTImageTextCell_new(_id);
	NSCell_setLineBreakMode(dataCell, NSLineBreakByTruncatingTail);
	NSTableColumn_setDataCell(firstColumn, dataCell);
	_W_WIDGET(widget)->handle = (NSView*) scrollWidget;
	return W_TRUE;
}
wresult _w_treeview_clear(w_treeview *tree, w_treeitem *item) {
	return W_FALSE;
}
wresult _w_treeview_deselect(w_treeview *tree, w_treeitem *item) {
	return W_FALSE;
}
wresult _w_treeview_get_item_from_point(w_treeview *tree, w_point *point,
		w_treeitem *item) {
	return W_FALSE;
}
wresult _w_treeview_get_parent_item(w_treeview *tree, w_treeitem *item) {
	return W_FALSE;
}
wresult _w_treeview_get_root_item(w_treeview *tree, w_treeitem *root) {
	_w_control_priv *priv = _W_CONTROL_GET_PRIV(tree);
	_W_WIDGETDATA(root)->clazz = _W_LISTVIEWBASE_GET_ITEM_CLASS(tree);
	_W_ITEM(root)->parent = W_WIDGET(tree);
	_W_ITEM(root)->index = -1;
	_W_TREEITEM(root)->view = (NSOutlineView*) priv->get_view(W_WIDGET(tree));
	_W_TREEITEM(root)->handle = 0;
	return W_TRUE;
}
wresult _w_treeview_get_top_item(w_treeview *tree, w_treeitem *topitem) {
	return W_FALSE;
}
wresult _w_treeview_remove(w_treeview *tree, w_treeitem *item) {
	return W_FALSE;
}
wresult _w_treeview_set_insert_mark(w_treeview *tree, w_treeitem *item,
		int before) {
	return W_FALSE;
}
wresult _w_treeview_set_selection(w_treeview *tree, w_treeitem *item) {
	return W_FALSE;
}
wresult _w_treeview_set_top_item(w_treeview *tree, w_treeitem *item) {
	return W_FALSE;
}
wresult _w_treeview_select(w_treeview *tree, w_treeitem *item) {
	return W_FALSE;
}
wresult _w_treeview_show_item(w_treeview *tree, w_treeitem *item) {
	return W_FALSE;
}
/*
 * messages
 */
wresult _ns_treeview_outlineViewItemWillExpand(w_widget *widget,
		_w_event_platform *e, _w_control_priv *priv) {
	w_treeitem item;
	w_event_list event;
	memset(&event, 0, sizeof(event));
	event.event.type = W_EVENT_ITEM_EXPAND;
	event.event.widget = widget;
	event.item = &item;
	_W_WIDGETDATA(&item)->clazz = _W_LISTVIEWBASE_GET_ITEM_CLASS(widget);
	_W_ITEM(&item)->parent = widget;
	_W_ITEM(&item)->index = -1;
	_W_TREEITEM(&item)->handle = e->args[0]._ptr;
	NSOutlineView *view = priv->get_view(widget);
	_W_TREEITEM(&item)->view = view;
	_w_widget_send_event(widget, W_EVENT(&event));
	return W_FALSE;
}
wresult _ns_treeview_outlineViewSelectionDidChange(w_widget *widget,
		_w_event_platform *e, _w_control_priv *priv) {
	//if (ignoreSelect) return;
	w_treeitem item;
	w_event_list event;
	memset(&event, 0, sizeof(event));
	event.event.type = W_EVENT_ITEM_SELECTION;
	event.event.widget = widget;
	NSUInteger row = NSTableView_selectedRow((NSTableView*) e->handle);
	if (row == -1) {
		event.item = 0;
	} else {
		SWTTreeItem *handle = (SWTTreeItem*) NSOutlineView_itemAtRow(
				(NSOutlineView*) e->handle, row);
		event.item = &item;
		_W_WIDGETDATA(&item)->clazz = _W_LISTVIEWBASE_GET_ITEM_CLASS(widget);
		_W_ITEM(&item)->parent = widget;
		_W_TREEITEM(&item)->handle = handle;
		NSOutlineView *view = (NSOutlineView *)priv->get_view(widget);
		_W_TREEITEM(&item)->view = view;
	}
	_w_widget_send_event(widget, (w_event*) &event);
	return W_FALSE;
}
void _w_treeview_class_init(struct _w_treeview_class *clazz) {
	_w_listviewbase_class_init(W_LISTVIEWBASE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_TREEVIEW;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_treeview_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_treeview);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_treeview);
	/*
	 * functions
	 */
	clazz->clear = _w_treeview_clear;
	clazz->deselect = _w_treeview_deselect;
	clazz->get_item_from_point = _w_treeview_get_item_from_point;
	clazz->get_parent_item = _w_treeview_get_parent_item;
	clazz->get_root_item = _w_treeview_get_root_item;
	clazz->get_top_item = _w_treeview_get_top_item;
	clazz->remove = _w_treeview_remove;
	clazz->set_insert_mark = _w_treeview_set_insert_mark;
	clazz->set_selection = _w_treeview_set_selection;
	clazz->set_top_item = _w_treeview_set_top_item;
	clazz->select = _w_treeview_select;
	clazz->show_item = _w_treeview_show_item;
	/*
	 * tree item
	 */
	struct _w_treeitem_class *treeitem = W_TREEITEM_CLASS(
			clazz->base.class_item);
	_w_item_class_init(W_ITEM_CLASS(treeitem));
	W_ITEM_CLASS(treeitem)->get_data = _w_treeitem_get_data;
	W_ITEM_CLASS(treeitem)->get_text = _w_treeitem_get_text;
	W_ITEM_CLASS(treeitem)->set_data = _w_treeitem_set_data;
	W_ITEM_CLASS(treeitem)->set_text = _w_treeitem_set_text;
	W_LISTITEM_CLASS(treeitem)->get_bounds = _w_treeitem_get_bounds;
	W_LISTITEM_CLASS(treeitem)->get_checked = _w_treeitem_get_checked;
	W_LISTITEM_CLASS(treeitem)->get_grayed = _w_treeitem_get_grayed;
	W_LISTITEM_CLASS(treeitem)->get_image = _w_treeitem_get_image;
	W_LISTITEM_CLASS(treeitem)->set_checked = _w_treeitem_set_checked;
	W_LISTITEM_CLASS(treeitem)->set_grayed = _w_treeitem_set_grayed;
	W_LISTITEM_CLASS(treeitem)->set_image = _w_treeitem_set_image;
	treeitem->clear = _w_treeitem_clear;
	treeitem->clear_all = _w_treeitem_clear_all;
	treeitem->get_expanded = _w_treeitem_get_expanded;
	treeitem->get_item = _w_treeitem_get_item;
	treeitem->get_item_count = _w_treeitem_get_item_count;
	treeitem->get_items = _w_treeitem_get_items;
	treeitem->insert_item = _w_treeitem_insert_item;
	treeitem->remove_all = _w_treeitem_remove_all;
	treeitem->set_expanded = _w_treeitem_set_expanded;
	treeitem->set_has_children = _w_treeitem_set_has_children;
	/*
	 * private
	 */
	_w_control_priv *priv = _W_CONTROL_PRIV(W_WIDGET_CLASS(clazz)->reserved[0]);
	_W_WIDGET_PRIV(priv)->check_style = _w_treeview_check_style;
	_W_WIDGET_PRIV(priv)->create_handle = _w_treeview_create_handle;
	/*
	 * messages
	 */
	dispatch_message *msgs = _W_WIDGET_PRIV(priv)->msgs;
    msgs[_NS_outlineViewItemWillExpand]=_ns_treeview_outlineViewItemWillExpand;
    msgs[_NS_outlineViewSelectionDidChange]=_ns_treeview_outlineViewSelectionDidChange;
}
