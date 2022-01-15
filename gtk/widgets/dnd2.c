/*
 * dnd2.c
 *
 *  Created on: 13 janv. 2022
 *      Author: azeddine
 */
#include "dnd.h"
#include "toolkit.h"
/*
 * Transfer
 */
int _w_toolkit_register_type(w_toolkit *toolkit, const char *formatName) {

}
wresult w_transfer_registre(w_transfer *transfer) {
	size_t types_length = 0;
	w_transfer_type *types = transfer->get_types(transfer, &types_length);
	for (size_t i = 0; i < types_length; i++) {
		types[i].id = (wintptr) gdk_atom_intern(types[i].name, W_FALSE);
	}
	return W_TRUE;
}
wresult w_transfer_registre_with_name(w_transfer *transfer, const char *name) {
	transfer->type.name = name;
	transfer->is_supported_type = w_transfer_is_supported_type;
	transfer->get_data = w_transfer_get_data;
	transfer->set_data = w_transfer_set_data;
	return w_transfer_registre(transfer);
}
wresult w_transfer_is_supported_type(w_transfer *transfer,
		w_transfer_data *transferData) {
	if (transferData == 0)
		return W_FALSE;
	size_t types_length = 0;
	w_transfer_type *types = transfer->get_types(transfer, &types_length);
	for (size_t i = 0; i < types_length; i++) {
		if ((GdkAtom) types[i].id == transferData->type) {
			return W_TRUE;
		}
	}
	return W_FALSE;
}
w_transfer_type* w_transfer_get_types(w_transfer *transfer, size_t *length) {
	if (length != 0)
		*length = 1;
	return (w_transfer_type*) &transfer->type;
}
size_t w_transfer_get_data(w_transfer *transfer, w_transfer_data *transferData,
		w_alloc alloc, void *user_data) {
	if (!transfer->is_supported_type(transfer, transferData)
			|| transferData->pValue == 0)
		return 0;
	size_t size = transferData->format * transferData->length / 8;
	if (size == 0)
		return 0;
	if (alloc != 0) {
		void *data = 0;
		size_t newsize = alloc(user_data, size, &data);
		if (data != 0) {
			int sz = MIN(size, newsize);
			memmove(data, transferData->pValue, sz);
			return sz;
		}
	}
	return size;
}
wresult w_transfer_set_data(w_transfer *transfer, w_transfer_data *transferData,
		const void *data, size_t size) {
	transferData->result = 0;
	if (size == 0 || data == 0
			|| !transfer->is_supported_type(transfer, transferData))
		return W_FALSE;
	void *pValue = g_malloc(size);
	if (pValue == 0)
		return W_FALSE;
	memmove(pValue, data, size);
	transferData->length = size;
	transferData->format = 8;
	transferData->pValue = pValue;
	transferData->result = 1;
	return W_TRUE;
}
/*
 * TextTransfer
 */
w_transfer_type w_transfer_text_types[] = { //
		{ "UTF8_STRING" }, //
				{ "STRING" }, //
				{ "COMPOUND_TEXT" }, //
		};
w_transfer_type* w_transfer_text_get_types(w_transfer *transfer,
		size_t *length) {
	if (gtk_toolkit->ISX11) {
		*length = 3;
	} else {
		*length = 2;
	}
	return w_transfer_text_types;
}
size_t w_transfer_text_get_data(w_transfer *transfer,
		w_transfer_data *transferData, w_alloc alloc, void *user_data) {
	if (!transfer->is_supported_type(transfer, transferData)
			|| transferData->pValue == 0)
		return 0;
	gchar **list;
	int count = gdk_text_property_to_utf8_list_for_display(
			gdk_display_get_default(), transferData->type, transferData->format,
			transferData->pValue, transferData->length, &list);
	if (count == 0)
		return 0;
	gchar *ptr = list[0];
	size_t length = strlen(ptr), ret_length;
	if (alloc != 0) {
		char *utf8 = 0;
		ret_length = alloc(user_data, length, (void**) &utf8);
		if (utf8 != 0) {
			memcpy(utf8, ptr, MIN(ret_length, length));
		}
	}
	g_strfreev(list);
	return length;
}
typedef gboolean (*__gdk_x11_display_utf8_to_compound_text)(GdkDisplay *display,
		const gchar *str, GdkAtom *encoding, gint *format, guchar **ctext,
		gint *length);
__gdk_x11_display_utf8_to_compound_text _gdk_x11_display_utf8_to_compound_text =
		0;
wresult w_transfer_text_set_data(w_transfer *transfer,
		w_transfer_data *transferData, const void *data, size_t size) {
	transferData->result = 0;
	if (data == 0 || size == 0
			|| !transfer->is_supported_type(transfer, transferData)) {
		return W_ERROR_INVALID_DATA;
	}
	const char *utf8 = data;
	if (gtk_toolkit->ISX11
			&& transferData->type == (GdkAtom) w_transfer_text_types[2].id) { //COMPOUND_TEXT_ID
		if (_gdk_x11_display_utf8_to_compound_text == 0) {
			if (gtk_toolkit->libX11 != 0) {
				_gdk_x11_display_utf8_to_compound_text = dlsym(
						gtk_toolkit->libX11,
						"gdk_x11_display_utf8_to_compound_text");
			}
			if (_gdk_x11_display_utf8_to_compound_text == 0)
				return W_FALSE;
		}
		GdkAtom encoding = 0;
		gint format = 0;
		guchar *ctext = 0;
		gint length = 0;
		gint result = _gdk_x11_display_utf8_to_compound_text(
				gdk_display_get_default(), utf8, &encoding, &format, &ctext,
				&length);
		if (!result)
			return W_FALSE;
		transferData->type = encoding;
		transferData->format = format;
		transferData->length = length;
		transferData->pValue = ctext;
		transferData->result = 1;
	}
	if (transferData->type == (GdkAtom) w_transfer_text_types[0].id) { //UTF8_STRING_ID
		guchar *pValue = g_malloc(size);
		if (pValue == 0)
			return W_ERROR_NO_MEMORY;
		memcpy(pValue, utf8, size);
		transferData->type = (GdkAtom) w_transfer_text_types[0].id;
		transferData->format = 8;
		transferData->length = size;
		transferData->pValue = pValue;
		transferData->result = 1;
	}
	if (transferData->type == (GdkAtom) w_transfer_text_types[1].id) { // STRING_ID
		gchar *string_target = gdk_utf8_to_string_target(utf8);
		if (string_target == 0)
			return W_ERROR_NO_MEMORY;
		transferData->type = (GdkAtom) w_transfer_text_types[1].id;
		transferData->format = 8;
		transferData->length = strlen(string_target);
		transferData->pValue = string_target;
		transferData->result = 1;
	}
	return W_TRUE;
}
w_transfer w_transfer_text = { //
		w_transfer_is_supported_type, //
				w_transfer_text_get_types, //
				w_transfer_text_get_data, //
				w_transfer_text_set_data, //
				"UTF8_STRING", 0, 0 };
/*
 * FileTransfer
 */
w_transfer_type w_transfer_file_types[] = { //
		{ "text/uri-list" }, //
				{ "x-special/gnome-copied-files" }, //
		};
w_transfer_type* w_transfer_file_get_types(w_transfer *transfer,
		size_t *length) {
	*length = 2;
	return w_transfer_file_types;
}
size_t w_transfer_file_get_data(w_transfer *transfer,
		w_transfer_data *transferData, w_alloc alloc, void *user_data) {
	if (!transfer->is_supported_type(transfer, transferData)
			|| transferData->pValue == 0 || transferData->length <= 0)
		return 0;
	int length = transferData->length;
	void *temp = transferData->pValue;
	gboolean gnomeList = transferData->type
			== (GdkAtom) w_transfer_file_types[0].id;
	/*int sepLength = gnomeList ? 1 : 2;
	 long [] files = new long[0];
	 int offset = 0;
	 for (int i = 0; i < length - 1; i++) {
	 gboolean terminator = gnomeList ? temp[i] == '\n' : temp[i] == '\r' && temp[i+1] == '\n';
	 if (terminator) {
	 if (!(gnomeList && offset == 0)) {
	 // The content of the first line in a gnome-list is always either 'copy' or 'cut'
	 int size =  i - offset;
	 long file = OS.g_malloc(size + 1);
	 byte[] fileBuffer = new byte[size + 1];
	 System.arraycopy(temp, offset, fileBuffer, 0, size);
	 OS.memmove(file, fileBuffer, size + 1);
	 long[] newFiles = new long[files.length + 1];
	 System.arraycopy(files, 0, newFiles, 0, files.length);
	 newFiles[files.length] = file;
	 files = newFiles;
	 }
	 offset = i + sepLength;
	 }
	 }
	 if (offset < temp.length - sepLength) {
	 int size =  temp.length - offset;
	 long file = OS.g_malloc(size + 1);
	 byte[] fileBuffer = new byte[size + 1];
	 System.arraycopy(temp, offset, fileBuffer, 0, size);
	 OS.memmove(file, fileBuffer, size + 1);
	 long[] newFiles = new long[files.length + 1];
	 System.arraycopy(files, 0, newFiles, 0, files.length);
	 newFiles[files.length] = file;
	 files = newFiles;
	 }
	 String[] fileNames = new String[0];
	 for (int i = 0; i < files.length; i++) {
	 long [] error = new long[1];
	 long localePtr = g_filename_from_uri(files[i], null, error);
	 OS.g_free(files[i]);
	 if (error[0] != 0 || localePtr == 0) continue;
	 long utf8Ptr = OS.g_filename_to_utf8(localePtr, -1, null, null, null);
	 if (utf8Ptr == 0) utf8Ptr = OS.g_filename_display_name (localePtr);
	 if (localePtr != utf8Ptr) OS.g_free (localePtr);
	 if (utf8Ptr == 0) continue;
	 long[] items_written = new long[1];
	 long utf16Ptr = OS.g_utf8_to_utf16(utf8Ptr, -1, null, items_written, null);
	 OS.g_free(utf8Ptr);
	 if (utf16Ptr == 0) continue;
	 length = (int)items_written[0];
	 char[] buffer = new char[length];
	 OS.memmove(buffer, utf16Ptr, length * 2);
	 OS.g_free(utf16Ptr);
	 String name = new String(buffer);
	 String[] newFileNames = new String[fileNames.length + 1];
	 System.arraycopy(fileNames, 0, newFileNames, 0, fileNames.length);
	 newFileNames[fileNames.length] = name;
	 fileNames = newFileNames;
	 }
	 if (fileNames.length == 0) return null;
	 return fileNames;*/
}
wresult w_transfer_file_set_data(w_transfer *transfer,
		w_transfer_data *transferData, const void *data, size_t size) {
	transferData->result = 0;
	if (transfer->is_supported_type(transfer, transferData) <= 0) {
		return W_ERROR_INVALID_DATA;
	}
	gboolean gnomeList = transferData->type
			== (GdkAtom) w_transfer_file_types[0].id;
	/*byte[] buffer, separator;
	 if (gnomeList) {
	 buffer = new byte[] { 'c', 'o', 'p', 'y' };
	 separator = new byte[] { '\n' };
	 } else {
	 buffer = new byte[0];
	 separator = new byte[] { '\r', '\n' };
	 }
	 String[] files = (String[])object;
	 for (int i = 0; i < files.length; i++) {
	 String string = files[i];
	 if (string == null)
	 continue;
	 int length = string.length();
	 if (length == 0)
	 continue;
	 char[] chars = new char[length];
	 string.getChars(0, length, chars, 0);
	 long [] error = new long[1];
	 long utf8Ptr = OS.g_utf16_to_utf8(chars, chars.length, null,
	 null, error);
	 if (error[0] != 0 || utf8Ptr == 0)
	 continue;
	 long localePtr = OS.g_filename_from_utf8(utf8Ptr, -1, null, null,
	 error);
	 OS.g_free(utf8Ptr);
	 if (error[0] != 0 || localePtr == 0)
	 continue;
	 long uriPtr = OS.g_filename_to_uri(localePtr, 0, error);
	 OS.g_free(localePtr);
	 if (error[0] != 0 || uriPtr == 0)
	 continue;
	 length = OS.strlen(uriPtr);
	 byte[] temp = new byte[length];
	 OS.memmove(temp, uriPtr, length);
	 OS.g_free(uriPtr);
	 int newLength =
	 (buffer.length > 0) ?
	 buffer.length + separator.length + temp.length :
	 temp.length;
	 byte[] newBuffer = new byte[newLength];
	 int offset = 0;
	 if (buffer.length > 0) {
	 System.arraycopy(buffer, 0, newBuffer, 0, buffer.length);
	 offset += buffer.length;
	 System.arraycopy(separator, 0, newBuffer, offset, separator.length);
	 offset += separator.length;
	 }
	 System.arraycopy(temp, 0, newBuffer, offset, temp.length);
	 buffer = newBuffer;
	 }
	 if (buffer.length == 0)
	 return;
	 long ptr = OS.g_malloc(buffer.length + 1);
	 OS.memset(ptr, '\0', buffer.length + 1);
	 OS.memmove(ptr, buffer, buffer.length);
	 transferData->pValue = ptr;
	 transferData->length = buffer.length;
	 transferData->format = 8;
	 transferData->result = 1;*/

}
w_transfer w_transfer_file = { //
		w_transfer_is_supported_type, //
				w_transfer_file_get_types, //
				w_transfer_file_get_data, //
				w_transfer_file_set_data, //
				"text/uri-list", 0, 0 };
/*
 * HTMLTransfer
 */
w_transfer_type w_transfer_html_types[] = { //
		{ "text/html" }, //
				{ "TEXT/HTML" }, //
		};
w_transfer_type* w_transfer_html_get_types(w_transfer *transfer,
		size_t *length) {
	*length = 2;
	return w_transfer_html_types;
}
size_t w_transfer_html_get_data(w_transfer *transfer,
		w_transfer_data *transferData, w_alloc alloc, void *user_data) {
	if (transfer->is_supported_type(transfer, transferData) <= 0
			|| transferData->pValue == 0)
		return 0;
	/* Ensure byteCount is a multiple of 2 bytes */
	int size = (transferData->format * transferData->length / 8) / 2 * 2;
	if (size <= 0)
		return 0;
	wchar bom = 0; // look for a Byte Order Mark
	if (size > 1) {
		memcpy(&bom, transferData->pValue, 2);
	}
	if (bom == 0xfeff || bom == 0xfffe) {
		// utf16
		int sz = w_utf8_from_utf16((wchar*) transferData->pValue, size / 2, 0,
				0);
		if (alloc != 0) {
			char *buf = 0;
			size_t newsize = alloc(user_data, sz, &buf);
			if (buf != 0) {
				int sz = w_utf8_from_utf16((wchar*) transferData->pValue,
						size / 2, buf, newsize);
			}
		}
		return sz;
	} else {
		int sz = size;
		if (alloc != 0) {
			char *buf = 0;
			size_t newsize = alloc(user_data, sz, &buf);
			if (buf != 0) {
				memcpy(buf, transferData->pValue, sz);
			}
		}
		return sz;
	}
}
wresult w_transfer_html_set_data(w_transfer *transfer,
		w_transfer_data *transferData, const void *data, size_t size) {
	transferData->result = 0;
	if (transfer->is_supported_type(transfer, transferData) <= 0) {
		return W_ERROR_INVALID_DATA;
	}
	void *pValue = g_malloc(size);
	if (pValue == 0)
		return W_ERROR_NO_MEMORY;
	memcpy(pValue, data, size);
	transferData->length = size;
	transferData->format = 8;
	transferData->pValue = pValue;
	transferData->result = 1;
	return W_TRUE;
}
w_transfer w_transfer_html = { //
		w_transfer_is_supported_type, //
				w_transfer_html_get_types, //
				w_transfer_html_get_data, //
				w_transfer_html_set_data, //
				"text/html", 0, 0 };
/*
 * ImageTransfer
 */
w_transfer_type w_transfer_image_types[] = { //
		{ "image/jpeg" }, //
				{ "image/png" }, //
				{ "image/bmp" }, //
				{ "image/eps" }, //
				{ "image/pcx" }, //
				{ "image/ppm" }, //
				{ "image/rgb" }, //
				{ "image/tga" }, //
				{ "image/xbm" }, //
				{ "image/xpm" }, //
				{ "image/xv" }, //
		};
w_transfer_type* w_transfer_image_get_types(w_transfer *transfer,
		size_t *length) {
	*length = sizeof(w_transfer_image_types)
			/ sizeof(w_transfer_image_types[0]);
	return w_transfer_image_types;
}
size_t w_transfer_image_get_data(w_transfer *transfer,
		w_transfer_data *transferData, w_alloc alloc, void *user_data) {
	if (transferData->length > 0) {
		if (alloc != 0) {
			w_image *img = 0;
			size_t newsize = alloc(user_data, sizeof(w_image), (void**) &img);
			if (img != 0 && newsize >= sizeof(w_image)) {
				w_image_dispose(img);
				GdkPixbufLoader *loader = gdk_pixbuf_loader_new();
				gdk_pixbuf_loader_write(loader, transferData->pValue,
						transferData->length, 0);
				gdk_pixbuf_loader_close(loader, 0);
				GdkPixbuf *pixbuf = gdk_pixbuf_loader_get_pixbuf(loader);
				_W_IMAGE(img)->pixbuf = pixbuf;
			}
		}
		return sizeof(w_image);
	} else
		return 0;
}
wresult w_transfer_image_set_data(w_transfer *transfer,
		w_transfer_data *transferData, const void *data, size_t size) {
	if (transfer->is_supported_type(transfer, transferData) <= 0) {
		return W_ERROR_INVALID_DATA;
	}
	w_image *imgData = (w_image*) data;
	if (imgData == 0)
		return W_ERROR_NULL_ARGUMENT;
	GdkPixbuf *pixbuf = _W_IMAGE(imgData)->pixbuf;
	if (pixbuf != 0) {
		const char *type = "";
		if (transferData->type == (GdkAtom) w_transfer_image_types[0].id)
			type = "jpeg";
		else if (transferData->type == (GdkAtom) w_transfer_image_types[1].id)
			type = "png";
		else if (transferData->type == (GdkAtom) w_transfer_image_types[2].id)
			type = "bmp";
		else if (transferData->type == (GdkAtom) w_transfer_image_types[3].id)
			type = "eps";
		else if (transferData->type == (GdkAtom) w_transfer_image_types[4].id)
			type = "pcx";
		else if (transferData->type == (GdkAtom) w_transfer_image_types[5].id)
			type = "ppm";
		else if (transferData->type == (GdkAtom) w_transfer_image_types[6].id)
			type = "rgb";
		else if (transferData->type == (GdkAtom) w_transfer_image_types[7].id)
			type = "tga";
		else if (transferData->type == (GdkAtom) w_transfer_image_types[8].id)
			type = "xbm";
		else if (transferData->type == (GdkAtom) w_transfer_image_types[9].id)
			type = "xpm";
		else if (transferData->type == (GdkAtom) w_transfer_image_types[10].id)
			type = "xv";
		char *buffer = 0;
		gsize len = 0;
		gdk_pixbuf_save_to_bufferv(pixbuf, &buffer, &len, type, 0, 0, 0);
		transferData->pValue = buffer;
		transferData->length = (int) (len + 3) / 4 * 4;
		transferData->result = 1;
		transferData->format = 32;
	}
	return W_TRUE;
}
w_transfer w_transfer_image = { //
		w_transfer_is_supported_type, //
				w_transfer_image_get_types, //
				w_transfer_image_get_data, //
				w_transfer_image_set_data, //
				"image/jpeg", 0, 0 };
/*
 * RTFTransfer
 */
w_transfer_type w_transfer_rtf_types[] = { //
		{ "text/rtf" }, //
				{ "TEXT/RTF" }, //
				{ "application/rtf" }, //
		};
w_transfer_type* w_transfer_rtf_get_types(w_transfer *transfer,
		size_t *length) {
	*length = sizeof(w_transfer_rtf_types) / sizeof(w_transfer_rtf_types[0]);
	return w_transfer_rtf_types;
}
size_t w_transfer_rtf_get_data(w_transfer *transfer,
		w_transfer_data *transferData, w_alloc alloc, void *user_data) {
	if (transfer->is_supported_type(transfer, transferData) <= 0
			|| transferData->pValue == 0)
		return 0;
	int size = transferData->format * transferData->length / 8;
	if (size == 0)
		return 0;
	if (alloc != 0) {
		char *utf8 = 0;
		size_t newsize = alloc(user_data, size, (void**) &utf8);
		if (utf8 != 0) {
			memcpy(utf8, transferData->pValue, size);
		}
	}
	return size;

}
wresult w_transfer_rtf_set_data(w_transfer *transfer,
		w_transfer_data *transferData, const void *data, size_t size) {
	transferData->result = 0;
	if (transfer->is_supported_type(transfer, transferData) <= 0) {
		return W_ERROR_INVALID_DATA;
	}
	guchar *pValue = g_malloc(size);
	if (pValue == 0)
		return W_ERROR_NO_MEMORY;
	memcpy(pValue, data, size);
	transferData->length = size - 1;
	transferData->format = 8;
	transferData->pValue = pValue;
	transferData->result = 1;
	return W_TRUE;
}
w_transfer w_transfer_rtf = { //
		w_transfer_is_supported_type, //
				w_transfer_rtf_get_types, //
				w_transfer_rtf_get_data, //
				w_transfer_rtf_set_data, //
				"text/rtf", 0, 0 };
/*
 * URLTransfer
 */
w_transfer_type w_transfer_url_types[] = { //
		{ "text/unicode" }, //
				{ "text/x-moz-url" }, //
		};
w_transfer_type* w_transfer_url_get_types(w_transfer *transfer,
		size_t *length) {
	*length = sizeof(w_transfer_url_types) / sizeof(w_transfer_url_types[0]);
	return w_transfer_url_types;
}
size_t w_transfer_url_get_data(w_transfer *transfer,
		w_transfer_data *transferData, w_alloc alloc, void *user_data) {
	if (transfer->is_supported_type(transfer, transferData) <= 0
			|| transferData->pValue == 0)
		return 0;
	int size = (transferData->format * transferData->length / 8) / 2 * 2;
	if (size == 0)
		return 0;
	size_t sz = w_utf8_from_utf16((wchar*) transferData->pValue, size, 0, 0);
	if (alloc != 0) {
		char *utf8 = 0;
		size_t newsize = alloc(user_data, sz, (void**) &utf8);
		if (utf8 != 0) {
			w_utf8_from_utf16((wchar*) transferData->pValue, size, utf8,
					newsize);
		}
	}
	return sz;
}
wresult w_transfer_url_set_data(w_transfer *transfer,
		w_transfer_data *transferData, const void *data, size_t size) {
	transferData->result = 0;
	if (transfer->is_supported_type(transfer, transferData) <= 0) {
		return W_ERROR_INVALID_DATA;
	}
	int newsz = w_utf8_to_utf16((char*) data, size, 0, 0);
	guchar *pValue = g_malloc(newsz);
	if (pValue == 0)
		return W_ERROR_NO_MEMORY;
	w_utf8_to_utf16((char*) data, size, (wchar*) pValue, newsz);
	transferData->length = newsz;
	transferData->format = 8;
	transferData->pValue = pValue;
	transferData->result = 1;
	return W_TRUE;
}
w_transfer w_transfer_url = { //
		w_transfer_is_supported_type, //
				w_transfer_url_get_types, //
				w_transfer_url_get_data, //
				w_transfer_url_set_data, //
				"text/unicode", 0, 0 };
wresult _w_transfers_registre() {
	w_transfer_registre(&w_transfer_text);
	w_transfer_registre(&w_transfer_file);
	w_transfer_registre(&w_transfer_url);
	w_transfer_registre(&w_transfer_image);
	w_transfer_registre(&w_transfer_html);
	w_transfer_registre(&w_transfer_rtf);
	return W_TRUE;
}
w_transfer* w_transfer_get(int transfer) {
	w_transfer* _tr = 0;
	switch (transfer) {
	case W_TRANSFER_TEXT:
		_tr = &w_transfer_text;
		break;
	case W_TRANSFER_FILE:
		_tr = &w_transfer_file;
		break;
	case W_TRANSFER_HTML:
		_tr = &w_transfer_html;
		break;
	case W_TRANSFER_IMAGE:
		_tr = &w_transfer_image;
		break;
	case W_TRANSFER_RTF:
		_tr = &w_transfer_rtf;
		break;
	case W_TRANSFER_URL:
		_tr = &w_transfer_url;
		break;
	}
	if(_tr != 0){
		w_transfer_registre(_tr);
	}
	return _tr;
}

