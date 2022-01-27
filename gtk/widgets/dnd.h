/*
 * Name:        dnd.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef GTK_WIDGETS_DND_H_
#define GTK_WIDGETS_DND_H_
#include "control.h"
struct _w_transfer_data {
	GdkAtom type;
	int length;
	int format;
	guchar *pValue;
	int result;
};
wresult _w_transfers_registre();
#define _W_TRANSFER(x) ((_w_transfer*)x)
typedef struct _w_clipboard {
	int clipboards;
	GtkClipboard *GTKCLIPBOARD;

} _w_clipboard;
#define _W_CLIPBOARD(x) ((_w_clipboard*)x)

wresult _w_toolkit_open_clipboard(w_toolkit *toolkit, w_clipboard *clipboard,
		int clipboards);
int _w_toolkit_register_type(w_toolkit *toolkit, const char *formatName);

#define STATE_DRAGSOURCE_TRANSFER_LOCAL (1 << (STATE_WIDGET_END + 1))
#define STATE_DRAGSOURCE_MOVEDATA (1 << (STATE_WIDGET_END + 2))

typedef struct _w_dragsource {
	_w_widget widget;
	w_control *control;
	GtkTargetList *targetList;
	size_t transfer_length;
	w_transfer *transfer[0];
} _w_dragsource;
#define _W_DRAGSOURCE(x) ((_w_dragsource*)x)
void _w_dragsource_class_init(w_toolkit *toolkit, wushort classId,
		struct _w_dragsource_class *clazz);
#define STATE_DROPTARGET_TRANSFER_LOCAL (1 << (STATE_WIDGET_END + 1))
#define STATE_DROPTARGET_HAS_EVENT (1 << (STATE_WIDGET_END + 2))
typedef struct _w_droptarget {
	_w_widget widget;
	w_control *control;
	guint drag_motion_handler;
	guint drag_leave_handler;
	guint drag_data_received_handler;
	guint drag_drop_handler;
	int selectedOperation;
	GdkAtom selectedDataType;
	int keyOperation;
	int dragOverEvent_x;
	int dragOverEvent_y;
	int dragOverEvent_operations;
	int dragOverEvent_time;
	wuint64 dragOverStart;
	GList* targets;
	size_t transfer_length;
	w_transfer *transfer[0];
} _w_droptarget;
typedef struct _w_droptarget_iterator {
	w_basic_iterator iterator;
	w_droptarget* target;
	GList *targets;
	GList *i;
	size_t count;
	w_transfer_data current_data;
	w_transfer_data temp_data;
} _w_droptarget_iterator;
#define _W_DROPTARGET(x) ((_w_droptarget*)x)
void _w_droptarget_class_init(w_toolkit *toolkit, wushort classId,
		struct _w_droptarget_class *clazz);
/*
 *
 */
void _w_control_drag_begin(GtkWidget *widget, GdkDragContext *context);
void _w_control_drag_end(GtkWidget *widget, GdkDragContext *context);
void _w_control_drag_data_get(GtkWidget *widget, GdkDragContext *context,
		GtkSelectionData *selection_data, guint info, guint time_);
void _w_control_drag_data_delete(GtkWidget *widget, GdkDragContext *context);
gboolean _w_control_drag_failed(GtkWidget *widget, GdkDragContext *context,
		GtkDragResult result);
void _w_control_registre_drag_function(GtkWidgetClass *clazz);

#endif /* GTK_WIDGETS_DND_H_ */
