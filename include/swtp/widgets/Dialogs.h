/*
 * Dialogs.h
 *
 *  Created on: 6 f�vr. 2020
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_WIDGETS_DIALOGS_H_
#define SWTP_WIDGETS_DIALOGS_H_
#include "Frame.h"
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
inline int WMessageBox(WFrame *parent, wuint64 style, const char *title,
		const char *message) {
	w_messagebox messagebox;
	messagebox.parent = W_SHELL(parent);
	messagebox.style = style;
	messagebox.title = title;
	messagebox.message = message;
	return w_messagebox_open(0, &messagebox);
}

class WColorDialog: public w_dialog_color {
public:
	WColorDialog() {
		memset(this, 0, sizeof(WColorDialog));
	}
	int _Open(WToolkit *toolkit) {
		return w_dialog_color_open(W_TOOLKIT(toolkit), this);
	}
	int Open(WToolkit *toolkit, WFrame *parent, const char *title) {
		this->parent =(w_shell*) parent;
		this->title = title;
		return _Open(toolkit);
	}
	int Open(WFrame *parent, const char *title) {
		return Open(0, parent, title);
	}
	int Open(WFrame *parent) {
		this->parent =(w_shell*) parent;
		return _Open(0);
	}
	int Open() {
		return _Open(0);
	}
};
class WFileDialogSelectedFile: protected w_dialog_selected_file {
public:
	static size_t _alloc(void *user_data, size_t size, void **buf);
	size_t mem_alloc;
	char *text;
	WFileDialogSelectedFile() {
		this->mem_alloc = 0;
		this->text = 0;
		this->user_data = this;
		this->enc = W_ENCODING_UTF8;
		this->alloc = _alloc;
	}
	~WFileDialogSelectedFile() {
		if (this->text != 0) {
			free(this->text);
		}
	}
	const char* GetFile() {
		return this->text;
	}
};
class WFileDialog: public w_dialog_file {
public:
	WFileDialog() {
		memset(this, 0, sizeof(WFileDialog));
		w_iterator_init(&selectedfiles);
	}
	~WFileDialog() {
		w_iterator_close(&selectedfiles);
	}
public:
	WResult _Open(WToolkit *toolkit) {
		return w_dialog_file_open(W_TOOLKIT(toolkit), this);
	}
	WResult Open() {
		return _Open(0);
	}
	bool NextSelected(WFileDialogSelectedFile &selectedFile) {
		return w_iterator_next(&this->selectedfiles, (void*) &selectedFile);
	}
	bool NextSelected(WString &str) {
		str = (const char*) 0;
		w_dialog_selected_file selectedFile;
		selectedFile.alloc = w_alloc_string_ref;
		selectedFile.user_data = &str;
		selectedFile.enc = W_ENCODING_UTF8;
		return w_iterator_next(&this->selectedfiles, (void*) &selectedFile);
	}
	bool NextSelected(std::string &str) {
		str.clear();
		w_dialog_selected_file selectedFile;
		selectedFile.alloc = w_alloc_std_string;
		selectedFile.user_data = &str;
		selectedFile.enc = W_ENCODING_UTF8;
		return w_iterator_next(&this->selectedfiles, (void*) &selectedFile);
	}
	bool ResetSelected() {
		return w_iterator_reset(&this->selectedfiles) > 0;
	}
	WString GetSelectedFile() {
		WString str;
		w_dialog_selected_file selectedFile;
		selectedFile.alloc = w_alloc_string_ref;
		selectedFile.user_data = &str;
		selectedFile.enc = W_ENCODING_UTF8;
		w_iterator_reset(&this->selectedfiles);
		w_iterator_next(&this->selectedfiles, (void*) &selectedFile);
		return str;
	}
	std::string GetSelectedFileStd() {
		std::string str;
		w_dialog_selected_file selectedFile;
		selectedFile.alloc = w_alloc_std_string;
		selectedFile.user_data = &str;
		selectedFile.enc = W_ENCODING_UTF8;
		w_iterator_reset(&this->selectedfiles);
		w_iterator_next(&this->selectedfiles, (void*) &selectedFile);
		return str;
	}
	size_t GetSelectedCount() {
		return w_iterator_get_count(&this->selectedfiles);
	}
};
#endif /* SWTP_WIDGETS_DIALOGS_H_ */
