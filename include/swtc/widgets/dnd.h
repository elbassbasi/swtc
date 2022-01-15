/*
 * Name:        dnd.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef SWT_WIDGETS_DND_H_
#define SWT_WIDGETS_DND_H_
#include "widget.h"
#ifdef __cplusplus
extern "C" {
#endif
enum {
	W_CLIPBOARD = 1 << 0,
	W_SELECTION_CLIPBOARD = 1 << 1,
	W_DROP_NONE = 0,
	W_DROP_COPY = 1 << 1,
	W_DROP_MOVE = 1 << 2,
	W_DROP_LINK = 1 << 3,
	W_DROP_TARGET_MOVE = 1 << 4,
	W_DROP_DEFAULT = 1 << 5,
	W_EVENT_DRAGFINISHED = 2000,
	W_EVENT_DRAGSETDATA = 2001,
	W_EVENT_DRAGENTER = 2002,
	W_EVENT_DRAGLEAVE = 2003,
	W_EVENT_DRAGOVER = 2004,
	W_EVENT_DRAGOPERATIONCHANGED = 2005,
	W_EVENT_DROP = 2006,
	W_EVENT_DROPACCEPT = 2007,
	W_EVENT_DRAGSTART = 2008,
	W_FEEDBACK_NONE = 0,
	W_FEEDBACK_SELECT = 1,
	W_FEEDBACK_INSERT_BEFORE = 2,
	W_FEEDBACK_INSERT_AFTER = 4,
	W_FEEDBACK_SCROLL = 8,
	W_FEEDBACK_EXPAND = 16,
	W_ERROR_CANNOT_INIT_DRAG = -2000,
	W_ERROR_CANNOT_INIT_DROP = -2001,
	W_ERROR_CANNOT_SET_CLIPBOARD = -2002,
	W_ERROR_INVALID_DATA = -2003
};
typedef struct _w_transfer_data w_transfer_data;
typedef struct _w_transfer w_transfer;
typedef struct w_transfer_type {
	const char *name;
	wintptr id;
	void *reserved;
} w_transfer_type;
struct _w_transfer {
	wresult (*is_supported_type)(w_transfer *currenttransfer,
			w_transfer_data *transferData);
	w_transfer_type* (*get_types)(w_transfer *currenttransfer, size_t *length);
	size_t (*get_data)(w_transfer *currenttransfer,
			w_transfer_data *transferData, w_alloc alloc, void *user_data);
	wresult (*set_data)(w_transfer *currenttransfer,
			w_transfer_data *transferData, const void *data, size_t size);
	w_transfer_type type;
};

typedef struct w_transfers {
	w_transfer *transfer;
	const void *data;
	size_t size;
} w_transfers;

SWT_PUBLIC wresult w_transfer_registre(w_transfer *transfer);
SWT_PUBLIC wresult w_transfer_registre_with_name(w_transfer *transfer,
		const char *name);
SWT_PUBLIC wresult w_transfer_is_supported_type(w_transfer *transfer,
		w_transfer_data *transferData);
SWT_PUBLIC w_transfer_type* w_transfer_get_types(w_transfer *transfer,
		size_t *length);
SWT_PUBLIC size_t w_transfer_get_data(w_transfer *transfer,
		w_transfer_data *transferData, w_alloc alloc, void *user_data);
SWT_PUBLIC wresult w_transfer_set_data(w_transfer *transfer,
		w_transfer_data *transferData, const void *data, size_t size);
/*
 * Clipboard
 */
typedef struct w_clipboard {
	void *handles[0x5];
} w_clipboard;
#define W_CLIPBOARD(x) ((w_clipboard*)x)
SWT_PUBLIC void w_clipboard_init(w_clipboard *clipboard);
SWT_PUBLIC wresult w_clipboard_clear_contents(w_clipboard *clipboard);
SWT_PUBLIC wresult w_clipboard_close(w_clipboard *clipboard);
SWT_PUBLIC wresult w_clipboard_is_ok(w_clipboard *clipboard);
SWT_PUBLIC wresult w_clipboard_get_available_types(w_clipboard *clipboard,
		w_iterator *types);
SWT_PUBLIC wresult w_clipboard_get_contents(w_clipboard *clipboard,
		w_transfer *transfer, w_alloc alloc, void *user_data);
SWT_PUBLIC wresult w_clipboard_set_contents(w_clipboard *clipboard,
		w_transfers *transfers, size_t length);
/*
 * DragSource
 */
typedef struct w_dragsource {
	w_widget widget;
	void *handle[0x30];
} w_dragsource;
struct _w_dragsource_class {
	struct _w_widget_class widget;
	w_control* (*get_control)(w_dragsource *dragsource);
	w_transfer** (*get_transfer)(w_dragsource *dragsource, size_t *length);
	wresult (*set_transfer)(w_dragsource *dragsource,
			w_transfer **transferAgents, size_t length);
};
typedef struct w_event_dragsource {
	w_event event;
	union {
		struct {
			unsigned doit :1;
			unsigned dropcopy :1;
			unsigned dropmove :1;
			unsigned droplink :1;
			unsigned droptargetmove :1;
			unsigned dropdefault :1;
		};
		int detail;
	};
	int x;
	int y;
	int offsetX;
	int offsetY;
	w_transfer *transfer;
	w_transfer_data *dataType;
	w_image image;
} w_event_dragsource;
#define W_DRAGSOURCE(x) ((w_dragsource*)x)
#define W_DRAGSOURCE_GET_CLASS(x) ((struct _w_dragsource_class*)W_WIDGET_GET_CLASS(x))
SWT_PUBLIC w_control* w_dragsource_get_control(w_dragsource *dragsource);
SWT_PUBLIC w_transfer** w_dragsource_get_transfer(w_dragsource *dragsource,
		size_t *length);
SWT_PUBLIC wresult w_dragsource_set_transfer(w_dragsource *dragsource,
		w_transfer **transferAgents, size_t length);
/*
 * DropTarget
 */
typedef struct w_droptarget {
	w_widget widget;
	void *handle[0x30];
} w_droptarget;
struct _w_droptarget_class {
	struct _w_widget_class widget;
	w_control* (*get_control)(w_droptarget *droptarget);
	w_transfer** (*get_transfer)(w_droptarget *droptarget, size_t *length);
	wresult (*set_transfer)(w_droptarget *droptarget,
			w_transfer **transferAgents, size_t length);
};
typedef struct w_dnd_item {
	w_item item;
	void *reserved[10];
} w_dnd_item;
typedef struct w_event_droptarget {
	w_event event;
	union {
		struct {
			unsigned doit :1;
			unsigned dropcopy :1;
			unsigned dropmove :1;
			unsigned droplink :1;
			unsigned droptargetmove :1;
			unsigned dropdefault :1;
		};
		int detail;
	};
	int x;
	int y;
	int operations;
	int feedback;
	w_iterator *dataTypes;
	w_transfer_data *currentDataType;
	w_transfer *currenttransfer;
	w_dnd_item item;
} w_event_droptarget;
#define W_DROPTARGET(x) ((w_droptarget*)x)
#define W_DROPTARGET_GET_CLASS(x) ((struct _w_droptarget_class*)W_WIDGET_GET_CLASS(x))
SWT_PUBLIC w_control* w_droptarget_get_control(w_droptarget *droptarget);
SWT_PUBLIC w_transfer** w_droptarget_get_transfer(w_droptarget *droptarget,
		size_t *length);
SWT_PUBLIC wresult w_droptarget_set_transfer(w_droptarget *droptarget,
		w_transfer **transferAgents, size_t length);
/*
 * transfer
 */
enum {
	W_TRANSFER_TEXT = 1, //
	W_TRANSFER_FILE, //
	W_TRANSFER_HTML, //
	W_TRANSFER_IMAGE, //
	W_TRANSFER_RTF, //
	W_TRANSFER_URL, //

};
SWT_PUBLIC w_transfer* w_transfer_get(int transfer);
/*
 * TextTransfer
 */
SWT_PUBLIC w_transfer_type* w_transfer_text_get_types(w_transfer *transfer,
		size_t *length);
SWT_PUBLIC size_t w_transfer_text_get_data(w_transfer *transfer,
		w_transfer_data *transferData, w_alloc alloc, void *user_data);
SWT_PUBLIC wresult w_transfer_text_set_data(w_transfer *transfer,
		w_transfer_data *transferData, const void *data, size_t size);
/*
 * FileTransfer
 */
SWT_PUBLIC w_transfer_type* w_transfer_file_get_types(w_transfer *transfer,
		size_t *length);
SWT_PUBLIC size_t w_transfer_file_get_data(w_transfer *transfer,
		w_transfer_data *transferData, w_alloc alloc, void *user_data);
SWT_PUBLIC wresult w_transfer_file_set_data(w_transfer *transfer,
		w_transfer_data *transferData, const void *data, size_t size);
/*
 * HTMLTransfer
 */
SWT_PUBLIC w_transfer_type* w_transfer_html_get_types(w_transfer *transfer,
		size_t *length);
SWT_PUBLIC size_t w_transfer_html_get_data(w_transfer *transfer,
		w_transfer_data *transferData, w_alloc alloc, void *user_data);
SWT_PUBLIC wresult w_transfer_html_set_data(w_transfer *transfer,
		w_transfer_data *transferData, const void *data, size_t size);
/*
 * ImageTransfer
 */
SWT_PUBLIC w_transfer_type* w_transfer_image_get_types(w_transfer *transfer,
		size_t *length);
SWT_PUBLIC size_t w_transfer_image_get_data(w_transfer *transfer,
		w_transfer_data *transferData, w_alloc alloc, void *user_data);
SWT_PUBLIC wresult w_transfer_image_set_data(w_transfer *transfer,
		w_transfer_data *transferData, const void *data, size_t size);
/*
 * RTFTransfer
 */
SWT_PUBLIC w_transfer_type* w_transfer_rtf_get_types(w_transfer *transfer,
		size_t *length);
SWT_PUBLIC size_t w_transfer_rtf_get_data(w_transfer *transfer,
		w_transfer_data *transferData, w_alloc alloc, void *user_data);
SWT_PUBLIC wresult w_transfer_rtf_set_data(w_transfer *transfer,
		w_transfer_data *transferData, const void *data, size_t size);
/*
 * URLTransfer
 */
SWT_PUBLIC w_transfer_type* w_transfer_url_get_types(w_transfer *transfer,
		size_t *length);
SWT_PUBLIC size_t w_transfer_url_get_data(w_transfer *transfer,
		w_transfer_data *transferData, w_alloc alloc, void *user_data);
SWT_PUBLIC wresult w_transfer_url_set_data(w_transfer *transfer,
		w_transfer_data *transferData, const void *data, size_t size);

#ifdef __cplusplus
}
#endif
#endif /* SWT_WIDGETS_DND_H_ */
