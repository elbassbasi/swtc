/*
 * Dnd.h
 *
 *  Created on: 9 déc. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_WIDGETS_DND_H_
#define SWTP_WIDGETS_DND_H_
#include "Clipboard.h"
class WDNDItem: public WItem {
private:
	void *reserved[(sizeof(w_dnd_item) - sizeof(w_item)) / sizeof(void*)];
public:
	WDNDItem() {

	}
};
/**
 * The DropTargetEvent contains the event information passed in the methods of the DropTargetListener.
 *
 */
class WDropTargetEvent: public WEvent {
public:
	/**
	 * The operation being performed.
	 * @see DND#DROP_NONE
	 * @see DND#DROP_MOVE
	 * @see DND#DROP_COPY
	 * @see DND#DROP_LINK
	 * @see DND#DROP_DEFAULT
	 */
	union {
		int detail;
		struct {
			unsigned doit :1;
			unsigned dropcopy :1;
			unsigned dropmove :1;
			unsigned droplink :1;
			unsigned droptargetmove :1;
			unsigned dropdefault :1;
		};
	};
	/**
	 * The x-cordinate of the cursor relative to the <code>Display</code>
	 */
	int x;

	/**
	 * The y-cordinate of the cursor relative to the <code>Display</code>
	 */
	int y;

	/**
	 * A bitwise OR'ing of the operations that the DragSource can support
	 * (e.g. DND.DROP_MOVE | DND.DROP_COPY | DND.DROP_LINK).
	 * The detail value must be a member of this list or DND.DROP_NONE.
	 * @see DND#DROP_NONE
	 * @see DND#DROP_MOVE
	 * @see DND#DROP_COPY
	 * @see DND#DROP_LINK
	 * @see DND#DROP_DEFAULT
	 */
	int operations;

	/**
	 * A bitwise OR'ing of the drag under effect feedback to be displayed to the user
	 * (e.g. DND.FEEDBACK_SELECT | DND.FEEDBACK_SCROLL | DND.FEEDBACK_EXPAND).
	 * <p>A value of DND.FEEDBACK_NONE indicates that no drag under effect will be displayed.</p>
	 * <p>Feedback effects will only be applied if they are applicable.</p>
	 * <p>The default value is DND.FEEDBACK_SELECT.</p>
	 * @see DND#FEEDBACK_NONE
	 * @see DND#FEEDBACK_SELECT
	 * @see DND#FEEDBACK_INSERT_BEFORE
	 * @see DND#FEEDBACK_INSERT_AFTER
	 * @see DND#FEEDBACK_SCROLL
	 * @see DND#FEEDBACK_EXPAND
	 */
	int feedback;
	WIterator<WTransferData*> *dataTypes;
	/**
	 * The type of data that will be dropped.
	 */
	WTransferData *currentDataType;
	WTransfer *currentTransfer;
	/**
	 * If the associated control is a table or tree, this field contains the item located
	 * at the cursor coordinates.
	 */
	WDNDItem item;
	size_t GetData(w_alloc alloc, void *user_data) {
		return this->currentTransfer->get_data(this->currentTransfer,
				this->currentDataType, alloc, user_data);
	}
	size_t GetData(void *data, size_t size) {
		w_alloc_buffer alloc;
		alloc.buffer = data;
		alloc.size = size;
		alloc.total_size = 0;
		return GetData(w_alloc_buffer_copy, &alloc);
	}
	size_t GetData(void **data) {
		return GetData(w_alloc_buffer_new, (void*) data);
	}
	size_t GetData(WString &str) {
		w_string_ref_dec(str.ref);
		str.ref = 0;
		return GetData(w_alloc_string_ref, &str.ref);
	}
};
/**
 *
 * Class <code>DropTarget</code> defines the target object for a drag and drop transfer.
 *
 * <p>This class identifies the <code>Control</code> over which the user must position the cursor
 * in order to drop the data being transferred.  It also specifies what data types can be dropped on
 * this control and what operations can be performed.  You may have several DropTragets in an
 * application but there can only be a one to one mapping between a <code>Control</code> and a <code>DropTarget</code>.
 * The DropTarget can receive data from within the same application or from other applications
 * (such as text dragged from a text editor like Word).</p>
 *
 * <code><pre>
 *	int operations = DND.DROP_MOVE | DND.DROP_COPY | DND.DROP_LINK;
 *	Transfer[] types = new Transfer[] {TextTransfer.getInstance()};
 *	DropTarget target = new DropTarget(label, operations);
 *	target.setTransfer(types);
 * </code></pre>
 *
 * <p>The application is notified of data being dragged over this control and of when a drop occurs by
 * implementing the interface <code>DropTargetListener</code> which uses the class
 * <code>DropTargetEvent</code>.  The application can modify the type of drag being performed
 * on this Control at any stage of the drag by modifying the <code>event.detail</code> field or the
 * <code>event.currentDataType</code> field.  When the data is dropped, it is the responsibility of
 * the application to copy this data for its own purposes.
 *
 * <code><pre>
 *	target.addDropListener (new DropTargetListener() {
 *		public void dragEnter(DropTargetEvent event) {};
 *		public void dragOver(DropTargetEvent event) {};
 *		public void dragLeave(DropTargetEvent event) {};
 *		public void dragOperationChanged(DropTargetEvent event) {};
 *		public void dropAccept(DropTargetEvent event) {}
 *		public void drop(DropTargetEvent event) {
 *			// A drop has occurred, copy over the data
 *			if (event.data == null) { // no data to copy, indicate failure in event.detail
 *				event.detail = DND.DROP_NONE;
 *				return;
 *			}
 *			label.setText ((String) event.data); // data copied to label text
 *		}
 * 	});
 * </pre></code>
 *
 * <dl>
 *	<dt><b>Styles</b></dt> <dd>DND.DROP_NONE, DND.DROP_COPY, DND.DROP_MOVE, DND.DROP_LINK</dd>
 *	<dt><b>Events</b></dt> <dd>DND.DragEnter, DND.DragLeave, DND.DragOver, DND.DragOperationChanged,
 *                             DND.DropAccept, DND.Drop </dd>
 * </dl>
 */
class SWTP_PUBLIC WDropTarget: public WWidget {
public:
	WDropTarget() {

	}
	/**
	 * Creates a new <code>DropTarget</code> to allow data to be dropped on the specified
	 * <code>Control</code>.
	 * Creating an instance of a DropTarget may cause system resources to be allocated
	 * depending on the platform.  It is therefore mandatory that the DropTarget instance
	 * be disposed when no longer required.
	 *
	 * @param control the <code>Control</code> over which the user positions the cursor to drop the data
	 * @param style the bitwise OR'ing of allowed operations; this may be a combination of any of
	 *		   DND.DROP_NONE, DND.DROP_COPY, DND.DROP_MOVE, DND.DROP_LINK
	 *
	 * <p>NOTE: ERROR_CANNOT_INIT_DROP should be an SWTException, since it is a
	 * recoverable error, but can not be changed due to backward compatibility.</p>
	 *
	 * @see Widget#dispose
	 * @see DropTarget#checkSubclass
	 * @see DND#DROP_NONE
	 * @see DND#DROP_COPY
	 * @see DND#DROP_MOVE
	 * @see DND#DROP_LINK
	 */
	bool Create(WControl *control, wuint64 style) {
		return Create(0, control, style) > 0;
	}
	bool Create(WToolkit *toolkit, WControl *control, wuint64 style) {
		return control->_create_droptarget(W_DROPTARGET(this), style,
				WWidget::post_event_proc) > 0;
	}
	/**
	 * Returns the Control which is registered for this DropTarget.  This is the control over which the
	 * user positions the cursor to drop the data.
	 *
	 * @return the Control which is registered for this DropTarget
	 */
	WControl* GetControl() {
		return _get_control();
	}

	/**
	 * Returns a list of the data types that can be transferred to this DropTarget.
	 *
	 * @return a list of the data types that can be transferred to this DropTarget
	 */
	WTransfer** GetTransfer(size_t *length) {
		return _get_transfer(length);
	}
	/**
	 * Specifies the data types that can be transferred to this DropTarget.  If data is
	 * being dragged that does not match one of these types, the drop target will be notified of
	 * the drag and drop operation but the currentDataType will be null and the operation
	 * will be DND.NONE.
	 *
	 * @param transferAgents a list of Transfer objects which define the types of data that can be
	 *						 dropped on this target
	 */
	bool SetTransfer(WTransfer **transferAgents, size_t length) {
		return _set_transfer(transferAgents, length) > 0;
	}
	bool SetTransfer(WTransfer *transferAgents) {
		return _set_transfer(&transferAgents, 1);
	}
	bool SetTransfer(size_t length, ...) {
		va_list args;
		va_start(args, length);
		WResult result = _set_transfer(length, args);
		va_end(args);
		return result > 0;
	}
#if __cplusplus >= 201103L
	bool SetTransfer(std::initializer_list<WTransfer*> &transfers) {
		return SetTransfer((WTransfer**) transfers.begin(), transfers.size());
	}
#endif
public:
	WControl* _get_control() {
		return (WControl*) w_droptarget_get_control(W_DROPTARGET(this));
	}
	WTransfer** _get_transfer(size_t *length) {
		return (WTransfer**) w_droptarget_get_transfer(W_DROPTARGET(this),
				length);
	}
	WResult _set_transfer(WTransfer **transferAgents, size_t length) {
		return w_droptarget_set_transfer(W_DROPTARGET(this),
				(w_transfer**) transferAgents, length);
	}
	WResult _set_transfer(size_t length, va_list args) {
		/*WTransfer *transferAgents[length];
		 for (size_t i = 0; i < length; i++) {
		 transferAgents[i] = va_arg(args, WTransfer*);
		 }
		 return _set_transfer(transferAgents, length);*/
		return W_FALSE;
	}
protected:
	w_class_id _GetClassID();
	bool PostEvent(WEvent *e);
	virtual bool OnDragEnter(WDropTargetEvent &e);
	virtual bool OnDragOver(WDropTargetEvent &e);
	virtual bool OnDragLeave(WDropTargetEvent &e);
	virtual bool OnDragOperationChanged(WDropTargetEvent &e);
	virtual bool OnDropAccept(WDropTargetEvent &e);
	virtual bool OnDrop(WDropTargetEvent &e);
private:
	void *handle[(sizeof(w_droptarget) - sizeof(w_widget)) / sizeof(void*)];
};

/**
 * The DragSourceEvent contains the event information passed in the methods of the DragSourceListener.
 *
 */
class WDragSourceEvent: public WEvent {
public:
	/**
	 * The operation that was performed.
	 * @see DND#DROP_NONE
	 * @see DND#DROP_MOVE
	 * @see DND#DROP_COPY
	 * @see DND#DROP_LINK
	 * @see DND#DROP_TARGET_MOVE
	 */
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

	/**
	 * In dragStart, the x coordinate (relative to the control) of the
	 * position the mouse went down to start the drag.
	 *
	 * @since 3.2
	 */
	int x;
	/**
	 * In dragStart, the y coordinate (relative to the control) of the
	 * position the mouse went down to start the drag.
	 *
	 * @since 3.2
	 */
	int y;
	/**
	 * In dragStart, the x offset (relative to the image) where the drag source image will be displayed.
	 */
	int offsetX;
	/**
	 * In dragStart, the y offset (relative to the image) where the drag source image will be displayed.
	 */
	int offsetY;
	WTransfer *transfer;
	/**
	 * The type of data requested.
	 * Data provided in the data field must be of the same type.
	 */
	WTransferData *dataType;

	/**
	 * The drag source image to be displayed during the drag.
	 * <p>A value of null indicates that no drag image will be displayed.</p>
	 * <p>The default value is null.</p>
	 */
	WImage image;
	void SetImage(WImage &image) {
		image.CopyTo(this->image);
	}
	bool SetData(const void *data, size_t size) {
		return this->transfer->set_data(this->transfer, this->dataType, data,
				size) > 0;
	}
};
/**
 *
 * <code>DragSource</code> defines the source object for a drag and drop transfer.
 *
 * <p>A drag source is the object which originates a drag and drop operation. For the specified widget,
 * it defines the type of data that is available for dragging and the set of operations that can
 * be performed on that data.  The operations can be any bit-wise combination of DND.MOVE, DND.COPY or
 * DND.LINK.  The type of data that can be transferred is specified by subclasses of Transfer such as
 * TextTransfer or FileTransfer.  The type of data transferred can be a predefined system type or it
 * can be a type defined by the application.  For instructions on how to define your own transfer type,
 * refer to <code>ByteArrayTransfer</code>.</p>
 *
 * <p>You may have several DragSources in an application but you can only have one DragSource
 * per Control.  Data dragged from this DragSource can be dropped on a site within this application
 * or it can be dropped on another application such as an external Text editor.</p>
 *
 * <p>The application supplies the content of the data being transferred by implementing the
 * <code>DragSourceListener</code> and associating it with the DragSource via DragSource#addDragListener.</p>
 *
 * <p>When a successful move operation occurs, the application is required to take the appropriate
 * action to remove the data from its display and remove any associated operating system resources or
 * internal references.  Typically in a move operation, the drop target makes a copy of the data
 * and the drag source deletes the original.  However, sometimes copying the data can take a long
 * time (such as copying a large file).  Therefore, on some platforms, the drop target may actually
 * move the data in the operating system rather than make a copy.  This is usually only done in
 * file transfers.  In this case, the drag source is informed in the DragEnd event that a
 * DROP_TARGET_MOVE was performed.  It is the responsibility of the drag source at this point to clean
 * up its displayed information.  No action needs to be taken on the operating system resources.</p>
 *
 * <p> The following example shows a Label widget that allows text to be dragged from it.</p>
 *
 * <code><pre>
 *	// Enable a label as a Drag Source
 *	Label label = new Label(shell, SWT.NONE);
 *	// This example will allow text to be dragged
 *	Transfer[] types = new Transfer[] {TextTransfer.getInstance()};
 *	// This example will allow the text to be copied or moved to the drop target
 *	int operations = DND.DROP_MOVE | DND.DROP_COPY;
 *
 *	DragSource source = new DragSource(label, operations);
 *	source.setTransfer(types);
 *	source.addDragListener(new DragSourceListener() {
 *		public void dragStart(DragSourceEvent e) {
 *			// Only start the drag if there is actually text in the
 *			// label - this text will be what is dropped on the target.
 *			if (label.getText().length() == 0) {
 *				event.doit = false;
 *			}
 *		};
 *		public void dragSetData(DragSourceEvent event) {
 *			// A drop has been performed, so provide the data of the
 *			// requested type.
 *			// (Checking the type of the requested data is only
 *			// necessary if the drag source supports more than
 *			// one data type but is shown here as an example).
 *			if (TextTransfer.getInstance().isSupportedType(event.dataType)){
 *				event.data = label.getText();
 *			}
 *		}
 *		public void dragFinished(DragSourceEvent event) {
 *			// A Move operation has been performed so remove the data
 *			// from the source
 *			if (event.detail == DND.DROP_MOVE)
 *				label.setText("");
 *		}
 *	});
 * </pre></code>
 *
 *
 * <dl>
 *	<dt><b>Styles</b></dt> <dd>DND.DROP_NONE, DND.DROP_COPY, DND.DROP_MOVE, DND.DROP_LINK</dd>
 *	<dt><b>Events</b></dt> <dd>DND.DragStart, DND.DragSetData, DND.DragEnd</dd>
 * </dl>
 */
class SWTP_PUBLIC WDragSource: public WWidget {
public:
	WDragSource() {

	}
	/**
	 * Creates a new <code>DragSource</code> to handle dragging from the specified <code>Control</code>.
	 * Creating an instance of a DragSource may cause system resources to be allocated depending on the platform.
	 * It is therefore mandatory that the DragSource instance be disposed when no longer required.
	 *
	 * @param control the <code>Control</code> that the user clicks on to initiate the drag
	 * @param style the bitwise OR'ing of allowed operations; this may be a combination of any of
	 *					DND.DROP_NONE, DND.DROP_COPY, DND.DROP_MOVE, DND.DROP_LINK
	 *
	 * <p>NOTE: ERROR_CANNOT_INIT_DRAG should be an SWTException, since it is a
	 * recoverable error, but can not be changed due to backward compatibility.</p>
	 *
	 * @see Widget#dispose
	 * @see DragSource#checkSubclass
	 * @see DND#DROP_NONE
	 * @see DND#DROP_COPY
	 * @see DND#DROP_MOVE
	 * @see DND#DROP_LINK
	 */
	bool Create(WControl *control, int style) {
		return Create(0, control, style);
	}
	bool Create(WToolkit *toolkit, WControl *control, int style) {
		return create(toolkit, control, style, WWidget::post_event_proc) > 0;
	}

	/**
	 * Returns the Control which is registered for this DragSource.  This is the control that the
	 * user clicks in to initiate dragging.
	 *
	 * @return the Control which is registered for this DragSource
	 */
	WControl* GetControl() {
		return get_control();
	}

	/**
	 * Returns the list of data types that can be transferred by this DragSource.
	 *
	 * @return the list of data types that can be transferred by this DragSource
	 */
	WTransfer** GetTransfer(size_t *length) {
		return get_transfer(length);
	}

	/**
	 * Specifies the list of data types that can be transferred by this DragSource.
	 * The application must be able to provide data to match each of these types when
	 * a successful drop has occurred.
	 *
	 * @param transferAgents a list of Transfer objects which define the types of data that can be
	 * dragged from this source
	 */
	bool SetTransfer(WTransfer **transferAgents, size_t length) {
		return set_transfer(transferAgents, length);
	}
	bool SetTransfer(WTransfer *transferAgents) {
		return set_transfer(&transferAgents, 1);
	}
	bool SetTransfer(size_t length, ...) {
		va_list args;
		va_start(args, length);
		WResult result = set_transfer(length, args);
		va_end(args);
		return result > 0;
	}
public:
	WResult create(WToolkit *toolkit, WControl *control, wuint64 style,
			w_widget_post_event_proc post_event) {
		return control->_create_dragsource(W_DRAGSOURCE(this), style,
				post_event);
	}
	WControl* get_control() {
		return (WControl*) w_dragsource_get_control(W_DRAGSOURCE(this));
	}
	WTransfer** get_transfer(size_t *length) {
		return (WTransfer**) w_dragsource_get_transfer(W_DRAGSOURCE(this),
				length);
	}
	WResult set_transfer(WTransfer **transferAgents, size_t length) {
		return w_dragsource_set_transfer(W_DRAGSOURCE(this),
				(w_transfer**) transferAgents, length);
	}
	WResult set_transfer(size_t length, va_list args) {
		/*WTransfer *transferAgents[length];
		 for (size_t i = 0; i < length; i++) {
		 transferAgents[i] = va_arg(args, WTransfer*);
		 }
		 return set_transfer(transferAgents, length);*/
		return W_FALSE;
	}
protected:
	w_class_id _GetClassID();
	bool PostEvent(WEvent *e);
	virtual bool OnDragStart(WDragSourceEvent &e);
	virtual bool OnDragSetData(WDragSourceEvent &e);
	virtual bool OnDragFinished(WDragSourceEvent &e);
private:
	void *handle[(sizeof(w_dragsource) - sizeof(w_widget)) / sizeof(void*)];
};

#endif /* SWTP_WIDGETS_DND_H_ */
