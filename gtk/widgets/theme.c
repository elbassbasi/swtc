/*
 * Name:        theme.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "toolkit.h"
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
	for (int i = GTK_THEME_HANDLE_BUTTON; i < GTK_THEME_HANDLE_LAST; i++) {
		gtk_container_add(GTK_CONTAINER(handles[GTK_THEME_HANDLE_FIXED]),
				handles[i]);
		gtk_widget_realize(handles[i]);
	}
}
typedef void (*_gtk_theme_part_id)(w_theme *theme, w_themedata *data, int *ids,
		int part);
typedef void (*_gtk_theme_draw_background)(w_theme *theme, w_themedata *data,
		w_graphics *gc, w_rect *bounds, wuint clazz);
typedef struct _gtk_theme_clazz_info {
	_gtk_theme_part_id partId;
	_gtk_theme_draw_background draw_background;
} _gtk_theme_clazz_info;
void _gtk_theme_transfer_clipping(w_graphics *gc, GtkStyleContext *style) {
}
void _gtk_render_box(GtkStyleContext *style, cairo_t *window, int x, int y,
		int width, int height) {
	gtk_render_frame(style, window, x, y, width, height);
	gtk_render_background(style, window, x, y, width, height);
}
void _gtk_themedata_part_id(w_theme *theme, w_themedata *data, int *ids,
		int part) {
	int state = data->state; // this.state[part];
	int state_type = GTK_STATE_NORMAL;
	if ((state & W_THEME_DISABLED) != 0) {
		state_type = GTK_STATE_INSENSITIVE;
	} else {
		if ((state & W_THEME_SELECTED) != 0)
			state_type = GTK_STATE_ACTIVE;
		if ((state & W_THEME_HOT) != 0) {
			if ((state & W_THEME_PRESSED) != 0) {
				state_type = GTK_STATE_ACTIVE;
			} else {
				state_type = GTK_STATE_PRELIGHT;
			}
		}
	}
	ids[0] = state_type;
}
void _gtk_theme_button_draw_background(w_theme *theme, w_themedata *data,
		w_graphics *gc, w_rect *bounds, wuint clazz) {
	_gtk_theme *gtktheme = (_gtk_theme*) theme;
	int ids[3];
	_gtk_themedata_part_id(theme, data, ids, W_THEME_WIDGET_WHOLE);
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
		int indicator_size;
		gtk_widget_style_get(buttonHandle, "indicator-size", &indicator_size,
		NULL);
		int indicator_spacing;
		gtk_widget_style_get(buttonHandle, "indicator-spacing",
				&indicator_spacing, NULL);
		int interior_focus;
		gtk_widget_style_get(buttonHandle, "interior-focus", &interior_focus,
		NULL);
		int focus_line_width;
		gtk_widget_style_get(buttonHandle, "focus-line-width",
				&focus_line_width, NULL);
		int focus_padding;
		gtk_widget_style_get(buttonHandle, "focus-padding", &focus_padding,
		NULL);
		int border_width = gtk_container_get_border_width(
				GTK_CONTAINER(buttonHandle));

		int x = bounds->x + indicator_spacing + border_width;
		int y = bounds->y + (bounds->height - indicator_size) / 2;

		if (interior_focus == 0) {
			x += focus_line_width + focus_padding;
		}

		int shadow_type;
		if ((data->state & W_THEME_GRAYED) != 0) {
			shadow_type = GTK_SHADOW_ETCHED_IN;
		} else if ((data->state & W_THEME_SELECTED) != 0) {
			shadow_type = GTK_SHADOW_IN;
		} else {
			shadow_type = GTK_SHADOW_OUT;
		}

		const char *detail =
				(data->style & W_RADIO) != 0 ? "radiobutton" : "checkbutton";
		if ((state & W_THEME_HOT) != 0) {
			int prelight_x, prelight_y, prelight_width, prelight_height;
			prelight_x = bounds->x + border_width;
			prelight_y = bounds->y + border_width;
			prelight_width = bounds->width - (2 * border_width);
			prelight_height = bounds->height - (2 * border_width);
			gtk_render_frame(gtkStyle, drawable, prelight_x, prelight_y,
					prelight_width, prelight_height);
		}
		if ((data->style & W_RADIO) != 0) {
			gtk_render_option(gtkStyle, drawable, x, y, indicator_size,
					indicator_size);
		} else {
			gtk_render_check(gtkStyle, drawable, x, y, indicator_size,
					indicator_size);
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
				(data->state & W_THEME_DEFAULTED) != 0 ?
						"buttondefault" : "button";
		if ((data->state & W_THEME_DEFAULTED) != 0
				&& relief == GTK_RELIEF_NORMAL) {
			gtk_style_context_set_state(gtkStyle, state);
			_gtk_render_box(gtkStyle, drawable, x, y, width, height);
			x += default_border.left;
			y += default_border.top;
			width -= default_border.left + default_border.right;
			height -= default_border.top + default_border.bottom;
		} else if ((data->state & W_THEME_DEFAULTED) != 0) {
			x += default_outside_border.left;
			y += default_outside_border.top;
			width -= default_outside_border.left + default_outside_border.right;
			height -= default_outside_border.top
					+ default_outside_border.bottom;
		}

		int shadow_type = GTK_SHADOW_OUT;
		if ((data->state & (W_THEME_SELECTED | W_THEME_PRESSED)) != 0)
			shadow_type = GTK_SHADOW_IN;
		if (relief != GTK_RELIEF_NONE
				|| ((data->state & (W_THEME_PRESSED | W_THEME_HOT)) != 0)) {
			_gtk_render_box(gtkStyle, drawable, x, y, width, height);
		}

		if ((data->state & W_THEME_FOCUSED) != 0) {
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

			if ((data->state & W_THEME_PRESSED) != 0 && displace_focus != 0) {
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
	height -= data->tab.tabsHeight;
	int gap_x = data->tab.selectedX, gap_width = data->tab.selectedWidth;
	GtkPositionType gap_side = GTK_POS_TOP;
	if ((data->style & W_BOTTOM) != 0) {
		gap_side = GTK_POS_BOTTOM;
	} else {
		y += data->tab.tabsHeight;
	}
	gtk_render_frame_gap(context, _W_GRAPHICS(gc)->cairo, x, y, width, height,
			gap_side, gap_x, gap_width);
	/*	if (tabsArea != null) {
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
	if ((data->state & W_THEME_SELECTED) == 0) {
		if ((data->style & W_BOTTOM) == 0) {
			y += TAB_CURVATURE;
		}
		height -= TAB_CURVATURE;
	}
	if (data->state & W_THEME_SELECTED) {
		state_type |= GTK_STATE_FLAG_SELECTED;
	}
	if (data->state & W_THEME_FOCUSED) {
		state_type |= GTK_STATE_FLAG_FOCUSED;
	}
	if (data->state & W_THEME_HOT) {
		state_type |= GTK_STATE_FLAG_PRELIGHT;
	}
	if (data->state & W_THEME_PRESSED) {
		//state_type |= GTK_STATE_FLAG_PRELIGHT;
	}
	if (data->state & W_THEME_ACTIVE) {
		state_type |= GTK_STATE_FLAG_ACTIVE;
	}
	if (data->state & W_THEME_DISABLED) {
		state_type |= GTK_STATE_FLAG_INSENSITIVE;
	}
	if (data->state & W_THEME_DEFAULTED) {
		state_type |= GTK_STATE_FLAG_ACTIVE;
	}
	if (data->state & W_THEME_GRAYED) {
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
_gtk_theme_clazz_info __gtk_theme_clazz_info[] = { { }, //_W_THEME_CLASS_UNKNOWN =0,
		{ _gtk_themedata_part_id, }, //_W_THEME_CLASS_MENU,
		{ _gtk_themedata_part_id, _gtk_theme_button_draw_background }, //_W_THEME_CLASS_BUTTON,
		{ _gtk_themedata_part_id, }, //_W_THEME_CLASS_LABEL,
		{ _gtk_themedata_part_id, }, //_W_THEME_CLASS_LINK,
		{ _gtk_themedata_part_id, _gtk_theme_progressbar_draw_background }, //_W_THEME_CLASS_PROGRESSBAR,
		{ _gtk_themedata_part_id, }, //_W_THEME_CLASS_SASH,
		{ _gtk_themedata_part_id, }, //_W_THEME_CLASS_SCALE,
		{ _gtk_themedata_part_id, }, //_W_THEME_CLASS_SLIDER,
		{ _gtk_themedata_part_id, }, // _W_THEME_CLASS_SCROLLBAR,
		{ _gtk_themedata_part_id, }, //_W_THEME_CLASS_TEXT,
		{ _gtk_themedata_part_id, }, //_W_THEME_CLASS_LIST,
		{ _gtk_themedata_part_id, }, //_W_THEME_CLASS_COMPOSITE,
		{ _gtk_themedata_part_id, }, //_W_THEME_CLASS_BROWSER,
		{ _gtk_themedata_part_id, }, //_W_THEME_CLASS_TREE,
		{ _gtk_themedata_part_id, _gtk_theme_tabfolder_draw_background }, //_W_THEME_CLASS_TABFOLDER,
		{ _gtk_themedata_part_id, _gtk_theme_tabitem_draw_background }, //_W_THEME_CLASS_TABITEM,
		{ _gtk_themedata_part_id, _gtk_theme_combo_draw_background }, //_W_THEME_CLASS_COMBO,
		{ _gtk_themedata_part_id, }, //_W_THEME_CLASS_COOLBAR,
		{ _gtk_themedata_part_id, }, //_W_THEME_CLASS_DATETIME,
		{ _gtk_themedata_part_id, }, // _W_THEME_CLASS_EXPANDBAR,
		{ _gtk_themedata_part_id, }, // _W_THEME_CLASS_GROUP,
		{ _gtk_themedata_part_id, }, // _W_THEME_CLASS_SPINNER,
		{ _gtk_themedata_part_id, }, // _W_THEME_CLASS_TABLE,
		{ _gtk_themedata_part_id, }, // _W_THEME_CLASS_TOOLBAR,
		{ _gtk_themedata_part_id, }, // _W_THEME_CLASS_TOOLITEM,
		};
void _gtk_theme_dispose(w_theme *theme) {
}
const char* _gtk_theme_get_name(w_theme *theme) {
	return "gtk";
}
void _gtk_theme_compute_trim(w_theme *theme, w_themedata *data, w_graphics *gc,
		w_rect *result) {
}
void _gtk_theme_draw_background_0(w_theme *theme, w_themedata *data,
		w_graphics *gc, w_rect *bounds) {
	wuint id = w_theme_internal_class_id(data->clazz);
	_gtk_theme_draw_background draw_background =
			__gtk_theme_clazz_info[id].draw_background;
	if (draw_background != 0) {
		draw_background(theme, data, gc, bounds, id);
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
	wuint id = w_theme_internal_class_id(data->clazz);
	_gtk_theme_part_id part_id = __gtk_theme_clazz_info[id].partId;
	part_id(theme, data, ids, W_THEME_WIDGET_WHOLE);
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
	char *t = 0;
	if (length < 0) {
		layout = gtk_widget_create_pango_layout(widget, text);
	} else {
		t = _w_toolkit_malloc(length + 1);
		if (t == 0)
			return;
		memcpy(t, text, length);
		t[length] = 0;
		layout = gtk_widget_create_pango_layout(widget, t);
	}
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
	if (t != 0) {
		_w_toolkit_free(t, length + 1);
	}
}
void _gtk_theme_get_bounds(w_theme *theme, w_themedata *data, int part,
		w_rect *bounds, w_rect *result) {
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
	char *t = 0;
	if (length < 0) {
		layout = gtk_widget_create_pango_layout(widget, text);
	} else {
		t = _w_toolkit_malloc(length + 1);
		if (t == 0)
			return;
		memcpy(t, text, length);
		t[length] = 0;
		layout = gtk_widget_create_pango_layout(widget, t);
	}
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
	if (t != 0)
		_w_toolkit_free(W_TOOLKIT(gtk_toolkit), length + 1);
}
w_color _gtk_theme_get_color(w_theme *theme, wuint colorid) {
	return 0;
}
w_font* _gtk_theme_get_font(w_theme *theme) {
	return 0;
}
w_cursor* _gtk_theme_get_cursor(w_theme *theme, wuint id) {
	return 0;
}
w_image* _gtk_theme_get_image(w_theme *theme, wuint id) {
	return 0;
}
_w_theme_class _gtk_theme_clazz = { _gtk_theme_dispose, _gtk_theme_get_name,
		_gtk_theme_compute_trim, _gtk_theme_draw_background_0,
		_gtk_theme_draw_focus, _gtk_theme_draw_image, _gtk_theme_draw_text,
		_gtk_theme_get_bounds, _gtk_theme_get_selection,
		_gtk_theme_hit_background, _gtk_theme_measure_text,
		_gtk_theme_get_color, _gtk_theme_get_font, _gtk_theme_get_cursor,
		_gtk_theme_get_image };
void _w_theme_init() {
	_gtk_theme_init_widget(&gtk_toolkit->gtktheme);
	gtk_toolkit->gtktheme.theme.clazz = &_gtk_theme_clazz;
	gtk_toolkit->theme = (w_theme*) &gtk_toolkit->gtktheme;
}

