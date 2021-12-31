/*
 * Name:        theme.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "toolkit.h"
typedef struct _gtk_theme_info {
	int flags;
	int state;
	GtkWidget *handle;

} _gtk_theme_info;
typedef void (*_gtk_theme_part_id)(w_theme *theme, int mask, w_themedata *data,
		_gtk_theme_info *info);
void _gtk_theme_transfer_clipping(w_graphics *gc, GtkStyleContext *style) {
}
void _gtk_render_box(GtkStyleContext *style, cairo_t *window, int x, int y,
		int width, int height) {
	gtk_render_frame(style, window, x, y, width, height);
	gtk_render_background(style, window, x, y, width, height);
}
void _gtk_themedata_part_id(w_theme *theme, w_themedata *data, int *ids,
		int part) {
	int state = data->state;
	int state_type = GTK_STATE_NORMAL;
	if (state & W_THEME_STATE_SELECTED) {
		state_type |= GTK_STATE_FLAG_SELECTED;
	}
	if (state & W_THEME_STATE_FOCUSED) {
		state_type |= GTK_STATE_FLAG_FOCUSED;
	}
	if (state & W_THEME_STATE_HOT) {
		state_type |= GTK_STATE_FLAG_PRELIGHT;
	}
	if (state & W_THEME_STATE_PRESSED) {
		state_type |= GTK_STATE_FLAG_SELECTED;
	}
	if (state & W_THEME_STATE_ACTIVE) {
		state_type |= GTK_STATE_FLAG_ACTIVE;
	}
	if (state & W_THEME_STATE_DISABLED) {
		state_type |= GTK_STATE_FLAG_INSENSITIVE;
	}
	if (state & W_THEME_STATE_DEFAULTED) {
		state_type = GTK_STATE_FLAG_PRELIGHT;
	}
	if (state & W_THEME_STATE_GRAYED) {
		state_type = GTK_STATE_FLAG_VISITED;
	}
	ids[0] = state_type;
}
void _gtk_themedata_button_part_id(w_theme *theme, w_themedata *data, int *ids,
		int part) {
	_gtk_themedata_part_id(theme, data, ids, part);
	if (data->style & (W_RADIO | W_CHECK)) {
		if (data->state & W_THEME_STATE_SELECTED) {
			ids[0] |= GTK_STATE_FLAG_CHECKED;
		}
	}
}
static GtkStyleContext*
create_context_for_path(GtkWidgetPath *path, GtkStyleContext *parent) {
	GtkStyleContext *context;

	context = gtk_style_context_new();
	gtk_style_context_set_path(context, path);
	//gtk_style_context_set_parent(context, parent);
	/* Unfortunately, we have to explicitly set the state again here
	 * for it to take effect
	 */
	gtk_style_context_set_state(context,
			gtk_widget_path_iter_get_state(path, -1));
	gtk_widget_path_unref(path);

	return context;
}

static GtkStyleContext*
get_style(GtkStyleContext *parent, const char *selector) {
	GtkWidgetPath *path;

	if (parent)
		path = gtk_widget_path_copy(gtk_style_context_get_path(parent));
	else
		path = gtk_widget_path_new();

	gtk_widget_path_append_type(path, G_TYPE_NONE);
	gtk_widget_path_iter_set_object_name(path, -1, selector);

	return create_context_for_path(path, parent);
}

static void draw_style_common(GtkStyleContext *context, cairo_t *cr, gint x,
		gint y, gint width, gint height, gint *contents_x, gint *contents_y,
		gint *contents_width, gint *contents_height) {
	GtkBorder margin, border, padding;
	int min_width, min_height;

	gtk_style_context_get_margin(context, gtk_style_context_get_state(context),
			&margin);
	gtk_style_context_get_border(context, gtk_style_context_get_state(context),
			&border);
	gtk_style_context_get_padding(context, gtk_style_context_get_state(context),
			&padding);

	gtk_style_context_get(context, gtk_style_context_get_state(context),
			"min-width", &min_width, "min-height", &min_height,
			NULL);
	x += margin.left;
	y += margin.top;
	width -= margin.left + margin.right;
	height -= margin.top + margin.bottom;

	width = MAX(width, min_width);
	height = MAX(height, min_height);

	gtk_render_background(context, cr, x, y, width, height);
	gtk_render_frame(context, cr, x, y, width, height);

	if (contents_x)
		*contents_x = x + border.left + padding.left;
	if (contents_y)
		*contents_y = y + border.top + padding.top;
	if (contents_width)
		*contents_width = width - border.left - border.right - padding.left
				- padding.right;
	if (contents_height)
		*contents_height = height - border.top - border.bottom - padding.top
				- padding.bottom;
}

static void query_size(GtkStyleContext *context, gint *width, gint *height) {
	GtkBorder margin, border, padding;
	int min_width, min_height;

	gtk_style_context_get_margin(context, gtk_style_context_get_state(context),
			&margin);
	gtk_style_context_get_border(context, gtk_style_context_get_state(context),
			&border);
	gtk_style_context_get_padding(context, gtk_style_context_get_state(context),
			&padding);

	gtk_style_context_get(context, gtk_style_context_get_state(context),
			"min-width", &min_width, "min-height", &min_height,
			NULL);

	min_width += margin.left + margin.right + border.left + border.right
			+ padding.left + padding.right;
	min_height += margin.top + margin.bottom + border.top + border.bottom
			+ padding.top + padding.bottom;

	if (width)
		*width = MAX(*width, min_width);
	if (height)
		*height = MAX(*height, min_height);
}

static void draw_check(GtkWidget *widget, cairo_t *cr, gint x, gint y,
		GtkStateFlags state, gint *width, gint *height) {
	GtkStyleContext *button_context = 0;
	GtkStyleContext *check_context;
	gint contents_x, contents_y, contents_width, contents_height;

	/* This information is taken from the GtkCheckButton docs, see "CSS nodes" */
	//button_context = get_style(NULL, "checkbutton");
	check_context = get_style(button_context, "check");

	gtk_style_context_set_state(check_context, state);

	*width = *height = 0;
	//query_size(button_context, width, height);
	query_size(check_context, width, height);

	/*draw_style_common(button_context, cr, x, y, *width, *height, NULL, NULL,
	 NULL, NULL);*/
	draw_style_common(check_context, cr, x, y, *width, *height, &contents_x,
			&contents_y, &contents_width, &contents_height);
	gtk_render_check(check_context, cr, contents_x, contents_y, contents_width,
			contents_height);

	g_object_unref(check_context);
	g_object_unref(button_context);

}
void draw_radio(GtkWidget *widget, cairo_t *cr, gint x, gint y,
		GtkStateFlags state, gint *width, gint *height) {
	GtkStyleContext *button_context;
	GtkStyleContext *check_context;
	gint contents_x, contents_y, contents_width, contents_height;

	/* This information is taken from the GtkRadioButton docs, see "CSS nodes" */
	button_context = get_style(NULL, "radiobutton");
	check_context = get_style(button_context, "radio");

	gtk_style_context_set_state(check_context, state);

	*width = *height = 0;
	query_size(button_context, width, height);
	query_size(check_context, width, height);

	draw_style_common(button_context, cr, x, y, *width, *height, NULL, NULL,
	NULL, NULL);
	draw_style_common(check_context, cr, x, y, *width, *height, &contents_x,
			&contents_y, &contents_width, &contents_height);
	gtk_render_check(check_context, cr, contents_x, contents_y, contents_width,
			contents_height);

	g_object_unref(check_context);
	g_object_unref(button_context);

}
void _gtk_theme_button_draw_background(w_theme *theme, w_themedata *data,
		w_graphics *gc, w_rect *bounds, wuint clazz) {
	_gtk_theme *gtktheme = (_gtk_theme*) theme;
	int ids[3];
	_gtk_themedata_button_part_id(theme, data, ids, W_THEME_PART_WHOLE);
	GtkStateFlags state = ids[0];
	cairo_t *drawable = _W_GRAPHICS(gc)->cairo;
	if ((data->style & (W_RADIO | W_CHECK)) != 0) {
		int handle_id =
				(data->style & W_RADIO) != 0 ?
						GTK_THEME_HANDLE_RADIO_BUTTON :
						GTK_THEME_HANDLE_CHECK_BUTTON;
		GtkWidget *buttonHandle = gtktheme->handle[handle_id];
		GtkStyleContext *gtkStyle = gtk_widget_get_style_context(buttonHandle);
		_gtk_theme_transfer_clipping(gc, gtkStyle);
		int indicator_size = 0;
		int indicator_spacing = 0;
		int focus_line_width = 0;
		int interior_focus = 0;
		int focus_padding = 0;
		int border_width, x, y;
		gtk_widget_style_get(buttonHandle, "indicator-size", &indicator_size,
		NULL);
		gtk_widget_style_get(buttonHandle, "indicator-spacing",
				&indicator_spacing, NULL);
		gtk_widget_style_get(buttonHandle, "interior-focus", &interior_focus,
		NULL);
		gtk_widget_style_get(buttonHandle, "focus-line-width",
				&focus_line_width, NULL);
		gtk_widget_style_get(buttonHandle, "focus-padding", &focus_padding,
		NULL);
		border_width = gtk_container_get_border_width(
				GTK_CONTAINER(buttonHandle));

		x = bounds->x + indicator_spacing + border_width;
		y = bounds->y + (bounds->height - indicator_size) / 2;

		if (interior_focus == 0) {
			x += focus_line_width + focus_padding;
		}

		int shadow_type;
		if ((data->state & W_THEME_STATE_GRAYED) != 0) {
			shadow_type = GTK_SHADOW_ETCHED_IN;
		} else if ((data->state & W_THEME_STATE_SELECTED) != 0) {
			shadow_type = GTK_SHADOW_IN;
		} else {
			shadow_type = GTK_SHADOW_OUT;
		}
		gtk_style_context_set_state(gtkStyle, state);
		const char *selector[2];
		if ((data->style & W_RADIO) != 0) {
			selector[0] = "radiobutton";
			selector[1] = GTK_STYLE_CLASS_RADIO;
		} else {
			selector[0] = "checkbutton";
			selector[1] = GTK_STYLE_CLASS_CHECK;
		}
		GtkWidgetPath *path = gtk_widget_path_new();
		for (int i = 0; i < 2; i++) {
			gtk_widget_path_append_type(path, G_TYPE_NONE);
			gtk_widget_path_iter_set_object_name(path, -1, selector[i]);
			gtk_style_context_set_path(gtkStyle, path);
			//if ((state & W_THEME_HOT) != 0) {
			int prelight_x, prelight_y, prelight_width, prelight_height;
			prelight_x = bounds->x + border_width;
			prelight_y = bounds->y + border_width;
			prelight_width = bounds->width - (2 * border_width);
			prelight_height = bounds->height - (2 * border_width);
			gtk_render_background(gtkStyle, drawable, prelight_x, prelight_y,
					prelight_width, prelight_height);
			//}
			gtk_render_frame(gtkStyle, drawable, x, y, indicator_size,
					indicator_size);
		}
		gtk_widget_path_unref(path);
		if ((data->style & W_RADIO) != 0) {
			/*gtk_render_option(gtkStyle, drawable, x, y, indicator_size,
			 indicator_size);*/
			int width = bounds->width;
			int height = bounds->height;
			draw_radio(buttonHandle, drawable, x, y, state, &width, &height);
		} else {
			int width = bounds->width;
			int height = bounds->height;
			draw_check(buttonHandle, drawable, x, y, state, &width, &height);
			/*gtk_render_check(gtkStyle, drawable, x, y, indicator_size,
			 indicator_size);*/
		}
		if (data->clientArea != 0) {
			data->clientArea->x = bounds->x + 2 * indicator_spacing
					+ border_width + indicator_size;
			data->clientArea->y = bounds->y + border_width;
			data->clientArea->width =
					bounds->width
							- (2 * indicator_spacing + 2 * border_width
									+ indicator_size);
			data->clientArea->height = bounds->height - 2 * border_width;
		}
		return;
	}
	if ((data->style & W_PUSH) != 0) {
		GtkWidget *buttonHandle = gtktheme->handle[GTK_THEME_HANDLE_BUTTON];
		GtkStyleContext *gtkStyle = gtk_widget_get_style_context(buttonHandle);
		gtk_style_context_set_state(gtkStyle, state);
		_gtk_theme_transfer_clipping(gc, gtkStyle);
		int focus_line_width;
		gtk_widget_style_get(buttonHandle, "focus-line-width",
				&focus_line_width, NULL);
		int focus_padding;
		gtk_widget_style_get(buttonHandle, "focus-padding", &focus_padding,
		NULL);
		int border_width = gtk_container_get_border_width(
				GTK_CONTAINER(buttonHandle));

		GtkBorder *default_border_ptr = 0, default_border;
		gtk_widget_style_get(buttonHandle, "default-border",
				&default_border_ptr, NULL);
		if (default_border_ptr != 0) {
			memcpy(&default_border, default_border_ptr, sizeof(GtkBorder));
			gtk_border_free(default_border_ptr);
		} else {
			default_border.left = default_border.right = default_border.top =
					default_border.bottom = 1;
		}

		GtkBorder default_outside_border, *default_outside_border_ptr = 0;
		gtk_widget_style_get(buttonHandle, "default-outside-border",
				&default_outside_border_ptr, NULL);
		if (default_outside_border_ptr != 0) {
			memcpy(&default_outside_border, default_outside_border_ptr,
					sizeof(GtkBorder));
			gtk_border_free(default_outside_border_ptr);
		} else {
			default_outside_border.left = default_outside_border.right =
					default_outside_border.top = default_outside_border.bottom =
							0;
		}

		int x = bounds->x + border_width;
		int y = bounds->y + border_width;
		int width = bounds->width - border_width * 2;
		int height = bounds->height - border_width * 2;

		int relief = gtk_button_get_relief(GTK_BUTTON(buttonHandle));
		char *detail =
				(data->state & W_THEME_STATE_DEFAULTED) != 0 ?
						"buttondefault" : "button";
		if ((data->state & W_THEME_STATE_DEFAULTED) != 0
				&& relief == GTK_RELIEF_NORMAL) {
			gtk_style_context_set_state(gtkStyle, state);
			_gtk_render_box(gtkStyle, drawable, x, y, width, height);
			x += default_border.left;
			y += default_border.top;
			width -= default_border.left + default_border.right;
			height -= default_border.top + default_border.bottom;
		} else if ((data->state & W_THEME_STATE_DEFAULTED) != 0) {
			x += default_outside_border.left;
			y += default_outside_border.top;
			width -= default_outside_border.left + default_outside_border.right;
			height -= default_outside_border.top
					+ default_outside_border.bottom;
		}

		int shadow_type = GTK_SHADOW_OUT;
		if ((data->state & (W_THEME_STATE_SELECTED | W_THEME_STATE_PRESSED))
				!= 0)
			shadow_type = GTK_SHADOW_IN;
		if (relief != GTK_RELIEF_NONE
				|| ((data->state & (W_THEME_STATE_PRESSED | W_THEME_STATE_HOT))
						!= 0)) {
			_gtk_render_box(gtkStyle, drawable, x, y, width, height);
		}

		if ((data->state & W_THEME_STATE_FOCUSED) != 0) {
			int child_displacement_y;
			gtk_widget_style_get(buttonHandle, "child-displacement-y",
					&child_displacement_y, NULL);
			int child_displacement_x;
			gtk_widget_style_get(buttonHandle, "child-displacement-x",
					&child_displacement_x, NULL);
			int displace_focus;
			gtk_widget_style_get(buttonHandle, "displace-focus",
					&displace_focus, NULL);
			int interior_focus;
			gtk_widget_style_get(buttonHandle, "interior-focus",
					&interior_focus, NULL);

			if (interior_focus != 0) {
				int xthickness = 0; //gtk_style_get_xthickness(_gtkStyle);
				int ythickness = 0; // gtk_style_get_ythickness(_gtkStyle);
				x += xthickness + focus_padding;
				y += ythickness + focus_padding;
				width -= 2 * (xthickness + focus_padding);
				height -= 2 * (ythickness + focus_padding);
			} else {
				x -= focus_line_width + focus_padding;
				y -= focus_line_width + focus_padding;
				width += 2 * (focus_line_width + focus_padding);
				height += 2 * (focus_line_width + focus_padding);
			}

			if ((data->state & W_THEME_STATE_PRESSED) != 0
					&& displace_focus != 0) {
				x += child_displacement_x;
				y += child_displacement_y;
			}

			gtk_render_focus(gtkStyle, drawable, x, y, width, height);
		}
		if (data->clientArea != 0) {
			data->clientArea->x = bounds->x + border_width;
			data->clientArea->y = bounds->y + border_width;
			data->clientArea->width = bounds->width - 2 * border_width;
			data->clientArea->height = bounds->height - 2 * border_width;
		}
		return;
	}
}
void _gtk_theme_combo_draw_background(w_theme *theme, w_themedata *data,
		w_graphics *gc, w_rect *bounds, wuint clazz) {
	_gtk_theme *gtktheme = (_gtk_theme*) theme;

}
void _gtk_theme_progressbar_part_id(w_themedata *data, int *ids, int part) {
}

void _gtk_theme_progressbar_draw_background(w_theme *theme, w_themedata *data,
		w_graphics *gc, w_rect *bounds, wuint clazz) {
	_gtk_theme *gtktheme = (_gtk_theme*) theme;
	GtkWidget *progressHandle = gtktheme->handle[GTK_THEME_HANDLE_PROGRESS];
	GtkStyleContext *context = gtk_widget_get_style_context(progressHandle);
	_gtk_theme_transfer_clipping(gc, context);
	int x = bounds->x, y = bounds->y, width = bounds->width, height =
			bounds->height;
	/*draw_check(context, _W_GRAPHICS(gc)->cairo, x, y, GTK_STATE_NORMAL, &width,
	 &height);*/
	gtk_style_context_save(context);
	gtk_style_context_add_class(context, GTK_STYLE_CLASS_TROUGH);
	_gtk_render_box(context, _W_GRAPHICS(gc)->cairo, x, y, width, height);
	gtk_style_context_restore(context);
	w_point thickness;
	_w_control_get_thickness(progressHandle, &thickness);
	if ((data->style & W_VERTICAL) != 0) {
		/*gtk_orientable_set_orientation(GTK_ORIENTABLE(progressHandle),
		 GTK_ORIENTATION_VERTICAL);
		 gtk_progress_bar_set_inverted(GTK_PROGRESS_BAR(progressHandle), TRUE);*/
		x += thickness.x;
		width -= thickness.x * 2;
		height -= thickness.y * 2;
		height *= data->range.selection
				/ (float) WMAX(1, (data->range.maximum - data->range.minimum));
		y += bounds->height - thickness.y - height;
	} else {
		/*gtk_orientable_set_orientation(GTK_ORIENTABLE(progressHandle),
		 GTK_ORIENTATION_HORIZONTAL);
		 gtk_progress_bar_set_inverted(GTK_PROGRESS_BAR(progressHandle), FALSE);*/
		x += thickness.x;
		y += thickness.y;
		width -= thickness.x * 2;
		height -= thickness.y * 2;
		width *= data->range.selection
				/ (float) WMAX(1, data->range.maximum - data->range.minimum);
	}
	gtk_style_context_save(context);
	gtk_style_context_add_class(context, GTK_STYLE_CLASS_PROGRESSBAR);
	_gtk_render_box(context, _W_GRAPHICS(gc)->cairo, x, y, width, height);
	gtk_style_context_restore(context);
}
void _gtk_theme_tabfolder_part_id(w_themedata *data, int *ids, int part) {
}
void _gtk_theme_tabfolder_draw_background(w_theme *theme, w_themedata *data,
		w_graphics *gc, w_rect *bounds, wuint clazz) {
	_gtk_theme *gtktheme = (_gtk_theme*) theme;
	GtkWidget *notebookHandle = gtktheme->handle[GTK_THEME_HANDLE_NOTEBOOK];
	GtkStyleContext *context = gtk_widget_get_style_context(notebookHandle);
	_gtk_theme_transfer_clipping(gc, context);
	int x = bounds->x, y = bounds->y, width = bounds->width, height =
			bounds->height;
	/*height -= data->tab.tabsHeight;
	 int gap_x = data->tab.selectedX, gap_width = data->tab.selectedWidth;
	 GtkPositionType gap_side = GTK_POS_TOP;
	 if ((data->style & W_BOTTOM) != 0) {
	 gap_side = GTK_POS_BOTTOM;
	 } else {
	 y += data->tab.tabsHeight;
	 }
	 gtk_render_frame_gap(context, _W_GRAPHICS(gc)->cairo, x, y, width, height,
	 gap_side, gap_x, gap_width);
	 if (tabsArea != null) {
	 tabsArea.x = bounds.x;
	 tabsArea.y = bounds.y;
	 tabsArea.width = bounds.width;
	 tabsArea.height = tabsHeight;
	 if ((style & SWT.BOTTOM) != 0) {
	 tabsArea.y += bounds.height - tabsHeight;
	 }
	 }*/
}
void _gtk_theme_tabitem_part_id(w_themedata *data, int *ids, int part) {
}
#define TAB_CURVATURE 1
void _gtk_theme_tabitem_draw_background(w_theme *theme, w_themedata *data,
		w_graphics *gc, w_rect *bounds, wuint clazz) {
	_gtk_theme *gtktheme = (_gtk_theme*) theme;
	GtkWidget *notebookHandle = gtktheme->handle[GTK_THEME_HANDLE_NOTEBOOK];
	GtkStyleContext *context = gtk_widget_get_style_context(notebookHandle);
	_gtk_theme_transfer_clipping(gc, context);
	int x = bounds->x, y = bounds->y, width = bounds->width, height =
			bounds->height;
	GtkStateFlags state_type = GTK_STATE_FLAG_NORMAL;
	if ((data->state & W_THEME_STATE_SELECTED) == 0) {
		if ((data->style & W_BOTTOM) == 0) {
			y += TAB_CURVATURE;
		}
		height -= TAB_CURVATURE;
	}
	if (data->state & W_THEME_STATE_SELECTED) {
		state_type |= GTK_STATE_FLAG_SELECTED;
	}
	if (data->state & W_THEME_STATE_FOCUSED) {
		state_type |= GTK_STATE_FLAG_FOCUSED;
	}
	if (data->state & W_THEME_STATE_HOT) {
		state_type |= GTK_STATE_FLAG_PRELIGHT;
	}
	if (data->state & W_THEME_STATE_PRESSED) {
		//state_type |= GTK_STATE_FLAG_PRELIGHT;
	}
	if (data->state & W_THEME_STATE_ACTIVE) {
		state_type |= GTK_STATE_FLAG_ACTIVE;
	}
	if (data->state & W_THEME_STATE_DISABLED) {
		state_type |= GTK_STATE_FLAG_INSENSITIVE;
	}
	if (data->state & W_THEME_STATE_DEFAULTED) {
		state_type |= GTK_STATE_FLAG_ACTIVE;
	}
	if (data->state & W_THEME_STATE_GRAYED) {
		state_type |= GTK_STATE_FLAG_INCONSISTENT;
	}
	gtk_style_context_set_state(context, state_type);
	GtkPositionType gap_side = GTK_POS_BOTTOM;
	if ((data->style & W_BOTTOM) != 0) {
		gap_side = GTK_POS_TOP;
	}
	gtk_render_extension(context, _W_GRAPHICS(gc)->cairo, x, y, width, height,
			gap_side);
	if (data->clientArea != 0) {
		/*		int hborder = gtk_widget_style_get(notebookHandle, "tab-hborder");
		 int vborder = gtk_widget_style_get(notebookHandle, "tab-vborder");
		 int focus_line_width = gtk_widget_style_get(notebookHandle,
		 "focus-line-width");
		 int xthickness = OS.gtk_style_get_xthickness(gtkStyle);
		 int ythickness = OS.gtk_style_get_ythickness(gtkStyle);
		 int borderX = xthickness + TAB_CURVATURE + focus_line_width + hborder;
		 int borderY = ythickness + TAB_CURVATURE + focus_line_width + vborder;
		 clientArea.x = bounds.x + borderX;
		 clientArea.y = bounds.y + borderY;
		 clientArea.width = bounds.width - 2 * borderX;
		 clientArea.height = bounds.height - 2 * borderY;*/
	}
}
void _gtk_theme_draw_focus(w_theme *theme, w_themedata *data, w_graphics *gc,
		w_rect *bounds) {
}
void _gtk_theme_draw_image(w_theme *theme, w_themedata *data, w_graphics *gc,
		w_rect *bounds, w_image *image, int flags) {
	if (!w_image_is_ok(image))
		return;
	w_rect rect;
	int ids[3];
	w_image_get_size(image, &rect.sz);
	wuint id = data->clazz;
	_gtk_theme_part_id part_id = 0; // __gtk_theme_clazz_info[id].partId;
	//part_id(theme, data, ids, W_THEME_PART_WHOLE);
	GtkStateType state_type = ids[0];
	if (state_type == GTK_STATE_NORMAL) {
		rect.x = 0;
		rect.y = 0;
		w_graphics_draw_image(gc, image, &rect, bounds, 0);
	} else {
		GdkPixbuf *pixbuf = _W_IMAGE(image)->pixbuf;
		GtkIconSource *source = gtk_icon_source_new();
		if (source != 0) {
			gtk_icon_source_set_pixbuf(source, pixbuf);
			_gtk_theme *gtktheme = (_gtk_theme*) theme;
			GtkWidget *buttonHandle = gtktheme->handle[GTK_THEME_HANDLE_BUTTON];
			GtkStyle *gtkStyle = gtk_widget_get_style(buttonHandle);
			//_w_theme_transfer_clipping(gc,gtkStyle);
			GdkPixbuf *rendered = gtk_style_render_icon(gtkStyle, source,
					GTK_TEXT_DIR_NONE, state_type, GTK_ICON_SIZE_INVALID,
					buttonHandle, NULL);
			g_object_unref(pixbuf);
			if (rendered != 0) {
#if USE_CAIRO
				cairo_t *cairo = _W_GRAPHICS(gc)->cairo;
				gdk_cairo_set_source_pixbuf(cairo, rendered, 0, 0);
				cairo_rectangle(cairo, bounds->x, bounds->y, bounds->width,
						bounds->height);
				cairo_fill(cairo);
				cairo_destroy(cairo);
#else
	#endif
				g_object_unref(rendered);
			}
			gtk_icon_source_free(source);
		}
	}
}
void _gtk_theme_draw_text(w_theme *theme, w_themedata *data, w_graphics *gc,
		w_rect *bounds, const char *text, int length, int flags) {
	if (!(flags
			& (W_THEME_DRAW_BOTTOM | W_THEME_DRAW_HCENTER | W_THEME_DRAW_LEFT
					| W_THEME_DRAW_RIGHT | W_THEME_DRAW_TOP
					| W_THEME_DRAW_VCENTER))) {
		flags |= W_THEME_DRAW_HCENTER | W_THEME_DRAW_VCENTER;
	}
	_gtk_theme *gtktheme = (_gtk_theme*) theme;
	GtkWidget *widget = gtktheme->handle[GTK_THEME_HANDLE_LABEL];
	GtkStyleContext *gtkStyle = gtk_widget_get_style_context(widget);
	cairo_t *drawable = _W_GRAPHICS(gc)->cairo;
	_gtk_theme_transfer_clipping(gc, gtkStyle);
	PangoLayout *layout;
	int newlength, mnemonic;
	char *t = _gtk_text_fix(text, length, flags, &newlength, &mnemonic);
	layout = gtk_widget_create_pango_layout(widget, t);
	int width, height;
	pango_layout_get_pixel_size(layout, &width, &height);
	pango_layout_set_width(layout, bounds->width * PANGO_SCALE);
	int x = bounds->x;
	int y = bounds->y;
	if ((flags & W_THEME_DRAW_LEFT) != 0) {
		pango_layout_set_alignment(layout, PANGO_ALIGN_LEFT);
	}
	if ((flags & W_THEME_DRAW_HCENTER) != 0) {
		pango_layout_set_alignment(layout, PANGO_ALIGN_CENTER);
	}
	if ((flags & W_THEME_DRAW_RIGHT) != 0) {
		pango_layout_set_alignment(layout, PANGO_ALIGN_RIGHT);
	}
	if ((flags & W_THEME_DRAW_VCENTER) != 0) {
		y += (bounds->height - height) / 2;
	}
	if ((flags & W_THEME_DRAW_BOTTOM) != 0) {
		y += bounds->height - height;
	}
	/*int ids[2];
	 ((_w_theme_part_id) data->clazz->reserved[0])(data, ids,
	 W_THEME_WIDGET_WHOLE);
	 int state_type = ids[0];*/
	gtk_render_layout(gtkStyle, drawable, x, y, layout);
	g_object_unref(layout);
	_gtk_text_free(text, t, newlength);
}
int _gtk_theme_get_selection(w_theme *theme, w_themedata *data, w_point *offset,
		w_rect *bounds) {
	return 0;
}
int _gtk_theme_hit_background(w_theme *theme, w_themedata *data,
		w_point *position, w_rect *bounds) {
	return 0;
}
void _gtk_theme_measure_text(w_theme *theme, w_themedata *data, w_graphics *gc,
		w_rect *bounds, w_rect *result, const char *text, size_t length,
		int flags) {
	_gtk_theme *gtktheme = (_gtk_theme*) theme;
	GtkWidget *widget = gtktheme->handle[GTK_THEME_HANDLE_LABEL];
	PangoLayout *layout;
	int newlength, mnemonic;
	char *t = _gtk_text_fix(text, length, flags, &newlength, &mnemonic);
	layout = gtk_widget_create_pango_layout(widget, t);
	if (bounds != 0)
		pango_layout_set_width(layout, bounds->width);
	if ((flags & W_THEME_DRAW_LEFT) != 0) {
		pango_layout_set_alignment(layout, PANGO_ALIGN_LEFT);
	}
	if ((flags & W_THEME_DRAW_HCENTER) != 0) {
		pango_layout_set_alignment(layout, PANGO_ALIGN_CENTER);
	}
	if ((flags & W_THEME_DRAW_RIGHT) != 0) {
		pango_layout_set_alignment(layout, PANGO_ALIGN_RIGHT);
	}
	int width, height;
	pango_layout_get_pixel_size(layout, &width, &height);
	g_object_unref(layout);
	result->x = 0;
	result->y = 0;
	result->width = width;
	result->height = height;
	_gtk_text_free(text, t, newlength);
}
/* button */
void _gtk_themedata_button_compute_trim(w_theme *theme, int mask,
		w_themedata *data, w_rect *result) {
}
void _gtk_themedata_button_draw(w_theme *theme, int mask, w_themedata *data) {
}
void _gtk_themedata_button_measure(w_theme *theme, int mask, w_themedata *data,
		w_size *result) {
}
void _gtk_themedata_button_get_bounds(w_theme *theme, int mask, int part,
		w_themedata *data, w_rect *result) {
}
int _gtk_themedata_button_hit(w_theme *theme, int mask, w_themedata *data,
		w_point *position) {
}
/* expandbar */
void _gtk_themedata_expandbar_compute_trim(w_theme *theme, int mask,
		w_themedata *data, w_rect *result) {
}
void _gtk_themedata_expandbar_draw(w_theme *theme, int mask,
		w_themedata *data) {
}
void _gtk_themedata_expandbar_measure(w_theme *theme, int mask,
		w_themedata *data, w_size *result) {
}
void _gtk_themedata_expandbar_get_bounds(w_theme *theme, int mask, int part,
		w_themedata *data, w_rect *result) {
}
int _gtk_themedata_expandbar_hit(w_theme *theme, int mask, w_themedata *data,
		w_point *position) {
}
/* expanditem */
void _gtk_themedata_expanditem_compute_trim(w_theme *theme, int mask,
		w_themedata *data, w_rect *result) {
}
void _gtk_themedata_expanditem_draw(w_theme *theme, int mask,
		w_themedata *data) {
}
void _gtk_themedata_expanditem_measure(w_theme *theme, int mask,
		w_themedata *data, w_size *result) {
}
void _gtk_themedata_expanditem_get_bounds(w_theme *theme, int mask, int part,
		w_themedata *data, w_rect *result) {
}
int _gtk_themedata_expanditem_hit(w_theme *theme, int mask, w_themedata *data,
		w_point *position) {
}
_w_themedata_class _gtk_themedata_fun[W_THEME_CLASS_LAST] = {
		[W_THEME_CLASS_UNKNOWN] = { }, //
		[W_THEME_CLASS_MENU] = { }, //
		[W_THEME_CLASS_MENUITEM] = { }, //
		[W_THEME_CLASS_BUTTON] = { _gtk_themedata_button_compute_trim,
				_gtk_themedata_button_draw, _gtk_themedata_button_measure,
				_gtk_themedata_button_get_bounds, _gtk_themedata_button_hit }, //
		[W_THEME_CLASS_LABEL] = { }, //
		[W_THEME_CLASS_PROGRESSBAR] = { }, //
		[W_THEME_CLASS_SASH] = { }, //
		[W_THEME_CLASS_SLIDER] = { }, //
		[W_THEME_CLASS_SCROLLBAR] = { }, //
		[W_THEME_CLASS_TEXTEDIT] = { }, //
		[W_THEME_CLASS_COMPOSITE] = { }, //
		[W_THEME_CLASS_WEBVIEW] = { }, //
		[W_THEME_CLASS_TREEVIEW] = { }, //
		[W_THEME_CLASS_TREEITEM] = { }, //
		[W_THEME_CLASS_COLUMNITEM] = { }, //
		[W_THEME_CLASS_TABVIEW] = { }, //
		[W_THEME_CLASS_TABITEM] = { }, //
		[W_THEME_CLASS_COMBOBOX] = { }, //
		[W_THEME_CLASS_COOLBAR] = { }, //
		[W_THEME_CLASS_COOLITEM] = { }, //
		[W_THEME_CLASS_DATETIME] = { }, //
		[W_THEME_CLASS_EXPANDBAR] = { _gtk_themedata_expandbar_compute_trim,
				_gtk_themedata_expandbar_draw, _gtk_themedata_expandbar_measure,
				_gtk_themedata_expandbar_get_bounds,
				_gtk_themedata_expandbar_hit }, //
		[W_THEME_CLASS_EXPANDITEM] = { _gtk_themedata_expanditem_compute_trim,
				_gtk_themedata_expanditem_draw,
				_gtk_themedata_expanditem_measure,
				_gtk_themedata_expanditem_get_bounds,
				_gtk_themedata_expanditem_hit }, //
		[W_THEME_CLASS_GROUPBOX] = { }, //
		[W_THEME_CLASS_SPINNER] = { }, //
		[W_THEME_CLASS_LISTVIEW] = { }, //
		[W_THEME_CLASS_LISTITEM] = { }, //
		[W_THEME_CLASS_TOOLBAR] = { }, //
		[W_THEME_CLASS_TOOLITEM] = { }, //
		};
_gtk_theme_part_id _gtk_themedata_part_fun[W_THEME_CLASS_LAST] = {
		[W_THEME_CLASS_UNKNOWN] = 0, //
		[W_THEME_CLASS_MENU] = 0, //
		[W_THEME_CLASS_MENUITEM] = 0, //
		[W_THEME_CLASS_BUTTON] = 0, //
		[W_THEME_CLASS_LABEL] = 0, //
		[W_THEME_CLASS_PROGRESSBAR] = 0, //
		[W_THEME_CLASS_SASH] = 0, //
		[W_THEME_CLASS_SLIDER] = 0, //
		[W_THEME_CLASS_SCROLLBAR] = 0, //
		[W_THEME_CLASS_TEXTEDIT] = 0, //
		[W_THEME_CLASS_COMPOSITE] = 0, //
		[W_THEME_CLASS_WEBVIEW] = 0, //
		[W_THEME_CLASS_TREEVIEW] = 0, //
		[W_THEME_CLASS_TREEITEM] = 0, //
		[W_THEME_CLASS_COLUMNITEM] = 0, //
		[W_THEME_CLASS_TABVIEW] = 0, //
		[W_THEME_CLASS_TABITEM] = 0, //
		[W_THEME_CLASS_COMBOBOX] = 0, //
		[W_THEME_CLASS_COOLBAR] = 0, //
		[W_THEME_CLASS_COOLITEM] = 0, //
		[W_THEME_CLASS_DATETIME] = 0, //
		[W_THEME_CLASS_EXPANDBAR] = 0, //
		[W_THEME_CLASS_EXPANDITEM] = 0, //
		[W_THEME_CLASS_GROUPBOX] = 0, //
		[W_THEME_CLASS_SPINNER] = 0, //
		[W_THEME_CLASS_LISTVIEW] = 0, //
		[W_THEME_CLASS_LISTITEM] = 0, //
		[W_THEME_CLASS_TOOLBAR] = 0, //
		[W_THEME_CLASS_TOOLITEM] = 0, //
		};
#if defined(__GNUC__) || defined(__GNUG__)
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
#if GTK3
const char *gtk_tooltip_name = "gtk-tooltip";
#endif
#if GTK2
	const char* gtk_tooltip_name = "gtk-tooltips";
#endif
void _gtk_theme_init_widget(_gtk_theme *theme) {
	GtkWidget **handles = theme->handle;
	handles[GTK_THEME_HANDLE_SHELL] = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	handles[GTK_THEME_HANDLE_TOOLTIP] = gtk_window_new(GTK_WINDOW_POPUP);
	gtk_widget_set_name(theme->handle[GTK_THEME_HANDLE_TOOLTIP],
			gtk_tooltip_name);
	handles[GTK_THEME_HANDLE_FIXED] = gtk_fixed_new();
	handles[GTK_THEME_HANDLE_BUTTON] = gtk_button_new();
#if GTK3
	handles[GTK_THEME_HANDLE_ARROW] = gtk_arrow_new(GTK_ARROW_DOWN,
			GTK_SHADOW_NONE);
#endif
	handles[GTK_THEME_HANDLE_CHECK_BUTTON] = gtk_check_button_new();
	handles[GTK_THEME_HANDLE_FRAME] = gtk_check_button_new();
	handles[GTK_THEME_HANDLE_ENTRY] = gtk_entry_new();
	handles[GTK_THEME_HANDLE_RADIO_BUTTON] = gtk_radio_button_new(0);
	handles[GTK_THEME_HANDLE_NOTEBOOK] = gtk_notebook_new();
	handles[GTK_THEME_HANDLE_PROGRESS] = gtk_progress_bar_new();
	handles[GTK_THEME_HANDLE_TOOLBAR] = gtk_toolbar_new();
	handles[GTK_THEME_HANDLE_TREE] = gtk_tree_view_new_with_model(0);
#if GTK3
	handles[GTK_THEME_HANDLE_SEPARATOR] = gtk_separator_new(
			GTK_ORIENTATION_VERTICAL);
#endif
	handles[GTK_THEME_HANDLE_LABEL] = gtk_label_new(NULL);
	/*for (int i = 0; i < GTK_THEME_HANDLE_LAST; i++) {
		if(handles[i] != 0){
			g_object_ref(handles[i]);
		}
	}*/
	GtkContainer *fixed = GTK_CONTAINER(handles[GTK_THEME_HANDLE_FIXED]);
	for (int i = 0; i < GTK_THEME_HANDLE_LAST; i++) {
		g_object_ref(G_OBJECT(handles[i]));
		g_object_ref_sink(G_OBJECT(handles[i]));
		//gtk_container_add(fixed, handles[i]);
		//gtk_widget_realize(handles[i]);
	}
}
void _gtk_theme_dispose(w_theme *theme) {
	_gtk_theme *_theme = (_gtk_theme*) theme;
	GtkWidget **handles = _theme->handle;
	GtkContainer *fixed = GTK_CONTAINER(handles[GTK_THEME_HANDLE_FIXED]);
	for (int i = GTK_THEME_HANDLE_LAST - 1; i >= 0; i--) {
		if (handles[i] != 0) {
			gtk_widget_destroy(handles[i]);
			g_object_unref(handles[i]);
		}
	}
}
const char* _gtk_theme_get_name(w_theme *theme) {
	return "gtk";
}
void _gtk_theme_compute_trim(w_theme *theme, int mask, w_themedata *data,
		w_rect *result) {
}
void _gtk_theme_draw(w_theme *theme, int mask, w_themedata *data) {
	if (data->clazz < W_THEME_CLASS_LAST
			&& _gtk_themedata_fun[data->clazz].draw != 0) {
		_gtk_themedata_fun[data->clazz].draw(theme, mask, data);
	}
}
void _gtk_theme_measure(w_theme *theme, int mask, w_themedata *data,
		w_size *result) {
	if (data->clazz < W_THEME_CLASS_LAST
			&& _gtk_themedata_fun[data->clazz].measure != 0) {
		_gtk_themedata_fun[data->clazz].measure(theme, mask, data, result);
	}
}
void _gtk_theme_get_bounds(w_theme *theme, int mask, int part,
		w_themedata *data, w_rect *result) {
	if (data->clazz < W_THEME_CLASS_LAST
			&& _gtk_themedata_fun[data->clazz].get_bounds) {
		_gtk_themedata_fun[data->clazz].get_bounds(theme, mask, part, data,
				result);
	}
}
int _gtk_theme_hit(w_theme *theme, int mask, w_themedata *data,
		w_point *position) {
	if (data->clazz < W_THEME_CLASS_LAST
			&& _gtk_themedata_fun[data->clazz].hit != 0) {
		return _gtk_themedata_fun[data->clazz].hit(theme, mask, data, position);
	}
	return W_THEME_PART_NOWHERE;
}
wresult _gtk_theme_get_color(w_theme *theme, wuint colorid, w_color *color) {
	*color = 0x00000000;
	return W_FALSE;
}
wresult _gtk_theme_get_font(w_theme *theme, w_font **font) {
	_gtk_theme *t = (_gtk_theme*) theme;
	w_toolkit *toolkit = w_app_get_platform_toolkit(w_app_get());
	*font = w_toolkit_get_system_font(toolkit);
	return W_TRUE;
}
wresult _gtk_theme_get_cursor(w_theme *theme, wuint id, w_cursor **cursor) {
	_gtk_theme *t = (_gtk_theme*) theme;
	if (id <= W_CURSOR_HAND) {
		//*cursor = (w_cursor*) &t->cursors[id];
		return W_TRUE;
	} else {
		*cursor = 0;
		return W_FALSE;
	}
}
wresult _gtk_theme_get_image(w_theme *theme, wuint id, w_image **image) {
	_gtk_theme *t = (_gtk_theme*) theme;
	*image = 0;
	return W_FALSE;
}
_w_theme_class _gtk_theme_clazz = { //
		_gtk_theme_dispose, //
				_gtk_theme_get_name, //
				{ _gtk_theme_compute_trim, //
						_gtk_theme_draw, //
						_gtk_theme_measure, //
						_gtk_theme_get_bounds, //
						_gtk_theme_hit }, //
				_gtk_theme_get_color, //
				_gtk_theme_get_font, //
				_gtk_theme_get_cursor, //
				_gtk_theme_get_image };
void _w_theme_init() {
	_gtk_theme_init_widget(&gtk_toolkit->gtktheme);
	gtk_toolkit->gtktheme.theme.clazz = &_gtk_theme_clazz;
	gtk_toolkit->theme = (w_theme*) &gtk_toolkit->gtktheme;
}

