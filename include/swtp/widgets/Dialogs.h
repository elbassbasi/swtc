/*
 * Dialogs.h
 *
 *  Created on: 6 f�vr. 2020
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_WIDGETS_DIALOGS_H_
#define SWTP_WIDGETS_DIALOGS_H_
#include "Shell.h"
/**
 * Show message box
 * <p>
 * The style value is either one of the style constants defined in
 * class <code>SWT</code> which is applicable to instances of this
 * class, or must be built by <em>bitwise OR</em>'ing together
 * (that is, using the <code>int</code> "|" operator) two or more
 * of those <code>SWT</code> style constants. The class description
 * lists the style constants that are applicable to the class.
 * Style bits are also inherited from superclasses.
 *
 * @param parent a shell which will be the parent of the new instance
 * @param style the style of dialog to construct
 *
 * @see W_ICON_ERROR
 * @see W_ICON_INFORMATION
 * @see W_ICON_QUESTION
 * @see W_ICON_WARNING
 * @see W_ICON_WORKING
 * @see W_OK
 * @see W_CANCEL
 * @see W_YES
 * @see W_NO
 * @see W_ABORT
 * @see W_RETRY
 * @see W_IGNORE
 */
inline int WMessageBox(WShell *parent, wuint64 style, const char *title,
		const char *message) {
	w_messagebox messagebox;
	messagebox.parent = W_SHELL(parent);
	messagebox.style = style;
	messagebox.title = title;
	messagebox.message = message;
	return w_messagebox_open(0, &messagebox);
}

class WColorDialog {
public:
	WColorDialog() {
		memset(this, 0, sizeof(WColorDialog));
	}
	WShell *parent;
	wuint64 style;
	const char *title;
	w_color selected_color;
	w_color *custom_colors;
	size_t count;
	int Open(WToolkit *toolkit) {
		return w_dialog_color_open((w_toolkit*) toolkit, (w_dialog_color*) this);
	}
	int Open() {
		return Open(0);
	}
};

class WFileDialog {
public:
	WFileDialog(){
		memset(this, 0, sizeof(WFileDialog));
	}
	WShell *parent;
	wuint64 style;
	WIterator<char*> files;
public:
	void GetFileNames(WIterator<char*>& files){

	}
	int Open(WToolkit *toolkit) {
		return false;
	}
	int Open() {
		return Open(0);
	}
};

#endif /* SWTP_WIDGETS_DIALOGS_H_ */
