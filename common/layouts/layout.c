/*
 * layout.c
 *
 *  Created on: 5 août 2019
 *      Author: azeddine El Bassbasi
 */
#include <swtc.h>
void w_layout_compute_size(w_layout *layout, struct w_composite *composite,
		w_size *size, int wHint, int hHint, int flushCache) {
	if (layout != 0 && layout->clazz != 0) {
		layout->clazz->compute_size(layout, composite, size, wHint, hHint,flushCache);
	}
}
wresult w_layout_flush_cache(w_layout *layout, w_control *control) {
	if (layout != 0 && layout->clazz != 0) {
		return layout->clazz->flush_cache(layout, control);
	} else
		return W_FALSE;
}
wresult w_layout_flush_cache0(w_layout *_layout, w_control *control){
	return W_FALSE;
}
void w_layout_do_layout(w_layout *layout, struct w_composite *composite, int flushCache) {
	if (layout != 0 && layout->clazz != 0) {
		layout->clazz->do_layout(layout, composite,flushCache);
	}
}
void w_layout_set_data(w_layout *layout, w_control *control,
		const w_layout_data *data) {
	if (layout != 0 && layout->clazz != 0) {
		layout->clazz->set_data(layout, control, data);
	}
}
void w_layout_get_data(w_layout *layout, w_control *control,
		w_layout_data *data) {
	if (layout != 0 && layout->clazz != 0) {
		layout->clazz->get_data(layout, control, data);
	}
}

void w_layout_stream_write_int(w_layout_stream *stream, wuchar pos, int value) {
	if (value == 0) {
		//stream->flags |= (0 << pos);
		return;
	} else if (value <= 0xFF) {
		stream->flags |= (1 << pos);
		stream->tmp[stream->sz] = value;
		stream->sz += 1;
		return;
	} else if (value <= 0xFFFF) {
		stream->flags |= (2 << pos);
		*((short*) &stream->tmp[stream->sz]) = value;
		stream->sz += sizeof(short);
		return;
	} else {
		stream->flags |= (3 << pos);
		*((int*) &stream->tmp[stream->sz]) = value;
		stream->sz += sizeof(int);
		return;
	}
}
void w_layout_stream_write_ptr(w_layout_stream *stream, wuchar pos, void *ptr) {
	if (ptr == 0) {
		//stream->flags |= (0 << pos);
		return;
	} else {
		stream->flags |= (1 << pos);
		*((void**) &stream->tmp[stream->sz]) = ptr;
		stream->sz += sizeof(void*);
	}
}
int w_layout_stream_read_int(w_layout_stream *stream, wuchar pos) {
	char nb_bytes = (stream->flags >> pos) & 3;
	int value;
	switch (nb_bytes) {
	case 0:
		return 0;
		break;
	case 1:
		value = stream->tmp[stream->sz];
		stream->sz++;
		break;
	case 2:
		value = *((short*) &stream->tmp[stream->sz]);
		stream->sz += 2;
		break;
	default:
		value = *((int*) &stream->tmp[stream->sz]);
		stream->sz += 4;
		break;
	}
	return value;
}
void* w_layout_stream_read_ptr(w_layout_stream *stream, wuchar pos) {
	if (stream->flags & (1 << pos)) {
		void *ptr = *((void**) &stream->tmp[stream->sz]);
		stream->sz += sizeof(void*);
		return ptr;
	} else
		return 0;
}
w_layout_sdata* w_layout_sdata_set(w_control *control, w_layout_stream *stream,
		size_t flags_size) {
	w_layout_sdata *d;
	w_control_new_layout_data(control, (void**) &d,
			stream->sz + sizeof(w_layout_sdata) + flags_size);
	if (d != 0) {
		d->sz = stream->sz;
		size_t fsz = flags_size < 8 ? flags_size : 8;
		memcpy(&d->tmp[0], &stream->flags, fsz);
		memcpy(&d->tmp[flags_size], stream->tmp, stream->sz);
	}
	return d;
}
w_layout_sdata* w_layout_sdata_get(w_control *control, w_layout_stream *stream,
		size_t flags_size) {
	w_layout_sdata *d;
	w_control_get_layout_data(control, (void**) &d);
	if (d != 0) {
		stream->sz = 0;
		stream->tmp = &d->tmp[flags_size];
		stream->flags = 0;
		size_t fsz = flags_size < 8 ? flags_size : 8;
		memcpy(&stream->flags, &d->tmp[0], fsz);
	}
	return d;
}
