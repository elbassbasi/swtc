/*
 * Image.h
 *
 *  Created on: 17 sept. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_GRAPHICS_IMAGE_H_
#define SWTP_GRAPHICS_IMAGE_H_
#include "Color.h"
class WGraphics;
class WToolkit;
/**
 * Instances of this class are graphics which have been prepared
 * for display on a specific device. That is, they are ready
 * to paint using methods such as <code>GC.drawImage()</code>
 * and display on widgets with, for example, <code>Button.setImage()</code>.
 * <p>
 * If loaded from a file format that supports it, an
 * <code>Image</code> may have transparency, meaning that certain
 * pixels are specified as being transparent when drawn. Examples
 * of file formats that support transparency are GIF and PNG.
 * </p><p>
 * There are two primary ways to use <code>Images</code>.
 * The first is to load a graphic file from disk and create an
 * <code>Image</code> from it. This is done using an <code>Image</code>
 * constructor, for example:
 * <pre>
 *    Image i = new Image(device, "C:\\graphic.bmp");
 * </pre>
 * A graphic file may contain a color table specifying which
 * colors the image was intended to possess. In the above example,
 * these colors will be mapped to the closest available color in
 * SWT. It is possible to get more control over the mapping of
 * colors as the image is being created, using code of the form:
 * <pre>
 *    ImageData data = new ImageData("C:\\graphic.bmp");
 *    RGB[] rgbs = data.getRGBs();
 *    // At this point, rgbs contains specifications of all
 *    // the colors contained within this image. You may
 *    // allocate as many of these colors as you wish by
 *    // using the Color constructor Color(RGB), then
 *    // create the image:
 *    Image i = new Image(device, data);
 * </pre>
 * <p>
 * Applications which require even greater control over the image
 * loading process should use the support provided in class
 * <code>ImageLoader</code>.
 * </p><p>
 * Application code must explicitly invoke the <code>Image.dispose()</code>
 * method to release the operating system resources managed by each instance
 * when those instances are no longer required.
 * </p>
 *
 * @see Color
 */
class WImage: public WResource {
public:
	WImage() {
		w_image_init(W_IMAGE(this));
	}
	~WImage() {
		w_image_dispose(W_IMAGE(this));
	}
	void Dispose() {
		w_image_dispose(W_IMAGE(this));
	}
	bool Create(const char *file) {
		return w_image_create_from_file(W_IMAGE(this), file, -1,
				W_ENCODING_UTF8) > 0;
	}
	bool Create(const void *buffer, size_t size) {
		return w_image_create_from_buffer(W_IMAGE(this), buffer, size) > 0;
	}
	bool Create(void *data, wuint format, int width, int height,
			int rowstride) {
		WSize size(width, height);
		return w_image_create_from_data(W_IMAGE(this), data, format,
				(w_size*) &size, rowstride) > 0;
	}
	bool Create(void *data, wuint format, int width, int height) {
		return Create(data, format, width, height, -1);
	}
	bool CreateARGB32(w_color *colors, int width, int height) {
		return Create(colors, W_IMAGE_ARGB32, width, height);
	}
	bool Create(WImage &from) {
		return from.CopyTo(*this);
	}
	bool Create(WImage &from, const WRect &rect) {
		return from.CopyArea(*this, rect);
	}
	bool IsOk() {
		return w_image_is_ok(W_IMAGE(this));
	}
	bool IsDisposed() {
		return !IsOk();
	}
	void GetSize(WSize &size) {
		w_image_get_size(W_IMAGE(this), (w_size*) &size);
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
	bool GetPixels(int *colors, size_t length) {
		return w_image_get_pixels(W_IMAGE(this), (w_color*) colors, length) > 0;
	}
	bool Save(const char *file, const char *type) {
		return w_image_save_to_file(W_IMAGE(this), file, -1, W_ENCODING_UTF8,
				type) > 0;
	}
	bool Save(w_alloc alloc, void *user_data, const char *type) {
		return w_image_save_to_buffer(W_IMAGE(this), alloc, user_data, type) > 0;
	}
	bool Save(IWStream *stream, const char *type) {
		return w_image_save_to_stream(W_IMAGE(this), (w_stream*) stream, type)
				> 0;
	}
	bool CopyArea(WImage &to, const WRect &rect) {
		return w_image_copy(W_IMAGE(this), (w_rect*) &rect, W_IMAGE(&to)) > 0;
	}
	bool CopyArea(WImage &to, int x, int y, int width, int height) {
		WRect rect(x, y, width, height);
		return CopyArea(to, rect);
	}
	bool CopyTo(WImage &to) {
		WRect rect(0, 0, -1, -1);
		return CopyArea(to, rect);
	}
	bool Resize(WImage &to, const WSize &size) {
		return w_image_resize(W_IMAGE(this), (w_size*) &size, W_IMAGE(&to)) > 0;
	}
	bool Resize(WImage &to, int width, int height) {
		return Resize(to, WSize(width, height));
	}
	bool ToGrayScale(WImage &to) {
		return w_image_to_greyscale(W_IMAGE(this), W_IMAGE(&to)) > 0;
	}
	bool ToDisabled(WImage &to, w_color bg, w_color fg) {
		return w_image_to_disabled(W_IMAGE(this), bg, fg, W_IMAGE(&to)) > 0;
	}
private:
	void *handle[sizeof(w_image) / sizeof(void*)];
};
class WImageMatrix {
public:
	wuint width;
	wuint heigth;
	w_color *colors;
	WImageMatrix() {
		this->width = 0;
		this->heigth = 0;
		this->colors = 0;
	}
	WImageMatrix(wuint width, wuint heigth) {
		this->width = width;
		this->heigth = heigth;
		this->colors = new w_color[width * heigth];
	}
	~WImageMatrix() {
		if (colors != 0)
			delete[] colors;
	}
	w_color Get(wuint i, wuint j) {
		return colors[i * heigth + j];
	}
	void Set(wuint i, wuint j, w_color color) {
		colors[i * heigth + j] = color;
	}
	bool CreateImage(WImage &image) {
		return image.CreateARGB32(colors, width, heigth);
	}
};
class WSurface: public WResource {
public:
	WSurface() {
		w_surface_init(W_SURFACE(this));
	}
	WSurface(int width, int height) {
		w_surface_init(W_SURFACE(this));
		Create(width, height);
	}
	WSurface(const WSize &size) {
		w_surface_init(W_SURFACE(this));
		Create(size);
	}
	~ WSurface() {
		w_surface_dispose(W_SURFACE(this));
	}
	void Dispose() {
		w_surface_dispose(W_SURFACE(this));
	}
	bool Create(const WSize &size) {
		return w_surface_create(W_SURFACE(this), (w_size*) &size) > 0;
	}
	/**
	 * Constructs an empty instance of this class with the
	 * specified width and height. The result may be drawn upon
	 * by creating a GC and using any of its drawing operations,
	 * as shown in the following example:
	 * <pre>
	 *    WSurface surface;
	 *    WImage image;
	 *    WGraphics gc;
	 *    surface.Create(width, height);
	 *    surface.GetGraphics(gc);
	 *    gc.DrawRectangle(0, 0, 50, 50);
	 *    surface.ToImage(image);
	 * </pre>
	 * <p>
	 * for drawing transparent image you need advanced graphics
	 * <pre>
	 *    WSurface surface;
	 *    WImage image;
	 *    WGraphics gc;
	 *    surface.Create(width, height);
	 *    surface.GetGraphics(gc);
	 *    gc.SetAlpha(0);
	 *    gc.FillRectangle(0, 0, width, height);
	 *    gc.SetAlpha(0xff);
	 *    gc.DrawRectangle(0, 0, 50, 50);
	 *    surface.GetImage(image);
	 * </pre>
	 * <p>
	 * Note: Some platforms may have a limitation on the size
	 * of image that can be created (size depends on width, height,
	 * and depth). For example, Windows 95, 98, and ME do not allow
	 * images larger than 16M.
	 * </p>
	 * <p>
	 * You must dispose the image when it is no longer required.
	 * </p>
	 *
	 * @param width the width of the new image
	 * @param height the height of the new image
	 *
	 * @see #dispose()
	 */
	bool Create(int width, int height) {
		return Create(WSize(width, height));
	}
	bool IsOk() {
		return w_surface_is_ok(W_SURFACE(this));
	}
	bool GetImage(WImage &to) {
		return w_surface_get_image(W_SURFACE(this), W_IMAGE(&to)) > 0;
	}
	bool GetGraphics(WGraphics &graphics) {
		return w_surface_get_graphics(W_SURFACE(this), W_GRAPHICS(&graphics))
				> 0;
	}
private:
	void *handle[sizeof(w_surface) / sizeof(void*)];
};

#endif /* SWTP_GRAPHICS_IMAGE_H_ */
