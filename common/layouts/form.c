/*
 * form.c
 *
 *  Created on: 28 sept. 2020
 *      Author: azeddine
 */
#include <swtc.h>
void w_layout_form_do_layout_0(w_layout_form *layout, w_composite *composite,
		int move, w_rect *r, w_size *sz, int flushCache);
void w_layout_form_data_init(w_layout_form_data *formdata) {
	memset(formdata, 0, sizeof(w_layout_form_data));
	formdata->width = W_DEFAULT;
	formdata->height = W_DEFAULT;
	formdata->cacheWidth = -1;
	formdata->cacheHeight = -1;
	formdata->defaultWidth = -1;
	formdata->defaultHeight = -1;
	formdata->currentWidth = -1;
	formdata->currentHeight = -1;
}
void w_layout_form_init(w_layout_form *layout) {
	memset(&layout->marginWidth, 0, sizeof(w_layout_form) - sizeof(w_layout));
}
void w_layout_form_compute_size(w_layout *layout, w_composite *composite,
		w_size *size, int wHint, int hHint, int flushCache) {
	w_rect r;
	r.x = 0;
	r.y = 0;
	r.width = wHint;
	r.width = hHint;
	w_layout_form_do_layout_0((w_layout_form*) layout, composite, W_FALSE, &r,
			size, flushCache);
	if (wHint != W_DEFAULT)
		size->width = wHint;
	if (hHint != W_DEFAULT)
		size->height = hHint;
}
void w_layout_form_do_layout(w_layout *layout, w_composite *composite,
		int flushCache) {
	w_rect r;
	w_size sz;
	w_layout_form *lay = (w_layout_form*) layout;
	w_scrollable_get_client_area(W_SCROLLABLE(composite), &r);
	r.x += lay->marginLeft + lay->marginWidth;
	r.y += lay->marginTop + lay->marginHeight;
	r.width = WMAX(0,
			r.width - lay->marginLeft - 2 * lay->marginWidth
					- lay->marginRight);
	r.height = WMAX(0,
			r.height - lay->marginTop - 2 * lay->marginHeight
					- lay->marginBottom);
	w_layout_form_do_layout_0(lay, composite, W_TRUE, &r, &sz, flushCache);
}
void w_layout_stream_write_attachment(w_layout_stream *stream, wuchar pos,
		w_layout_form_attachment *att) {
	w_layout_stream_write_int(stream, pos, att->numerator);
	w_layout_stream_write_int(stream, pos + 2, att->denominator);
	w_layout_stream_write_int(stream, pos + 4, att->offset);
	w_layout_stream_write_int(stream, pos + 6, att->alignment);
}
void w_layout_stream_read_attachment(w_layout_stream *stream, wuchar pos,
		w_layout_form_attachment *att) {
	att->numerator = w_layout_stream_read_int(stream, pos);
	att->denominator = w_layout_stream_read_int(stream, pos + 2);
	att->offset = w_layout_stream_read_int(stream, pos + 4);
	att->alignment = w_layout_stream_read_int(stream, pos + 6);
}
void w_layout_form_set_data(w_layout *layout, w_control *control,
		const w_layout_data *_data) {
	w_layout_form_data *data = (w_layout_form_data*) _data;
	w_layout_stream stream;
	wuchar tmp[sizeof(w_layout_form_data)];
	wuint64 savedflags, ftmp;
	stream.sz = 0;
	stream.flags = 0;
	stream.tmp = tmp;
	w_layout_stream_write_int(&stream, 0, data->width + 1);
	w_layout_stream_write_int(&stream, 2, data->height + 1);
	/*
	 * caches
	 */
	w_layout_stream_write_int(&stream, 4, data->cacheWidth + 1);
	w_layout_stream_write_int(&stream, 6, data->cacheHeight + 1);
	w_layout_stream_write_int(&stream, 8, data->defaultWhint);
	w_layout_stream_write_int(&stream, 10, data->defaultHhint);
	w_layout_stream_write_int(&stream, 12, data->defaultWidth + 1);
	w_layout_stream_write_int(&stream, 14, data->defaultHeight + 1);
	w_layout_stream_write_int(&stream, 16, data->currentWhint);
	w_layout_stream_write_int(&stream, 18, data->currentHhint);
	w_layout_stream_write_int(&stream, 20, data->currentWidth + 1);
	w_layout_stream_write_int(&stream, 22, data->currentHeight + 1);
	/*
	 * attachement control
	 */
	w_layout_stream_write_ptr(&stream, 24, data->left.control);
	w_layout_stream_write_ptr(&stream, 25, data->right.control);
	w_layout_stream_write_ptr(&stream, 26, data->top.control);
	w_layout_stream_write_ptr(&stream, 27, data->bottom.control);
	w_layout_stream_write_ptr(&stream, 28, data->cacheLeft.control);
	w_layout_stream_write_ptr(&stream, 29, data->cacheRight.control);
	w_layout_stream_write_ptr(&stream, 30, data->cacheTop.control);
	w_layout_stream_write_ptr(&stream, 31, data->cacheBottom.control);
	/*
	 * attachement
	 */
	savedflags = stream.flags;
	stream.flags = 0;
	w_layout_stream_write_attachment(&stream, 0, &data->left);
	w_layout_stream_write_attachment(&stream, 8, &data->right);
	w_layout_stream_write_attachment(&stream, 16, &data->top);
	w_layout_stream_write_attachment(&stream, 24, &data->bottom);
	w_layout_stream_write_attachment(&stream, 32, &data->cacheLeft);
	w_layout_stream_write_attachment(&stream, 40, &data->cacheRight);
	w_layout_stream_write_attachment(&stream, 48, &data->cacheTop);
	w_layout_stream_write_attachment(&stream, 56, &data->cacheBottom);
	w_layout_sdata *d = w_layout_sdata_set(control, &stream, 13);
	if (d != 0) {
		*((int*) &d->tmp[0]) = savedflags;
		*((wuint64*) &d->tmp[4]) = stream.flags;
		d->tmp[12] = data->flags;
	}
}
void w_layout_form_get_data(w_layout *layout, w_control *control,
		w_layout_data *_data) {
	w_layout_form_data *data = (w_layout_form_data*) _data;
	w_layout_stream stream;
	w_layout_sdata *d = w_layout_sdata_get(control, &stream, 13);
	if (d != 0) {
		data->flags = d->tmp[12];
		data->data.size = d->sz;
		stream.flags = *((int*) &d->tmp[0]);
		data->width = w_layout_stream_read_int(&stream, 0) - 1;
		data->height = w_layout_stream_read_int(&stream, 2) - 1;
		/*
		 * caches
		 */
		data->cacheWidth = w_layout_stream_read_int(&stream, 4) - 1;
		data->cacheHeight = w_layout_stream_read_int(&stream, 6) - 1;
		data->defaultWhint = w_layout_stream_read_int(&stream, 8);
		data->defaultHhint = w_layout_stream_read_int(&stream, 10);
		data->defaultWidth = w_layout_stream_read_int(&stream, 12) - 1;
		data->defaultHeight = w_layout_stream_read_int(&stream, 14) - 1;
		data->currentWhint = w_layout_stream_read_int(&stream, 16);
		data->currentHhint = w_layout_stream_read_int(&stream, 18);
		data->currentWidth = w_layout_stream_read_int(&stream, 20) - 1;
		data->currentHeight = w_layout_stream_read_int(&stream, 22) - 1;
		/*
		 * attachement control
		 */
		data->left.control = w_layout_stream_read_ptr(&stream, 24);
		data->right.control = w_layout_stream_read_ptr(&stream, 25);
		data->top.control = w_layout_stream_read_ptr(&stream, 26);
		data->bottom.control = w_layout_stream_read_ptr(&stream, 27);
		data->cacheLeft.control = w_layout_stream_read_ptr(&stream, 28);
		data->cacheRight.control = w_layout_stream_read_ptr(&stream, 29);
		data->cacheTop.control = w_layout_stream_read_ptr(&stream, 30);
		data->cacheBottom.control = w_layout_stream_read_ptr(&stream, 31);
		/*
		 * attachement
		 */
		stream.flags = *((wuint64*) &d->tmp[4]);
		w_layout_stream_read_attachment(&stream, 0, &data->left);
		w_layout_stream_read_attachment(&stream, 8, &data->right);
		w_layout_stream_read_attachment(&stream, 16, &data->top);
		w_layout_stream_read_attachment(&stream, 24, &data->bottom);
		w_layout_stream_read_attachment(&stream, 32, &data->cacheLeft);
		w_layout_stream_read_attachment(&stream, 40, &data->cacheRight);
		w_layout_stream_read_attachment(&stream, 48, &data->cacheTop);
		w_layout_stream_read_attachment(&stream, 56, &data->cacheBottom);
	}
}

_w_layout_class w_layout_form_class = { //
		w_layout_form_compute_size, //
				w_layout_flush_cache0, //
				w_layout_form_do_layout, //
				w_layout_form_set_data, //
				w_layout_form_get_data, //
		};
void w_form_attachment_divide(w_layout_form_attachment *_this,
		w_layout_form_attachment *result, int value) {
	result->numerator = _this->numerator;
	result->denominator = _this->denominator * value;
	result->offset = _this->offset / value;
	result->control = 0;
	result->alignment = 0;
}

int w_form_attachment_gcd(int m, int n) {
	int temp;
	m = abs(m);
	n = abs(n);
	if (m < n) {
		temp = m;
		m = n;
		n = temp;
	}
	while (n != 0) {
		temp = m;
		m = n;
		n = temp % n;
	}
	return m;
}

void w_form_attachment_minus(w_layout_form_attachment *_this,
		w_layout_form_attachment *attachment,
		w_layout_form_attachment *solution) {
	solution->numerator = _this->numerator * attachment->denominator
			- _this->denominator * attachment->numerator;
	solution->denominator = _this->denominator * attachment->denominator;
	int _gcd = w_form_attachment_gcd(solution->denominator,
			solution->numerator);
	solution->numerator = solution->numerator / _gcd;
	solution->denominator = solution->denominator / _gcd;
	solution->offset = _this->offset - attachment->offset;
	solution->control = 0;
	solution->alignment = 0;
}

void w_form_attachment_minus_0(w_layout_form_attachment *_this,
		w_layout_form_attachment *result, int value) {
	result->numerator = _this->numerator;
	result->denominator = _this->denominator;
	result->offset = _this->offset - value;
	result->control = 0;
	result->alignment = 0;
}

void w_form_attachment_plus(w_layout_form_attachment *_this,
		w_layout_form_attachment *attachment,
		w_layout_form_attachment *solution) {
	solution->numerator = _this->numerator * attachment->denominator
			+ _this->denominator * attachment->numerator;
	solution->denominator = _this->denominator * attachment->denominator;
	int _gcd = w_form_attachment_gcd(solution->denominator,
			solution->numerator);
	solution->numerator = solution->numerator / _gcd;
	solution->denominator = solution->denominator / _gcd;
	solution->offset = _this->offset + attachment->offset;
	solution->control = 0;
	solution->alignment = 0;
}

void w_form_attachment_plus_0(w_layout_form_attachment *_this,
		w_layout_form_attachment *result, int value) {
	result->numerator = _this->numerator;
	result->denominator = _this->denominator;
	result->offset = _this->offset + value;
	result->control = 0;
	result->alignment = 0;
}

int w_form_attachment_solveX(w_layout_form_attachment *_this, int value) {
	return ((_this->numerator * value) / _this->denominator) + _this->offset;
}

int w_form_attachment_solveY(w_layout_form_attachment *_this, int value) {
	return (value - _this->offset) * _this->denominator / _this->numerator;
}
void w_layout_form_do_layout_0(w_layout_form *layout, w_composite *composite,
		int move, w_rect *r, w_size *sz, int flushCache) {

}
