/*
 * Name:        stream.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "gc.h"
struct _WInputStream {
	GInputStream parent_instance;
	w_stream *stream;
	int flags;
};
void w_input_stream_finalize(GObject *object) {
}
gssize w_input_stream_read_fn(GInputStream *stream, void *buffer, gsize count,
		GCancellable *cancellable, GError **error) {
	struct _WInputStream *_s = (struct _WInputStream*) stream;
	w_stream *_stream = _s->stream;
	if (_stream != 0 && _stream->clazz != 0) {
		size_t pcbRead = 0;
		wresult result = _stream->clazz->read(_stream, buffer, count, &pcbRead);
		if (result > 0) {
			return pcbRead;
		}
	}
	return 0;
}
gssize w_input_stream_skip(GInputStream *stream, gsize count,
		GCancellable *cancellable, GError **error) {
	struct _WInputStream *_s = (struct _WInputStream*) stream;
	w_stream *_stream = _s->stream;
	if (_stream != 0 && _stream->clazz != 0) {
		wresult result = _stream->clazz->seek(_stream, count, SEEK_SET);
		if (result > 0) {
			return count;
		}
	}
	return 0;
}
gboolean w_input_stream_close_fn(GInputStream *stream,
		GCancellable *cancellable, GError **error) {
	struct _WInputStream *_s = (struct _WInputStream*) stream;
	return TRUE;
}
void w_input_stream_init(struct _WInputStream *stream) {
	stream->stream = 0;
}
void w_input_stream_class_init(GMemoryInputStreamClass *klass) {
	GObjectClass *object_class;
	GInputStreamClass *istream_class;

	object_class = G_OBJECT_CLASS(klass);
	//object_class->finalize     = w_input_stream_finalize;

	istream_class = G_INPUT_STREAM_CLASS(klass);
	istream_class->read_fn = w_input_stream_read_fn;
	istream_class->skip = w_input_stream_skip;
	istream_class->close_fn = w_input_stream_close_fn;

	/*istream_class->skip_async  = w_input_stream_skip_async;
	 istream_class->skip_finish  = w_input_stream_skip_finish;
	 istream_class->close_async = w_input_stream_close_async;
	 istream_class->close_finish = w_input_stream_close_finish;*/
}
GType _w_input_stream_type = 0;
GInputStream* w_input_stream_create(w_stream *stream, int flags) {
	if (_w_input_stream_type == 0) {
		GTypeInfo fixed_info = { sizeof(GInputStreamClass),
		NULL, /* base_init */
		NULL, /* base_finalize */
		(GClassInitFunc) w_input_stream_class_init,
		NULL, /* class_finalize */
		NULL, /* class_data */
		sizeof(struct _WInputStream), 0, /* n_preallocs */
		(GInstanceInitFunc) w_input_stream_init, NULL };
		//GType parent_class = GTK_TYPE_CONTAINER;

		_w_input_stream_type = g_type_register_static(G_TYPE_INPUT_STREAM,
				"w_input_stream", &fixed_info, (GTypeFlags) 0);
		/*GInterfaceInfo interface_info = { NULL, NULL, NULL };
		 g_type_add_interface_static(_w_input_stream_type, GTK_TYPE_SCROLLABLE, &interface_info);*/
	}
	struct _WInputStream *inputStream = g_object_new(_w_input_stream_type,
	NULL);
	if (inputStream != 0) {
		inputStream->stream = stream;
		inputStream->flags = flags;
	}
	return (GInputStream*) inputStream;
}
