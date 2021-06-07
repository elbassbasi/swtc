/*
 * Sash.h
 *
 *  Created on: 9 ao�t 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_CONTROLS_SASH_H_
#define SWTP_CONTROLS_SASH_H_
#include "../widgets/Control.h"
class WSashEvent: public WEvent {
public:
	int detail;
	WRect bounds;
};

/**
 * Instances of the receiver represent a selectable user interface object
 * that allows the user to drag a rubber banded outline of the sash within
 * the parent control.
 * <dl>
 * <dt><b>Styles:</b></dt>
 * <dd>HORIZONTAL, VERTICAL, SMOOTH</dd>
 * <dt><b>Events:</b></dt>
 * <dd>Selection</dd>
 * </dl>
 * <p>
 * Note: Only one of the styles HORIZONTAL and VERTICAL may be specified.
 * </p><p>
 * IMPORTANT: This class is <em>not</em> intended to be subclassed.
 * </p>
 */
class SWTP_PUBLIC WSash: public WControl {
public:
	WSash() {

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
	 * @see SWT#SMOOTH
	 * @see Widget#checkSubclass
	 * @see Widget#getStyle
	 */
	WSash(WComposite *parent, wuint64 style) {
		Create(parent, style);
	}
	bool DefaultSelection(WSashEvent &e) {
		return w_layout_sash_default_selection(W_WIDGET(this), (w_event*) &e);
	}
	static bool IsSash(WWidget *widget) {
		return widget->GetClassId() == _W_CLASS_SASH;
	}
protected:
	w_class_id _GetClassID();
	bool PostEvent(WEvent *e);
	virtual bool OnSelection(WSashEvent &e);
private:
	void *handles[(sizeof(w_sash) - sizeof(w_control)) / sizeof(void*)];
};
#endif /* SWTP_CONTROLS_SASH_H_ */
