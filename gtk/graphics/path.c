/*
 * Name:        path.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "gc.h"
void w_path_init(w_path *path) {
	_W_PATH(path)->handle = 0;
}
wresult w_path_is_ok(w_path *path) {
	return _W_PATH(path)->handle == 0;
}
void w_path_dispose(w_path *path) {
	if (path != 0 && _W_PATH(path)->handle != 0) {
		cairo_destroy(_W_PATH(path)->handle);
		_W_PATH(path)->handle = 0;
	}
}
wresult w_path_create(w_path *path) {
	if (path == 0)
		return W_ERROR_NULL_ARGUMENT;
	w_path_dispose(path);
	cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32,
			1, 1);
	if (surface == 0)
		return W_ERROR_NO_HANDLES;
	_W_PATH(path)->handle = cairo_create(surface);
	cairo_surface_destroy(surface);
	if (_W_PATH(path)->handle == 0)
		return W_ERROR_NO_HANDLES;
	_W_PATH(path)->moved = FALSE;
	_W_PATH(path)->closed = TRUE;
	return W_TRUE;
}
wresult w_path_create_from_path(w_path *path, w_path *frompath,
		float flatness) {
	w_path_dispose(path);
	if (path == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (frompath == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (w_path_is_ok(frompath) <= 0)
		return W_ERROR_INVALID_ARGUMENT;
	cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32,
			1, 1);
	if (surface == 0)
		return W_ERROR_NO_HANDLES;
	_W_PATH(path)->handle = cairo_create(surface);
	cairo_surface_destroy(surface);
	if (_W_PATH(path)->handle == 0)
		return W_ERROR_NO_HANDLES;
	cairo_path_t *copy;
	flatness = WMAX(0, flatness);
	if (flatness == 0) {
		copy = cairo_copy_path(_W_PATH(frompath)->handle);
	} else {
		double tolerance = cairo_get_tolerance(_W_PATH(frompath)->handle);
		cairo_set_tolerance(_W_PATH(frompath)->handle, flatness);
		copy = cairo_copy_path_flat(_W_PATH(frompath)->handle);
		cairo_set_tolerance(_W_PATH(frompath)->handle, tolerance);
	}
	if (copy == 0) {
		cairo_destroy(_W_PATH(path)->handle);
		return W_ERROR_NO_HANDLES;
	}
	cairo_append_path(_W_PATH(path)->handle, copy);
	cairo_path_destroy(copy);
	_W_PATH(path)->moved = FALSE;
	_W_PATH(path)->closed = TRUE;
	return W_TRUE;
}
wresult w_path_create_from_data(w_path *path, size_t length, char *types,
		float *points) {
	w_path_dispose(path);
	if (length == 0)
		return W_ERROR_NULL_ARGUMENT;
	wresult result = w_path_create(path);
	if (result < 0)
		return result;
	for (size_t i = 0, j = 0; i < length; i++) {
		switch (types[i]) {
		case W_PATH_MOVE_TO:
			w_path_move_to(path, points[j++], points[j++]);
			break;
		case W_PATH_LINE_TO:
			w_path_lineto(path, points[j++], points[j++]);
			break;
		case W_PATH_CUBIC_TO:
			w_path_cubicto(path, points[j++], points[j++], points[j++],
					points[j++], points[j++], points[j++]);
			break;
		case W_PATH_QUAD_TO:
			w_path_quadto(path, points[j++], points[j++], points[j++],
					points[j++]);
			break;
		case W_PATH_CLOSE:
			w_path_close(path);
			break;
		default:
			w_path_dispose(path);
			return W_ERROR_INVALID_ARGUMENT;
		}
	}
	return result;
}
wresult w_path_add_arc(w_path *path, w_rectf *rect, float startAngle,
		float arcAngle) {
	if (path == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_W_PATH(path)->handle == 0)
		return W_ERROR_GRAPHIC_DISPOSED;
	if (rect == 0)
		return W_ERROR_NULL_ARGUMENT;
	_W_PATH(path)->moved = TRUE;
	if (rect->width == rect->height) {
		float angle = -startAngle * (float) M_PI / 180;
		if (_W_PATH(path)->closed)
			cairo_move_to(_W_PATH(path)->handle,
					(rect->x + rect->width / 2.0)
							+ rect->width / 2.0 * cos(angle),
					(rect->y + rect->height / 2.0)
							+ rect->height / 2.0 * sin(angle));
		if (arcAngle >= 0) {
			cairo_arc_negative(_W_PATH(path)->handle,
					rect->x + rect->width / 2., rect->y + rect->height / 2.,
					rect->width / 2., angle,
					-(startAngle + arcAngle) * (float) M_PI / 180);
		} else {
			cairo_arc(_W_PATH(path)->handle, rect->x + rect->width / 2.,
					rect->y + rect->height / 2., rect->width / 2., angle,
					-(startAngle + arcAngle) * (float) M_PI / 180);
		}
	} else {
		cairo_save(_W_PATH(path)->handle);
		cairo_translate(_W_PATH(path)->handle, rect->x + rect->width / 2.,
				rect->y + rect->height / 2.);
		cairo_scale(_W_PATH(path)->handle, rect->width / 2., rect->height / 2.);
		float angle = -startAngle * (float) M_PI / 180;
		if (_W_PATH(path)->closed)
			cairo_move_to(_W_PATH(path)->handle, cos(angle), sin(angle));
		if (arcAngle >= 0) {
			cairo_arc_negative(_W_PATH(path)->handle, 0, 0, 1, angle,
					-(startAngle + arcAngle) * (float) M_PI / 180);
		} else {
			cairo_arc(_W_PATH(path)->handle, 0, 0, 1, angle,
					-(startAngle + arcAngle) * (float) M_PI / 180);
		}
		cairo_restore(_W_PATH(path)->handle);
	}
	_W_PATH(path)->closed = FALSE;
	if (abs(arcAngle) >= 360)
		w_path_close(path);
	return W_TRUE;
}
wresult w_path_add_path(w_path *path, w_path *frompath) {
	if (path == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_W_PATH(path)->handle == 0)
		return W_ERROR_GRAPHIC_DISPOSED;
	if (frompath == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (w_path_is_ok(frompath) <= 0)
		return W_ERROR_INVALID_ARGUMENT;
	_W_PATH(path)->moved = FALSE;
	cairo_path_t *copy = cairo_copy_path(_W_PATH(frompath)->handle);
	if (copy == 0)
		return W_ERROR_NO_HANDLES;
	cairo_append_path(_W_PATH(path)->handle, copy);
	cairo_path_destroy(copy);
	_W_PATH(path)->closed = _W_PATH(frompath)->closed;
	return W_TRUE;
}
wresult w_path_add_rectangle(w_path *path, w_rectf *rect) {
	if (path == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_W_PATH(path)->handle == 0)
		return W_ERROR_GRAPHIC_DISPOSED;
	_W_PATH(path)->moved = FALSE;
	cairo_rectangle(_W_PATH(path)->handle, rect->x, rect->y, rect->width,
			rect->height);
	_W_PATH(path)->closed = TRUE;
	return W_TRUE;
}
wresult w_path_add_string(w_path *path, const char *string, int length, int enc,
		w_pointf *pt, w_font *font) {
	if (path == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_W_PATH(path)->handle == 0)
		return W_ERROR_GRAPHIC_DISPOSED;
	if (font == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (!w_font_is_ok(font))
		return W_ERROR_INVALID_ARGUMENT;
	_W_PATH(path)->moved = FALSE;
	PangoLayout *layout = pango_cairo_create_layout(_W_PATH(path)->handle);
	if (layout == 0)
		return W_ERROR_NO_HANDLES;
	pango_layout_set_text(layout, string, length);
	pango_layout_set_font_description(layout, (PangoFontDescription*) font);
	double currentX, currentY;
	cairo_get_current_point(_W_PATH(path)->handle, &currentX, &currentY);
	if (currentX != pt->x || currentY != pt->y) {
		cairo_move_to(_W_PATH(path)->handle, pt->x, pt->y);
	}
	pango_cairo_layout_path(_W_PATH(path)->handle, layout);
	g_object_unref(layout);
	_W_PATH(path)->closed = TRUE;
	return W_TRUE;
}
wresult w_path_close(w_path *path) {
	if (path == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_W_PATH(path)->handle == 0)
		return W_ERROR_GRAPHIC_DISPOSED;
	cairo_close_path(_W_PATH(path)->handle);
	_W_PATH(path)->moved = FALSE;
	_W_PATH(path)->closed = TRUE;
	return W_TRUE;
}
wresult w_path_contains(w_path *path, w_pointf *pt, w_graphics *gc,
		int outline) {
	if (path == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_W_PATH(path)->handle == 0)
		return W_ERROR_GRAPHIC_DISPOSED;
	if (gc == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (!w_graphics_is_ok(gc))
		return W_ERROR_INVALID_ARGUMENT;
	_w_graphics_check(gc,
			GRAPHICS_STATE_LINE_CAP | GRAPHICS_STATE_LINE_JOIN
					| GRAPHICS_STATE_LINE_STYLE | GRAPHICS_STATE_LINE_WIDTH);
	gboolean result = FALSE;
	cairo_t *cairo = _W_GRAPHICS(gc)->cairo;
	cairo_path_t *copy = cairo_copy_path(_W_PATH(path)->handle);
	if (copy == 0)
		return W_ERROR_NO_HANDLES;
	cairo_append_path(cairo, copy);
	cairo_path_destroy(copy);
	if (outline) {
		result = cairo_in_stroke(cairo, pt->x, pt->y) != 0;
	} else {
		result = cairo_in_fill(cairo, pt->x, pt->y) != 0;
	}
	cairo_new_path(cairo);
	return result;
}
wresult w_path_cubicto(w_path *path, float cx1, float cy1, float cx2, float cy2,
		float x, float y) {
	if (path == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_W_PATH(path)->handle == 0)
		return W_ERROR_GRAPHIC_DISPOSED;
	if (!_W_PATH(path)->moved) {
		double currentX, currentY;
		cairo_get_current_point(_W_PATH(path)->handle, &currentX, &currentY);
		cairo_move_to(_W_PATH(path)->handle, currentX, currentY);
		_W_PATH(path)->moved = TRUE;
	}
	cairo_curve_to(_W_PATH(path)->handle, cx1, cy1, cx2, cy2, x, y);
	_W_PATH(path)->closed = FALSE;
	return W_TRUE;
}
wresult w_path_get_bounds(w_path *_path, w_rectf *bounds) {
	if (_path == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_W_PATH(_path)->handle == 0)
		return W_ERROR_GRAPHIC_DISPOSED;
	if (bounds == 0)
		return W_ERROR_NULL_ARGUMENT;
	cairo_path_t *path = cairo_copy_path(_W_PATH(_path)->handle);
	if (path == 0)
		return W_ERROR_NO_HANDLES;
	double minX = 0, minY = 0, maxX = 0, maxY = 0;
	if (path->num_data > 0) {
		minX = minY = INFINITY;
		maxX = maxY = -INFINITY;
		int i = 0;
		double *points;
		cairo_path_data_t *data;
		while (i < path->num_data) {
			data = &path->data[i];
			points = (double*) &path->data[i + 1];
			switch (data->header.type) {
			case CAIRO_PATH_MOVE_TO:
				minX = MIN(minX, points[0]);
				minY = MIN(minY, points[1]);
				maxX = MAX(maxX, points[0]);
				maxY = MAX(maxY, points[1]);
				break;
			case CAIRO_PATH_LINE_TO:
				minX = MIN(minX, points[0]);
				minY = MIN(minY, points[1]);
				maxX = MAX(maxX, points[0]);
				maxY = MAX(maxY, points[1]);
				break;
			case CAIRO_PATH_CURVE_TO:
				minX = MIN(minX, points[0]);
				minY = MIN(minY, points[1]);
				maxX = MAX(maxX, points[0]);
				maxY = MAX(maxY, points[1]);
				minX = MIN(minX, points[2]);
				minY = MIN(minY, points[3]);
				maxX = MAX(maxX, points[2]);
				maxY = MAX(maxY, points[3]);
				minX = MIN(minX, points[4]);
				minY = MIN(minY, points[5]);
				maxX = MAX(maxX, points[4]);
				maxY = MAX(maxY, points[5]);
				break;
			case CAIRO_PATH_CLOSE_PATH:
				break;
			}
			i += data->header.length;
		}
	}
	bounds->x = (float) minX;
	bounds->y = (float) minY;
	bounds->width = (float) (maxX - minX);
	bounds->height = (float) (maxY - minY);
	cairo_path_destroy(path);
	return W_TRUE;
}
wresult w_path_get_current_point(w_path *path, w_pointf *point) {
	if (path == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_W_PATH(path)->handle == 0)
		return W_ERROR_GRAPHIC_DISPOSED;
	if (point == 0)
		return W_ERROR_NULL_ARGUMENT;
	double x, y;
	cairo_get_current_point(_W_PATH(path)->handle, &x, &y);
	point->x = (float) x;
	point->y = (float) y;
	return W_TRUE;
}
typedef struct w_iterator_pathdata {
	w_basic_iterator base;
	cairo_path_t *path;
	size_t i;
	size_t count;
} w_iterator_pathdata;
wresult w_iterator_pathdata_close(w_iterator *it) {
	w_iterator_pathdata *data = (w_iterator_pathdata*) it;
	cairo_path_destroy(data->path);
	return W_TRUE;
}
wresult w_iterator_pathdata_next(w_iterator *_it, void *_obj) {
	w_iterator_pathdata *it = (w_iterator_pathdata*) _it;
	if (it->i >= it->path->num_data)
		return W_FALSE;
	w_pathdata *obj = (w_pathdata*) _obj;
	cairo_path_data_t *data = (cairo_path_data_t*) &it->path->data[it->i];
	double *points = (double*) &it->path->data[it->i + 1];
	switch (data->header.type) {
	case CAIRO_PATH_MOVE_TO:
		obj->type = W_PATH_MOVE_TO;
		obj->pt[0] = (float) points[0];
		obj->pt[1] = (float) points[1];
		break;
	case CAIRO_PATH_LINE_TO:
		obj->type = W_PATH_LINE_TO;
		obj->pt[0] = (float) points[0];
		obj->pt[1] = (float) points[1];
		break;
	case CAIRO_PATH_CURVE_TO:
		obj->type = W_PATH_CUBIC_TO;
		obj->pt[0] = (float) points[0];
		obj->pt[1] = (float) points[1];
		obj->pt[2] = (float) points[2];
		obj->pt[3] = (float) points[3];
		obj->pt[4] = (float) points[4];
		obj->pt[5] = (float) points[5];
		break;
	case CAIRO_PATH_CLOSE_PATH:
		obj->type = W_PATH_CLOSE;
		break;
	}
	it->i += data->header.length;
	return W_TRUE;
}
wresult w_iterator_pathdata_reset(w_iterator *it) {
	w_iterator_pathdata *data = (w_iterator_pathdata*) it;
	data->i = 0;
	return W_TRUE;
}
wresult w_iterator_pathdata_remove(w_iterator *it) {
	return W_ERROR_NOT_IMPLEMENTED;
}
size_t w_iterator_pathdata_get_count(w_iterator *_it) {
	w_iterator_pathdata *it = (w_iterator_pathdata*) _it;
	if (it->count == -1) {
		size_t count = 0;
		size_t i = 0;
		while (i < it->path->num_data) {
			cairo_path_data_t *data =
					(cairo_path_data_t*) &it->path->data[it->i];
			switch (data->header.type) {
			case CAIRO_PATH_MOVE_TO:
			case CAIRO_PATH_LINE_TO:
			case CAIRO_PATH_CURVE_TO:
			case CAIRO_PATH_CLOSE_PATH:
				count++;
				break;
			}
			i += data->header.length;
		}

	}
	return it->count;
}
struct _w_iterator_class w_iterator_pathdata_class = { //
		w_iterator_pathdata_close, //
				w_iterator_pathdata_next, //
				w_iterator_pathdata_reset, //
				w_iterator_pathdata_remove, //
				w_iterator_pathdata_get_count //
		};
wresult w_path_get_path_data(w_path *path, w_iterator *_data) {
	w_iterator_close(_data);
	if (path == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_W_PATH(path)->handle == 0)
		return W_ERROR_GRAPHIC_DISPOSED;
	cairo_path_t *copy = cairo_copy_path(_W_PATH(path)->handle);
	if (copy == 0)
		return W_ERROR_NO_HANDLES;
	w_iterator_pathdata *data = (w_iterator_pathdata*) _data;
	data->base.clazz = &w_iterator_pathdata_class;
	data->i = 0;
	data->count = -1;
	data->path = copy;
	return W_TRUE;
}
wresult w_path_lineto(w_path *path, float x, float y) {
	if (path == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_W_PATH(path)->handle == 0)
		return W_ERROR_GRAPHIC_DISPOSED;
	if (!_W_PATH(path)->moved) {
		double currentX, currentY;
		cairo_get_current_point(_W_PATH(path)->handle, &currentX, &currentY);
		cairo_move_to(_W_PATH(path)->handle, currentX, currentY);
		_W_PATH(path)->moved = TRUE;
	}
	cairo_line_to(_W_PATH(path)->handle, x, y);
	_W_PATH(path)->closed = FALSE;
	return W_TRUE;
}
wresult w_path_move_to(w_path *path, float x, float y) {
	if (path == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_W_PATH(path)->handle == 0)
		return W_ERROR_GRAPHIC_DISPOSED;
	/*
	 * Bug in Cairo.  If cairo_move_to() is not called at the
	 * begining of a subpath, the first cairo_line_to() or
	 * cairo_curve_to() segment do not output anything.  The fix
	 * is to detect that the app did not call cairo_move_to()
	 * before those calls and call it explicitly.
	 */
	_W_PATH(path)->moved = TRUE;
	cairo_move_to(_W_PATH(path)->handle, x, y);
	_W_PATH(path)->closed = TRUE;
	return W_TRUE;
}
wresult w_path_quadto(w_path *path, float cx, float cy, float x, float y) {
	if (path == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_W_PATH(path)->handle == 0)
		return W_ERROR_GRAPHIC_DISPOSED;
	double currentX, currentY;
	cairo_get_current_point(_W_PATH(path)->handle, &currentX, &currentY);
	if (!_W_PATH(path)->moved) {
		cairo_move_to(_W_PATH(path)->handle, currentX, currentY);
		_W_PATH(path)->moved = TRUE;
	}
	float x0 = (float) currentX;
	float y0 = (float) currentY;
	float cx1 = x0 + 2 * (cx - x0) / 3;
	float cy1 = y0 + 2 * (cy - y0) / 3;
	float cx2 = cx1 + (x - x0) / 3;
	float cy2 = cy1 + (y - y0) / 3;
	cairo_curve_to(_W_PATH(path)->handle, cx1, cy1, cx2, cy2, x, y);
	_W_PATH(path)->closed = FALSE;
	return W_TRUE;
}
