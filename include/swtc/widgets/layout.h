/*
 * Name:        layout.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef SWT_WIDGETS_LAYOUT_H_
#define SWT_WIDGETS_LAYOUT_H_
#ifdef __cplusplus
extern "C" {
#endif
#include "control.h"
typedef struct w_sash w_sash;
typedef struct w_layout {
	void *reserved; //private used in c++
	struct _w_layout_class *clazz;

} w_layout;

typedef struct _w_layout_class {
	void (*compute_size)(w_layout *layout, w_composite *composite, w_size *size,
			int wHint, int hHint, int flushCache);
	wresult (*flush_cache)(w_layout *_layout, w_control *control);
	void (*do_layout)(w_layout *_layout, w_composite *composite,
			int flushCache);
	void (*set_data)(w_layout *layout, w_control *control,
			const w_layout_data *data);
	void (*get_data)(w_layout *layout, w_control *control, w_layout_data *data);
} _w_layout_class;
SWT_PUBLIC void w_layout_compute_size(w_layout *layout, w_composite *composite,
		w_size *size, int wHint, int hHint, int flushCache);
SWT_PUBLIC wresult w_layout_flush_cache(w_layout *_layout, w_control *control);
SWT_PUBLIC wresult w_layout_flush_cache0(w_layout *_layout, w_control *control);
SWT_PUBLIC void w_layout_do_layout(w_layout *_layout, w_composite *composite,
		int flushCache);
SWT_PUBLIC void w_layout_set_data(w_layout *layout, w_control *control,
		const w_layout_data *data);
SWT_PUBLIC void w_layout_get_data(w_layout *layout, w_control *control,
		w_layout_data *data);
/*
 * FillLayout
 */
typedef struct w_layout_fill {
	w_layout layout;
	int type;
	int marginWidth;
	int marginHeight;
	int spacing;
} w_layout_fill;
SWT_PUBLIC void w_layout_fill_init(w_layout_fill *layout, int type);
SWT_PUBLIC void w_layout_fill_compute_size(w_layout *layout,
		struct w_composite *composite, w_size *size, int wHint, int hHint,
		int flushCache);
SWT_PUBLIC void w_layout_fill_do_layout(w_layout *_layout,
		struct w_composite *composite, int flushCache);
/*
 * RowLayout
 */
typedef struct w_layout_row {
	w_layout layout;
	int type;
	union {
		struct {
			unsigned wrap :1;
			unsigned pack :1;
			unsigned fill :1;
			unsigned center :1;
			unsigned justify :1;
		};
		int flags;
	};
	int marginWidth;
	int marginHeight;
	int spacing;
	int marginLeft;
	int marginTop;
	int marginRight;
	int marginBottom;
	int reserved[5];
} w_layout_row;
typedef struct w_layout_row_data {
	w_layout_data data;
	union {
		struct {
			unsigned exclude :1;
		};
		wushort flags;
	};
	int width;
	int height;
} w_layout_row_data;
SWT_PUBLIC extern _w_layout_class w_layout_row_class;
SWT_PUBLIC void w_layout_row_data_init(w_layout_row_data *rowdata);
SWT_PUBLIC void w_layout_row_init(w_layout_row *layout, int type);
SWT_PUBLIC void w_layout_row_compute_size(w_layout *layout,
		struct w_composite *composite, w_size *size, int wHint, int hHint,
		int flushCache);
SWT_PUBLIC void w_layout_row_do_layout(w_layout *layout,
		struct w_composite *composite, int flushCache);
SWT_PUBLIC void w_layout_row_set_data(w_layout *layout, w_control *control,
		const w_layout_data *_data);
SWT_PUBLIC void w_layout_row_get_data(w_layout *layout, w_control *control,
		w_layout_data *_data);
/*
 * GridLayout
 */
enum {
	W_GRID_BEGINNING = 0,
	W_GRID_CENTER = 1,
	W_GRID_END = 2,
	W_GRID_FILL = 3,
	W_GRID_VBEGINNING = W_GRID_BEGINNING << 0,
	W_GRID_VCENTER = W_GRID_CENTER << 0,
	W_GRID_VEND = W_GRID_END << 0,
	W_GRID_VFILL = W_GRID_FILL << 0,
	W_GRID_HBEGINNING = W_GRID_BEGINNING << 2,
	W_GRID_HCENTER = W_GRID_CENTER << 2,
	W_GRID_HEND = W_GRID_END << 2,
	W_GRID_HFILL = W_GRID_FILL << 2,
	W_GRID_GRAB_HORIZONTAL = 1 << 4,
	W_GRID_GRAB_VERTICAL = 1 << 5,
	W_GRID_EXCLUDE = 1 << 6,

	W_GRID_VERTICAL_ALIGN_BEGINNING = W_GRID_VBEGINNING,
	W_GRID_VERTICAL_ALIGN_CENTER = W_GRID_VCENTER,
	W_GRID_VERTICAL_ALIGN_END = W_GRID_VEND,
	W_GRID_VERTICAL_ALIGN_FILL = W_GRID_VFILL,
	W_GRID_HORIZONTAL_ALIGN_BEGINNING = W_GRID_HBEGINNING | W_GRID_VCENTER,
	W_GRID_HORIZONTAL_ALIGN_CENTER = W_GRID_HCENTER | W_GRID_VCENTER,
	W_GRID_HORIZONTAL_ALIGN_END = W_GRID_HEND | W_GRID_VCENTER,
	W_GRID_HORIZONTAL_ALIGN_FILL = W_GRID_HFILL | W_GRID_VCENTER,
	W_GRID_FILL_VERTICAL = W_GRID_VERTICAL_ALIGN_FILL | W_GRID_GRAB_VERTICAL,
	W_GRID_FILL_HORIZONTAL = W_GRID_HORIZONTAL_ALIGN_FILL
			| W_GRID_GRAB_HORIZONTAL,
	W_GRID_FILL_BOTH = W_GRID_FILL_VERTICAL | W_GRID_HFILL
			| W_GRID_GRAB_HORIZONTAL,
};
typedef struct w_layout_grid {
	w_layout layout;
	int numColumns;
	unsigned makeColumnsEqualWidth :1;
	int marginWidth;
	int marginHeight;
	int marginLeft;
	int marginTop;
	int marginRight;
	int marginBottom;
	int horizontalSpacing;
	int verticalSpacing;
	int reserved[5];
} w_layout_grid;
typedef struct w_layout_grid_data {
	w_layout_data data;
	union {
		struct {
			unsigned verticalAlignment :2;
			unsigned horizontalAlignment :2;
			unsigned grabExcessHorizontalSpace :1;
			unsigned grabExcessVerticalSpace :1;
			unsigned exclude :1;
		};
		wushort flags;
	};
	int widthHint;
	int heightHint;
	int horizontalIndent;
	int verticalIndent;
	int horizontalSpan;
	int verticalSpan;
	int minimumWidth;
	int minimumHeight;
	/*
	 * caches
	 */
	int cacheWidth;
	int cacheHeight;
	int defaultWhint;
	int defaultHhint;
	int defaultWidth;
	int defaultHeight;
	int currentWhint;
	int currentHhint;
	int currentWidth;
	int currentHeight;
} w_layout_grid_data;
SWT_PUBLIC extern _w_layout_class w_layout_grid_class;
SWT_PUBLIC void w_layout_grid_data_init(w_layout_grid_data *griddata);
SWT_PUBLIC void w_layout_grid_init(w_layout_grid *layout, int numColumns,
		int makeColumnsEqualWidth);
SWT_PUBLIC void w_layout_grid_compute_size(w_layout *layout,
		struct w_composite *composite, w_size *size, int wHint, int hHint,
		int flushCache);
SWT_PUBLIC void w_layout_grid_do_layout(w_layout *layout,
		struct w_composite *composite, int flushCache);
SWT_PUBLIC void w_layout_grid_set_data(w_layout *layout, w_control *control,
		const w_layout_data *_data);
SWT_PUBLIC void w_layout_grid_get_data(w_layout *layout, w_control *control,
		w_layout_data *_data);
/*
 * FormLayout
 */
typedef struct w_layout_form {
	w_layout layout;
	int marginWidth;
	int marginHeight;
	int marginLeft;
	int marginTop;
	int marginRight;
	int marginBottom;
	int spacing;
	int reserved[5];
} w_layout_form;
typedef struct w_layout_form_attachment {
	w_control *control;
	int numerator;
	int denominator;
	int offset;
	int alignment;
} w_layout_form_attachment;
typedef struct w_layout_form_data {
	w_layout_data data;
	union {
		struct {
			unsigned isVisited :1;
			unsigned needed :1;
		};
		wushort flags;
	};
	int width;
	int height;
	w_layout_form_attachment left;
	w_layout_form_attachment right;
	w_layout_form_attachment top;
	w_layout_form_attachment bottom;
	/*
	 * caches
	 */
	int cacheWidth;
	int cacheHeight;
	int defaultWhint;
	int defaultHhint;
	int defaultWidth;
	int defaultHeight;
	int currentWhint;
	int currentHhint;
	int currentWidth;
	int currentHeight;
	w_layout_form_attachment cacheLeft;
	w_layout_form_attachment cacheRight;
	w_layout_form_attachment cacheTop;
	w_layout_form_attachment cacheBottom;
} w_layout_form_data;
SWT_PUBLIC extern _w_layout_class w_layout_form_class;
SWT_PUBLIC void w_layout_form_data_init(w_layout_form_data *formdata);
SWT_PUBLIC void w_layout_form_init(w_layout_form *layout);
SWT_PUBLIC void w_layout_form_compute_size(w_layout *layout,
		w_composite *composite, w_size *size, int wHint, int hHint,
		int flushCache);
SWT_PUBLIC void w_layout_form_do_layout(w_layout *layout,
		w_composite *composite, int flushCache);
SWT_PUBLIC void w_layout_form_set_data(w_layout *layout, w_control *control,
		const w_layout_data *_data);
SWT_PUBLIC void w_layout_form_get_data(w_layout *layout, w_control *control,
		w_layout_data *_data);
/*
 * SashLayout
 */
typedef struct w_layout_sash {
	w_layout layout;
	w_sash *sashes;
	wuint64 style;
	size_t count;
} w_layout_sash;
typedef struct w_layout_sash_data {
	w_layout_data data;
	wushort flags;
	wushort index;
	size_t weight;
} w_layout_sash_data;
SWT_PUBLIC void w_layout_sash_init(w_layout_sash *layout, int style);
SWT_PUBLIC void w_layout_sash_free(w_layout_sash *layout);
SWT_PUBLIC void w_layout_sash_set_sashes(w_layout_sash *layout,
		struct w_sash *sashes, size_t count);
SWT_PUBLIC void w_layout_sash_set_weights(w_layout_sash *layout,
		w_composite *composite, int *weights, size_t count);
SWT_PUBLIC void w_layout_sash_compute_size(w_layout *layout,
		w_composite *composite, w_size *size, int wHint, int hHint,
		int flushCache);
SWT_PUBLIC void w_layout_sash_do_layout(w_layout *_layout,
		w_composite *composite, int flushCache);
SWT_PUBLIC int w_layout_sash_default_selection(w_widget *widget,
		w_event *event);
/*
 *
 */
typedef struct w_layout_stream {
	size_t sz;
	wuint64 flags;
	wuchar *tmp;
} w_layout_stream;
typedef struct w_layout_sdata {
	wushort sz;
	wushort id;
	wuchar tmp[0];
} w_layout_sdata;
SWT_PUBLIC void w_layout_stream_write_int(w_layout_stream *stream, wuchar pos,
		int value);
SWT_PUBLIC void w_layout_stream_write_ptr(w_layout_stream *stream, wuchar pos,
		void *ptr);
SWT_PUBLIC int w_layout_stream_read_int(w_layout_stream *stream, wuchar pos);
SWT_PUBLIC void* w_layout_stream_read_ptr(w_layout_stream *stream, wuchar pos);
SWT_PUBLIC w_layout_sdata* w_layout_sdata_set(w_control *control,
		w_layout_stream *stream, size_t flags_size);
SWT_PUBLIC w_layout_sdata* w_layout_sdata_get(w_control *control,
		w_layout_stream *stream, size_t flags_size);
#ifdef __cplusplus
}
#endif
#endif /* SWT_WIDGETS_LAYOUT_H_ */
