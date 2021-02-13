/*
 * ProgressBar.h
 *
 *  Created on: 9 ao�t 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_CONTROLS_PROGRESSBAR_H_
#define SWTP_CONTROLS_PROGRESSBAR_H_
#include "../widgets/Control.h"
/**
 * Instances of the receiver represent an unselectable
 * user interface object that is used to display progress,
 * typically in the form of a bar.
 * <dl>
 * <dt><b>Styles:</b></dt>
 * <dd>SMOOTH, HORIZONTAL, VERTICAL, INDETERMINATE</dd>
 * <dt><b>Events:</b></dt>
 * <dd>(none)</dd>
 * </dl>
 * <p>
 * Note: Only one of the styles HORIZONTAL and VERTICAL may be specified.
 * </p><p>
 * IMPORTANT: This class is <em>not</em> intended to be subclassed.
 * </p>
 */
class SWTP_PUBLIC WProgressBar: public WControl {
public:
	WProgressBar() {

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
	 * @see SWT#SMOOTH
	 * @see SWT#HORIZONTAL
	 * @see SWT#VERTICAL
	 * @see SWT#INDETERMINATE
	 * @see Widget#checkSubclass
	 * @see Widget#getStyle
	 */
	WProgressBar(WComposite *parent, wuint64 style) {
		Create(parent, style);
	}
	/**
	 * Returns the maximum value which the receiver will allow.
	 *
	 * @return the maximum
	 */
	int GetMaximum() {
		return w_progressbar_get_maximum(W_PROGRESSBAR(this));
	}
	/**
	 * Returns the single 'selection' that is the receiver's position.
	 *
	 * @return the selection
	 */
	int GetSelection() {
		return w_progressbar_get_selection(W_PROGRESSBAR(this));
	}
	/**
	 * Returns the state of the receiver. The value will be one of:
	 * <ul>
	 * 	<li>{@link SWT#NORMAL}</li>
	 * 	<li>{@link SWT#ERROR}</li>
	 * 	<li>{@link SWT#PAUSED}</li>
	 * </ul>
	 *
	 * @return the state
	 */
	int GetState() {
		return w_progressbar_get_state(W_PROGRESSBAR(this));
	}
	/**
	 * Sets the maximum value that the receiver will allow.  This new
	 * value will be ignored if it is not greater than the receiver's current
	 * minimum value.  If the new maximum is applied then the receiver's
	 * selection value will be adjusted if necessary to fall within its new range.
	 *
	 * @param value the new maximum, which must be greater than the current minimum
	 */
	bool SetMaximum(int value) {
		return w_progressbar_set_maximum(W_PROGRESSBAR(this), value) > 0;
	}
	/**
	 * Sets the single 'selection' that is the receiver's
	 * position to the argument which must be greater than or equal
	 * to zero.
	 *
	 * @param value the new selection (must be zero or greater)
	 */
	bool SetSelection(int value) {
		return w_progressbar_set_selection(W_PROGRESSBAR(this), value) > 0;
	}
	/**
	 * Sets the state of the receiver. The state must be one of these values:
	 * <ul>
	 * 	<li>{@link W_NORMAL}</li>
	 * 	<li>{@link W_ERROR}</li>
	 * 	<li>{@link W_PAUSED}</li>
	 * </ul>
	 *
	 * @param state the new state
	 */
	bool SetState(int state) {
		return w_progressbar_set_state(W_PROGRESSBAR(this), state) > 0;
	}
protected:
	w_class_id _GetClassID();
private:
	void *handles[(sizeof(w_progressbar) - sizeof(w_control)) / sizeof(void*)];
};

#endif /* SWTP_CONTROLS_PROGRESSBAR_H_ */
