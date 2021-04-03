/*
 * sash.c
 *
 *  Created on: 22 oct. 2019
 *      Author: azeddine El Bassbasi
 */
#include <swtc.h>
#define DRAG_MINIMUM 20
void w_layout_sash_compute_size(w_layout *layout, w_composite *composite,
		w_size *size, int wHint, int hHint, int flushCache);
void w_layout_sash_do_layout(w_layout *_layout, w_composite *composite,
		int flushCache);
void w_layout_sash_set_data(w_layout *layout, w_control *control,
		const w_layout_data *_data) {
	w_layout_set_data(layout, control, _data);
}
void w_layout_sash_get_data(w_layout *layout, w_control *control,
		w_layout_data *_data) {
	void *data;
	w_control_get_layout_data(control, &data);
}
_w_layout_class w_layout_sash_class = { //
		w_layout_sash_compute_size, //
				w_layout_flush_cache0, //
				w_layout_sash_do_layout, //
				w_layout_sash_set_data, //
				w_layout_sash_get_data //
		};
int w_layout_sash_default_selection(w_widget *widget, w_event *event) {
	w_event_sash *e = (w_event_sash*) event;
	w_composite *parent;
	w_layout_sash_data *d;
	w_control_get_parent(W_CONTROL(widget), &parent);
	if (parent == 0)
		return W_FALSE;
	w_control_get_layout_data(W_CONTROL(widget), (void**) &d);
	if (d == 0)
		return W_FALSE;
	int sashIndex = d->index;
	w_control *c, *c1 = 0, *c2 = 0;
	w_iterator children;
	w_iterator_init(&children);
	w_composite_get_children(parent, &children);
	int i = 0;
	while (w_iterator_next(&children, &c)) {
		if (w_widget_class_id(W_WIDGET(c)) != _W_CLASS_SASH) {
			if (i == sashIndex)
				c1 = c;
			if (i == (sashIndex + 1)) {
				c2 = c;
				break;
			}
			i++;
		}
	}
	if (c1 == 0 || c2 == 0)
		return W_FALSE;
	w_rect b1, b2, sashBounds, area;
	w_control_get_bounds(c1, &b1.pt, &b1.sz);
	w_control_get_bounds(c2, &b2.pt, &b2.sz);
	w_control_get_bounds(W_CONTROL(widget), &sashBounds.pt, &sashBounds.sz);
	w_scrollable_get_client_area(W_SCROLLABLE(parent), &area);
	int correction = W_FALSE, doit = W_TRUE;
	if (w_widget_get_style(widget) & W_VERTICAL) {
		correction = b1.width < DRAG_MINIMUM || b2.width < DRAG_MINIMUM;
		int totalWidth = b2.x + b2.width - b1.x;
		int shift = e->bounds.x - sashBounds.x;
		b1.width += shift;
		b2.x += shift;
		b2.width -= shift;
		if (b1.width < DRAG_MINIMUM) {
			b1.width = DRAG_MINIMUM;
			b2.x = b1.x + b1.width + sashBounds.width;
			b2.width = totalWidth - b2.x;
			e->bounds.x = b1.x + b1.width;
			doit = W_FALSE;
		}
		if (b2.width < DRAG_MINIMUM) {
			b1.width = totalWidth - DRAG_MINIMUM - sashBounds.width;
			b2.x = b1.x + b1.width + sashBounds.width;
			b2.width = DRAG_MINIMUM;
			e->bounds.x = b1.x + b1.width;
			doit = W_FALSE;
		}

		w_control_new_layout_data(c1, (void**) &d, sizeof(w_layout_sash_data));
		if (d != 0) {
			d->weight = ((b1.width << 16) + area.width - 1) / area.width;
		}
		w_control_new_layout_data(c2, (void**) &d, sizeof(w_layout_sash_data));
		if (d != 0) {
			d->weight = ((b2.width << 16) + area.width - 1) / area.width;
		}
	} else {
		correction = b1.height < DRAG_MINIMUM || b2.height < DRAG_MINIMUM;
		int totalHeight = b2.y + b2.height - b1.y;
		int shift = e->bounds.y - sashBounds.y;
		b1.height += shift;
		b2.y += shift;
		b2.height -= shift;
		if (b1.height < DRAG_MINIMUM) {
			b1.height = DRAG_MINIMUM;
			b2.y = b1.y + b1.height + sashBounds.height;
			b2.height = totalHeight - b2.y;
			e->bounds.y = b1.y + b1.height;
			doit = W_FALSE;
		}
		if (b2.height < DRAG_MINIMUM) {
			b1.height = totalHeight - DRAG_MINIMUM - sashBounds.height;
			b2.y = b1.y + b1.height + sashBounds.height;
			b2.height = DRAG_MINIMUM;
			e->bounds.y = b1.y + b1.height;
			doit = W_FALSE;
		}
		w_control_new_layout_data(c1, (void**) &d, sizeof(w_layout_sash_data));
		if (d != 0) {
			d->weight = (((long) b1.height << 16) + area.height - 1)
					/ area.height;
		}
		w_control_new_layout_data(c2, (void**) &d, sizeof(w_layout_sash_data));
		if (d != 0) {
			d->weight = (((long) b2.height << 16) + area.height - 1)
					/ area.height;
		}
	}
	if (correction || (doit /*&& event.detail != SWT.DRAG*/)) {
		w_control_set_bounds(c1, &b1.pt, &b1.sz);
		w_control_set_bounds(W_CONTROL(widget), &e->bounds.pt, &e->bounds.sz);
		w_control_set_bounds(c2, &b2.pt, &b2.sz);
	}
	return doit;
}
wresult w_layout_sash_event_proc(w_widget *widget, w_event *event) {
	if (event->type == W_EVENT_SELECTION) {
		return w_layout_sash_default_selection(widget, event);
	}
	return w_widget_default_post_event(widget, event);
}
void w_layout_sash_init(w_layout_sash *layout, int style) {
	layout->layout.clazz = &w_layout_sash_class;
	layout->count = 0;
	layout->style = style;
	layout->sashes = 0;
}
void w_layout_sash_free(w_layout_sash *layout) {
	if (layout->style & W_FREE_MEMORY) {
		if (layout->sashes != 0) {
			for (unsigned int i = 0; i < layout->count; i++) {
				w_widget_dispose(W_WIDGET(&layout->sashes[i]));
			}
			free(layout->sashes);
			layout->sashes = 0;
			layout->count = 0;
			layout->style &= ~W_FREE_MEMORY;
		}
	}
}
void w_layout_sash_set_sashes(w_layout_sash *layout, struct w_sash *sashes,
		size_t count) {
	w_layout_sash_free(layout);
	if (sashes != 0) {
		layout->sashes = sashes;
		layout->count = count;
	}
}
void w_layout_sash_set_weights(w_layout_sash *layout, w_composite *composite,
		int *weights, size_t count) {
	w_iterator children;
	w_control *c;
	w_layout_sash_data *d;
	w_iterator_init(&children);
	w_composite_get_children(composite, &children);
	size_t total = 0, i;
	for (i = 0; i < count; i++) {
		total += weights[i];
	}
	i = 0;
	while (w_iterator_next(&children, &c)) {
		if (i < count) {
			w_control_new_layout_data(c, (void**) &d,
					sizeof(w_layout_sash_data));
			if (d != 0) {
				d->weight = (((long) weights[i] << 16) + total - 1) / total;
			}
		}
		i++;
	}
	w_layout_sash_do_layout((w_layout*) layout, composite, W_FALSE);
}
void w_layout_sash_compute_size(w_layout *layout, struct w_composite *composite,
		w_size *size, int wHint, int hHint, int flushCache) {

}

void w_layout_sash_do_layout(w_layout *_layout, struct w_composite *composite,
		int flushCache) {
	w_layout_sash *layout = (w_layout_sash*) _layout;
	w_layout_sash_data *d;
	w_rect area, r;
	w_scrollable_get_client_area(W_SCROLLABLE(composite), &area);
	if (area.width <= 1 || area.height <= 1)
		return;
	w_iterator children;
	w_control *c;
	w_iterator_init(&children);
	w_composite_get_children(composite, &children);
	unsigned int length = 0, total, width, height;
	while (w_iterator_next(&children, &c)) {
		if (w_widget_class_id(W_WIDGET(c)) != _W_CLASS_SASH) {
			length++;
		}
	}
	if (length == 0)
		return;
	if (layout->sashes == 0) {
		layout->sashes = malloc(sizeof(w_sash) * length);
		if (layout->sashes != 0) {
			layout->count = length;
			for (unsigned int i = 0; i < length; i++) {
				w_widget_init(W_WIDGET(&layout->sashes[i]));
			}
			layout->style |= W_FREE_MEMORY;
		}
	}
	int min_length;
	if (layout->count > (length - 1)) {
		min_length = length - 1;
	} else {
		min_length = layout->count;
	}
	int sash_style;
	if (layout->style & W_HORIZONTAL) {
		sash_style = W_VERTICAL;
	} else {
		sash_style = W_HORIZONTAL;
	}
	for (int i = 0; i < min_length; i++) {
		w_sash *sash = &layout->sashes[i];
		if (!w_widget_is_ok(W_WIDGET(sash))) {
			w_sash_create(sash, 0, composite, sash_style,
					w_layout_sash_event_proc);
			w_control_new_layout_data(W_CONTROL(sash), (void**) &d,
					sizeof(w_layout_sash_data));
			if (d != 0) {
				d->index = i;
				d->flags = 0;
				d->weight = 0;
			}
		}
	}
	for (unsigned int i = length; i < layout->count; i++) {
		if (w_widget_is_ok(W_WIDGET(&layout->sashes[i]))) {
			w_widget_dispose(W_WIDGET(&layout->sashes[i]));
		}
	}
	total = 0;
	w_iterator_reset(&children);
	while (w_iterator_next(&children, &c)) {
		if (w_widget_class_id(W_WIDGET(c)) != _W_CLASS_SASH) {
			w_control_get_layout_data(c, (void**) &d);
			if (d != 0) {
				total += d->weight;
			} else {
				total += ((200 << 16) + 999) / 1000;
			}
		}
	}
	int sashwidth = 3 /*sashes.length > 0 ? sashForm.SASH_WIDTH + sashes [0].getBorderWidth() * 2 : sashForm.SASH_WIDTH*/;
	if (layout->style & W_HORIZONTAL) {
		int x = area.x;
		w_iterator_reset(&children);
		length = 0;
		while (w_iterator_next(&children, &c)) {
			if (w_widget_class_id(W_WIDGET(c)) != _W_CLASS_SASH) {
				if (length > 0) {
					if ((length - 1) < layout->count) {
						r.x = x;
						r.y = area.y;
						r.width = sashwidth;
						r.height = area.height;
						w_control_set_bounds(
								W_CONTROL(&layout->sashes[length - 1]), &r.pt,
								&r.sz);
					}
					x += sashwidth;
				}
				w_control_get_layout_data(c, (void**) &d);
				size_t weight;
				if (d != 0) {
					weight = d->weight;
				} else {
					weight = ((200 << 16) + 999) / 1000;
				}
				width = (int) (weight * (area.width - layout->count * sashwidth)
						/ total);
				r.x = x;
				r.y = area.y;
				r.width = width;
				r.height = area.height;
				w_control_set_bounds(W_CONTROL(c), &r.pt, &r.sz);
				x += width;
				length++;
			}
		}
	} else {
		int y = area.y;
		w_iterator_reset(&children);
		length = 0;
		while (w_iterator_next(&children, &c)) {
			if (w_widget_class_id(W_WIDGET(c)) != _W_CLASS_SASH) {
				if (length > 0) {
					if ((length - 1) < layout->count) {
						r.x = area.x;
						r.y = y;
						r.width = area.width;
						r.height = sashwidth;
						w_control_set_bounds(
								W_CONTROL(&layout->sashes[length - 1]), &r.pt,
								&r.sz);
					}
					y += sashwidth;
				}
				w_control_get_layout_data(c, (void**) &d);
				size_t weight;
				if (d != 0) {
					weight = d->weight;
				} else {
					weight = ((200 << 16) + 999) / 1000;
				}
				height = (int) (weight
						* (area.width - layout->count * sashwidth) / total);
				r.x = area.x;
				r.y = y;
				r.width = area.width;
				r.height = height;
				w_control_set_bounds(W_CONTROL(c), &r.pt, &r.sz);
				y += height;
				length++;
			}
		}
	}
	return;
}
