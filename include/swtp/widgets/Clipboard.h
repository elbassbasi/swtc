/*
 * Transfer.h
 *
 *  Created on: 4 oct. 2020
 *      Author: azeddine
 */

#ifndef SWTP_WIDGETS_CLIPBOARD_H_
#define SWTP_WIDGETS_CLIPBOARD_H_
#include "Control.h"
typedef w_transfer_type WTransferType;
//typedef w_transfer WTransfer;
typedef w_transfer_data WTransferData;
typedef w_transfers WTransfers;

class WTransfer: public w_transfer {
public:
	static WTransfer* GetTransfer(int type) {
		return (WTransfer*) w_transfer_get(type);
	}
	static WTransfer* GetTextTransfer() {
		return GetTransfer(W_TRANSFER_TEXT);
	}
	static WTransfer* GetFileTransfer() {
		return GetTransfer(W_TRANSFER_FILE);
	}
	static WTransfer* GetHTMLTransfer() {
		return GetTransfer(W_TRANSFER_HTML);
	}
	static WTransfer* GetImageTransfer() {
		return GetTransfer(W_TRANSFER_IMAGE);
	}
	static WTransfer* GetRTFTransfer() {
		return GetTransfer(W_TRANSFER_RTF);
	}
	static WTransfer* GetURLTransfer() {
		return GetTransfer(W_TRANSFER_URL);
	}
public:
	bool IsSupportedType(WTransferData *transferData) {
		return this->is_supported_type(this, transferData) > 0;
	}
	WTransferType* GetTypes(size_t *length) {
		return this->get_types(this, length);
	}
	size_t GetData(WTransferData *transferData, w_alloc alloc,
			void *user_data) {
		return this->get_data(this, transferData, alloc, user_data);
	}
	bool SetData(WTransferData *transferData, const void *data, size_t size) {
		return this->set_data(this, transferData, data, size) > 0;
	}
	bool Registre() {
		return w_transfer_registre(this) > 0;
	}
	bool Registre(const char *name) {
		return w_transfer_registre_with_name(this, name);
	}
};

/**
 * The <code>Clipboard</code> provides a mechanism for transferring data from one
 * application to another or within an application.
 *
 * <p>IMPORTANT: This class is <em>not</em> intended to be subclassed.</p>
 */
class SWTP_PUBLIC WClipboard {
public:
	WClipboard() {
		w_clipboard_init(W_CLIPBOARD(this));
	}
	/**
	 * Constructs a new instance of this class.  Creating an instance of a Clipboard
	 * may cause system resources to be allocated depending on the platform.  It is therefore
	 * mandatory that the Clipboard instance be disposed when no longer required.
	 *
	 * <p>The clipboards value is either one of the clipboard constants defined in
	 * class <code>DND</code>, or must be built by <em>bitwise OR</em>'ing together
	 * (that is, using the <code>int</code> "|" operator) two or more
	 * of those <code>DND</code> clipboard constants.</p>
	 *
	 * @param toolkit the toolkit on which to allocate the clipboard
	 * @param clipboards to be cleared
	 *
	 * @see DND#CLIPBOARD
	 * @see DND#SELECTION_CLIPBOARD
	 *
	 * @see Clipboard#close
	 */
	bool Open(WToolkit *toolkit, int clipboards) {
		return w_toolkit_open_clipboard((w_toolkit*) toolkit, W_CLIPBOARD(this),
				clipboards) > 0;
	}
	bool Open(int clipboards) {
		return Open(0, clipboards);
	}
	/**
	 * Constructs a new instance of this class.  Creating an instance of a Clipboard
	 * may cause system resources to be allocated depending on the platform.  It is therefore
	 * mandatory that the Clipboard instance be disposed when no longer required.
	 *
	 * @param display the display on which to allocate the clipboard
	 *
	 * @see Clipboard#close
	 */
	bool Open() {
		return Open(W_CLIPBOARD);
	}
	bool OpenSelection() {
		return Open(W_SELECTION_CLIPBOARD);
	}
	/**
	 * If this clipboard is currently the owner of the data on the system clipboard,
	 * clear the contents.  If this clipboard is not the owner, then nothing is done.
	 * Note that there are clipboard assistant applications that take ownership of
	 * data or make copies of data when it is placed on the clipboard.  In these
	 * cases, it may not be possible to clear the clipboard.
	 */
	bool ClearContents() {
		return w_clipboard_clear_contents(W_CLIPBOARD(this)) > 0;
	}
	/**
	 * Disposes of the operating system resources associated with the clipboard.
	 * The data will still be available on the system clipboard after the dispose
	 * method is called.
	 *
	 * <p>NOTE: On some platforms the data will not be available once the application
	 * has exited or the display has been disposed.</p>
	 */
	void Close() {
		w_clipboard_close(W_CLIPBOARD(this));
	}
	/**
	 * Retrieve the data of the specified type currently available on the system
	 * clipboard.  Refer to the specific subclass of <code>Transfer</code> to
	 * determine the type of object returned.
	 *
	 * <p>The following snippet shows text and RTF text being retrieved from the
	 * clipboard:</p>
	 *
	 *<code><pre>
	 *	WClipboard clipboard;
	 *	clipboard.Open();
	 *	WString str;
	 *	if(clipboard.GetContents(WTransfer::GetTextTransfer(),str)){
	 *		printf("Text is %s",str.GetCharsNotNull());
	 *	}
	 *	if(clipboard.GetContents(WTransfer::GetRTFTransfer(),str)){
	 *		printf("RTF Text is %s",str.GetCharsNotNull());
	 *	}
	 *	clipboard.Close();
	 *</code></pre>
	 *
	 * @param transfer the transfer agent for the type of data being requested
	 * @return the data obtained from the clipboard or null if no data of this type is available
	 *
	 * @see Transfer
	 */
	bool GetContents(WTransfer *transfer, w_alloc alloc, void *user_data) {
		return get_contents(transfer, alloc, user_data) > 0;
	}
	bool GetContents(WTransfer *transfer, void *data, size_t size,
			size_t *needsize) {
		w_alloc_buffer alloc;
		alloc.buffer = data;
		alloc.size = size;
		alloc.total_size = 0;
		bool ret = GetContents(transfer, w_alloc_buffer_copy, &alloc);
		if (needsize != 0)
			*needsize = alloc.total_size;
		return ret;
	}
	bool GetContents(WTransfer *transfer, WString &str) {
		w_string_ref_dec(str.ref);
		str.ref = 0;
		return GetContents(transfer, w_alloc_string_ref, &str.ref);
	}
	size_t GetContentsSize(WTransfer *transfer) {
		size_t size = 0;
		GetContents(transfer, 0, 0, &size);
		return size;
	}
	bool IsSupported(WTransfer *transfer) {
		return get_contents(transfer, 0, 0) > 0;
	}
	/**
	 * Place data of the specified type on the system clipboard.  More than one type
	 * of data can be placed on the system clipboard at the same time.  Setting the
	 * data clears any previous data from the system clipboard, regardless of type.
	 *
	 * <p>NOTE: On some platforms, the data is immediately copied to the system
	 * clipboard but on other platforms it is provided upon request.  As a result,
	 * if the application modifies the data object it has set on the clipboard, that
	 * modification may or may not be available when the data is subsequently
	 * requested.</p>
	 *
	 * <p>The following snippet shows text and RTF text being set on the copy/paste
	 * clipboard:
	 * </p>
	 *
	 * <code><pre>
	 * 	WClipboard clipboard;
	 *	clipboard.Open();
	 *	WTransfers transfers[2];
	 *	transfers[0].transfer = WTransfer::GetTextTransfer();
	 *	transfers[0].data = "Hello World";
	 *	transfers[0].size = strlen((char*)transfers[0].data);
	 *	transfers[1].transfer = WTransfer::GetRTFTransfer();
	 *	transfers[1].data = "{\\rtf1\\b\\i Hello World}";
	 *	transfers[1].size = strlen((char*)transfers[0].data);
	 *	SetContents(transfers, 2);
	 *	clipboard.Close();
	 * </code></pre>
	 *
	 * @param data the data to be set in the clipboard
	 * @param dataTypes the transfer agents that will convert the data to its
	 * platform specific format; each entry in the data array must have a
	 * corresponding dataType
	 *
	 * <p>NOTE: ERROR_CANNOT_SET_CLIPBOARD should be an SWTException, since it is a
	 * recoverable error, but can not be changed due to backward compatibility.</p>
	 */
	bool SetContents(WTransfers *transfers, size_t length) {
		return set_contents(transfers, length) > 0;
	}
	bool SetContents(WTransfer *transfer, void *data, size_t size) {
		WTransfers transfers;
		transfers.transfer = transfer;
		transfers.data = data;
		transfers.size = size;
		return SetContents(&transfers, 1);
	}
	bool SetContents(WTransfer *transfer, WString str) {
		WTransfers transfers;
		transfers.transfer = transfer;
		transfers.data = str.GetCharsNotNull();
		transfers.size = str.GetLength();
		return SetContents(&transfers, 1);
	}
	bool getAvailableTypes(WIterator<WTransferType> &types) {
		return get_available_types(types) > 0;
	}
public:
	void init() {
		w_clipboard_init(W_CLIPBOARD(this));
	}
	WResult clear_contents() {
		return w_clipboard_clear_contents(W_CLIPBOARD(this));
	}
	WResult close() {
		return w_clipboard_close(W_CLIPBOARD(this));
	}
	bool is_ok() {
		return w_clipboard_is_ok(W_CLIPBOARD(this));
	}
	WResult get_available_types(WIterator<WTransferType> &types) {
		return w_clipboard_get_available_types(W_CLIPBOARD(this),
				(w_iterator*) &types);
	}
	WResult get_contents(WTransfer *transfer, w_alloc alloc, void *user_data) {
		return w_clipboard_get_contents(W_CLIPBOARD(this),
				(w_transfer*) transfer, alloc, user_data);
	}
	WResult set_contents(WTransfers *transfers, size_t length) {
		return w_clipboard_set_contents(W_CLIPBOARD(this),
				(w_transfers*) transfers, length);
	}
private:
	void *data[sizeof(w_clipboard) / sizeof(void*)];
};

#endif /* SWTP_WIDGETS_CLIPBOARD_H_ */
