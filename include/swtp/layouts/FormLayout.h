/*
 * FormLayout.h
 *
 *  Created on: 28 sept. 2020
 *      Author: azeddine
 */

#ifndef SWTP_LAYOUTS_FORMLAYOUT_H_
#define SWTP_LAYOUTS_FORMLAYOUT_H_
#include "Layout.h"
/**
 * Instances of this class are used to define the edges of a control
 * within a <code>FormLayout</code>.
 * <p>
 * <code>FormAttachments</code> are set into the top, bottom, left,
 * and right fields of the <code>FormData</code> for a control.
 * For example:
 * </p>
 * <pre>
 * 		WFormData data;
 * 		data.top.Set(0,5);
 * 		data.bottom.Set(100,-5);
 * 		data.left.Set(0,5);
 * 		data.right.Set(100,-5);
 * 		button.SetLayoutData(data);
 * </pre>
 * <p>
 * A <code>FormAttachment</code> defines where to attach the side of
 * a control by using the equation, y = ax + b. The "a" term represents
 * a fraction of the parent composite's width (from the left) or height
 * (from the top). It can be defined using a numerator and denominator,
 * or just a percentage value. If a percentage is used, the denominator
 * is set to 100. The "b" term in the equation represents an offset, in
 * points, from the attachment position. For example:</p>
 * <pre>
 * 		FormAttachment attach = new FormAttachment (20, -5);
 * </pre>
 * <p>
 * specifies that the side to which the <code>FormAttachment</code>
 * object belongs will lie at 20% of the parent composite, minus 5 points.
 * </p>
 * <p>
 * Control sides can also be attached to another control.
 * For example:</p>
 * <pre>
 * 		FormAttachment attach = new FormAttachment (button, 10);
 * </pre>
 * specifies that the side to which the <code>FormAttachment</code>
 * object belongs will lie in the same position as the adjacent side of
 * the <code>button</code> control, plus 10 points. The control side can
 * also be attached to the opposite side of the specified control.
 * For example:
 * <pre>
 * 		FormData data = new FormData ();
 * 		data.left = new FormAttachment (button, 0, SWT.LEFT);
 * </pre>
 * specifies that the left side of the control will lie in the same position
 * as the left side of the <code>button</code> control. The control can also
 * be attached in a position that will center the control on the specified
 * control. For example:
 * <pre>
 * 		data.left = new FormAttachment (button, 0, SWT.CENTER);
 * </pre>
 * specifies that the left side of the control will be positioned so that it is
 * centered between the left and right sides of the <code>button</code> control.
 * If the alignment is not specified, the default is to attach to the adjacent side.
 *
 * @see FormLayout
 * @see FormData
 * @see <a href="http://www.eclipse.org/swt/">Sample code and further information</a>
 *
 * @since 2.0
 */
class SWTP_PUBLIC WFormAttachment {
public:
	/**
	 * control specifies the control to which the control side is
	 * attached.
	 */
	WControl *control;
	/**
	 * numerator specifies the numerator of the "a" term in the
	 * equation, y = ax + b, which defines the attachment.
	 */
	int numerator;

	/**
	 * denominator specifies the denominator of the "a" term in the
	 * equation, y = ax + b, which defines the attachment.
	 *
	 * The default value is 100.
	 */
	int denominator;

	/**
	 * offset specifies the offset, in points, of the control side
	 * from the attachment position.
	 * If the offset is positive, then the control side is offset
	 * to the right of or below the attachment position. If it is
	 * negative, then the control side is offset to the left of or
	 * above the attachment position.
	 *
	 * This is equivalent to the "b" term in the equation y = ax + b.
	 * The default value is 0.
	 */
	int offset;

	/**
	 * alignment specifies the alignment of the control side that is
	 * attached to a control.
	 * <p>
	 * For top and bottom attachments, TOP, BOTTOM and CENTER are used. For left
	 * and right attachments, LEFT, RIGHT and CENTER are used. If any other case
	 * occurs, the default will be used instead.
	 * </p>
	 *
	 * <br>Possible values are: <ul>
	 *    <li>{@link SWT#TOP}: Attach the side to the top side of the specified control.</li>
	 *    <li>{@link SWT#BOTTOM}: Attach the side to the bottom side of the specified control.</li>
	 *    <li>{@link SWT#LEFT}: Attach the side to the left side of the specified control.</li>
	 *    <li>{@link SWT#RIGHT}: Attach the side to the right side of the specified control.</li>
	 *    <li>{@link SWT#CENTER}: Attach the side at a position which will center the control on the specified control.</li>
	 *    <li>{@link SWT#DEFAULT}: Attach the side to the adjacent side of the specified control.</li>
	 * </ul>
	 */
	int alignment;
public:
	void Init0() {
	}
	void Init() {
		this->control = 0;
		this->offset = 0;
		this->alignment = 0;
		this->denominator = 100;
		this->numerator = 0;
	}
	void Init(WControl *control, int offset, int alignment) {
		this->control = control;
		this->offset = offset;
		this->alignment = alignment;
		this->denominator = 100;
		this->numerator = 0;
	}
	void Init(int numerator, int denominator, int offset) {
		if (denominator == 0)
			denominator = 1;
		this->numerator = numerator;
		this->denominator = denominator;
		this->offset = offset;
		this->control = 0;
		this->alignment = 0;
	}
public:
	WFormAttachment() {
		Init0();
	}
	/**
	 * Constructs a new instance of this class given a numerator
	 * Since no denominator or offset is specified, the default
	 * is to treat the numerator as a percentage of the form, with a
	 * denominator of 100. The offset is zero.
	 *
	 * @param numerator the percentage of the position
	 *
	 * @since 3.0
	 */
	WFormAttachment(int numerator) {
		Init(numerator, 100, 0);
	}

	void Set(int numerator) {
		Init(numerator, 100, 0);
	}
	/**
	 * Constructs a new instance of this class given a numerator
	 * and an offset. Since no denominator is specified, the default
	 * is to treat the numerator as a percentage of the form, with a
	 * denominator of 100.
	 *
	 * @param numerator the percentage of the position
	 * @param offset the offset of the side from the position
	 */
	WFormAttachment(int numerator, int offset) {
		Init(numerator, 100, offset);
	}
	void Set(int numerator, int offset) {
		Init(numerator, 100, offset);
	}

	/**
	 * Constructs a new instance of this class given a numerator
	 * and denominator and an offset. The position of the side is
	 * given by the fraction of the form defined by the numerator
	 * and denominator.
	 *
	 * @param numerator the numerator of the position
	 * @param denominator the denominator of the position
	 * @param offset the offset of the side from the position
	 */
	WFormAttachment(int numerator, int denominator, int offset) {
		Init(numerator, denominator, offset);
	}
	void Set(int numerator, int denominator, int offset) {
		Init(numerator, denominator, offset);
	}

	/**
	 * Constructs a new instance of this class given a control.
	 * Since no alignment is specified, the default alignment is
	 * to attach the side to the adjacent side of the specified
	 * control. Since no offset is specified, an offset of 0 is
	 * used.
	 *
	 * @param control the control the side is attached to
	 */
	WFormAttachment(WControl *control) {
		Init(control, 0, W_DEFAULT);
	}
	void Set(WControl *control) {
		Init(control, 0, W_DEFAULT);
	}

	/**
	 * Constructs a new instance of this class given a control
	 * and an offset. Since no alignment is specified, the default
	 * alignment is to attach the side to the adjacent side of the
	 * specified control.
	 *
	 * @param control the control the side is attached to
	 * @param offset the offset of the side from the control
	 */
	WFormAttachment(WControl *control, int offset) {
		Init(control, offset, W_DEFAULT);
	}
	void Set(WControl *control, int offset) {
		Init(control, offset, W_DEFAULT);
	}

	/**
	 * Constructs a new instance of this class given a control,
	 * an offset and an alignment.  The possible alignment values are:
	 * <dl>
	 * <dt><b>{@link SWT#TOP}</b></dt>
	 * <dd>the side will be attached to the top side of the specified control</dd>
	 * <dt><b>{@link SWT#BOTTOM}</b></dt>
	 * <dd>the side will be attached to the bottom side of the specified control</dd>
	 * <dt><b>{@link SWT#LEFT}</b></dt>
	 * <dd>the side will be attached to the left side of the specified control</dd>
	 * <dt><b>{@link SWT#RIGHT}</b></dt>
	 * <dd>the side will be attached to the right side of the specified control</dd>
	 * <dt><b>{@link SWT#CENTER}</b></dt>
	 * <dd>the side will be centered on the same side of the specified control</dd>
	 * <dt><b>{@link SWT#DEFAULT}</b></dt>
	 * <dd>the side will be attached to the adjacent side of the specified control</dd>
	 * </dl>
	 *
	 * @param control the control the side is attached to
	 * @param offset the offset of the side from the control
	 * @param alignment the alignment of the side to the control it is attached to,
	 * 		one of TOP, BOTTOM, LEFT, RIGHT, CENTER, or DEFAULT
	 */
	WFormAttachment(WControl *control, int offset, int alignment) {
		Init(control, offset, alignment);
	}
	void Set(WControl *control, int offset, int alignment) {
		Init(control, offset, alignment);
	}
};
/**
 * Instances of this class are used to define the attachments
 * of a control in a <code>FormLayout</code>.
 * <p>
 * To set a <code>FormData</code> object into a control, you use the
 * <code>setLayoutData ()</code> method. To define attachments for the
 * <code>FormData</code>, set the fields directly, like this:
 * </p>
 * <pre>
 * 		FormData data = new FormData();
 * 		data.left = new FormAttachment(0,5);
 * 		data.right = new FormAttachment(100,-5);
 * 		button.setLayoutData(formData);
 * </pre>
 * <p>
 * <code>FormData</code> contains the <code>FormAttachments</code> for
 * each edge of the control that the <code>FormLayout</code> uses to
 * determine the size and position of the control. <code>FormData</code>
 * objects also allow you to set the width and height of controls within
 * a <code>FormLayout</code>.
 * </p>
 *
 * @see FormLayout
 * @see FormAttachment
 * @see <a href="http://www.eclipse.org/swt/">Sample code and further information</a>
 *
 * @since 2.0
 */
class SWTP_PUBLIC WFormData {
private:
	wushort size;
	union {
		struct {
			unsigned isVisited :1;
			unsigned needed :1;
		};
		wushort flags;
	};
public:
	/**
	 * width specifies the preferred width in points. This value
	 * is the wHint passed into Control.computeSize(int, int, boolean)
	 * to determine the preferred size of the control.
	 *
	 * The default value is SWT.DEFAULT.
	 *
	 * @see Control#computeSize(int, int, boolean)
	 */
	int width;
	/**
	 * height specifies the preferred height in points. This value
	 * is the hHint passed into Control.computeSize(int, int, boolean)
	 * to determine the preferred size of the control.
	 *
	 * The default value is SWT.DEFAULT.
	 *
	 * @see Control#computeSize(int, int, boolean)
	 */
	int height;
	/**
	 * left specifies the attachment of the left side of
	 * the control.
	 */
	WFormAttachment left;
	/**
	 * right specifies the attachment of the right side of
	 * the control.
	 */
	WFormAttachment right;
	/**
	 * top specifies the attachment of the top of the control.
	 */
	WFormAttachment top;
	/**
	 * bottom specifies the attachment of the bottom of the
	 * control.
	 */
	WFormAttachment bottom;
private:
	int cacheWidth;
	int cacheHeight;
	int defaultWhint;
	int defaultHhint;
	int defaultWidth;
	int defaultHeight;
	int currentWhint;
	int currentHhint;
	int currentWidth;
	int currentHeight;
	WFormAttachment cacheLeft;
	WFormAttachment cacheRight;
	WFormAttachment cacheTop;
	WFormAttachment cacheBottom;
public:
	void Init() {
		w_layout_form_data_init((w_layout_form_data*) this);
	}
	void Init(int width, int height) {
		Init();
		this->width = width;
		this->height = height;
	}
	/**
	 * Constructs a new instance of FormData using
	 * default values.
	 */
	WFormData() {
		Init();
	}

	/**
	 * Constructs a new instance of FormData according to the parameters.
	 * A value of SWT.DEFAULT indicates that no minimum width or
	 * no minimum height is specified.
	 *
	 * @param width a minimum width for the control
	 * @param height a minimum height for the control
	 */
	WFormData(int width, int height) {
		Init(width, height);
	}
};
/**
 * Instances of this class control the position and size of the
 * children of a composite control by using <code>FormAttachments</code>
 * to optionally configure the left, top, right and bottom edges of
 * each child.
 * <p>
 * The following example code creates a <code>FormLayout</code> and then sets
 * it into a <code>Shell</code>:
 * </p>
 * <pre>
 * 		Display display = new Display ();
 *		Shell shell = new Shell(display);
 *		FormLayout layout = new FormLayout();
 *		layout.marginWidth = 3;
 *		layout.marginHeight = 3;
 *		shell.setLayout(layout);
 * </pre>
 * <p>
 * To use a <code>FormLayout</code>, create a <code>FormData</code> with
 * <code>FormAttachment</code> for each child of <code>Composite</code>.
 * The following example code attaches <code>button1</code> to the top
 * and left edge of the composite and <code>button2</code> to the right
 * edge of <code>button1</code> and the top and right edges of the
 * composite:
 * </p>
 * <pre>
 *		FormData data1 = new FormData();
 *		data1.left = new FormAttachment(0, 0);
 *		data1.top = new FormAttachment(0, 0);
 *		button1.setLayoutData(data1);
 *		FormData data2 = new FormData();
 *		data2.left = new FormAttachment(button1);
 *		data2.top = new FormAttachment(0, 0);
 *		data2.right = new FormAttachment(100, 0);
 *		button2.setLayoutData(data2);
 * </pre>
 * <p>
 * Each side of a child control can be attached to a position in the parent
 * composite, or to other controls within the <code>Composite</code> by
 * creating instances of <code>FormAttachment</code> and setting them into
 * the top, bottom, left, and right fields of the child's <code>FormData</code>.
 * </p>
 * <p>
 * If a side is not given an attachment, it is defined as not being attached
 * to anything, causing the child to remain at its preferred size.  If a child
 * is given no attachment on either the left or the right or top or bottom, it is
 * automatically attached to the left and top of the composite respectively.
 * The following code positions <code>button1</code> and <code>button2</code>
 * but relies on default attachments:
 * </p>
 * <pre>
 *		FormData data2 = new FormData();
 *		data2.left = new FormAttachment(button1);
 *		data2.right = new FormAttachment(100, 0);
 *		button2.setLayoutData(data2);
 * </pre>
 * <p>
 * IMPORTANT: Do not define circular attachments.  For example, do not attach
 * the right edge of <code>button1</code> to the left edge of <code>button2</code>
 * and then attach the left edge of <code>button2</code> to the right edge of
 * <code>button1</code>.  This will over constrain the layout, causing undefined
 * behavior.  The algorithm will terminate, but the results are undefined.
 * </p>
 *
 * @see FormData
 * @see FormAttachment
 * @see <a href="http://www.eclipse.org/swt/snippets/#formlayout">FormLayout snippets</a>
 * @see <a href="http://www.eclipse.org/swt/examples.php">SWT Example: LayoutExample</a>
 * @see <a href="http://www.eclipse.org/swt/">Sample code and further information</a>
 *
 * @since 2.0
 */
class SWTP_PUBLIC WFormLayout: public WLayout {
public:
	/**
	 * marginWidth specifies the number of points of horizontal margin
	 * that will be placed along the left and right edges of the layout.
	 *
	 * The default value is 0.
	 */
	int marginWidth;

	/**
	 * marginHeight specifies the number of points of vertical margin
	 * that will be placed along the top and bottom edges of the layout.
	 *
	 * The default value is 0.
	 */
	int marginHeight;

	/**
	 * marginLeft specifies the number of points of horizontal margin
	 * that will be placed along the left edge of the layout.
	 *
	 * The default value is 0.
	 *
	 * @since 3.1
	 */
	int marginLeft;

	/**
	 * marginTop specifies the number of points of vertical margin
	 * that will be placed along the top edge of the layout.
	 *
	 * The default value is 0.
	 *
	 * @since 3.1
	 */
	int marginTop;

	/**
	 * marginRight specifies the number of points of horizontal margin
	 * that will be placed along the right edge of the layout.
	 *
	 * The default value is 0.
	 *
	 * @since 3.1
	 */
	int marginRight;

	/**
	 * marginBottom specifies the number of points of vertical margin
	 * that will be placed along the bottom edge of the layout.
	 *
	 * The default value is 0.
	 *
	 * @since 3.1
	 */
	int marginBottom;

	/**
	 * spacing specifies the number of points between the edge of one control
	 * and the edge of its neighbouring control.
	 *
	 * The default value is 0.
	 *
	 * @since 3.0
	 */
	int spacing;
private:
	int reserved[5];
public:
	void Init() {
		w_layout_form_init((w_layout_form*) this);
	}
	/**
	 * Constructs a new instance of this class.
	 */
	WFormLayout();

	void ComputeSize(WComposite *composite, WSize &result, int wHint, int hHint,
			bool flushCache);
	void DoLayout(WComposite *composite, bool flushCache);
	void SetData(WControl *control, const WLayoutData &data);
	void GetData(WControl *control, WLayoutData &data);
};

#endif /* SWTP_LAYOUTS_FORMLAYOUT_H_ */
