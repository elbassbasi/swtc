/*
 * Name:        imagelist.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "gc.h"
typedef struct IImageList {
	struct IImageListVtbl *lpVtbl;
} IImageList;
typedef struct IImageListVtbl {
	/*** IUnknown methods ***/
	HRESULT (STDMETHODCALLTYPE *QueryInterface)(IImageList *This,
	REFIID riid, void **ppvObject);

	ULONG (STDMETHODCALLTYPE *AddRef)(IImageList *This);

	ULONG (STDMETHODCALLTYPE *Release)(IImageList *This);

	/*** IImageList methods ***/
	HRESULT (STDMETHODCALLTYPE *Add)(IImageList *This, HBITMAP hbmImage,
			HBITMAP hbmMask, int *pi);

	HRESULT (STDMETHODCALLTYPE *ReplaceIcon)(IImageList *This, int i,
			HICON hicon, int *pi);

	HRESULT (STDMETHODCALLTYPE *SetOverlayImage)(IImageList *This, int iImage,
			int iOverlay);

	HRESULT (STDMETHODCALLTYPE *Replace)(IImageList *This, int i,
			HBITMAP hbmImage, HBITMAP hbmMask);

	HRESULT (STDMETHODCALLTYPE *AddMasked)(IImageList *This, HBITMAP hbmImage,
			COLORREF crMask, int *pi);

	HRESULT (STDMETHODCALLTYPE *Draw)(IImageList *This,
			IMAGELISTDRAWPARAMS *pimldp);

	HRESULT (STDMETHODCALLTYPE *Remove)(IImageList *This, int i);

	HRESULT (STDMETHODCALLTYPE *GetIcon)(IImageList *This, int i, UINT flags,
			HICON *picon);

	HRESULT (STDMETHODCALLTYPE *GetImageInfo)(IImageList *This, int i,
			IMAGEINFO *pImageInfo);

	HRESULT (STDMETHODCALLTYPE *Copy)(IImageList *This, int iDst,
			IUnknown *punkSrc, int iSrc, UINT uFlags);

	HRESULT (STDMETHODCALLTYPE *Merge)(IImageList *This, int i1,
			IUnknown *punk2, int i2, int dx, int dy,
			REFIID riid, void **ppv);

	HRESULT (STDMETHODCALLTYPE *Clone)(IImageList *This,
	REFIID riid, void **ppv);

	HRESULT (STDMETHODCALLTYPE *GetImageRect)(IImageList *This, int i,
			RECT *prc);

	HRESULT (STDMETHODCALLTYPE *GetIconSize)(IImageList *This, int *cx,
			int *cy);

	HRESULT (STDMETHODCALLTYPE *SetIconSize)(IImageList *This, int cx, int cy);

	HRESULT (STDMETHODCALLTYPE *GetImageCount)(IImageList *This, int *pi);

	HRESULT (STDMETHODCALLTYPE *SetImageCount)(IImageList *This,
			UINT uNewCount);

	HRESULT (STDMETHODCALLTYPE *SetBkColor)(IImageList *This, COLORREF clrBk,
			COLORREF *pclr);

	HRESULT (STDMETHODCALLTYPE *GetBkColor)(IImageList *This, COLORREF *pclr);

	HRESULT (STDMETHODCALLTYPE *BeginDrag)(IImageList *This, int iTrack,
			int dxHotspot, int dyHotspot);

	HRESULT (STDMETHODCALLTYPE *EndDrag)(IImageList *This);

	HRESULT (STDMETHODCALLTYPE *DragEnter)(IImageList *This, HWND hwndLock,
			int x, int y);

	HRESULT (STDMETHODCALLTYPE *DragLeave)(IImageList *This, HWND hwndLock);

	HRESULT (STDMETHODCALLTYPE *DragMove)(IImageList *This, int x, int y);

	HRESULT (STDMETHODCALLTYPE *SetDragCursorImage)(IImageList *This,
			IUnknown *punk, int iDrag, int dxHotspot, int dyHotspot);

	HRESULT (STDMETHODCALLTYPE *DragShowNolock)(IImageList *This,
			WINBOOL fShow);

	HRESULT (STDMETHODCALLTYPE *GetDragImage)(IImageList *This, POINT *ppt,
			POINT *pptHotspot,
			REFIID riid, void **ppv);

	HRESULT (STDMETHODCALLTYPE *GetItemFlags)(IImageList *This, int i,
			DWORD *dwFlags);

	HRESULT (STDMETHODCALLTYPE *GetOverlayImage)(IImageList *This, int iOverlay,
			int *piIndex);
} IImageListVtbl;
void w_imagelist_init(w_imagelist *imagelist) {
	_W_IMAGELIST(imagelist)->imagelist = 0;
}
void w_imagelist_dispose(w_imagelist *imagelist) {
	if (imagelist != 0 && _W_IMAGELIST(imagelist)->imagelist != 0) {
		ImageList_Destroy(_W_IMAGELIST(imagelist)->imagelist);
		_W_IMAGELIST(imagelist)->imagelist = 0;
	}
}
IImageListVtbl w_IImageListVtbl;
typedef HRESULT (STDMETHODCALLTYPE *__Draw)(IImageList *This,
		IMAGELISTDRAWPARAMS *pimldp);
__Draw last_Draw = 0;
HRESULT STDMETHODCALLTYPE w_IImageList_Draw(IImageList *This,
		IMAGELISTDRAWPARAMS *pimldp) {
	IMAGEINFO imageInfo;
	HRESULT res = This->lpVtbl->GetImageInfo(This, pimldp->i, &imageInfo);
	if (res == S_OK) {
		BITMAP bm, bm2;
		INT cx, cy;
		INT _cx, _cy;
		GetObjectW(imageInfo.hbmImage, sizeof(bm), &bm);
		if (bm.bmBitsPixel == 32) {
			ImageList_GetIconSize(pimldp->himl, &cx, &cy);
			if (pimldp->cx != 0)
				_cx = pimldp->cx;
			else
				_cx = cx;
			if (pimldp->cy != 0)
				_cy = pimldp->cy;
			else
				_cy = cy;
			BLENDFUNCTION blend;
			blend.BlendOp = AC_SRC_OVER;
			blend.BlendFlags = 0;
			blend.SourceConstantAlpha = 0xFF;
			blend.AlphaFormat = AC_SRC_ALPHA;
			HDC srcHdc = CreateCompatibleDC(NULL);
			BITMAPINFO bmi;
			memset(&bmi, 0, sizeof(bmi));
			bmi.bmiHeader.biSize = sizeof(BITMAPINFO);
			bmi.bmiHeader.biWidth = cx;
			bmi.bmiHeader.biHeight = cy;
			bmi.bmiHeader.biPlanes = 1;
			bmi.bmiHeader.biBitCount = 32;
			bmi.bmiHeader.biCompression = BI_RGB;
			VOID *pBits;
			HBITMAP hbmp = CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, &pBits,
			NULL, 0);
			void *src = bm.bmBits
					+ (bm.bmHeight - imageInfo.rcImage.top - cy)
							* bm.bmWidthBytes + imageInfo.rcImage.left * 4,
					*dst = pBits;
			GetObjectW(hbmp, sizeof(bm2), &bm2);
			for (int j = 0; j < cy; j++) {
				memcpy(dst, src, cy * 4);
				dst += bm2.bmWidthBytes;
				src += bm.bmWidthBytes;
			}
			HBITMAP oldSrcBitmap = SelectObject(srcHdc, hbmp);
			BOOL ret = AlphaBlend(pimldp->hdcDst, pimldp->x, pimldp->y, _cx,
					_cy, srcHdc, 0, 0, _cx, _cy, blend);
			SelectObject(srcHdc, oldSrcBitmap);
			DeleteDC(srcHdc);
			DeleteObject(hbmp);
			return S_OK;
		}
	}
	return last_Draw(This, pimldp);
}
wresult w_imagelist_create(w_imagelist *imagelist, w_size *size,
		int initialCount) {
	w_imagelist_dispose(imagelist);
	_W_IMAGELIST(imagelist)->imagelist = ImageList_Create(size->width,
			size->height,
			ILC_COLOR32, initialCount, 4);
	if (_W_IMAGELIST(imagelist)->imagelist == 0) {
		return W_FALSE;
	} else {
		IImageList *list = (IImageList*) _W_IMAGELIST(imagelist)->imagelist;
		/*HIMAGELIST_QueryInterface(_W_IMAGELIST(imagelist)->imagelist,
		 &IID_IImageList, (void**) &list);*/
		if (list != 0) {
			if (w_IImageListVtbl.QueryInterface == 0) {
				memcpy(&w_IImageListVtbl, list->lpVtbl, sizeof(IImageListVtbl));
				last_Draw = w_IImageListVtbl.Draw;
				w_IImageListVtbl.Draw = w_IImageList_Draw;
			}
			list->lpVtbl = &w_IImageListVtbl;
		}
		return W_TRUE;
	}
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
	/*if (copy) {
	 _W_IMAGE(image)->handle = CopyIcon(icon);
	 _W_IMAGE(image)->type = _IMAGE_ICON;
	 _W_IMAGE(image)->nodispose = 0;
	 if (_W_IMAGE(image)->handle == 0)
	 return W_ERROR_NO_HANDLES;
	 } else {*/
	_W_IMAGE(image)->handle = icon;
	_W_IMAGE(image)->type = _IMAGE_ICON;
	_W_IMAGE(image)->nodispose = 1;
	//}
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
	IMAGELISTDRAWPARAMS imldp;
	memset(&imldp, 0, sizeof(imldp));
	imldp.cbSize = sizeof(imldp);
	imldp.himl = _W_IMAGELIST(imagelist)->imagelist;
	imldp.hdcDst = _W_GRAPHICS(graphics)->handle;
	imldp.i = index;
	imldp.x = pt->x;
	imldp.y = pt->y;
	imldp.fState = ILS_ALPHA | ILS_SATURATE;
	imldp.rgbBk = CLR_NONE;
	imldp.rgbFg = CLR_NONE;
	imldp.dwRop = SRCCOPY;
	imldp.Frame = 0xFF;
	switch (state) {
	case W_IMAGELIST_DRAW_TRANSPARENT:
		imldp.fStyle = ILD_NORMAL | ILD_TRANSPARENT;
		break;
	case W_IMAGELIST_DRAW_SELECTED:
		imldp.fStyle = ILD_SELECTED;
		break;
	case W_IMAGELIST_DRAW_FOCUSED:
		imldp.fStyle = ILD_FOCUS;
		break;
	case W_IMAGELIST_DRAW_NORMAL:
	default:
		imldp.fStyle = ILD_NORMAL;
		break;
	}
	ImageList_DrawIndirect(&imldp);
	return W_TRUE;
}
