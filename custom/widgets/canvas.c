/*
 * canvas.c
 *
 *  Created on: 28 juin 2021
 *      Author: Azeddine
 */
#include "canvas.h"
/*
 * control
 */
wresult cw_control_create(w_widget *widget, w_widget *parent, wuint64 style,
		w_widget_post_event_proc post_event, size_t sizepriv) {
	wresult result = widget->clazz->parentClass->create(widget, parent, style,
			post_event);
	if (result > 0) {
		void *ptr = cw_control_alloc_priv(W_CONTROL(widget), sizepriv);
		if (ptr == 0) {
			w_widget_dispose(widget);
			return W_ERROR_NO_MEMORY;
		}
		memset(ptr, 0, sizepriv);
	}
	return result;
}
int wc_canvas_post_event(w_widget *widget, w_event *e) {
	return widget->clazz->parentClass->post_event(widget, e);
}
void* cw_control_get_priv(w_control *c) {
	struct _w_widget_class *clazz = c->widget.clazz;
	void *ptr = ((void*) c) + clazz->parentClass->object_used_size;
	if (CW_CLASS_RESERVED(clazz)->state & CW_USED_INTERNAL_MEMORY) {
		return ptr;
	} else {
		return *((void**) ptr);
	}
}
void* cw_control_alloc_priv(w_control *c, size_t size) {
	struct _w_widget_class *clazz = c->widget.clazz;
	void *ptr = ((void*) c) + clazz->parentClass->object_used_size;
	if (CW_CLASS_RESERVED(clazz)->state & CW_USED_INTERNAL_MEMORY) {
		return ptr;
	} else {
		void **priv = (void**) ptr;
		*priv = malloc(size);
		return *priv;
	}
}
/*
 * control canvas
 */
void cw_ccanvas_init_class(w_toolkit *toolkit, wushort classId,
		struct _w_widget_class *clazz) {
	w_toolkit *platformToolkit = w_app_get_platform_toolkit(w_app_get());
	struct _w_widget_class *canvasclass = w_toolkit_get_class(platformToolkit,
			_W_CLASS_CCANVAS);
	w_toolkit_init_class(platformToolkit, _W_CLASS_CCANVAS,
			W_WIDGET_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_CCANVAS;
}
void cw_ccanvas_init_class_priv(w_toolkit *toolkit, wushort classId,
		struct _w_widget_class *clazz, size_t sizeof_priv) {
	cw_ccanvas_init_class(toolkit, classId, clazz);
	struct _w_widget_class *next = clazz->parentClass;
	if ((next->object_used_size + sizeof_priv) > next->object_total_size) {
		CW_CLASS_RESERVED(clazz)->state |= CW_USED_INTERNAL_MEMORY;
		clazz->object_used_size += sizeof_priv;
	} else {
		clazz->object_used_size += sizeof(void*);
	}
}
/*
 * canvas
 */
void cw_canvas_init_class(w_toolkit *toolkit, wushort classId,
		struct _w_canvas_class *clazz) {
	w_toolkit *platformToolkit = w_app_get_platform_toolkit(w_app_get());
	struct _w_widget_class *canvasclass = w_toolkit_get_class(platformToolkit,
			_W_CLASS_CANVAS);
	w_toolkit_init_class(platformToolkit, _W_CLASS_CANVAS,
			W_WIDGET_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_CANVAS;
}
/*
 *	composite
 */
void cw_composite_init_class(w_toolkit *toolkit, wushort classId,
		struct _w_composite_class *clazz) {
	w_toolkit *platformToolkit = w_app_get_platform_toolkit(w_app_get());
	struct _w_widget_class *compositeclass = w_toolkit_get_class(platformToolkit,
			_W_CLASS_COMPOSITE);
	w_toolkit_init_class(platformToolkit, _W_CLASS_COMPOSITE,
			W_WIDGET_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->parentClass = compositeclass;
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_COMPOSITE;
}
void cw_composite_init_class_priv(w_toolkit *toolkit, wushort classId,
		struct _w_composite_class *clazz, size_t priv_size) {
	struct _w_widget_class *next = W_WIDGET_CLASS(clazz)->parentClass;
	if ((next->object_used_size + priv_size) < W_WIDGET_CLASS(clazz)->object_total_size) {
		CW_CLASS_RESERVED(clazz)->state |= CW_USED_INTERNAL_MEMORY;
		W_WIDGET_CLASS(clazz)->object_used_size += priv_size;
	} else {
		W_WIDGET_CLASS(clazz)->object_used_size += sizeof(void*);
	}
}
