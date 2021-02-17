/*
 * Name:        image.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "gc.h"
#include "gdip.h"
/*
 * image
 */
void w_image_init(w_image *image) {
	_W_IMAGE(image)->handle = 0;
	_W_IMAGE(image)->flags = 0;
}
void w_image_dispose(w_image *image) {
	if (_W_IMAGE(image)->handle != 0) {
		if (_W_IMAGE(image)->nodispose == 0) {
			switch (_W_IMAGE(image)->type) {
			case _IMAGE_HBITMAP:
				DeleteObject((HBITMAP) _W_IMAGE(image)->handle);
				break;
			case _IMAGE_ICON:
				DestroyIcon((HICON) _W_IMAGE(image)->handle);
				break;
			case _IMAGE_GPBITMAP:
				GdipDisposeImage((GpImage*) _W_IMAGE(image)->handle);
				break;
			}
		}
		_W_IMAGE(image)->handle = 0;
	}
	_W_IMAGE(image)->flags = 0;
}
wresult w_image_is_ok(w_image *image) {
	return _W_IMAGE(image)->handle != 0;
}
void _w_image_get_hbitmap(w_image *image, _w_image_hbitmap *hbitmap) {
	ICONINFO iconinfo;
	memset(hbitmap, 0, sizeof(hbitmap));
	if (_W_IMAGE(image)->handle != 0) {
		switch (_W_IMAGE(image)->type) {
		case _IMAGE_HBITMAP:
			hbitmap->hbmColor = (HBITMAP) _W_IMAGE(image)->handle;
			break;
		case _IMAGE_ICON:
			GetIconInfo((HICON) _W_IMAGE(image)->handle, &iconinfo);
			hbitmap->hbmColor = iconinfo.hbmColor;
			hbitmap->hbmMask = iconinfo.hbmMask;
			break;
		case _IMAGE_GPBITMAP: {
			GdipCreateHICONFromBitmap((GpImage*) _W_IMAGE(image)->handle,
					&hbitmap->hicon);
			if (hbitmap->hicon != 0) {
				GetIconInfo(hbitmap->hicon, &iconinfo);
				hbitmap->hbmColor = iconinfo.hbmColor;
				hbitmap->hbmMask = iconinfo.hbmMask;
			}
		}
			break;
		}
	}
}
void _w_image_dispose_hbitmap(w_image *image, _w_image_hbitmap *hbitmap) {
	if (_W_IMAGE(image)->handle != 0
			&& _W_IMAGE(image)->type == _IMAGE_GPBITMAP) {
		DestroyIcon(hbitmap->hicon);
	}
}
void _w_image_get_gpimage(w_image *image, GpImage **gpimage) {
	if (_W_IMAGE(image)->handle != 0) {
		switch (_W_IMAGE(image)->type) {
		case _IMAGE_HBITMAP:
			GdipCreateBitmapFromHBITMAP((HBITMAP) _W_IMAGE(image)->handle, NULL,
					(GpImage**) gpimage);
			break;
		case _IMAGE_ICON:
			GdipCreateBitmapFromHICON((HICON) _W_IMAGE(image)->handle,
					(GpImage**) gpimage);
			break;
		case _IMAGE_GPBITMAP: {
			*gpimage = (GpImage*) _W_IMAGE(image)->handle;
		}
			break;
		}
	}
}
void _w_image_dispose_gpimage(w_image *image, GpImage *gpimage) {
	if (_W_IMAGE(image)->handle != 0) {
		switch (_W_IMAGE(image)->type) {
		case _IMAGE_HBITMAP:
		case _IMAGE_ICON:
			GdipDisposeImage(gpimage);
			break;
		}
	}
}
wresult w_image_create_from_file(w_image *image, const char *file, int length,
		int enc) {
	w_image_dispose(image);
	size_t newlength;
	WCHAR *_f = _win_text_fix(file, length, &newlength, enc);
	if (_f == 0)
		return W_ERROR_NO_MEMORY;
	for (int i = 0; i < newlength; i++) {
		if (_f[i] == '/')
			_f[i] = '\\';
	}
	GpImage *bitmap = 0;
	GdipCreateBitmapFromFile(_f, &bitmap);
	wresult result;
	if (bitmap != 0) {
		_W_IMAGE(image)->handle = bitmap;
		_W_IMAGE(image)->type = _IMAGE_GPBITMAP;
		result = W_TRUE;
	} else
		result = W_ERROR_NO_HANDLES;
	_win_text_free(file, _f, newlength);
	return result;
}
wresult w_image_create_from_data(w_image *image, void *data, wuint format,
		w_size *size, int rowstride) {
	w_image_dispose(image);
	BITMAP pbm;
	pbm.bmType = 0;
	pbm.bmWidth = size->width;
	pbm.bmHeight = size->height;
	switch (format) {
	case W_IMAGE_ARGB32:
		pbm.bmWidthBytes = size->width * 4;
		pbm.bmPlanes = 32;
		pbm.bmBitsPixel = 32;
		break;
	case W_IMAGE_RGB24:
		pbm.bmWidthBytes = size->width * 3;
		pbm.bmPlanes = 24;
		pbm.bmBitsPixel = 24;
		break;
	case W_IMAGE_A8:
		pbm.bmWidthBytes = size->width;
		pbm.bmPlanes = 8;
		pbm.bmBitsPixel = 8;
		break;
	case W_IMAGE_A1:
		pbm.bmWidthBytes = size->width / 8;
		pbm.bmPlanes = 1;
		pbm.bmBitsPixel = 1;
		break;
	default:
		pbm.bmWidthBytes = size->width * 4;
		pbm.bmPlanes = 32;
		pbm.bmBitsPixel = 32;
		break;
	}
	if (rowstride > 0)
		pbm.bmWidthBytes = rowstride;
	pbm.bmBits = data;
	_W_IMAGE(image)->handle = CreateBitmapIndirect(&pbm);
	if (_W_IMAGE(image)->handle != 0) {
		_W_IMAGE(image)->type = _IMAGE_HBITMAP;
		return W_TRUE;
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_image_create_from_stream(w_image *image, w_stream *stream) {
	swt_stream s;
	swt_stream_init(&s, stream);
	GpImage *bitmap = 0;
	GdipCreateBitmapFromStream((IStream*) &s, &bitmap);
	wresult result;
	if (bitmap != 0) {
		_W_IMAGE(image)->handle = bitmap;
		_W_IMAGE(image)->type = _IMAGE_GPBITMAP;
		result = W_TRUE;
	} else
		result = W_ERROR_NO_HANDLES;
	return result;
}
int w_image_get_rowstride(wuint format, wuint width) {
	switch (format) {
	case W_IMAGE_ARGB32:
		return width * 4;
		break;
	case W_IMAGE_RGB24:
		return width * 3;
		break;
	case W_IMAGE_A8:
		return width;
		break;
	case W_IMAGE_A1:
		return width / 8;
		break;
	default:
		return width * 4;
		break;
	}
}
wresult w_image_create_from_buffer(w_image *image, const void *buffer,
		size_t size) {
	return W_FALSE;
}
wresult w_image_get_size(w_image *image, w_size *size) {
	if (size == 0)
		return W_ERROR_NULL_ARGUMENT;
	size->width = 0;
	size->height = 0;
	if (image == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_W_IMAGE(image)->handle == 0)
		return W_ERROR_NO_HANDLES;
	switch (_W_IMAGE(image)->type) {
	case _IMAGE_HBITMAP: {
		BITMAP bm;
		GetObjectW(_W_IMAGE(image)->handle, sizeof(BITMAP), &bm);
		size->width = bm.bmWidth;
		size->height = bm.bmHeight;
		return W_TRUE;
	}
		break;
	case _IMAGE_ICON: {
		ICONINFO iconinfo;
		if (GetIconInfo((HICON) _W_IMAGE(image)->handle, &iconinfo)) {
			BITMAP bm;
			GetObjectW(iconinfo.hbmColor, sizeof(BITMAP), &bm);
			size->width = bm.bmWidth;
			size->height = bm.bmHeight;
			return W_TRUE;
		}
	}
		break;
	case _IMAGE_GPBITMAP: {
		UINT t;
		GdipGetImageWidth((GpImage*) _W_IMAGE(image)->handle, &t);
		size->width = t;
		GdipGetImageHeight((GpImage*) _W_IMAGE(image)->handle, &t);
		size->height = t;
		return W_TRUE;
	}
		break;
	}
	return W_TRUE;
}
wresult w_image_get_pixels(w_image *image, w_color *colors, size_t length) {
	return W_FALSE;
}
wresult w_image_save_to_file(w_image *image, const char *file, int length,
		int enc, const char *type) {
	return W_FALSE;
}
wresult w_image_save_to_buffer(w_image *image, w_alloc alloc, void *user_data,
		const char *type) {
	return W_FALSE;
}
wresult w_image_save_to_stream(w_image *image, w_stream *stream,
		const char *type) {
	return W_FALSE;
}
wresult w_image_copy(w_image *image, w_rect *rect, w_image *destimage) {
	w_image_dispose(destimage);
	if (rect == 0) {

	}
	return W_TRUE;
}
wresult w_image_resize_0(w_image *image, w_size *size, GpImage **dstimg) {
	wresult result = W_ERROR_NO_HANDLES;
	*dstimg = 0;
	GpImage *img = 0;
	switch (_W_IMAGE(image)->type) {
	case _IMAGE_HBITMAP: {
		GdipCreateBitmapFromHBITMAP((HBITMAP) _W_IMAGE(image)->handle,
		NULL, &img);
	}
		break;
	case _IMAGE_ICON: {
		GdipCreateBitmapFromHICON((HICON) _W_IMAGE(image)->handle, &img);
	}
		break;
	case _IMAGE_GPBITMAP: {
		img = (GpImage*) _W_IMAGE(image)->handle;
	}
		break;
	}
	GpImage *bm2 = 0;
	GpGraphics *gc = 0;
	if (img != 0) {
		GdipCreateBitmapFromScan0(size->width, size->height, 0,
		PixelFormat32bppPARGB,
		NULL, &bm2);
		if (bm2 != 0) {
			GdipGetImageGraphicsContext(bm2, &gc);
			if (gc != 0) {
				GdipSetInterpolationMode(gc, InterpolationModeHighQuality);
				GdipDrawImageRect(gc, img, 0, 0, size->width, size->height);
				GdipDeleteGraphics(gc);
				result = W_TRUE;
			}
		}
		if (img != _W_IMAGE(image)->handle)
			GdipDisposeImage(img);
	}
	*dstimg = bm2;
	return result;
}
wresult w_image_resize(w_image *image, w_size *size, w_image *destimage) {
	w_rect r;
	w_image_dispose(destimage);
	if (w_image_is_ok(image)) {
		w_image_get_size(image, &r.sz);
		if (r.sz.width == size->width && r.sz.height == size->height) {
			switch (_W_IMAGE(image)->type) {
			case _IMAGE_HBITMAP:
				_W_IMAGE(destimage)->handle = CopyImage(_W_IMAGE(image)->handle,
				IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);
				_W_IMAGE(destimage)->type = _IMAGE_HBITMAP;
				_W_IMAGE(destimage)->nodispose = 0;
				break;
			case _IMAGE_ICON:
				_W_IMAGE(destimage)->handle = CopyIcon(
						(HICON) _W_IMAGE(image)->handle);
				_W_IMAGE(destimage)->type = _IMAGE_ICON;
				_W_IMAGE(destimage)->nodispose = 0;
				break;
			case _IMAGE_GPBITMAP:
				GdipCloneImage((GpImage*) _W_IMAGE(image)->handle,
						(GpImage**) &_W_IMAGE(destimage)->handle);
				_W_IMAGE(destimage)->type = _IMAGE_GPBITMAP;
				_W_IMAGE(destimage)->nodispose = 0;
				break;
			}
		} else {
			GpImage *img = 0;
			w_image_resize_0(image, size, &img);
			_W_IMAGE(destimage)->handle = img;
			_W_IMAGE(destimage)->type = _IMAGE_GPBITMAP;
			_W_IMAGE(destimage)->nodispose = 0;
		}
		return W_TRUE;
	} else {
		return W_ERROR_NO_HANDLES;
	}
}
wresult w_image_to_greyscale(w_image *image, w_image *destimage) {
	return W_FALSE;
}
wresult w_image_to_disabled(w_image *image, w_color background, w_color shadow,
		w_image *destimage) {
	return W_FALSE;
}
/*
 * surface
 */
void w_surface_init(w_surface *surface) {
	w_image_init(W_IMAGE(surface));
}
void w_surface_dispose(w_surface *surface) {
	w_image_dispose(W_IMAGE(surface));
}
wresult w_surface_is_ok(w_surface *surface) {
	return w_image_is_ok(W_IMAGE(surface));
}
HBITMAP w_surface_create_dib(int width, int height, int depth) {
	BITMAPINFO bmi;
	bmi.bmiHeader.biSize = sizeof(BITMAPINFO);
	bmi.bmiHeader.biWidth = width;
	bmi.bmiHeader.biHeight = -height;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = (short) depth;
	bmi.bmiHeader.biCompression = BI_RGB;
	VOID *pBits;
	return CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, &pBits, NULL, 0);
}
wresult w_surface_create(w_surface *surface, w_size *size) {
	w_surface_dispose(surface);
	HDC hdc = 0;
	_W_IMAGE(surface)->handle = CreateBitmap(size->width, size->height, 1, 32,
	NULL);
	if (_W_IMAGE(surface)->handle == 0) {
		hdc = CreateCompatibleDC(NULL);
		int bits = GetDeviceCaps(hdc, BITSPIXEL);
		int planes = GetDeviceCaps(hdc, PLANES);
		int depth = bits * planes;
		if (depth < 16)
			depth = 16;
		_W_IMAGE(surface)->handle = w_surface_create_dib(size->width,
				size->height, depth);
	}
	if (_W_IMAGE(surface)->handle != 0) {
		if (hdc == 0)
			hdc = CreateCompatibleDC(NULL);
		HBITMAP hOldBitmap = SelectObject(hdc, _W_IMAGE(surface)->handle);
		PatBlt(hdc, 0, 0, size->width, size->height, PATCOPY);
		SelectObject(hdc, hOldBitmap);
	}
	if (hdc != 0)
		DeleteDC(hdc);
	if (_W_IMAGE(surface)->handle == 0)
		return W_ERROR_NO_HANDLES;
	_W_IMAGE(surface)->type = _IMAGE_HBITMAP;
	_W_IMAGE(surface)->nodispose = 0;
	return W_TRUE;
}
wresult w_surface_get_image(w_surface *surface, w_image *destimage) {
	if (destimage == 0)
		return W_ERROR_INVALID_ARGUMENT;
	w_image_dispose(destimage);
	if (w_image_is_ok(W_IMAGE(surface))) {
		_W_IMAGE(destimage)->handle = CopyImage(
				(HANDLE) _W_IMAGE(surface)->handle,
				IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);
		_W_IMAGE(destimage)->type = _IMAGE_HBITMAP;
		return W_TRUE;
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_surface_get_graphics(w_surface *surface, w_graphics *graphics) {
	if (graphics == 0)
		return W_ERROR_INVALID_ARGUMENT;
	w_graphics_dispose(graphics);
	if (w_image_is_ok(W_IMAGE(surface))) {
		HDC hdc = CreateCompatibleDC(NULL);
		HBITMAP hOldBitmap = SelectObject(hdc, _W_IMAGE(surface)->handle);
		_w_graphics_init(graphics, hdc);
		_W_GRAPHICS(graphics)->oldbitmap = hOldBitmap;
		_W_GRAPHICS(graphics)->state |= GRAPHICS_STATE_DELETEDC;
		return W_TRUE;
	} else
		return W_ERROR_NO_HANDLES;
}
