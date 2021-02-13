/*
 * Spinner.h
 *
 *  Created on: 10 août 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_CONTROLS_SPINNER_H_
#define SWTP_CONTROLS_SPINNER_H_
#include "../widgets/Composite.h"
typedef w_scrollbar_value WSpinnerValue;
/**
 * Instances of this class are selectable user interface objects that allow the
 * user to enter and modify numeric values.
 * <p>
 * Note that although this class is a subclass of <code>Composite</code>, it
 * does not make sense to add children to it, or set a layout on it.
 * </p>
 * <p>
 * <dl>
 * <dt><b>Styles:</b></dt>
 * <dd>READ_ONLY, WRAP</dd>
 * <dt><b>Events:</b></dt>
 * <dd>Selection, Modify, Verify</dd>
 * </dl>
 * </p>
 * <p>
 * IMPORTANT: This class is <em>not</em> intended to be subclassed.
 * </p>
 */
class SWTP_PUBLIC WSpinner: public WComposite {
public:
	WSpinner() {

	}
	/**
	 * Constructs a new instance of this class given its parent and a style
	 * value describing its behavior and appearance.
	 * <p>
	 * The style value is either one of the style constants defined in class
	 * <code>SWT</code> which is applicable to instances of this class, or must
	 * be built by <em>bitwise OR</em>'ing together (that is, using the
	 * <code>int</code> "|" operator) two or more of those <code>SWT</code>
	 * style constants. The class description lists the style constants that are
	 * applicable to the class. Style bits are also inherited from superclasses.
	 * </p>
	 *
	 * @param parent
	 *            a composite control which will be the parent of the new
	 *            instance (cannot be null)
	 * @param style
	 * @see SWT#READ_ONLY
	 * @see SWT#WRAP
	 * @see Widget#checkSubclass
	 * @see Widget#getStyle
	 */
	WSpinner(WComposite *parent, wuint64 style) {
		Create(parent, style);
	}
	/**
	 * Copies the selected text.
	 * <p>
	 * The current selection is copied to the clipboard.
	 * </p>
	 */
	bool Copy() {
		return w_spinner_copy(W_SPINNER(this)) > 0;
	}
	/**
	 * Cuts the selected text.
	 * <p>
	 * The current selection is first copied to the clipboard and then deleted
	 * from the widget.
	 * </p>
	 */
	bool Cut() {
		return w_spinner_cut(W_SPINNER(this)) > 0;
	}
	/**
	 * Returns the number of decimal places used by the receiver.
	 *
	 * @return the digits
	 */
	int GetDigits() {
		return w_spinner_get_digits(W_SPINNER(this));
	}
	/**
	 * Returns the amount that the receiver's value will be modified by when the
	 * up/down arrows are pressed.
	 *
	 * @return the increment
	 */
	int GetIncrement() {
		return w_spinner_get_increment(W_SPINNER(this));
	}
	/**
	 * Returns the maximum value which the receiver will allow.
	 *
	 * @return the maximum
	 */
	int GetMaximum() {
		return w_spinner_get_maximum(W_SPINNER(this));
	}
	/**
	 * Returns the minimum value which the receiver will allow.
	 *
	 * @return the minimum
	 */
	int GetMinimum() {
		return w_spinner_get_minimum(W_SPINNER(this));
	}
	/**
	 * Returns the amount that the receiver's position will be modified by when
	 * the page up/down keys are pressed.
	 *
	 * @return the page increment
	 */
	int GetPageIncrement() {
		return w_spinner_get_page_increment(W_SPINNER(this));
	}
	/**
	 * Returns the <em>selection</em>, which is the receiver's position.
	 *
	 * @return the selection
	 */
	int GetSelection() {
		return w_spinner_get_selection(W_SPINNER(this));
	}
	/**
	 * Returns a string containing a copy of the contents of the receiver's text
	 * field, or an empty string if there are no contents.
	 *
	 * @return the receiver's text
	 */
	WString GetText() {
		w_string_ref *ref = 0;
		w_spinner_get_text(W_SPINNER(this), w_alloc_string_ref, &ref,
				W_ENCODING_UTF8);
		return ref;
	}
	/**
	 * Returns the maximum number of characters that the receiver's text field
	 * is capable of holding. If this has not been changed by
	 * <code>setTextLimit()</code>, it will be the constant
	 * <code>Spinner.LIMIT</code>.
	 *
	 * @return the text limit
	 *
	 * @see #LIMIT
	 */
	int GetTextLimit() {
		return w_spinner_get_text_limit(W_SPINNER(this));
	}
	/**
	 * Pastes text from clipboard.
	 * <p>
	 * The selected text is deleted from the widget and new text inserted from
	 * the clipboard.
	 * </p>
	 */
	bool Paste() {
		return w_spinner_paste(W_SPINNER(this)) > 0;
	}
	/**
	 * Sets the number of decimal places used by the receiver.
	 * <p>
	 * The digit setting is used to allow for floating point values in the
	 * receiver. For example, to set the selection to a floating point value of
	 * 1.37 call setDigits() with a value of 2 and setSelection() with a value
	 * of 137. Similarly, if getDigits() has a value of 2 and getSelection()
	 * returns 137 this should be interpreted as 1.37. This applies to all
	 * numeric APIs.
	 * </p>
	 *
	 * @param value
	 *            the new digits (must be greater than or equal to zero)
	 */
	bool SetDigits(int value) {
		return w_spinner_set_digits(W_SPINNER(this), value) > 0;
	}
	/**
	 * Sets the amount that the receiver's value will be modified by when the
	 * up/down arrows are pressed to the argument, which must be at least one.
	 *
	 * @param value
	 *            the new increment (must be greater than zero)
	 */
	bool SetIncrement(int value) {
		return w_spinner_set_increment(W_SPINNER(this), value) > 0;
	}
	/**
	 * Sets the maximum value that the receiver will allow. This new value will
	 * be ignored if it is not greater than the receiver's current minimum
	 * value. If the new maximum is applied then the receiver's selection value
	 * will be adjusted if necessary to fall within its new range.
	 *
	 * @param value
	 *            the new maximum, which must be greater than the current
	 *            minimum
	 */
	bool SetMaximum(int value) {
		return w_spinner_set_maximum(W_SPINNER(this), value) > 0;
	}
	/**
	 * Sets the minimum value that the receiver will allow. This new value will
	 * be ignored if it is not less than the receiver's current maximum value.
	 * If the new minimum is applied then the receiver's selection value will be
	 * adjusted if necessary to fall within its new range.
	 *
	 * @param value
	 *            the new minimum, which must be less than the current maximum
	 */
	bool SetMinimum(int value) {
		return w_spinner_set_minimum(W_SPINNER(this), value) > 0;
	}
	/**
	 * Sets the amount that the receiver's position will be modified by when the
	 * page up/down keys are pressed to the argument, which must be at least
	 * one.
	 *
	 * @param value
	 *            the page increment (must be greater than zero)
	 */
	bool SetPageIncrement(int value) {
		return w_spinner_set_page_increment(W_SPINNER(this), value) > 0;
	}
	/**
	 * Sets the <em>selection</em>, which is the receiver's position, to the
	 * argument. If the argument is not within the range specified by minimum
	 * and maximum, it will be adjusted to fall within this range.
	 *
	 * @param value
	 *            the new selection (must be zero or greater)
	 */
	bool SetSelection(int value) {
		return w_spinner_set_selection(W_SPINNER(this), value) > 0;
	}
	/**
	 * Sets the maximum number of characters that the receiver's text field is
	 * capable of holding to be the argument.
	 * <p>
	 * To reset this value to the default, use
	 * <code>setTextLimit(Spinner.LIMIT)</code>. Specifying a limit value larger
	 * than <code>Spinner.LIMIT</code> sets the receiver's limit to
	 * <code>Spinner.LIMIT</code>.
	 * </p>
	 *
	 * @param limit
	 *            new text limit
	 * @see #LIMIT
	 */
	bool SetTextLimit(int limit) {
		return w_spinner_set_text_limit(W_SPINNER(this), limit) > 0;
	}
	bool SetValues(WSpinnerValue &value) {
		return w_spinner_set_values(W_SPINNER(this), (w_spinner_value*) &value)
				> 0;
	}
	/**
	 * Sets the receiver's selection, minimum value, maximum value, digits,
	 * increment and page increment all at once.
	 * <p>
	 * Note: This is similar to setting the values individually using the
	 * appropriate methods, but may be implemented in a more efficient fashion
	 * on some platforms.
	 * </p>
	 *
	 * @param selection
	 *            the new selection value
	 * @param minimum
	 *            the new minimum value
	 * @param maximum
	 *            the new maximum value
	 * @param digits
	 *            the new digits value
	 * @param increment
	 *            the new increment value
	 * @param pageIncrement
	 *            the new pageIncrement value
	 */
	bool SetValues(int selection, int minimum, int maximum, int digits,
			int increment, int pageIncrement) {
		WSliderValue value;
		value.selection = selection;
		value.minimum = minimum;
		value.maximum = maximum;
		value.thumb = digits;
		value.increment = increment;
		value.pageIncrement = pageIncrement;
		return SetValues(value);
	}
protected:
	w_class_id _GetClassID();
private:
	void *handles[(sizeof(w_spinner) - sizeof(w_composite)) / sizeof(void*)];
};

#endif /* SWTP_CONTROLS_SPINNER_H_ */
