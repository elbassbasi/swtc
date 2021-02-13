/*
 * layoutfill.c
 *
 *  Created on: 5 août 2019
 *      Author: azeddine El Bassbasi
 */
#include <swtc.h>
void w_layout_fill_compute_size(w_layout *layout, struct w_composite *composite,
		w_size *size, int wHint, int hHint, int flushCache);
void w_layout_fill_do_layout(w_layout *layout, struct w_composite *composite,
		int flushCache);
_w_layout_class w_layout_fill_class = { //
		w_layout_fill_compute_size, //
				w_layout_flush_cache0, //
				w_layout_fill_do_layout //
		};
void w_layout_fill_init(w_layout_fill *layout, int type) {
	layout->layout.clazz = &w_layout_fill_class;
	layout->type = type;
	layout->marginWidth = 0;
	layout->marginHeight = 0;
	layout->spacing = 0;

}
void w_layout_fill_compute_child_size(w_control *control, w_size *size,
		int wHint, int hHint, int flushCache) {
	if (wHint == W_DEFAULT && hHint == W_DEFAULT) {
		w_control_compute_size(control, size, wHint, hHint);
	} else {
		// TEMPORARY CODE
		int trimX, trimY;
		w_rect trim, rect;
		if (w_widget_class_id(W_WIDGET(control)) >= _W_CLASS_SCROLLABLE) {
			memset(&rect, 0, sizeof(rect));
			w_scrollable_compute_trim(W_SCROLLABLE(control), &trim, &rect);
		} else {
			trim.width = trim.height = w_control_get_border_width(control) * 2;
		}
		int w = wHint == W_DEFAULT ? wHint : WMAX(0, wHint - trim.width);
		int h = hHint == W_DEFAULT ? hHint : WMAX(0, hHint - trim.height);
		w_control_compute_size(control, size, w, h);
	}
}
void w_layout_fill_compute_size(w_layout *layout, struct w_composite *composite,
		w_size *size, int wHint, int hHint, int flushCache) {
	w_iterator children;
	w_control *child = 0;
	w_iterator_init(&children);
	w_composite_get_children(composite, &children);
	int count = w_iterator_get_count(&children);
	int maxWidth = 0, maxHeight = 0;
	while (w_iterator_next(&children, &child)) {
		if (child != 0) {
			int w = wHint, h = hHint;
			if (count > 0) {
				if (((w_layout_fill*) layout)->type == W_HORIZONTAL
						&& wHint != W_DEFAULT) {
					w =
							WMAX(0,
									(wHint
											- (count - 1)
													* ((w_layout_fill*) layout)->spacing)
											/ count);
				}
				if (((w_layout_fill*) layout)->type == W_VERTICAL
						&& hHint != W_DEFAULT) {
					h =
							WMAX(0,
									(hHint
											- (count - 1)
													* ((w_layout_fill*) layout)->spacing)
											/ count);
				}
			}
			w_layout_fill_compute_child_size(child, size, w, h, W_FALSE);
			maxWidth = WMAX(maxWidth, size->width);
			maxHeight = WMAX(maxHeight, size->height);
		}

	}
	int width = 0, height = 0;
	if (((w_layout_fill*) layout)->type == W_HORIZONTAL) {
		width = count * maxWidth;
		if (count != 0)
			width += (count - 1) * ((w_layout_fill*) layout)->spacing;
		height = maxHeight;
	} else {
		width = maxWidth;
		height = count * maxHeight;
		if (count != 0)
			height += (count - 1) * ((w_layout_fill*) layout)->spacing;
	}
	width += ((w_layout_fill*) layout)->marginWidth * 2;
	height += ((w_layout_fill*) layout)->marginHeight * 2;
	if (wHint != W_DEFAULT)
		width = wHint;
	if (hHint != W_DEFAULT)
		height = hHint;
	size->width = width;
	size->height = height;
}
void w_layout_fill_do_layout(w_layout *layout, struct w_composite *composite,
		int flushCache) {
	w_iterator it;
	w_rect rect, r;
	w_control *child;
	int count = 0, i, extra, width, height, cellWidth, cellHeight;
	w_scrollable_get_client_area(W_SCROLLABLE(composite), &rect);
	w_iterator_init(&it);
	w_composite_get_children(composite, &it);
	count = w_iterator_get_count(&it);
	if (count == 0) {
		w_iterator_close(&it);
		return;
	}
	//w_iterator_reset(&it);
	width = rect.width - ((w_layout_fill*) layout)->marginWidth * 2;
	height = rect.height - ((w_layout_fill*) layout)->marginHeight * 2;
	if (((w_layout_fill*) layout)->type == W_HORIZONTAL) {
		width -= (count - 1) * ((w_layout_fill*) layout)->spacing;
		r.x = rect.x + ((w_layout_fill*) layout)->marginWidth;
		r.y = rect.y + ((w_layout_fill*) layout)->marginHeight;
		r.height = height;
		extra = width % count;
		cellWidth = width / count;
		i = 0;
		while (w_iterator_next(&it, &child)) {
			r.width = cellWidth;
			if (i == 0) {
				r.width += extra / 2;
			} else {
				if (i == count - 1)
					r.width += (extra + 1) / 2;
			}
            w_control_set_bounds(child, &r.pt,&r.sz);
			r.x += r.width + ((w_layout_fill*) layout)->spacing;
			i++;
		}
	} else {
		height -= (count - 1) * ((w_layout_fill*) layout)->spacing;
		r.x = rect.x + ((w_layout_fill*) layout)->marginWidth;
		r.y = rect.y + ((w_layout_fill*) layout)->marginHeight;
		r.width = width;
		cellHeight = height / count;
		extra = height % count;
		while (w_iterator_next(&it, &child)) {
			r.height = cellHeight;
			if (i == 0) {
				r.height += extra / 2;
			} else {
				if (i == count - 1)
					r.height += (extra + 1) / 2;
			}
            w_control_set_bounds(child, &r.pt,&r.sz);
			r.y += r.height + ((w_layout_fill*) layout)->spacing;
			i++;
		}
	}
	w_iterator_close(&it);
}

