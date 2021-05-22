/*
 * Button.h
 *
 *  Created on: 3 août 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_CONTROLS_BUTTON_H_
#define SWTP_CONTROLS_BUTTON_H_
#include "../widgets/Control.h"
/**
 * Instances of this class represent a selectable user interface object that
 * issues notification when pressed and released.
 * <dl>
 * <dt><b>Styles:</b></dt>
 * <dd>W_ARROW, W_CHECK, W_PUSH, W_RADIO, W_TOGGLE, W_FLAT</dd>
 * <dd>W_UP, W_DOWN, W_LEFT, W_RIGHT, W_CENTER</dd>
 * <dt><b>Events:</b></dt>
 * <dd>Selection</dd>
 * </dl>
 * <p>
 * Note: Only one of the styles W_ARROW, W_CHECK, W_PUSH, W_RADIO, and W_TOGGLE
 * may be specified.
 * </p><p>
 * Note: Only one of the styles W_LEFT, W_RIGHT, and W_CENTER may be specified.
 * </p><p>
 * Note: Only one of the styles W_UP, W_DOWN, W_LEFT, and W_RIGHT may be specified
 * when the W_ARROW style is specified.
 * </p>
 */
class SWTP_PUBLIC WButton: public WControl {
public:
	using WControl::Create;
	WButton() {

	}
	/**
	 * Constructs a new instance of this class given its parent
	 * and a style value describing its behavior and appearance.
	 * <p>
	 * The style value is either one of the style constants defined in
	 * class <code>SWT</code> which is applicable to instances of this
	 * class, or must be built by <em>bitwise OR</em>'ing together
	 * (that is, using the <code>int</code> "|" operator) two or more
	 * of those <code>SWT</code> style constants. The class description
	 * lists the style constants that are applicable to the class.
	 * Style bits are also inherited from superclasses.
	 * </p>
	 *
	 * @param parent a composite control which will be the parent of the new instance (cannot be null)
	 * @param style the style of control to construct
	 *
	 * @see W_ARROW
	 * @see W_CHECK
	 * @see W_PUSH
	 * @see W_RADIO
	 * @see W_TOGGLE
	 * @see W_FLAT
	 * @see W_UP
	 * @see W_DOWN
	 * @see W_LEFT
	 * @see W_RIGHT
	 * @see W_CENTER
	 * @see W_BORDER
	 * @see W_CLIP_CHILDREN
	 * @see W_CLIP_SIBLINGS
	 * @see W_LEFT_TO_RIGHT
	 * @see W_RIGHT_TO_LEFT
	 * @see W_MIRRORED
	 * @see W_DOUBLE_BUFFERED
	 * @see W_FLIP_TEXT_DIRECTION
	 * @see W_FREE_MEMORY
	 * @see Widget#getStyle
	 */
	WButton(WComposite *parent, wuint64 style) {
		Create(parent, style);
	}
	WResult Create(WToolkit *toolkit, WComposite *parent, wuint64 style);
	bool CreatePush(WComposite *parent, const char *text) {
		bool ret = Create(parent, W_PUSH);
		this->SetText(text);
		return ret;
	}
	bool CreatePush(WComposite &parent, const char *text) {
		return CreatePush(&parent, text);
	}
	bool CreateCheck(WComposite *parent, const char *text) {
		bool ret = Create(parent, W_CHECK);
		this->SetText(text);
		return ret;
	}
	bool CreateCheck(WComposite &parent, const char *text) {
		return CreateCheck(&parent, text);
	}
	bool CreateRadio(WComposite *parent, const char *text) {
		bool ret = Create(parent, W_RADIO);
		this->SetText(text);
		return ret;
	}
	bool CreateRadio(WComposite &parent, const char *text) {
		return CreateRadio(&parent, text);
	}
	bool CreateLink(WComposite *parent, const char *text) {
		bool ret = Create(parent, W_HYPERLINK);
		this->SetText(text);
		return ret;
	}
	/**
	 * Returns a value which describes the position of the
	 * text or image in the receiver. The value will be one of
	 * <code>LEFT</code>, <code>RIGHT</code> or <code>CENTER</code>
	 * unless the receiver is an <code>ARROW</code> button, in
	 * which case, the alignment will indicate the direction of
	 * the arrow (one of <code>LEFT</code>, <code>RIGHT</code>,
	 * <code>UP</code> or <code>DOWN</code>).
	 *
	 * @return the alignment
	 */
	int GetAlignment() {
		return _WReturnInt(_get_alignment());
	}
	/**
	 * Returns <code>true</code> if the receiver is grayed,
	 * and false otherwise. When the widget does not have
	 * the <code>CHECK</code> style, return false.
	 *
	 * @return the grayed state of the checkbox
	 */
	bool GetGrayed() {
		return _WReturnBool(_get_grayed());
	}
	/**
	 * Returns the receiver's image if it has one, or null
	 * if it does not.
	 *
	 * @return the receiver's image
	 */
	bool GetImage(WImage &image) {
		return GetImage(&image);
	}
	/**
	 * Returns the receiver's image if it has one, or null
	 * if it does not.
	 *
	 * @return the receiver's image
	 */
	bool GetImage(WImage *image) {
		return _WReturnBool(_get_image(image));
	}
	/**
	 * Returns <code>true</code> if the receiver is selected,
	 * and false otherwise.
	 * <p>
	 * When the receiver is of type <code>CHECK</code> or <code>RADIO</code>,
	 * it is selected when it is checked. When it is of type <code>TOGGLE</code>,
	 * it is selected when it is pushed in. If the receiver is of any other type,
	 * this method returns false.
	 *
	 * @return the selection state
	 */
	bool GetSelection() {
		return _WReturnBool(_get_selection());
	}
	/**
	 * Returns the receiver's text, which will be an empty
	 * string if it has never been set or if the receiver is
	 * an <code>ARROW</code> button.
	 *
	 * @return the receiver's text
	 */
	WString GetText() {
		w_string_ref *ref = 0;
		_get_text(w_alloc_string_ref, &ref, W_ENCODING_UTF8);
		return ref;
	}
	/**
	 * Controls how text, images and arrows will be displayed
	 * in the receiver. The argument should be one of
	 * <code>LEFT</code>, <code>RIGHT</code> or <code>CENTER</code>
	 * unless the receiver is an <code>ARROW</code> button, in
	 * which case, the argument indicates the direction of
	 * the arrow (one of <code>LEFT</code>, <code>RIGHT</code>,
	 * <code>UP</code> or <code>DOWN</code>).
	 *
	 * @param alignment the new alignment
	 */
	bool SetAlignment(int alignment) {
		return _WReturnBool(_set_alignment(alignment));
	}
	void SetAction(IWNotify *notify, IWNotify::SelectionAction action) {
		SetNotify(notify);
		SetData(__DATA_CPP_FUNCTION, *((void**) &action));
	}
	/**
	 * Sets the grayed state of the receiver.  This state change
	 * only applies if the control was created with the SWT.CHECK
	 * style.
	 *
	 * @param grayed the new grayed state
	 */
	bool SetGrayed(bool grayed) {
		return _WReturnBool(_set_grayed(grayed));
	}
	/**
	 * Sets the receiver's image to the argument, which may be
	 * <code>null</code> indicating that no image should be displayed.
	 * <p>
	 * Note that a Button can display an image and text simultaneously
	 * on Windows (starting with XP), GTK+ and OSX.  On other platforms,
	 * a Button that has an image and text set into it will display the
	 * image or text that was set most recently.
	 * </p>
	 * @param image the image to display on the receiver (may be <code>null</code>)
	 */
	bool SetImage(const WImage &image) {
		return SetImage(&image);
	}
	/**
	 * Sets the receiver's image to the argument, which may be
	 * <code>null</code> indicating that no image should be displayed.
	 * <p>
	 * Note that a Button can display an image and text simultaneously
	 * on Windows (starting with XP), GTK+ and OSX.  On other platforms,
	 * a Button that has an image and text set into it will display the
	 * image or text that was set most recently.
	 * </p>
	 * @param image the image to display on the receiver (may be <code>null</code>)
	 */
	bool SetImage(const WImage *image) {
		return _WReturnBool(_set_image(image));
	}
	/**
	 * Sets the selection state of the receiver, if it is of type <code>CHECK</code>,
	 * <code>RADIO</code>, or <code>TOGGLE</code>.
	 *
	 * <p>
	 * When the receiver is of type <code>CHECK</code> or <code>RADIO</code>,
	 * it is selected when it is checked. When it is of type <code>TOGGLE</code>,
	 * it is selected when it is pushed in.
	 *
	 * @param selected the new selection state
	 */
	bool SetSelection(bool selected) {
		return _WReturnBool(_set_selection(selected));
	}
	/**
	 * Sets the receiver's text.
	 * <p>
	 * This method sets the button label.  The label may include
	 * the mnemonic character but must not contain line delimiters.
	 * </p>
	 * <p>
	 * Mnemonics are indicated by an '&amp;' that causes the next
	 * character to be the mnemonic.  When the user presses a
	 * key sequence that matches the mnemonic, a selection
	 * event occurs. On most platforms, the mnemonic appears
	 * underlined but may be emphasized in a platform specific
	 * manner.  The mnemonic indicator character '&amp;' can be
	 * escaped by doubling it in the string, causing a single
	 * '&amp;' to be displayed.
	 * </p><p>
	 * Note that a Button can display an image and text simultaneously
	 * on Windows (starting with XP), GTK+ and OSX.  On other platforms,
	 * a Button that has an image and text set into it will display the
	 * image or text that was set most recently.
	 * </p>
	 * @param string the new text
	 */
	bool SetText(const char *text) {
		return SetText(text, -1);
	}
	bool SetText(const char *text, size_t length) {
		return _WReturnBool(_set_text(text, length, W_ENCODING_UTF8));
	}
public:
#if __cplusplus >= 201103L
	void SetSelectionFunction(const WSelectionFunction &function) {
		WWidget::SetSelectionFunction(function);
	}
	void operator <<=(const WListenerFunction &function) {
		WWidget::SetSelectionFunction(function);
	}
#endif
	static bool IsButton(WWidget *widget) {
		return widget->GetClassId() == _W_CLASS_BUTTON;
	}
protected:
	bool PostEvent(WEvent *e);
	w_class_id _GetClassID();
	virtual bool OnSelection(WSelectionEvent &e);
public:
	WResult _get_alignment() {
		return w_button_get_alignment(W_BUTTON(this));
	}
	WResult _get_grayed() {
		return w_button_get_grayed(W_BUTTON(this));
	}
	WResult _get_image(WImage *image) {
		return w_button_get_image(W_BUTTON(this), W_IMAGE(image));
	}
	WResult _get_selection() {
		return w_button_get_selection(W_BUTTON(this));
	}
	WResult _get_text(w_alloc alloc, void *user_data, int enc) {
		return w_button_get_text(W_BUTTON(this), alloc, user_data, enc);
	}
	WResult _set_alignment(int alignment) {
		return w_button_set_alignment(W_BUTTON(this), alignment);
	}
	WResult _set_grayed(bool grayed) {
		return w_button_set_grayed(W_BUTTON(this), grayed);
	}
	WResult _set_image(const WImage *image) {
		return w_button_set_image(W_BUTTON(this), W_IMAGE(image));
	}
	WResult _set_selection(bool selected) {
		return w_button_set_selection(W_BUTTON(this), selected);
	}
	WResult _set_text(const char *text, size_t length, int enc) {
		return w_button_set_text(W_BUTTON(this), text, length, enc);
	}
private:
	void *handles[(sizeof(w_button) - sizeof(w_control)) / sizeof(void*)];
};

#endif /* SWTP_CONTROLS_BUTTON_H_ */
