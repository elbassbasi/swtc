/*
 * Label.h
 *
 *  Created on: 9 ao�t 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_CONTROLS_LABEL_H_
#define SWTP_CONTROLS_LABEL_H_
#include "../widgets/Control.h"
/**
 * Instances of this class represent a non-selectable
 * user interface object that displays a string or image.
 * When SEPARATOR is specified, displays a single
 * vertical or horizontal line.
 * <p>
 * Shadow styles are hints and may not be honored
 * by the platform.  To create a separator label
 * with the default shadow style for the platform,
 * do not specify a shadow style.
 * </p>
 * <dl>
 * <dt><b>Styles:</b></dt>
 * <dd>SEPARATOR, HORIZONTAL, VERTICAL</dd>
 * <dd>SHADOW_IN, SHADOW_OUT, SHADOW_NONE</dd>
 * <dd>CENTER, LEFT, RIGHT, WRAP</dd>
 * <dt><b>Events:</b></dt>
 * <dd>(none)</dd>
 * </dl>
 * <p>
 * Note: Only one of SHADOW_IN, SHADOW_OUT and SHADOW_NONE may be specified.
 * SHADOW_NONE is a HINT. Only one of HORIZONTAL and VERTICAL may be specified.
 * Only one of CENTER, LEFT and RIGHT may be specified.
 * </p><p>
 * IMPORTANT: This class is <em>not</em> intended to be subclassed.
 * </p>
 */
class SWTP_PUBLIC WLabel: public WControl {
public:
	WLabel() {

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
	 * @see SWT#SEPARATOR
	 * @see SWT#HORIZONTAL
	 * @see SWT#VERTICAL
	 * @see SWT#SHADOW_IN
	 * @see SWT#SHADOW_OUT
	 * @see SWT#SHADOW_NONE
	 * @see SWT#CENTER
	 * @see SWT#LEFT
	 * @see SWT#RIGHT
	 * @see SWT#WRAP
	 * @see Widget#checkSubclass
	 * @see Widget#getStyle
	 */
	WLabel(WComposite *parent, wuint64 style) {
		Create(parent, style);
	}
	bool CreateLabel(WComposite *parent, const char *text) {
		bool result = Create(parent, W_NONE);
		SetText(text);
		return result;
	}
	bool CreateLabel(WComposite &parent, const char *text) {
		return CreateLabel(&parent, text);
	}
	/**
	 * Returns the receiver's image if it has one, or null
	 * if it does not.
	 *
	 * @return the receiver's image
	 */
	bool GetImage(WImage &image) {
		return w_label_get_image(W_LABEL(this), W_IMAGE(&image)) > 0;
	}
	/**
	 * Returns the receiver's text, which will be an empty
	 * string if it has never been set or if the receiver is
	 * a <code>SEPARATOR</code> label.
	 *
	 * @return the receiver's text
	 */
	WString GetText() {
		w_string_ref *ref = 0;
		w_label_get_text(W_LABEL(this), w_alloc_string_ref, &ref,
				W_ENCODING_UTF8);
		return ref;
	}
	char* GetTextChars() {
		char *buffer = 0;
		w_label_get_text(W_LABEL(this), w_alloc_buffer_new, &buffer,
				W_ENCODING_UTF8);
		return buffer;
	}
	/**
	 * Sets the receiver's image to the argument, which may be
	 * null indicating that no image should be displayed.
	 *
	 * @param image the image to display on the receiver (may be null)
	 */
	bool SetImage(WImage *image) {
		return w_label_set_image(W_LABEL(this), W_IMAGE(image)) > 0;
	}
	/**
	 * Sets the receiver's text.
	 * <p>
	 * This method sets the widget label.  The label may include
	 * the mnemonic character and line delimiters.
	 * </p>
	 * <p>
	 * Mnemonics are indicated by an '&amp;' that causes the next
	 * character to be the mnemonic.  When the user presses a
	 * key sequence that matches the mnemonic, focus is assigned
	 * to the control that follows the label. On most platforms,
	 * the mnemonic appears underlined but may be emphasised in a
	 * platform specific manner.  The mnemonic indicator character
	 * '&amp;' can be escaped by doubling it in the string, causing
	 * a single '&amp;' to be displayed.
	 * </p>
	 *
	 * @param string the new text
	 */
	bool SetText(const char *string) {
		return w_label_set_text(W_LABEL(this), string, -1, W_ENCODING_UTF8) > 0;
	}
protected:
	w_class_id _GetClassID();
private:
	void *handles[(sizeof(w_label) - sizeof(w_control)) / sizeof(void*)];
};
#endif /* SWTP_CONTROLS_LABEL_H_ */
