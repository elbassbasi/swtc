/*
 * Name:        dnd1.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "dnd.h"
#include "toolkit.h"
#include <sys/time.h>
/*
 * DragSource
 */
w_transfer** _w_dragsource_get_transfer(w_dragsource *dragsource,
		size_t *length) {
	*length = _W_DRAGSOURCE(dragsource)->transfer_length;
	if (_W_WIDGET(dragsource)->state & STATE_DRAGSOURCE_TRANSFER_LOCAL) {
		return (w_transfer**) _W_DRAGSOURCE(dragsource)->transfer;
	} else {
		return (w_transfer**) _W_DRAGSOURCE(dragsource)->transfer[0];
	}
}
void _w_dragsource_drag_get_data(GtkWidget *widget, GdkDragContext *context,
		GtkSelectionData *selection_data, int info, int time) {
	if (selection_data == 0)
		return;
	w_widget *control = (w_widget*) g_object_get_qdata(G_OBJECT(widget),
			gtk_toolkit->quark[0]);
	if (control == 0)
		return;
	w_dragsource *source = _W_CONTROL(control)->dragsource;
	if (source == 0)
		return;
	int length = gtk_selection_data_get_length(selection_data);
	int format = gtk_selection_data_get_format(selection_data);
	guchar *data = (guchar*) gtk_selection_data_get_data(selection_data);
	GdkAtom target = gtk_selection_data_get_target(selection_data);
	if (target == 0)
		return;

	w_transfer_data transferData;
	transferData.type = target;
	transferData.pValue = data;
	transferData.length = length;
	transferData.format = format;
	transferData.result = 0;

	w_event_dragsource event;
	event.event.type = W_EVENT_DRAGSETDATA;
	event.event.widget = W_WIDGET(source);
	event.event.time = time;
	event.event.platform_event = 0;
	event.event.data = 0;
	event.x = 0;
	event.y = 0;
	event.dataType = &transferData;
	event.offsetX = 0;
	event.offsetY = 0;
	w_image_init(&event.image);

	w_transfer *transfer = 0;
	size_t transferAgents_length = 0;
	w_transfer **transferAgents = _w_dragsource_get_transfer(source,
			&transferAgents_length);
	for (int i = 0; i < transferAgents_length; i++) {
		w_transfer *transferAgent = transferAgents[i];
		if (transferAgent != 0) {
			if (transferAgent->is_supported_type(transferAgent,
					&transferData)) {
				transfer = transferAgent;
				break;
			}
		}
	}
	if (transfer == 0)
		return;
	event.transfer = transfer;

	int doit = _w_widget_send_event(W_WIDGET(source), (w_event*) &event,
			W_EVENT_SEND);
	if (!doit)
		return;

	if (transferData.result != 1)
		return;
	gtk_selection_data_set(selection_data, transferData.type,
			transferData.format, transferData.pValue, transferData.length);
	g_free(transferData.pValue);
	return;
}
int _w_dragsource_opToOsOp(int operation) {
	int osOperation = 0;

	if ((operation & W_DROP_COPY) == W_DROP_COPY)
		osOperation |= GDK_ACTION_COPY;
	if ((operation & W_DROP_MOVE) == W_DROP_MOVE)
		osOperation |= GDK_ACTION_MOVE;
	if ((operation & W_DROP_LINK) == W_DROP_LINK)
		osOperation |= GDK_ACTION_LINK;

	return osOperation;
}
int _w_dragsource_osOpToOp(int osOperation) {
	int operation = W_DROP_NONE;

	if ((osOperation & GDK_ACTION_COPY) == GDK_ACTION_COPY)
		operation |= W_DROP_COPY;
	if ((osOperation & GDK_ACTION_MOVE) == GDK_ACTION_MOVE)
		operation |= W_DROP_MOVE;
	if ((osOperation & GDK_ACTION_LINK) == GDK_ACTION_LINK)
		operation |= W_DROP_LINK;

	return operation;
}
void _w_dragsource_drag_end(GtkWidget *widget, GdkDragContext *context) {
	w_widget *control = (w_widget*) g_object_get_qdata(G_OBJECT(widget),
			gtk_toolkit->quark[0]);
	if (control == 0)
		return;
	w_dragsource *source = _W_CONTROL(control)->dragsource;
	if (source == 0)
		return;
	/*
	 * Bug in GTK.  If a drag is initiated using gtk_drag_begin and the
	 * mouse is released immediately, the mouse and keyboard remain
	 * grabbed.  The fix is to release the grab on the mouse and keyboard
	 * whenever the drag is terminated.
	 *
	 * NOTE: We believe that it is never an error to ungrab when
	 * a drag is finished.
	 */
#if GTK3
	GdkDisplay *display = gdk_window_get_display(gtk_widget_get_window(widget));
	GdkDeviceManager *device_manager = gdk_display_get_device_manager(display);
	GdkDevice *pointer = gdk_device_manager_get_client_pointer(device_manager);
	GdkDevice *keyboard = gdk_device_get_associated_device(pointer);
	gdk_device_ungrab(pointer, GDK_CURRENT_TIME);
	gdk_device_ungrab(keyboard, GDK_CURRENT_TIME);
#endif
#if GTK2
	gdk_pointer_ungrab(GDK_CURRENT_TIME);
	gdk_keyboard_ungrab(GDK_CURRENT_TIME);
#endif

	int operation = W_DROP_NONE;
	if (context != 0) {
		GdkWindow *dest_window = 0;
		int action = 0;
#if GTK3
		dest_window = gdk_drag_context_get_dest_window(context);
		action = gdk_drag_context_get_selected_action(context);
#endif
#if GTK2
	dest_window = context->dest_window;
	action = context->action;
#endif
		if (dest_window != 0) { //NOTE: if dest_window is 0, drag was aborted
			if (_W_WIDGET(source)->state & STATE_DRAGSOURCE_MOVEDATA) {
				operation = W_DROP_MOVE;
			} else {
				operation = _w_dragsource_osOpToOp(action);
				if (operation == W_DROP_MOVE)
					operation = W_DROP_NONE;
			}
		}
	}
	w_event_dragsource event;
	event.event.type = W_EVENT_DRAGFINISHED;
	event.event.widget = W_WIDGET(source);
	//event.time = ???
	//event.doit = operation != 0;
	event.detail = operation;

	int doit = _w_widget_send_event(W_WIDGET(source), (w_event*) &event,
			W_EVENT_SEND);
	_W_WIDGET(source)->state &= ~STATE_DRAGSOURCE_MOVEDATA;
}
void _w_dragsource_drag_data_delete(GtkWidget *widget,
		GdkDragContext *context) {
	w_widget *control = (w_widget*) g_object_get_qdata(G_OBJECT(widget),
			gtk_toolkit->quark[0]);
	if (control == 0)
		return;
	w_dragsource *source = _W_CONTROL(control)->dragsource;
	if (source == 0)
		return;
	_W_WIDGET(source)->state |= STATE_DRAGSOURCE_MOVEDATA;
}
w_control* _w_dragsource_get_control(w_dragsource *dragsource) {
	return _W_DRAGSOURCE(dragsource)->control;
}
wresult _w_dragsource_set_transfer(w_dragsource *dragsource,
		w_transfer **transferAgents, size_t length) {
	if ((_W_WIDGET(dragsource)->state & STATE_DRAGSOURCE_TRANSFER_LOCAL) == 0) {
		if (_W_DRAGSOURCE(dragsource)->transfer[0] != 0) {
			free(_W_DRAGSOURCE(dragsource)->transfer[0]);
		}
	}
	if (length == -1) {
		size_t i = 0;
		while (transferAgents[i] != 0)
			i++;
		length = i;
	}
	if (length * sizeof(w_transfer*)
			> (sizeof(w_dragsource) - sizeof(_w_dragsource))) {
		_W_WIDGET(dragsource)->state &= ~ STATE_DRAGSOURCE_TRANSFER_LOCAL;
		w_transfer **mem = (w_transfer**) malloc(length * sizeof(w_transfer*));
		_W_DRAGSOURCE(dragsource)->transfer[0] = (w_transfer*) mem;
		if (mem == 0) {
			_W_DRAGSOURCE(dragsource)->transfer_length = 0;
			return W_ERROR_NO_MEMORY;
		}
		memcpy(mem, transferAgents, length * sizeof(w_transfer*));
		_W_DRAGSOURCE(dragsource)->transfer_length = length;
	} else {
		_W_WIDGET(dragsource)->state |= STATE_DRAGSOURCE_TRANSFER_LOCAL;
		memcpy(_W_DRAGSOURCE(dragsource)->transfer, transferAgents,
				length * sizeof(w_transfer*));
		_W_DRAGSOURCE(dragsource)->transfer_length = length;
	}
	if (_W_DRAGSOURCE(dragsource)->targetList != 0) {
		gtk_target_list_unref(_W_DRAGSOURCE(dragsource)->targetList);
		_W_DRAGSOURCE(dragsource)->targetList = 0;
	}

	GtkTargetEntry *targets = 0, *entry;
	size_t typelength, count = 0;
	for (int i = 0; i < length; i++) {
		w_transfer *transfer = transferAgents[i];
		if (transfer != 0) {
			typelength = 0;
			transfer->get_types(transfer, &typelength);
			count += typelength;
		}
	}
	targets = g_malloc(count * sizeof(GtkTargetEntry));
	if (targets == 0)
		return W_FALSE;
	entry = targets;
	for (int i = 0; i < length; i++) {
		w_transfer *transfer = transferAgents[i];
		if (transfer != 0) {
			typelength = 0;
			w_transfer_type *types = transfer->get_types(transfer, &typelength);
			for (int j = 0; j < typelength; j++) {
				entry->target = (gchar*) types->name;
				entry->info = types->id;
				entry++;
			}
		}
	}
	_W_DRAGSOURCE(dragsource)->targetList = gtk_target_list_new(targets, count);
	g_free(targets);
	return W_TRUE;
}
int _w_dragsource_dispose_0(w_widget *widget) {
	w_control *control = _W_DRAGSOURCE(widget)->control;
	if (control == 0)
		return W_TRUE;
	_W_CONTROL(control)->dragsource = 0;
	if (_W_DRAGSOURCE(widget)->targetList != 0) {
		gtk_target_list_unref(_W_DRAGSOURCE(widget)->targetList);
	}
	widget->clazz = 0;
	return W_TRUE;
}
int _w_dragsource_dragdetect(w_widget *widget, w_event *e) {
	_W_WIDGET(widget)->state &= ~STATE_DRAGSOURCE_MOVEDATA;
	w_event_mouse *em = (w_event_mouse*) e;
	w_event_dragsource event;
	event.event.type = W_EVENT_DRAGSTART;
	event.event.widget = widget;
	event.event.time = em->event.time;
	event.event.platform_event = em->event.platform_event;
	event.event.data = 0;
	event.x = em->x;
	event.y = em->y;
	event.dataType = 0;
	event.transfer = 0;
	event.offsetX = 0;
	event.offsetY = 0;
	event.doit = W_TRUE;
	w_image_init(&event.image);

	_w_widget_send_event(W_WIDGET(widget), (w_event*) &event, W_EVENT_SEND);
	size_t transferAgents_length = 0;
	w_transfer **transferAgents = _w_dragsource_get_transfer(
			W_DRAGSOURCE(widget), &transferAgents_length);
	if (!event.doit || transferAgents == 0 || transferAgents_length == 0)
		return W_FALSE;
	if (_W_DRAGSOURCE(widget)->targetList == 0)
		return W_FALSE;

	w_control *control = _W_DRAGSOURCE(widget)->control;

	GdkDragAction actions = (GdkDragAction) _w_dragsource_opToOsOp(
	_W_WIDGET(widget)->style);
	GdkDragContext *context;
	GtkWidget *handle = _W_WIDGET(control)->handle;
	GtkTargetList *targetList = _W_DRAGSOURCE(widget)->targetList;
	/*if (GTK_VERSION >= VERSION(3, 10, 0)) {
		context = gtk_drag_begin_with_coordinates(handle, targetList, actions,
				1, 0, -1, -1);
	} else {*/
		context = gtk_drag_begin(handle, targetList, actions, 1, 0);
	//}
	if (context != 0) {
		if (w_image_is_ok(&event.image)) {
			GdkPixbuf *pixbuf = _W_IMAGE(&event.image)->pixbuf;
			if (GTK3) {
				cairo_surface_t *surface = _w_image_create_surface(pixbuf);
				gtk_drag_set_icon_surface(context, surface);
				cairo_surface_destroy(surface);
			} else {
				gtk_drag_set_icon_pixbuf(context, pixbuf, 0, 0);
			}
			w_image_dispose(&event.image);
		}
	}
	return W_TRUE;
}
wresult _w_dragsource_post_event(w_widget *widget, w_event *e, int flags) {
	switch (e->type) {
	case W_EVENT_DRAGDETECT:
		_w_dragsource_dragdetect(widget, e);
		break;
	case W_EVENT_DISPOSE:
		_w_dragsource_dispose_0(widget);
		break;
	}
	return _w_widget_post_event(widget, e,flags);
}
wresult _w_dragsource_dispose(w_widget *widget) {
	_w_dragsource_dispose_0(widget);
	return W_TRUE;
}
wresult _w_dragsource_create(w_widget *widget, w_widget *parent, wuint64 style,
		w_widget_post_event_proc post_event) {

}
void _w_dragsource_class_init(w_toolkit *toolkit, wushort classId,
		struct _w_dragsource_class *clazz) {
	_w_widget_class_init(toolkit, classId, W_WIDGET_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_DRAGSOURCE;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_dragsource_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_dragsource);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_dragsource);
	W_WIDGET_CLASS(clazz)->create = _w_dragsource_create;
	W_WIDGET_CLASS(clazz)->post_event = _w_dragsource_post_event;
	W_WIDGET_CLASS(clazz)->dispose = _w_dragsource_dispose;
	clazz->get_control = _w_dragsource_get_control;
	//clazz->get_effect = _w_dragsource_get_effect;
	clazz->get_transfer = _w_dragsource_get_transfer;
	clazz->set_transfer = _w_dragsource_set_transfer;
	_gtk_signal *signals = gtk_toolkit->signals;
	signals[SIGNAL_DRAG_DATA_GET].number_of_args = 5;
	signals[SIGNAL_DRAG_DATA_GET].callback =
			(GCallback) _w_dragsource_drag_get_data;
	signals[SIGNAL_DRAG_END].number_of_args = 2;
	signals[SIGNAL_DRAG_END].callback = (GCallback) _w_dragsource_drag_end;
	signals[SIGNAL_DRAG_DATA_DELETE].number_of_args = 2;
	signals[SIGNAL_DRAG_DATA_DELETE].callback =
			(GCallback) _w_dragsource_drag_data_delete;

}
/*
 * DropTarget
 */
int _w_droptarget_opToOsOp(int operation) {
	int osOperation = 0;
	if ((operation & W_DROP_COPY) == W_DROP_COPY)
		osOperation |= GDK_ACTION_COPY;
	if ((operation & W_DROP_MOVE) == W_DROP_MOVE)
		osOperation |= GDK_ACTION_MOVE;
	if ((operation & W_DROP_LINK) == W_DROP_LINK)
		osOperation |= GDK_ACTION_LINK;
	return osOperation;
}

int _w_droptarget_osOpToOp(int osOperation) {
	int operation = W_DROP_NONE;
	if ((osOperation & GDK_ACTION_COPY) == GDK_ACTION_COPY)
		operation |= W_DROP_COPY;
	if ((osOperation & GDK_ACTION_MOVE) == GDK_ACTION_MOVE)
		operation |= W_DROP_MOVE;
	if ((osOperation & GDK_ACTION_LINK) == GDK_ACTION_LINK)
		operation |= W_DROP_LINK;
	return operation;
}
w_transfer** _w_droptarget_get_transfer(w_droptarget *droptarget,
		size_t *length) {
	*length = _W_DROPTARGET(droptarget)->transfer_length;
	if (_W_WIDGET(droptarget)->state & STATE_DROPTARGET_TRANSFER_LOCAL) {
		return (w_transfer**) _W_DROPTARGET(droptarget)->transfer;
	} else {
		return (w_transfer**) _W_DROPTARGET(droptarget)->transfer[0];
	}
}
wresult _w_droptarget_iterator_close(w_iterator *it) {
	return W_TRUE;
}
wresult _w_droptarget_iterator_next(w_iterator *it, void *obj) {
	_w_droptarget_iterator *iterator = (_w_droptarget_iterator*) it;
	size_t transferAgents_length = 0;
	size_t dataTypes_length = 0;
	w_transfer **transferAgents = _w_droptarget_get_transfer(iterator->target,
			&transferAgents_length);
	while (iterator->i != 0) {
		GdkAtom pData = (GdkAtom) iterator->i->data;
		iterator->temp_data.type = pData;
		for (int j = 0; j < transferAgents_length; j++) {
			w_transfer *transfer = transferAgents[j];
			if (transfer != 0
					&& transfer->is_supported_type(transfer,
							&iterator->temp_data)) {
				return W_TRUE;
			}
		}
		iterator->i = iterator->i->next;
	}
	return W_FALSE;
}
wresult _w_droptarget_iterator_reset(w_iterator *it) {
	_w_droptarget_iterator *iterator = (_w_droptarget_iterator*) it;
	iterator->i = iterator->targets;
	return W_TRUE;
}
wresult _w_droptarget_iterator_remove(w_iterator *it) {
	return W_ERROR_NOT_IMPLEMENTED;
}
size_t _w_droptarget_iterator_get_count(w_iterator *it) {
	_w_droptarget_iterator *iterator = (_w_droptarget_iterator*) it;
	if (iterator->count == -1) {
		size_t transferAgents_length = 0;
		size_t dataTypes_length = 0;
		w_transfer **transferAgents = _w_droptarget_get_transfer(
				iterator->target, &transferAgents_length);
		w_transfer_data data;
		GList *targets = iterator->targets;
		while (targets != 0) {
			GdkAtom pData = (GdkAtom) targets->data;
			data.type = pData;
			for (int j = 0; j < transferAgents_length; j++) {
				w_transfer *transfer = transferAgents[j];
				if (transfer != 0
						&& transfer->is_supported_type(transfer, &data)) {
					dataTypes_length++;
					break;
				}
			}
			targets = targets->next;
		}
		iterator->count = dataTypes_length;
	}
	return iterator->count;
}
_w_iterator_class _w_droptarget_iterator_class = {	//
		_w_droptarget_iterator_close,	//
				_w_droptarget_iterator_next,	//
				_w_droptarget_iterator_reset,	//
				_w_droptarget_iterator_remove,	//
				_w_droptarget_iterator_get_count,	//
		};
#define DRAGOVER_HYSTERESIS 50
gboolean _w_droptarget_drag_over_heartbeat_run(gpointer user_data) {
	w_droptarget *droptarget = (w_droptarget*) user_data;
	w_control *control = _W_DROPTARGET(droptarget)->control;
	if (!w_widget_is_ok(W_WIDGET(control))
			|| _W_DROPTARGET(droptarget)->dragOverStart == 0)
		return FALSE;
	struct timeval tp;
	gettimeofday(&tp, NULL);
	uint64_t time = tp.tv_sec * 1000 + tp.tv_usec / 1000;
	int delay = DRAGOVER_HYSTERESIS;
	if (time < _W_DROPTARGET(droptarget)->dragOverStart) {
		delay = (int) (_W_DROPTARGET(droptarget)->dragOverStart - time);
	} else {
		_W_DROPTARGET(droptarget)->dragOverEvent_time += DRAGOVER_HYSTERESIS;
		int allowedOperations =
		_W_DROPTARGET(droptarget)->dragOverEvent_operations;

		w_event_droptarget event;
		w_transfer_data dataType;
		_w_droptarget_iterator iterator;
		event.event.type = W_EVENT_DRAGOVER;
		event.event.widget = W_WIDGET(droptarget);	//dragOverEvent.widget;
		event.event.platform_event = 0;
		event.event.time = _W_DROPTARGET(droptarget)->dragOverEvent_time;
		event.event.data = 0;
		event.x = _W_DROPTARGET(droptarget)->dragOverEvent_x;
		event.y = _W_DROPTARGET(droptarget)->dragOverEvent_y;
		event.feedback = W_FEEDBACK_SELECT;
		event.dataTypes = (w_iterator*) &iterator;
		dataType.type = _W_DROPTARGET(droptarget)->selectedDataType;
		event.currentDataType = &dataType;
		// Get allowed transfer types
		size_t transferAgents_length = 0;
		w_transfer **transferAgents = _w_droptarget_get_transfer(droptarget,
				&transferAgents_length);
		for (int j = 0; j < transferAgents_length; j++) {
			w_transfer *transfer = transferAgents[j];
			if (transfer != 0
					&& transfer->is_supported_type(transfer,
							event.currentDataType)) {
				event.currenttransfer = transfer;
				break;
			}
		}
		event.operations = _W_DROPTARGET(droptarget)->dragOverEvent_operations;
		event.detail = _W_DROPTARGET(droptarget)->selectedOperation;
		w_widgetdata_init(W_WIDGETDATA(&event.item));
		/*w_droptarget_effect *dropEffect = &_W_DROPTARGET(droptarget)->effect;
		 if (dropEffect->get_item != 0) {
		 dropEffect->get_item(dropEffect, (w_item*) &event.item, event.x,
		 event.y);
		 }*/
		//init iterator
		iterator.iterator.clazz = &_w_droptarget_iterator_class;
		iterator.targets = _W_DROPTARGET(droptarget)->targets;
		iterator.count = -1;
		iterator.i = iterator.targets;
		iterator.target = droptarget;
		_W_DROPTARGET(droptarget)->selectedDataType = 0;
		_W_DROPTARGET(droptarget)->selectedOperation = W_DROP_NONE;
		_w_widget_send_event(W_WIDGET(droptarget), (w_event*) &event,
				W_EVENT_SEND);
		if (event.currentDataType->type != 0) {
			/*for (int i = 0; i < allowedTypes.length; i++) {
			 if (allowedTypes[i].type == event.currentDataType->type) {
			 _W_DROPTARGET(droptarget)->selectedDataType =
			 event.currentDataType->type;
			 break;
			 }
			 }*/
			_W_DROPTARGET(droptarget)->selectedDataType =
					event.currentDataType->type;
		}
		if (_W_DROPTARGET(droptarget)->selectedDataType != 0
				&& (event.detail & allowedOperations) != 0) {
			_W_DROPTARGET(droptarget)->selectedOperation = event.detail;
		}
	}
	if (!w_widget_is_ok(W_WIDGET(control)))
		return FALSE;
	g_timeout_add(delay, _w_droptarget_drag_over_heartbeat_run, user_data);
	return FALSE;
}
int _w_droptarget_get_operation_from_key_state() {
	GdkModifierType state;
#if GTK3
	GdkWindow *root = gdk_get_default_root_window();
	_gdk_window_get_device_position(root, 0, 0, &state);
#endif
#if GTK2
		gdk_window_get_pointer(0, 0, 0, &state);
#endif
	gboolean ctrl = (state & GDK_CONTROL_MASK) != 0;
	gboolean shift = (state & GDK_SHIFT_MASK) != 0;
	if (ctrl && shift)
		return W_DROP_LINK;
	if (ctrl)
		return W_DROP_COPY;
	if (shift)
		return W_DROP_MOVE;
	return W_DROP_DEFAULT;
}
gboolean _w_droptarget_set_event_data(w_droptarget *target,
		GdkDragContext *context, int x, int y, int time,
		w_event_droptarget *event, _w_droptarget_iterator *iterator) {
	if (context == 0)
		return FALSE;
	GList *targets = 0;
	int actions = 0;
#if GTK3
	targets = gdk_drag_context_list_targets(context);
	actions = gdk_drag_context_get_actions(context);
#endif
#if GTK2
#endif
	if (targets == 0)
		return FALSE;

	// get allowed operations
	int style = _W_WIDGET(target)->style;
	int operations = _w_droptarget_osOpToOp(actions) & style;
	if (operations == W_DROP_NONE)
		return FALSE;

	// get current operation
	int operation = _w_droptarget_get_operation_from_key_state();
	_W_DROPTARGET(target)->keyOperation = operation;
	if (operation == W_DROP_DEFAULT) {
		if ((style & W_DROP_DEFAULT) == 0) {
			operation =
					(operations & W_DROP_MOVE) != 0 ? W_DROP_MOVE : W_DROP_NONE;
		}
	} else {
		if ((operation & operations) == 0)
			operation = W_DROP_NONE;
	}
	//init iterator
	iterator->iterator.clazz = &_w_droptarget_iterator_class;
	iterator->targets = targets;
	iterator->count = -1;
	iterator->i = targets;
	iterator->target = target;

	// Get allowed transfer types
	size_t transferAgents_length = 0;
	size_t dataTypes_length = 0;
	w_transfer **transferAgents = _w_droptarget_get_transfer(target,
			&transferAgents_length);
	while (targets != 0 && dataTypes_length == 0) {
		GdkAtom pData = (GdkAtom) targets->data;
		iterator->current_data.type = pData;
		for (int j = 0; j < transferAgents_length; j++) {
			w_transfer *transfer = transferAgents[j];
			if (transfer != 0
					&& transfer->is_supported_type(transfer,
							&iterator->current_data)) {
				event->currentDataType = &iterator->current_data;
				event->currenttransfer = transfer;
				dataTypes_length++;
				break;
			}
		}
		targets = targets->next;
	}
	if (dataTypes_length == 0)
		return FALSE;
	w_control *control = _W_DROPTARGET(target)->control;
	GtkWidget *handle = _W_WIDGET(control)->handle;
	GdkWindow *window = gtk_widget_get_window(handle);
	int origin_x, origin_y;
	gdk_window_get_origin(window, &origin_x, &origin_y);

	event->event.widget = W_WIDGET(target);
	event->event.time = time;
	event->x = origin_x + x;
	event->y = origin_y + y;
	event->feedback = W_FEEDBACK_SELECT;
	event->currentDataType = &iterator->current_data;
	event->currenttransfer = 0;
	event->operations = operations;
	event->detail = operation;
	event->dataTypes = (w_iterator*) iterator;
	w_widgetdata_init(W_WIDGETDATA(&event->item));
	/*w_droptarget_effect *dropEffect = _w_droptarget_get_effect(target);
	 if (dropEffect->get_item != 0) {
	 dropEffect->get_item(dropEffect, (w_item*) &event->item, origin_x + x,
	 origin_y + y);
	 }*/
	return TRUE;
}

void _w_droptarget_update_drag_over_hover(w_droptarget *target, wuint64 delay,
		w_event_droptarget *event) {
	if (delay == 0) {
		_W_DROPTARGET(target)->dragOverStart = 0;
		_W_WIDGET(target)->state &= ~ STATE_DROPTARGET_HAS_EVENT;
		return;
	}
	struct timeval tp;
	gettimeofday(&tp, NULL);
	uint64_t time = tp.tv_sec * 1000 + tp.tv_usec / 1000;
	_W_DROPTARGET(target)->dragOverStart = time + delay;
	_W_WIDGET(target)->state |= STATE_DROPTARGET_HAS_EVENT;
	_W_DROPTARGET(target)->dragOverEvent_x = event->x;
	_W_DROPTARGET(target)->dragOverEvent_y = event->y;
	_W_DROPTARGET(target)->dragOverEvent_operations = event->operations;
	_W_DROPTARGET(target)->dragOverEvent_time = event->event.time;
}
gboolean _w_droptarget_drag_data_received(GtkWidget *widget,
		GdkDragContext *context, gint x, gint y,
		GtkSelectionData *selection_data, void *info, guint32 time) {
	gdk_pointer_ungrab(0);
	w_widget *control = (w_widget*) g_object_get_qdata(G_OBJECT(widget),
			gtk_toolkit->quark[0]);
	if (control == 0)
		return 0;
	w_droptarget *target = _W_CONTROL(control)->droptarget;
	if (target == 0)
		return 0;
	w_event_droptarget event;
	_w_droptarget_iterator iterator;
	if (selection_data == 0
			|| !_w_droptarget_set_event_data(target, context, x, y, time,
					&event, &iterator)) {
		_W_DROPTARGET(target)->keyOperation = -1;
		return 0;
	}
	_W_DROPTARGET(target)->keyOperation = -1;

	int allowedOperations = event.operations;

	w_transfer_data transferData;
	transferData.length = gtk_selection_data_get_length(selection_data);
	transferData.format = gtk_selection_data_get_format(selection_data);
	transferData.pValue = (guchar*) gtk_selection_data_get_data(selection_data);
	transferData.type = gtk_selection_data_get_data_type(selection_data);
	if (transferData.pValue != 0) {
		size_t transferAgents_length = 0;
		w_transfer **transferAgents = _w_droptarget_get_transfer(target,
				&transferAgents_length);
		w_transfer *transfer;
		for (int i = 0; i < transferAgents_length; i++) {
			transfer = transferAgents[i];
			if (transfer != 0
					&& transfer->is_supported_type(transfer, &transferData)) {
				event.currenttransfer = transfer;
				break;
			}
		}
	}
	if (event.currenttransfer == 0) {
		_W_DROPTARGET(target)->selectedOperation = W_DROP_NONE;
	}

	event.detail = _W_DROPTARGET(target)->selectedOperation;
	event.currentDataType = &transferData;
	_W_DROPTARGET(target)->selectedOperation = W_DROP_NONE;
	event.event.type = W_EVENT_DROP;
	_w_widget_send_event(W_WIDGET(target), (w_event*) &event, W_EVENT_SEND);
	if ((allowedOperations & event.detail) == event.detail) {
		_W_DROPTARGET(target)->selectedOperation = event.detail;
	}
//stop native handler
	g_signal_stop_emission_by_name(widget, "drag_data_received");

//notify source of action taken
	gtk_drag_finish(context,
	_W_DROPTARGET(target)->selectedOperation != W_DROP_NONE,
	_W_DROPTARGET(target)->selectedOperation == W_DROP_MOVE, time);
	return 0;
}

gboolean _w_droptarget_drag_drop(GtkWidget *widget, GdkDragContext *context,
		gint x, gint y, guint32 time) {
	w_widget *control = (w_widget*) g_object_get_qdata(G_OBJECT(widget),
			gtk_toolkit->quark[0]);
	if (control == 0)
		return 0;
	w_droptarget *target = _W_CONTROL(control)->droptarget;
	if (target == 0)
		return 0;
	w_event_droptarget event;
	_w_droptarget_iterator iterator;
	if (!_w_droptarget_set_event_data(target, context, x, y, time, &event,
			&iterator)) {
		_W_DROPTARGET(target)->keyOperation = -1;
		return FALSE;
	}
	_W_DROPTARGET(target)->keyOperation = -1;
	int allowedOperations = event.operations;
	/*TransferData[] allowedDataTypes = new TransferData[event.dataTypes.length];
	 System.arraycopy(event.dataTypes, 0, allowedDataTypes, 0,
	 allowedDataTypes.length);*/
	event.event.type = W_EVENT_DROPACCEPT;
	event.currentDataType->type = _W_DROPTARGET(target)->selectedDataType;
	event.detail = _W_DROPTARGET(target)->selectedOperation;
	_W_DROPTARGET(target)->selectedDataType = 0;
	_W_DROPTARGET(target)->selectedOperation = W_DROP_NONE;
	_w_widget_send_event(W_WIDGET(target), (w_event*) &event, W_EVENT_SEND);
	/*if (event.dataTypes != 0) {
	 for (int i = 0; i < allowedDataTypes.length; i++) {
	 if (allowedDataTypes[i].type == event.dataType.type) {
	 selectedDataType = allowedDataTypes[i];
	 break;
	 }
	 }
	 }*/
	if (_W_DROPTARGET(target)->selectedDataType != 0
			&& ((event.detail & allowedOperations) == event.detail)) {
		_W_DROPTARGET(target)->selectedOperation = event.detail;
	}
	if (_W_DROPTARGET(target)->selectedOperation == W_DROP_NONE) {
		// this was not a successful drop
		return FALSE;
	}
	// ask drag source for dropped data
	gtk_drag_get_data(widget, context, _W_DROPTARGET(target)->selectedDataType,
			time);
	return TRUE;
}

gboolean _w_droptarget_drag_leave(GtkWidget *widget, GdkDragContext *context,
		guint32 time) {
	w_widget *control = (w_widget*) g_object_get_qdata(G_OBJECT(widget),
			gtk_toolkit->quark[0]);
	if (control == 0)
		return 0;
	w_droptarget *target = _W_CONTROL(control)->droptarget;
	if (target == 0)
		return 0;
	_w_droptarget_update_drag_over_hover(target, 0, 0);

	if (_W_DROPTARGET(target)->keyOperation == -1)
		return FALSE;
	_W_DROPTARGET(target)->keyOperation = -1;

	w_event_droptarget event;
	event.event.type = W_EVENT_DRAGLEAVE;
	event.event.widget = W_WIDGET(target);
	event.event.time = time;
	event.event.platform_event = 0;
	event.event.data = 0;
	event.detail = W_DROP_NONE;
	event.currentDataType = 0;
	event.currenttransfer = 0;
	event.dataTypes = 0;
	event.feedback = 0;
	event.operations = W_NONE;
	event.x = 0;
	event.y = 0;
	w_widgetdata_init(W_WIDGETDATA(&event.item));
	_w_widget_send_event(W_WIDGET(target), (w_event*) &event, W_EVENT_SEND);
	return FALSE;
}

gboolean _w_droptarget_drag_motion(GtkWidget *widget, GdkDragContext *context,
		gint x, gint y, guint32 time) {
	w_widget *control = (w_widget*) g_object_get_qdata(G_OBJECT(widget),
			gtk_toolkit->quark[0]);
	if (control == 0)
		return 0;
	w_droptarget *target = _W_CONTROL(control)->droptarget;
	if (target == 0)
		return 0;
	int oldKeyOperation = _W_DROPTARGET(target)->keyOperation;
	/*
	 * Bug in GTK. GTK allows drag & drop on a shell even if a modal
	 * dialog/window is opened on that shell. The fix is to check for
	 * any active modal dialogs/shells before allowing the drop.
	 */
	if ((oldKeyOperation == -1)
			|| !(_W_WIDGET(control)->state & STATE_DROPTARGET_IS_ACTIVE)) { //drag enter
		_W_DROPTARGET(target)->selectedDataType = 0;
		_W_DROPTARGET(target)->selectedOperation = W_DROP_NONE;
	}

	w_event_droptarget event;
	_w_droptarget_iterator iterator;
	if (!_w_droptarget_set_event_data(target, context, x, y, time, &event,
			&iterator)) {
		_W_DROPTARGET(target)->keyOperation = -1;
		gdk_drag_status(context, (GdkDragAction) 0, time);
		return FALSE;
	}

	int allowedOperations = event.operations;
	/*TransferData[] allowedDataTypes = new TransferData[event.dataTypes.length];
	 System.arraycopy(event.dataTypes, 0, allowedDataTypes, 0,
	 allowedDataTypes.length);*/
	w_transfer_data data;
	data.type = 0;
	event.currentDataType = &data;

	if (oldKeyOperation == -1) {
		event.event.type = W_EVENT_DRAGENTER;
	} else {
		if (_W_DROPTARGET(target)->keyOperation == oldKeyOperation) {
			event.event.type = W_EVENT_DRAGOVER;
			data.type = _W_DROPTARGET(target)->selectedDataType;
			event.detail = _W_DROPTARGET(target)->selectedOperation;
		} else {
			event.event.type = W_EVENT_DRAGOPERATIONCHANGED;
			data.type = _W_DROPTARGET(target)->selectedDataType;
		}
	}
	_w_droptarget_update_drag_over_hover(target, DRAGOVER_HYSTERESIS, &event);
	_W_DROPTARGET(target)->selectedDataType = 0;
	_W_DROPTARGET(target)->selectedOperation = W_DROP_NONE;
	_w_widget_send_event(W_WIDGET(target), (w_event*) &event, W_EVENT_SEND);
	if (event.detail == W_DROP_DEFAULT) {
		event.detail =
				(allowedOperations & W_DROP_MOVE) != 0 ?
						W_DROP_MOVE : W_DROP_NONE;
	}
	if (event.currentDataType->type != 0) {
		/*for (int i = 0; i < allowedDataTypes.length; i++) {
		 if (allowedDataTypes[i].type == event.currentDataType->type) {
		 _W_DROPTARGET(target)->selectedDataType = allowedDataTypes[i];
		 break;
		 }
		 }*/
		_W_DROPTARGET(target)->selectedDataType = event.currentDataType->type;
	}
	if (_W_DROPTARGET(target)->selectedDataType != 0
			&& (allowedOperations & event.detail) != 0) {
		_W_DROPTARGET(target)->selectedOperation = event.detail;
	}
	GdkDragAction dragaction = -1;
	switch (_W_DROPTARGET(target)->selectedOperation) {
	case W_DROP_NONE:
		dragaction = 0;
		break;
	case W_DROP_COPY:
		dragaction = GDK_ACTION_COPY;
		break;
	case W_DROP_MOVE:
		dragaction = GDK_ACTION_MOVE;
		break;
	case W_DROP_LINK:
		dragaction = GDK_ACTION_LINK;
		break;
	}
	if (dragaction >= 0) {
		//gdk_drag_status(context, (GdkDragAction) dragaction, time);
	}
	if (oldKeyOperation == -1) {
		_w_droptarget_drag_over_heartbeat_run(target);
	}
	return TRUE;
}
w_control* _w_droptarget_get_control(w_droptarget *droptarget) {
	return _W_DROPTARGET(droptarget)->control;
}
wresult _w_droptarget_set_transfer(w_droptarget *droptarget,
		w_transfer **transferAgents, size_t length) {
	w_control *control = _W_DROPTARGET(droptarget)->control;
	GtkWidget *handle = _W_WIDGET(control)->handle;
	if (_W_DROPTARGET(droptarget)->transfer_length != 0) {
		gtk_drag_dest_unset(handle);
	}
	if ((_W_WIDGET(droptarget)->state & STATE_DROPTARGET_TRANSFER_LOCAL) == 0) {
		if (_W_DROPTARGET(droptarget)->transfer[0] != 0) {
			free(_W_DROPTARGET(droptarget)->transfer[0]);
		}
	}
	if (length == -1) {
		size_t i = 0;
		while (transferAgents[i] != 0)
			i++;
		length = i;
	}
	if ((length * sizeof(w_transfer*))
			> (sizeof(w_dragsource) - sizeof(_w_dragsource))) {
		_W_WIDGET(droptarget)->state &= ~ STATE_DROPTARGET_TRANSFER_LOCAL;
		w_transfer **mem = (w_transfer**) malloc(length * sizeof(w_transfer*));
		_W_DROPTARGET(droptarget)->transfer[0] = (w_transfer*) mem;
		if (mem == 0) {
			_W_DROPTARGET(droptarget)->transfer_length = 0;
			return W_ERROR_NO_MEMORY;
		}
		memcpy(mem, transferAgents, length * sizeof(w_transfer*));
		_W_DROPTARGET(droptarget)->transfer_length = length;
	} else {
		_W_WIDGET(droptarget)->state |= STATE_DROPTARGET_TRANSFER_LOCAL;
		memcpy(_W_DROPTARGET(droptarget)->transfer, transferAgents,
				length * sizeof(w_transfer*));
		_W_DROPTARGET(droptarget)->transfer_length = length;
	}
	size_t count = 0, types_length;
	w_transfer_type *types;
	for (size_t i = 0; i < length; i++) {
		w_transfer *transfer = transferAgents[i];
		if (transfer != 0) {
			types_length = 0;
			types = transfer->get_types(transfer, &types_length);
			count += types_length;
		}
	}
	GtkTargetEntry *pTargets = g_malloc(count * sizeof(GtkTargetEntry));
	count = 0;
	for (size_t i = 0; i < length; i++) {
		w_transfer *transfer = transferAgents[i];
		if (transfer != 0) {
			types_length = 0;
			types = transfer->get_types(transfer, &types_length);
			for (size_t j = 0; j < types_length; j++) {
				pTargets[count].info = types[j].id;
				pTargets[count].flags = 0;
				pTargets[count].target = (char*) types[j].name;
				count++;
			}
		}
	}
	GdkDragAction actions = (GdkDragAction) _w_droptarget_opToOsOp(
	_W_WIDGET(droptarget)->style);
	if (w_widget_class_id(W_WIDGET(control)) == _W_CLASS_COMBOBOX) {
		if ((_W_WIDGET(control)->style & W_READ_ONLY) == 0) {
			GtkWidget *entryHandle = gtk_bin_get_child(GTK_BIN(handle));
			if (entryHandle != 0) {
				gtk_drag_dest_unset(entryHandle);
			}
		}
	}
	gtk_drag_dest_set(handle, GTK_DEST_DEFAULT_ALL, pTargets, count, actions);
	g_free(pTargets);
	return W_TRUE;
}
int _w_droptarget_dispose_0(w_widget *widget) {
	w_control *control = _W_DROPTARGET(widget)->control;
	if (control == 0)
		return W_FALSE;
	g_source_remove_by_user_data(widget);
	GtkWidget *handle = _W_WIDGET(control)->handle;
	g_signal_handler_disconnect(handle,
	_W_DROPTARGET(widget)->drag_motion_handler);
	g_signal_handler_disconnect(handle,
	_W_DROPTARGET(widget)->drag_leave_handler);
	g_signal_handler_disconnect(handle,
	_W_DROPTARGET(widget)->drag_data_received_handler);
	g_signal_handler_disconnect(handle,
	_W_DROPTARGET(widget)->drag_drop_handler);
	/*_gtk_signal *signals = gtk_toolkit->signals;
	 g_signal_handler_disconnect(handle, signals[SIGNAL_DRAG_MOTION].id);
	 g_signal_handler_disconnect(handle, signals[SIGNAL_DRAG_LEAVE].id);
	 g_signal_handler_disconnect(handle, signals[SIGNAL_DRAG_DATA_RECEIVED].id);
	 g_signal_handler_disconnect(handle, signals[SIGNAL_DRAG_DROP].id);*/
	if (_W_DROPTARGET(widget)->transfer_length != 0)
		gtk_drag_dest_unset(handle);
	_W_WIDGET(widget)->clazz = 0;
	return W_TRUE;
}
wresult _w_droptarget_post_event(w_widget *widget, w_event *e, int flags) {
	wresult result = W_FALSE;
	if (e->type == W_EVENT_DISPOSE) {
		result = _w_droptarget_dispose_0(widget);
	}
	if (widget->post_event != 0) {
		result = widget->post_event(widget, e);
	}
	return result;
}
wresult _w_droptarget_dispose(w_widget *widget) {
	_w_droptarget_dispose_0(widget);
	return W_TRUE;
}
wresult _w_droptarget_create(w_widget *widget, w_widget *parent, wuint64 style,
		w_widget_post_event_proc post_event) {

}
void _w_droptarget_class_init(w_toolkit *toolkit, wushort classId,
		struct _w_droptarget_class *clazz) {
	_w_widget_class_init(toolkit, classId, W_WIDGET_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_DROPTARGET;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_droptarget_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_droptarget);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_droptarget);
	W_WIDGET_CLASS(clazz)->create = _w_droptarget_create;
	W_WIDGET_CLASS(clazz)->post_event = _w_droptarget_post_event;
	W_WIDGET_CLASS(clazz)->dispose = _w_droptarget_dispose;
	clazz->get_control = _w_droptarget_get_control;
	clazz->get_transfer = _w_droptarget_get_transfer;
	clazz->set_transfer = _w_droptarget_set_transfer;
	_gtk_signal *signals = gtk_toolkit->signals;
	signals[SIGNAL_DRAG_MOTION].number_of_args = 5;
	signals[SIGNAL_DRAG_MOTION].callback =
			(GCallback) _w_droptarget_drag_motion;
	signals[SIGNAL_DRAG_LEAVE].number_of_args = 3;
	signals[SIGNAL_DRAG_LEAVE].callback = (GCallback) _w_droptarget_drag_leave;
	signals[SIGNAL_DRAG_DATA_RECEIVED].number_of_args = 7;
	signals[SIGNAL_DRAG_DATA_RECEIVED].callback =
			(GCallback) _w_droptarget_drag_data_received;
	signals[SIGNAL_DRAG_DROP].number_of_args = 5;
	signals[SIGNAL_DRAG_DROP].callback = (GCallback) _w_droptarget_drag_drop;
}

