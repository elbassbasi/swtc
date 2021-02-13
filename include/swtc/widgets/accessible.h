/*
 * Name:        accessible.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef SWT_WIDGETS_ACCESSIBLE_H_
#define SWT_WIDGETS_ACCESSIBLE_H_
#include "control.h"
#ifdef __cplusplus
extern "C" {
#endif
struct w_accessible {
	void* v_table;
	struct _w_accessible_class* clazz;
	void *handle[0x10];
};

#define W_ACCESSIBLE(x) ((w_accessible*)x)

struct _w_accessible_class {
	void (*add_relation)(w_accessible *accessible, int type,
			w_accessible *target);
	struct w_control* (*get_control)(w_accessible *accessible);
	void (*remove_relation)(w_accessible *accessible, int type,
			w_accessible *target);
	void (*send_event)(w_accessible *accessible, int event, void *eventData);
	void (*selection_changed)(w_accessible *accessible);
	void (*set_focus)(w_accessible *accessible, int childID);
	void (*text_caret_moved)(w_accessible *accessible, int index);
	void (*text_changed)(w_accessible *accessible, int type, int startIndex,
			int length);
	void (*text_selection_changed)(w_accessible *accessible);
};
SWT_PUBLIC void w_accessible_add_relation(w_accessible *accessible, int type,
		w_accessible *target);
SWT_PUBLIC struct w_control* w_accessible_get_control(w_accessible *accessible);
SWT_PUBLIC void w_accessible_remove_relation(w_accessible *accessible, int type,
		w_accessible *target);
SWT_PUBLIC void w_accessible_send_event(w_accessible *accessible, int event,
		void *eventData);
SWT_PUBLIC void w_accessible_selection_changed(w_accessible *accessible);
SWT_PUBLIC void w_accessible_set_focus(w_accessible *accessible, int childID);
SWT_PUBLIC void w_accessible_text_caret_moved(w_accessible *accessible,
		int index);
SWT_PUBLIC void w_accessible_text_changed(w_accessible *accessible, int type,
		int startIndex, int length);
SWT_PUBLIC void w_accessible_text_selection_changed(w_accessible *accessible);
#ifdef __cplusplus
}
#endif
#endif /* SWT_WIDGETS_ACCESSIBLE_H_ */
