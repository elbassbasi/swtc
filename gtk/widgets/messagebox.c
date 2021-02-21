/*
 * messagebox.c
 *
 *  Created on: 21 févr. 2021
 *      Author: azeddine
 */
#include "toolkit.h"
gboolean _w_messagebox_emission_proc(GSignalInvocationHint *ihint,
		guint n_param_values, const GValue *param_values, gpointer data) {
	if (gtk_widget_get_toplevel(GTK_WIDGET(g_value_peek_pointer(param_values)))
			== data) {
		gtk_widget_set_direction(GTK_WIDGET(g_value_peek_pointer(param_values)),
				GTK_TEXT_DIR_RTL);
	}
	return 1;
}
void _w_messagebox_create_buttons(GtkWindow *handle,
		w_messagebox *messagebox, int style, int alignment) {
	if (alignment == W_LEFT) {
		if ((style & (1 << 5) /* W_OK*/) != 0) //W_OK
			gtk_dialog_add_button(GTK_DIALOG(handle), "gtk-ok", (1 << 5) /* W_OK*/);
//	 if ((style & W_ABORT) != 0) gtk_dialog_add_button(GTK_DIALOG(handle), Converter.wcsToMbcs (W_getMessage("SWT_Abort"), true), W_ABORT);
//	 if ((style & W_RETRY) != 0) gtk_dialog_add_button(GTK_DIALOG(handle), Converter.wcsToMbcs (W_getMessage("SWT_Retry"), true), W_RETRY);
		if ((style & W_YES) != 0)
			gtk_dialog_add_button(GTK_DIALOG(handle), "gtk-yes", W_YES);
		if ((style & W_NO) != 0)
			gtk_dialog_add_button(GTK_DIALOG(handle), "gtk-no", W_NO);
//	 if ((style & W_IGNORE) != 0) gtk_dialog_add_button(GTK_DIALOG(handle), Converter.wcsToMbcs (W_getMessage("SWT_Ignore"), true), W_IGNORE);
		if ((style & W_CANCEL) != 0)
			gtk_dialog_add_button(GTK_DIALOG(handle), "gtk-cancel", W_CANCEL);
	} else {
		if ((style & W_CANCEL) != 0)
			gtk_dialog_add_button(GTK_DIALOG(handle), "gtk-cancel", W_CANCEL);
		if ((style & (1 << 5) /* W_OK*/) != 0) //W_OK
			gtk_dialog_add_button(GTK_DIALOG(handle), "gtk-ok", (1 << 5) /* W_OK*/);
		if ((style & W_NO) != 0)
			gtk_dialog_add_button(GTK_DIALOG(handle), "gtk-no", W_NO);
		if ((style & W_YES) != 0)
			gtk_dialog_add_button(GTK_DIALOG(handle), "gtk-yes", W_YES);
//	 if ((style & W_IGNORE) != 0) gtk_dialog_add_button(GTK_DIALOG(handle), Converter.wcsToMbcs (W_getMessage("SWT_Ignore"), true), W_IGNORE);
//	 if ((style & W_RETRY) != 0) gtk_dialog_add_button(GTK_DIALOG(handle), Converter.wcsToMbcs (W_getMessage("SWT_Retry"), true), W_RETRY);
//	 if ((style & W_ABORT) != 0) gtk_dialog_add_button(GTK_DIALOG(handle), Converter.wcsToMbcs (W_getMessage("SWT_Abort"), true), W_ABORT);
	}
}

wuint64 _w_messagebox_check_style(wuint64 style) {
	wuint64 mask = (W_YES | W_NO | (1 << 5) /* W_OK*/ | W_CANCEL | W_ABORT | W_RETRY
			| W_IGNORE);
	wuint64 bits = style & mask;
	if (bits == (1 << 5) /* W_OK*/ || bits == W_CANCEL || bits == ((1 << 5) /* W_OK*/ | W_CANCEL))
		return style;
	if (bits == W_YES || bits == W_NO || bits == (W_YES | W_NO)
			|| bits == (W_YES | W_NO | W_CANCEL))
		return style;
	if (bits == (W_RETRY | W_CANCEL) || bits == (W_ABORT | W_RETRY | W_IGNORE))
		return style;
	style = (style & ~mask) | (1 << 5) /* W_OK*/;
	return style;
}
wresult _w_messagebox_open(w_toolkit *toolkit, w_messagebox *messagebox) {
	GtkWindow *parentHandle =
			GTK_WINDOW(
					(messagebox->parent != 0) ? _W_SHELL_HANDLE(messagebox->parent) : 0);
	wuint64 style = _w_messagebox_check_style(messagebox->style);
	GtkDialogFlags dialogFlags = GTK_DIALOG_DESTROY_WITH_PARENT;
	if ((style & (W_PRIMARY_MODAL | W_APPLICATION_MODAL | W_SYSTEM_MODAL))
			!= 0) {
		dialogFlags |= GTK_DIALOG_MODAL;
	}
	GtkMessageType messageType = GTK_MESSAGE_INFO;
	if ((style & (W_ICON_WARNING)) != 0)
		messageType = GTK_MESSAGE_WARNING;
	if ((style & (W_ICON_QUESTION)) != 0)
		messageType = GTK_MESSAGE_QUESTION;
	if ((style & (W_ICON_ERROR)) != 0)
		messageType = GTK_MESSAGE_ERROR;
	GtkButtonsType buttonsType = GTK_BUTTONS_NONE;
	if (style & (1 << 5)) { //W_OK
		if (style & W_CANCEL)
			buttonsType = GTK_BUTTONS_OK_CANCEL;
		else
			buttonsType = GTK_BUTTONS_OK;
	} else if (style & W_YES) {
		if (style & W_NO)
			buttonsType = GTK_BUTTONS_YES_NO;
		else
			buttonsType = GTK_BUTTONS_NONE;
	} else if (style & W_CLOSE) {
		buttonsType = GTK_BUTTONS_CLOSE;
	} else if (style & W_CANCEL) {
		buttonsType = GTK_BUTTONS_CANCEL;
	} else {
		buttonsType = GTK_BUTTONS_NONE;
	}
	GtkWindow *handle = (GtkWindow*) gtk_message_dialog_new(parentHandle,
			dialogFlags, messageType, buttonsType, "%s", messagebox->message);
	if (handle == 0)
		return W_ERROR_NO_HANDLES;
	if (parentHandle != 0) {
		GList *pixbufs = gtk_window_get_icon_list(parentHandle);
		if (pixbufs != 0) {
			gtk_window_set_icon_list(handle, pixbufs);
			g_list_free(pixbufs);
		}
	}
	/* Display display = parent != null ? parent.getDisplay (): Display.getCurrent ();*/
	//_w_messagebox_createButtons(handle, messagebox, style,
	//		W_LEFT/*display.getDismissalAlignment ()*/);
	gtk_window_set_title(handle, messagebox->title);
	/*display.addIdleProc ();
	 Dialog oldModal = null;*/
	/*
	 * In order to allow the dialog to be modal of it's
	 * parent shells, it is required to assign the
	 * dialog to the same window group as of the shells.
	 */
	GtkWindowGroup *group = gtk_window_get_group(0);
	gtk_window_group_add_window(group, handle);

	if (gtk_window_get_modal(handle)) {
		/*oldModal = display.getModalDialog ();
		 display.setModalDialog (this);*/
	}
	int signalId = 0;
	long hookId = 0;
	if ((style & W_RIGHT_TO_LEFT) != 0) {
		signalId = gtk_toolkit->signal_id[SIGNAL_MAP];
		hookId = g_signal_add_emission_hook(signalId, 0,
				_w_messagebox_emission_proc, handle, 0);
	}
	//display.sendPreExternalEventDispatchEvent ();
	int response = gtk_dialog_run(GTK_DIALOG(handle));
	/*
	 * This call to gdk_threads_leave() is a temporary work around
	 * to avoid deadlocks when gdk_threads_init() is called by native
	 * code outside of SWT (i.e AWT, etc). It ensures that the current
	 * thread leaves the GTK lock acquired by the function above.
	 */
	//gdk_threads_leave();
	//display.sendPostExternalEventDispatchEvent ();
	if ((style & W_RIGHT_TO_LEFT) != 0) {
		g_signal_remove_emission_hook(signalId, hookId);
	}
	if (gtk_window_get_modal(handle)) {
		//display.setModalDialog (oldModal);
	}
	//display.removeIdleProc ();
	gtk_widget_destroy(GTK_WIDGET(handle));
	return response;
}



