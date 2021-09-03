/*
 * ImageList.h
 *
 *  Created on: 17 sept. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_GRAPHICS_IMAGELIST_H_
#define SWTP_GRAPHICS_IMAGELIST_H_
#include "Image.h"
class WImageList: public WResource {
protected:
	int Add(WImage *image) {
		return w_imagelist_add(W_IMAGELIST(this), W_IMAGE(image));
	}
	int Replace(int index, WImage *image) {
		return w_imagelist_replace(W_IMAGELIST(this), index, W_IMAGE(image));
	}
	bool GetImage(int index, bool copy, WImage *image) {
		return w_imagelist_get_image(W_IMAGELIST(this), index, copy,
				W_IMAGE(image)) > 0;
	}
public:
	WImageList() {
		w_imagelist_init(W_IMAGELIST(this));
	}
	~WImageList() {
		w_imagelist_dispose(W_IMAGELIST(this));
	}
	void Dispose() {
		w_imagelist_dispose(W_IMAGELIST(this));
	}
	bool Create(const WSize &size, int initialCount = 8) {
		return w_imagelist_create(W_IMAGELIST(this), (w_size*) &size,
				initialCount) > 0;
	}
	bool Create(int width, int height, int initialCount = 8) {
		return Create(WSize(width, height), initialCount);
	}
	int Add(WImage &image) {
		return Add(&image);
	}
	int AddMimeType(const char *mimetype, int flags) {
		return w_imagelist_add_mimetype(W_IMAGELIST(this), mimetype, flags);
	}
	int AddMimeType(const char *mimetype) {
		return AddMimeType(mimetype, W_IMAGELIST_MIME_DEFAULT);
	}
	int AddMimeExtension(const char *ext) {
		return AddMimeType(ext, W_IMAGELIST_MIME_EXTENSION);
	}
	int AddMimeDirectory() {
		return AddMimeType(0, W_IMAGELIST_MIME_DIRECTORY);
	}
	bool IsOk() {
		return w_imagelist_is_ok(W_IMAGELIST(this));
	}
	int Replace(int index, WImage &image) {
		return Replace(index, &image);
	}
	bool Remove(int index) {
		return w_imagelist_remove(W_IMAGELIST(this), index) > 0;
	}
	bool RemoveAll() {
		return w_imagelist_remove_all(W_IMAGELIST(this));
	}
	bool GetImage(int index, WImage &image, bool copy = true) {
		return GetImage(index, copy, &image);
	}
	bool CopyImage(int index, WImage &image) {
		return GetImage(index, true, &image);
	}
	void GetSize(WSize &size) {
		w_imagelist_get_size(W_IMAGELIST(this), (w_size*) &size);
	}
	WSize GetSize() {
		WSize sz;
		GetSize(sz);
		return sz;
	}
	int GetWidth() {
		WSize sz;
		GetSize(sz);
		return sz.width;
	}
	int GetHeight() {
		WSize sz;
		GetSize(sz);
		return sz.height;
	}
	int GetCount() {
		return w_imagelist_get_count(W_IMAGELIST(this));
	}
	wresult _Draw(WGraphics &gc, int index, const WPoint &location, int flags) {
		return w_imagelist_draw(W_IMAGELIST(this), W_GRAPHICS(&gc), index,
				(w_point*) &location, flags);
	}
	bool Draw(WGraphics &gc, int index, const WPoint &location, int flags) {
		return _Draw(gc, index, location, flags) > 0;
	}
	bool Draw(WGraphics &gc, int index, int x, int y, int flags =
			W_IMAGELIST_DRAW_NORMAL) {
		return Draw(gc, index, WPoint(x, y), flags);
	}
	bool DrawNormal(WGraphics &gc, int index, int x, int y) {
		return Draw(gc, index, x, y, W_IMAGELIST_DRAW_NORMAL);
	}
	bool DrawTransparent(WGraphics &gc, int index, int x, int y) {
		return Draw(gc, index, x, y, W_IMAGELIST_DRAW_TRANSPARENT);
	}
	bool DrawSelected(WGraphics &gc, int index, int x, int y) {
		return Draw(gc, index, x, y, W_IMAGELIST_DRAW_SELECTED);
	}
	bool DrawFocused(WGraphics &gc, int index, int x, int y) {
		return Draw(gc, index, x, y, W_IMAGELIST_DRAW_FOCUSED);
	}
private:
	void *handle[sizeof(w_imagelist) / sizeof(void*)];
};

#endif /* SWTP_GRAPHICS_IMAGELIST_H_ */
