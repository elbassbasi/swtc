/*
 * canvas.h
 *
 *  Created on: 28 juin 2021
 *      Author: Azeddine
 */

#ifndef CUSTOM_WIDGETS_CANVAS_H_
#define CUSTOM_WIDGETS_CANVAS_H_
#include <swtc.h>
#ifdef CSWTBUILD
#define CSWT_PUBLIC DLL_EXPORT
#else
#define CSWT_PUBLIC DLL_IMPORT
#endif

typedef struct cw_reserved {
	void *reserved;
	int state;
} cw_reserved;
#define CW_USED_INTERNAL_MEMORY (1 << 0)
#define CW_CLASS_RESERVED(clazz) ((cw_reserved*)((struct _w_widget_class*)clazz)->reserved)
typedef void (*_init_class)(struct _w_widget_class *clazz);
wresult cw_control_create(w_widget *widget, w_widget *parent, wuint64 style,
		w_widget_post_event_proc post_event, size_t sizepriv);
void* cw_control_get_priv(w_control *c);
void* cw_control_alloc_priv(w_control *c, size_t size);
void cw_ccanvas_init_class(w_toolkit *toolkit, wushort classId,struct _w_widget_class *clazz);
void cw_ccanvas_init_class_priv(w_toolkit *toolkit, wushort classId,struct _w_widget_class *clazz,
		size_t sizeof_priv);
/*
 * canvas
 */
void cw_canvas_init_class(w_toolkit *toolkit, wushort classId,struct _w_canvas_class *clazz);
/*
 *	composite
 */
void cw_composite_init_class(w_toolkit *toolkit, wushort classId,struct _w_composite_class *clazz);
void cw_composite_init_class_priv(w_toolkit *toolkit, wushort classId,struct _w_composite_class *clazz,
		size_t priv_size);

#endif /* CUSTOM_WIDGETS_CANVAS_H_ */
