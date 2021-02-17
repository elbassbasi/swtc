/*
 * Name:        path.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "gc.h"
#include "../widgets/toolkit.h"
void w_path_init(w_path *path) {
	_W_PATH(path)->handle = 0;
}
void w_path_dispose(w_path *path) {
	if (_W_PATH(path)->handle != 0) {
		GdipDeletePath(_W_PATH(path)->handle);
		_W_PATH(path)->handle = 0;
	}
}
void _w_path_init(w_path *path, GpPath *handle) {
	_w_path *p = _W_PATH(path);
	p->handle = handle;
	p->currentPoint.X = 0;
	p->currentPoint.Y = 0;
	p->startPoint.X = 0;
	p->startPoint.Y = 0;
}
wresult w_path_create(w_path *path) {
	w_path_dispose(path);
	GpPath *_path = 0;
	GdipCreatePath(FillModeAlternate, &_path);
	if (_path == 0)
		return W_ERROR_NO_HANDLES;
	_w_path_init(path, _path);
	return W_TRUE;
}
wresult w_path_create_from_path(w_path *path, w_path *frompath,
		float flatness) {
	if (path == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (frompath == 0)
		return W_ERROR_NO_HANDLES;
	if (_W_PATH(frompath)->handle == 0)
		return W_ERROR_NO_HANDLES;
	GpPath *_path = 0;
	flatness = WMAX(0, flatness);
	GdipClonePath(_W_PATH(frompath)->handle, &_path);
	if (_path == 0)
		return W_ERROR_NO_HANDLES;
	if (flatness != 0)
		GdipFlattenPath(_path, 0, flatness);
	_w_path_init(path, _path);
	return W_TRUE;
}
wresult w_path_create_from_data(w_path *path, size_t length, char *types,
		float *points) {
	wresult result = w_path_create(path);
	if (result > 0) {
		for (int i = 0, j = 0; i < length; i++) {
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
	}
	return result;
}
wresult w_path_add_arc(w_path *path, w_rectf *rect, float startAngle,
		float arcAngle) {
	if (path == 0)
		return W_ERROR_NO_HANDLES;
	if (_W_PATH(path)->handle == 0)
		return W_ERROR_NO_HANDLES;
	wresult result = W_TRUE;
	_w_path *p = _W_PATH(path);
	REAL x = rect->x;
	REAL y = rect->y;
	REAL width = rect->width;
	REAL height = rect->height;
	if (width < 0) {
		x = x + width;
		width = -width;
	}
	if (height < 0) {
		y = y + height;
		height = -height;
	}
	if (width == 0 || height == 0 || arcAngle == 0)
		return W_TRUE;
	if (width == height) {
		GdipAddPathArc(p->handle, x, y, width, height, -startAngle, -arcAngle);
	} else {
		GpPath *_path = 0;
		GdipCreatePath(FillModeAlternate, &_path);
		if (_path == 0) {
			GpMatrix *matrix = 0;
			GdipCreateMatrix2(width, 0, 0, height, x, y, &matrix);
			if (matrix != 0) {
				GdipAddPathArc(_path, 0, 0, 1, 1, -startAngle, -arcAngle);
				GdipTransformPath(_path, matrix);
				GdipAddPathPath(p->handle, _path, TRUE);
				GdipDeleteMatrix(matrix);
			} else
				result = W_ERROR_NO_HANDLES;
			GdipDeletePath(_path);
		} else
			result = W_ERROR_NO_HANDLES;
	}
	GdipGetPathLastPoint(p->handle, &p->currentPoint);
	return result;
}
wresult w_path_add_path(w_path *path, w_path *frompath) {
	if (path == 0)
		return W_ERROR_NO_HANDLES;
	if (_W_PATH(path)->handle == 0)
		return W_ERROR_NO_HANDLES;
	_w_path *p = _W_PATH(path);
	GdipAddPathPath(p->handle, _W_PATH(frompath)->handle, FALSE);
	p->currentPoint.X = _W_PATH(frompath)->currentPoint.X;
	p->currentPoint.Y = _W_PATH(frompath)->currentPoint.Y;
	return W_TRUE;
}
wresult w_path_add_rectangle(w_path *path, w_rectf *r) {
	if (path == 0)
		return W_ERROR_NO_HANDLES;
	if (_W_PATH(path)->handle == 0)
		return W_ERROR_NO_HANDLES;
	_w_path *p = _W_PATH(path);
	GdipAddPathRectangle(p->handle, r->x, r->y, r->width, r->height);
	p->currentPoint.X = r->x;
	p->currentPoint.Y = r->y;
	return W_TRUE;
}
wresult w_path_add_string(w_path *path, const char *text, int length, int enc,
		w_pointf *pt, w_font *font) {
	if (path == 0)
		return W_ERROR_NO_HANDLES;
	if (_W_PATH(path)->handle == 0)
		return W_ERROR_NO_HANDLES;
	if (font == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_W_FONT(font)->handle == 0)
		return W_ERROR_INVALID_ARGUMENT;
	size_t newlength;
	wresult result = W_TRUE;
	WCHAR *s = _win_text_fix(text, length, &newlength, enc);
	if (s != 0) {
		HDC hDC = CreateCompatibleDC(NULL);
		GpFontFamily *family = 0;
		GpFont *gdipFont = w_graphics_create_gdip_font(hDC,
		_W_FONT(font)->handle, 0, win_toolkit->fontCollection, &family, 0);
		GpRectF rect;
		REAL fontSize;
		INT fontStyle;
		GdipGetFontSize(gdipFont, &fontSize);
		GdipGetFontStyle(gdipFont, &fontStyle);
		rect.X = pt->x - (fontSize / 6);
		rect.Y = pt->y;
		rect.Width = 0;
		rect.Height = 0;
		GdipAddPathString(_W_PATH(path)->handle, s, newlength, family,
				fontStyle, fontSize, &rect, 0);
		GdipGetPathLastPoint(_W_PATH(path)->handle,
				& _W_PATH(path)->currentPoint);
		GdipDeleteFontFamily(family);
		GdipDeleteFont(gdipFont);
		DeleteDC(hDC);
	}
	_win_text_free(text, s, newlength);
	return result;
}
wresult w_path_close(w_path *path) {
	if (path == 0)
		return W_ERROR_NO_HANDLES;
	if (_W_PATH(path)->handle == 0)
		return W_ERROR_NO_HANDLES;
	_w_path *p = _W_PATH(path);
	GdipClosePathFigure(p->handle);
	/*
	 * Feature in GDI+. CloseFigure() does affect the last
	 * point, so GetLastPoint() does not return the starting
	 * point of the subpath after calling CloseFigure().  The
	 * fix is to remember the subpath starting point and use
	 * it instead.
	 */
	p->currentPoint.X = p->startPoint.X;
	p->currentPoint.Y = p->startPoint.Y;
	return W_TRUE;
}
wresult w_path_contains(w_path *path, w_pointf *pt, w_graphics *gc,
		int outline) {
	if (path == 0)
		return W_ERROR_NO_HANDLES;
	if (_W_PATH(path)->handle == 0)
		return W_ERROR_NO_HANDLES;
	_w_path *p = _W_PATH(path);
	_w_graphics_init_gdip(gc);
	w_graphics_check(gc,
			GRAPHICS_STATE_LINE_CAP | GRAPHICS_STATE_LINE_JOIN
					| GRAPHICS_STATE_LINE_STYLE | GRAPHICS_STATE_LINE_WIDTH);
	GpFillMode mode =
			GetPolyFillMode(_W_GRAPHICS(gc)->handle) == WINDING ?
					FillModeWinding : FillModeAlternate;
	GdipSetPathFillMode(p->handle, mode);
	BOOL ret = FALSE;
	if (outline) {
		GdipIsOutlineVisiblePathPoint(p->handle, pt->x, pt->y,
		_W_GRAPHICS(gc)->gdipPen, _W_GRAPHICS(gc)->gdipGraphics, &ret);
	} else {
		GdipIsVisiblePathPoint(p->handle, pt->x, pt->y,
		_W_GRAPHICS(gc)->gdipGraphics, &ret);
	}
	return ret;
}
wresult w_path_cubicto(w_path *path, float cx1, float cy1, float cx2, float cy2,
		float x, float y) {
	if (path == 0)
		return W_ERROR_NO_HANDLES;
	if (_W_PATH(path)->handle == 0)
		return W_ERROR_NO_HANDLES;
	_w_path *p = _W_PATH(path);
	GdipAddPathBezier(p->handle, p->currentPoint.X, p->currentPoint.Y, cx1, cy1,
			cx2, cy2, x, y);
	GdipGetPathLastPoint(p->handle, &p->currentPoint);
	return W_TRUE;
}
wresult w_path_get_bounds(w_path *path, w_rectf *bounds) {
	if (path == 0)
		return W_ERROR_NO_HANDLES;
	if (_W_PATH(path)->handle == 0)
		return W_ERROR_NO_HANDLES;
	_w_path *p = _W_PATH(path);
	GpRectF rect;
	GdipGetPathWorldBounds(p->handle, &rect, 0, 0);
	bounds->x = rect.X;
	bounds->y = rect.Y;
	bounds->width = rect.Width;
	bounds->height = rect.Height;
	return W_TRUE;
}
wresult w_path_get_current_point(w_path *path, w_pointf *point) {
	if (path == 0)
		return W_ERROR_NO_HANDLES;
	if (_W_PATH(path)->handle == 0)
		return W_ERROR_NO_HANDLES;
	_w_path *p = _W_PATH(path);
	point->x = p->currentPoint.X;
	point->y = p->currentPoint.Y;
	return W_TRUE;
}
typedef struct w_iterator_pathdata {
	w_basic_iterator base;
	BYTE *types;
	unsigned close :1;
	size_t index;
	size_t count;
	size_t _count;
} w_iterator_pathdata;
wresult w_iterator_pathdata_close(w_iterator *it) {
	w_iterator_pathdata *data = (w_iterator_pathdata*) it;
	free(data->types);
	return W_TRUE;
}
wresult w_iterator_pathdata_next(w_iterator *_it, void *_obj) {
	w_iterator_pathdata *it = (w_iterator_pathdata*) _it;
	if (it->index >= it->count)
		return W_FALSE;
	w_pathdata *obj = (w_pathdata*) _obj;
	if (it->close) {
		obj->type = W_PATH_CLOSE;
		it->close = 0;
	} else {
		BYTE type = it->types[it->index];
		GpPointF *points = (GpPointF*) &it->types[it->count];
		int index = it->index;
		switch (type & PathPointTypePathTypeMask) {
		case PathPointTypeStart:
			obj->type = W_PATH_MOVE_TO;
			it->close = (type & PathPointTypeCloseSubpath) != 0;
			obj->pt[0] = points[index].X;
			obj->pt[1] = points[index].Y;
			it->index += 1;
			break;
		case PathPointTypeLine:
			obj->type = W_PATH_LINE_TO;
			it->close = (type & PathPointTypeCloseSubpath) != 0;
			obj->pt[0] = points[index].X;
			obj->pt[1] = points[index].Y;
			it->index += 1;
			break;
		case PathPointTypeBezier:
			obj->type = W_PATH_CUBIC_TO;
			it->close = (it->types[it->index + 2] & PathPointTypeCloseSubpath)
					!= 0;
			obj->pt[0] = points[index].X;
			obj->pt[1] = points[index].Y;
			obj->pt[2] = points[index + 1].X;
			obj->pt[3] = points[index + 1].Y;
			obj->pt[4] = points[index + 2].X;
			obj->pt[5] = points[index + 2].Y;
			it->index += 3;
			break;
		default:
			obj->type = 0;
			it->index++;
		}
	}
	return W_TRUE;
}
wresult w_iterator_pathdata_reset(w_iterator *it) {
	w_iterator_pathdata *data = (w_iterator_pathdata*) it;
	data->index = 0;
	data->close = 0;
	return W_TRUE;
}
wresult w_iterator_pathdata_remove(w_iterator *it) {
	return W_ERROR_NOT_IMPLEMENTED;
}
size_t w_iterator_pathdata_get_count(w_iterator *_it) {
	w_iterator_pathdata *it = (w_iterator_pathdata*) _it;
	if (it->_count == -1) {
		int index = 0, typesIndex = 0;
		BYTE *gdipTypes = it->types;
		int count = it->count;
		while (index < count) {
			byte type = gdipTypes[index];
			int close = FALSE;
			switch (type & PathPointTypePathTypeMask) {
			case PathPointTypeStart:
				typesIndex++;
				close = (type & PathPointTypeCloseSubpath) != 0;
				index += 1;
				break;
			case PathPointTypeLine:
				typesIndex++;
				close = (type & PathPointTypeCloseSubpath) != 0;
				index += 1;
				break;
			case PathPointTypeBezier:
				typesIndex++;
				close = (gdipTypes[index + 2] & PathPointTypeCloseSubpath) != 0;
				index += 3;
				break;
			default:
				index++;
			}
			if (close) {
				typesIndex++;
			}
		}
		it->_count = typesIndex;
	}
	return it->_count;
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
	_w_path *p = _W_PATH(path);
	w_iterator_pathdata *it = (w_iterator_pathdata*) _data;
	int count = 0;
	GdipGetPointCount(p->handle, &count);
	char *ptr = (char*) malloc((sizeof(BYTE) + sizeof(GpPointF)) * count);
	if (ptr != 0) {
		GdipGetPathTypes(p->handle, (BYTE*) ptr, count);
		GdipGetPathPoints(p->handle, (GpPointF*) &ptr[count], count);
		it->base.clazz = &w_iterator_pathdata_class;
		it->close = 0;
		it->count = count;
		it->index = 0;
		it->types = (BYTE*) ptr;
		it->_count = -1;
		return W_TRUE;
	} else {
		return W_ERROR_NO_MEMORY;
	}
}
wresult w_path_lineto(w_path *path, float x, float y) {
	if (path == 0)
		return W_ERROR_NO_HANDLES;
	if (_W_PATH(path)->handle == 0)
		return W_ERROR_NO_HANDLES;
	_w_path *p = _W_PATH(path);
	GdipAddPathLine(p->handle, p->currentPoint.X, p->currentPoint.Y, x, y);
	GdipGetPathLastPoint(p->handle, &p->currentPoint);
	return W_TRUE;
}
wresult w_path_is_ok(w_path *path) {
	if (path == 0)
		return W_ERROR_NO_HANDLES;
	if (_W_PATH(path)->handle == 0)
		return W_ERROR_NO_HANDLES;
	return W_TRUE;
}
wresult w_path_move_to(w_path *path, float x, float y) {
	if (path == 0)
		return W_ERROR_NO_HANDLES;
	if (_W_PATH(path)->handle == 0)
		return W_ERROR_NO_HANDLES;
	_w_path *p = _W_PATH(path);
	GdipStartPathFigure(p->handle);
	p->currentPoint.X = p->startPoint.X = x;
	p->currentPoint.Y = p->startPoint.Y = y;
	return W_TRUE;
}
wresult w_path_quadto(w_path *path, float cx, float cy, float x, float y) {
	if (path == 0)
		return W_ERROR_NO_HANDLES;
	if (_W_PATH(path)->handle == 0)
		return W_ERROR_NO_HANDLES;
	_w_path *p = _W_PATH(path);
	REAL cx1 = p->currentPoint.X + 2 * (cx - p->currentPoint.X) / 3;
	REAL cy1 = p->currentPoint.Y + 2 * (cy - p->currentPoint.Y) / 3;
	REAL cx2 = cx1 + (x - p->currentPoint.X) / 3;
	REAL cy2 = cy1 + (y - p->currentPoint.Y) / 3;
	GdipAddPathBezier(p->handle, p->currentPoint.X, p->currentPoint.Y, cx1, cy1,
			cx2, cy2, x, y);
	GdipGetPathLastPoint(p->handle, &p->currentPoint);
	return W_TRUE;
}
