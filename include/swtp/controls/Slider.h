/*
 * Slider.h
 *
 *  Created on: 9 ao�t 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_CONTROLS_SLIDER_H_
#define SWTP_CONTROLS_SLIDER_H_
#include "../widgets/Control.h"
typedef w_scrollbar_value WSliderValue;
/**
 * Instances of this class are selectable user interface
 * objects that represent a range of positive, numeric values.
 * <p>
 * At any given moment, a given slider will have a
 * single 'selection' that is considered to be its
 * value, which is constrained to be within the range of
 * values the slider represents (that is, between its
 * <em>minimum</em> and <em>maximum</em> values).
 * </p><p>
 * Typically, sliders will be made up of five areas:
 * <ol>
 * <li>an arrow button for decrementing the value</li>
 * <li>a page decrement area for decrementing the value by a larger amount</li>
 * <li>a <em>thumb</em> for modifying the value by mouse dragging</li>
 * <li>a page increment area for incrementing the value by a larger amount</li>
 * <li>an arrow button for incrementing the value</li>
 * </ol>
 * Based on their style, sliders are either <code>HORIZONTAL</code>
 * (which have a left facing button for decrementing the value and a
 * right facing button for incrementing it) or <code>VERTICAL</code>
 * (which have an upward facing button for decrementing the value
 * and a downward facing buttons for incrementing it).
 * </p><p>
 * On some platforms, the size of the slider's thumb can be
 * varied relative to the magnitude of the range of values it
 * represents (that is, relative to the difference between its
 * maximum and minimum values). Typically, this is used to
 * indicate some proportional value such as the ratio of the
 * visible area of a document to the total amount of space that
 * it would take to display it. SWT supports setting the thumb
 * size even if the underlying platform does not, but in this
 * case the appearance of the slider will not change.
 * </p>
 * <dl>
 * <dt><b>Styles:</b></dt>
 * <dd>HORIZONTAL, VERTICAL</dd>
 * <dt><b>Events:</b></dt>
 * <dd>Selection</dd>
 * </dl>
 * <p>
 * Note: Only one of the styles HORIZONTAL and VERTICAL may be specified.
 * </p><p>
 * IMPORTANT: This class is <em>not</em> intended to be subclassed.
 * </p>
 *
 * @see ScrollBar
 */
class SWTP_PUBLIC WSlider: public WControl {
public:
	WSlider() {

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
	 * @see SWT#HORIZONTAL
	 * @see SWT#VERTICAL
	 * @see Widget#checkSubclass
	 * @see Widget#getStyle
	 */
	WSlider(WComposite *parent, wuint64 style) {
		Create(parent, style);
	}
	bool CreateScale(WComposite *parent) {
		return Create(parent, W_SCALE);
	}
	bool CreateScale(WComposite &parent) {
		return CreateScale(&parent);
	}
	bool CreateSlider(WComposite *parent) {
		return Create(parent, W_NONE);
	}
	bool CreateSlider(WComposite &parent) {
		return CreateSlider(&parent);
	}
	/**
	 * Returns the amount that the receiver's value will be
	 * modified by when the up/down (or right/left) arrows
	 * are pressed.
	 *
	 * @return the increment
	 */
	int GetIncrement() {
		return w_slider_get_increment(W_SLIDER(this));
	}
	/**
	 * Returns the maximum value which the receiver will allow.
	 *
	 * @return the maximum
	 */
	int GetMaximum() {
		return w_slider_get_maximum(W_SLIDER(this));
	}
	/**
	 * Returns the minimum value which the receiver will allow.
	 *
	 * @return the minimum
	 */
	int GetMinimum() {
		return w_slider_get_minimum(W_SLIDER(this));
	}
	/**
	 * Returns the amount that the receiver's value will be
	 * modified by when the page increment/decrement areas
	 * are selected.
	 *
	 * @return the page increment
	 */
	int GetPageIncrement() {
		return w_slider_get_page_increment(W_SLIDER(this));
	}
	/**
	 * Returns the 'selection', which is the receiver's value.
	 *
	 * @return the selection
	 */
	int GetSelection() {
		return w_slider_get_selection(W_SLIDER(this));
	}
	/**
	 * Returns the receiver's thumb value.
	 *
	 * @return the thumb value
	 */
	int GetThumb() {
		return w_slider_get_thumb(W_SLIDER(this));
	}
	/**
	 * Gets the receiver's selection, minimum value, maximum
	 * value, thumb, increment and page increment all at once.
	 * <p>
	 * Note: This is similar to getting the values individually
	 * using the appropriate methods, but may be implemented in a
	 * more efficient fashion on some platforms.
	 * </p>
	 *
	 * @param value the all value of scrollbar
	 *
	 */
	bool GetValues(WSliderValue &value) {
		return w_slider_get_values(W_SLIDER(this), (w_slider_value*) &value) > 0;
	}
	static bool IsSlider(WWidget *widget) {
		return widget->GetClassId() == _W_CLASS_SLIDER;
	}
	/**
	 * Sets the amount that the receiver's value will be
	 * modified by when the up/down (or right/left) arrows
	 * are pressed to the argument, which must be at least
	 * one.
	 *
	 * @param value the new increment (must be greater than zero)
	 */
	bool SetIncrement(int value) {
		return w_slider_set_increment(W_SLIDER(this), value) > 0;
	}
	/**
	 * Sets the maximum. If this value is negative or less than or
	 * equal to the minimum, the value is ignored. If necessary, first
	 * the thumb and then the selection are adjusted to fit within the
	 * new range.
	 *
	 * @param value the new maximum, which must be greater than the current minimum
	 */
	bool SetMaximum(int value) {
		return w_slider_set_maximum(W_SLIDER(this), value) > 0;
	}
	/**
	 * Sets the minimum value. If this value is negative or greater
	 * than or equal to the maximum, the value is ignored. If necessary,
	 * first the thumb and then the selection are adjusted to fit within
	 * the new range.
	 *
	 * @param value the new minimum
	 */
	bool SetMinimum(int value) {
		return w_slider_set_minimum(W_SLIDER(this), value) > 0;
	}
	/**
	 * Sets the amount that the receiver's value will be
	 * modified by when the page increment/decrement areas
	 * are selected to the argument, which must be at least
	 * one.
	 *
	 * @param value the page increment (must be greater than zero)
	 */
	bool SetPageIncrement(int value) {
		return w_slider_set_page_increment(W_SLIDER(this), value) > 0;
	}
	/**
	 * Sets the 'selection', which is the receiver's
	 * value, to the argument which must be greater than or equal
	 * to zero.
	 *
	 * @param value the new selection (must be zero or greater)
	 */
	bool SetSelection(int value) {
		return w_slider_set_selection(W_SLIDER(this), value) > 0;
	}
	/**
	 * Sets the thumb value. The thumb value should be used to represent
	 * the size of the visual portion of the current range. This value is
	 * usually the same as the page increment value.
	 * <p>
	 * This new value will be ignored if it is less than one, and will be
	 * clamped if it exceeds the receiver's current range.
	 * </p>
	 *
	 * @param value the new thumb value, which must be at least one and not
	 * larger than the size of the current range
	 */
	bool SetThumb(int value) {
		return w_slider_set_thumb(W_SLIDER(this), value) > 0;
	}
	/**
	 * Sets the receiver's selection, minimum value, maximum
	 * value, thumb, increment and page increment all at once.
	 * <p>
	 * Note: This is similar to setting the values individually
	 * using the appropriate methods, but may be implemented in a
	 * more efficient fashion on some platforms.
	 * </p>
	 *
	 * @param value the all value
	 */
	bool SetValues(WSliderValue &value) {
		return w_slider_set_values(W_SLIDER(this), (w_slider_value*) &value) > 0;
	}
	/**
	 * Sets the receiver's selection, minimum value, maximum
	 * value, thumb, increment and page increment all at once.
	 * <p>
	 * Note: This is similar to setting the values individually
	 * using the appropriate methods, but may be implemented in a
	 * more efficient fashion on some platforms.
	 * </p>
	 *
	 * @param selection the new selection value
	 * @param minimum the new minimum value
	 * @param maximum the new maximum value
	 * @param thumb the new thumb value
	 * @param increment the new increment value
	 * @param pageIncrement the new pageIncrement value
	 */
	bool SetValues(int selection, int minimum, int maximum, int thumb,
			int increment, int pageIncrement) {
		WSliderValue value;
		value.selection = selection;
		value.minimum = minimum;
		value.maximum = maximum;
		value.thumb = thumb;
		value.increment = increment;
		value.pageIncrement = pageIncrement;
		return SetValues(value);
	}
protected:
	w_class_id _GetClassID();
	bool PostEvent(WEvent *e);
	virtual bool OnSelection(WSelectionEvent &e);
private:
	void *handles[(sizeof(w_slider) - sizeof(w_control)) / sizeof(void*)];
};

#endif /* SWTP_CONTROLS_SLIDER_H_ */
