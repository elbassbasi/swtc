/*
 * Name:        imagelist.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "gc.h"
void w_imagelist_init(w_imagelist *imagelist) {
	_W_IMAGELIST(imagelist)->imagelist = 0;
}
void w_imagelist_dispose(w_imagelist *imagelist) {
	if (imagelist != 0 && _W_IMAGELIST(imagelist)->imagelist != 0) {
		ImageList_Destroy(_W_IMAGELIST(imagelist)->imagelist);
		_W_IMAGELIST(imagelist)->imagelist = 0;
	}
}
wresult w_imagelist_create(w_imagelist *imagelist, w_size *size,
		int initialCount) {
	w_imagelist_dispose(imagelist);
	_W_IMAGELIST(imagelist)->imagelist = ImageList_Create(size->width,
			size->height, ILC_COLOR32 | ILC_MASK, initialCount, 4);
	if (_W_IMAGELIST(imagelist)->imagelist == 0) {
		return W_FALSE;
	} else
		return W_TRUE;
}
wresult w_imagelist_replace_hbitmap(w_imagelist *imagelist, int index,
		int *newindex, HBITMAP hbitmap, HBITMAP mask) {
	if (index == -1) {
		*newindex = ImageList_Add(_W_IMAGELIST(imagelist)->imagelist,
				(HBITMAP) hbitmap, mask);
	} else {
		*newindex = ImageList_Replace(
		_W_IMAGELIST(imagelist)->imagelist, index, (HBITMAP) hbitmap, mask);
	}
	return W_TRUE;
}
wresult w_imagelist_replace_gpimage(w_imagelist *imagelist, int index,
		int *newindex, GpImage *img) {
	wresult result = W_FALSE;
	/*HICON hicon = 0;
	 GdipCreateHICONFromBitmap(img, &hicon);
	 if (hicon != 0) {
	 ICONINFO info;
	 GetIconInfo(hicon, &info);
	 result = w_imagelist_replace_hbitmap(imagelist, index, newindex,
	 info.hbmColor, info.hbmMask);
	 DestroyIcon(hicon);
	 }*/
	HBITMAP bitmap;
	GdipCreateHBITMAPFromBitmap(img, &bitmap, 0);
	if (bitmap != 0) {
		result = w_imagelist_replace_hbitmap(imagelist, index, newindex, bitmap,
		NULL);
		DeleteObject(bitmap);
	}
	return result;
}
wresult w_imagelist_replace_0(w_imagelist *imagelist, int index, int *newindex,
		w_image *image) {
	if (imagelist == 0 || _W_IMAGELIST(imagelist)->imagelist == 0)
		return W_ERROR_NO_HANDLES;
	int cx, cy;
	w_size sz;
	int flags = 0;
	if (!w_image_is_ok(image))
		return W_ERROR_INVALID_ARGUMENT;
	ImageList_GetIconSize(_W_IMAGELIST(imagelist)->imagelist, &cx, &cy);
	w_image_get_size(image, &sz);
	if (sz.width != cx || sz.height != cy) {
		sz.width = cx;
		sz.height = cy;
		GpImage *_gpimg;
		w_image_resize_0(image, &sz, &_gpimg);
		if (_gpimg == 0)
			return W_ERROR_NO_HANDLES;
		wresult result = w_imagelist_replace_gpimage(imagelist, index, newindex,
				_gpimg);
		GdipDisposeImage(_gpimg);
		return result;
	} else {
		switch (_W_IMAGE(image)->type) {
		case _IMAGE_HBITMAP: {
			w_imagelist_replace_hbitmap(imagelist, index, newindex,
					(HBITMAP) _W_IMAGE(image)->handle, NULL);
		}
			break;
		case _IMAGE_ICON: {
			*newindex = ImageList_ReplaceIcon(
			_W_IMAGELIST(imagelist)->imagelist, index,
					(HICON) _W_IMAGE(image)->handle);
		}
			break;
		case _IMAGE_GPBITMAP: {
			w_imagelist_replace_gpimage(imagelist, index, newindex,
					(GpImage*) _W_IMAGE(image)->handle);
		}
			break;
		}
	}
	return W_TRUE;
}
int w_imagelist_add(w_imagelist *imagelist, w_image *image) {
	int index = -1;
	w_imagelist_replace_0(imagelist, -1, &index, image);
	return index;
}
wresult w_imagelist_is_ok(w_imagelist *imagelist) {
	if (imagelist != 0 && _W_IMAGELIST(imagelist)->imagelist != 0) {
		return W_TRUE;
	} else
		return W_FALSE;
}
wresult w_imagelist_replace(w_imagelist *imagelist, int index, w_image *image) {
	int newindex = -1;
	return w_imagelist_replace_0(imagelist, index, &newindex, image);
}
wresult w_imagelist_remove(w_imagelist *imagelist, int index) {
	if (imagelist == 0 || _W_IMAGELIST(imagelist)->imagelist == 0)
		return W_ERROR_NO_HANDLES;
	return ImageList_Remove(_W_IMAGELIST(imagelist)->imagelist, index);
}
wresult w_imagelist_remove_all(w_imagelist *imagelist) {
	if (imagelist == 0 || _W_IMAGELIST(imagelist)->imagelist == 0)
		return W_ERROR_NO_HANDLES;
	return ImageList_Remove(_W_IMAGELIST(imagelist)->imagelist, -1);
}
wresult w_imagelist_get_image(w_imagelist *imagelist, int index, int copy,
		w_image *image) {
	if (image != 0)
		w_image_dispose(image);
	else
		return W_ERROR_INVALID_ARGUMENT;
	if (imagelist == 0 || _W_IMAGELIST(imagelist)->imagelist == 0)
		return W_ERROR_NO_HANDLES;
	HICON icon = ImageList_GetIcon(_W_IMAGELIST(imagelist)->imagelist, index,
	ILD_NORMAL | ILD_TRANSPARENT);
	if (copy) {
		_W_IMAGE(image)->handle = CopyIcon(icon);
		_W_IMAGE(image)->type = _IMAGE_ICON;
		_W_IMAGE(image)->nodispose = 0;
		if (_W_IMAGE(image)->handle == 0)
			return W_ERROR_NO_HANDLES;
	} else {
		_W_IMAGE(image)->handle = icon;
		_W_IMAGE(image)->type = _IMAGE_ICON;
		_W_IMAGE(image)->nodispose = 1;
	}
	return W_TRUE;
}
wresult w_imagelist_get_size(w_imagelist *imagelist, w_size *size) {
	if (imagelist == 0 || _W_IMAGELIST(imagelist)->imagelist == 0)
		return W_ERROR_NO_HANDLES;
	int cx, cy;
	ImageList_GetIconSize(_W_IMAGELIST(imagelist)->imagelist, &cx, &cy);
	size->width = cx;
	size->height = cy;
	return W_TRUE;
}
int w_imagelist_get_count(w_imagelist *imagelist) {
	if (imagelist == 0 || _W_IMAGELIST(imagelist)->imagelist == 0)
		return 0;
	return ImageList_GetImageCount(_W_IMAGELIST(imagelist)->imagelist);
}
wresult w_imagelist_draw(w_imagelist *imagelist, w_graphics *graphics,
		int index, w_point *pt, int state) {
	return w_imagelist_draw_ex(imagelist, graphics, index, pt, state, 0, 0);
}
wresult w_imagelist_draw_ex(w_imagelist *imagelist, w_graphics *graphics,
		int index, w_point *pt, int state, w_color bg, w_color fg) {
	if (imagelist == 0 || _W_IMAGELIST(imagelist)->imagelist == 0)
		return W_ERROR_NO_HANDLES;
	UINT fStyle;
	switch (state) {
	case W_IMAGELIST_DRAW_TRANSPARENT:
		fStyle = ILD_NORMAL | ILD_TRANSPARENT;
		break;
	case W_IMAGELIST_DRAW_SELECTED:
		fStyle = ILD_SELECTED;
		break;
	case W_IMAGELIST_DRAW_FOCUSED:
		fStyle = ILD_FOCUS;
		break;
	case W_IMAGELIST_DRAW_NORMAL:
	default:
		fStyle = ILD_NORMAL;
		break;
	}
	return ImageList_Draw(_W_IMAGELIST(imagelist)->imagelist, index,
	_W_GRAPHICS(graphics)->handle, pt->x, pt->y, fStyle);
}
