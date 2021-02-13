/*
 * RowLayout.h
 *
 *  Created on: 28 sept. 2020
 *      Author: azeddine
 */

#ifndef SWTP_LAYOUTS_ROWLAYOUT_H_
#define SWTP_LAYOUTS_ROWLAYOUT_H_
#include "Layout.h"

/**
 * Each control controlled by a <code>RowLayout</code> can have its initial
 * width and height specified by setting a <code>RowData</code> object
 * into the control.
 * <p>
 * The following code uses a <code>RowData</code> object to change the initial
 * size of a <code>Button</code> in a <code>Shell</code>:
 * </p>
 * <pre>
 * 		Display display = new Display();
 * 		Shell shell = new Shell(display);
 * 		shell.setLayout(new RowLayout());
 * 		Button button1 = new Button(shell, SWT.PUSH);
 * 		button1.setText("Button 1");
 * 		button1.setLayoutData(new RowData(50, 40));
 * </pre>
 *
 * @see RowLayout
 * @see <a href="http://www.eclipse.org/swt/">Sample code and further information</a>
 */
class SWTP_PUBLIC WRowData {
private:
	wushort size;
	union {
		struct {

			/**
			 * exclude informs the layout to ignore this control when sizing
			 * and positioning controls.  If this value is <code>true</code>,
			 * the size and position of the control will not be managed by the
			 * layout.  If this	value is <code>false</code>, the size and
			 * position of the control will be computed and assigned.
			 *
			 * The default value is <code>false</code>.
			 */
			unsigned exclude :1;
		};
		wushort flags;
	};
public:
	/**
	 * width specifies the desired width in points. This value
	 * is the wHint passed into Control.computeSize(int, int, boolean)
	 * to determine the preferred size of the control.
	 *
	 * The default value is SWT.DEFAULT.
	 *
	 * @see org.eclipse.swt.widgets.Control#computeSize(int, int, boolean)
	 */
	int width;
	/**
	 * height specifies the preferred height in points. This value
	 * is the hHint passed into Control.computeSize(int, int, boolean)
	 * to determine the preferred size of the control.
	 *
	 * The default value is SWT.DEFAULT.
	 *
	 * @see org.eclipse.swt.widgets.Control#computeSize(int, int, boolean)
	 */
	int height;
public:
	void Init() {
		w_layout_row_data_init((w_layout_row_data*) this);
	}
	void Init(int width, int height) {
		Init();
		this->width = width;
		this->height = height;
	}

	/**
	 * Constructs a new instance of RowData using
	 * default values.
	 */
	WRowData() {
		Init();
	}

	/**
	 * Constructs a new instance of RowData according to the parameters.
	 * A value of SWT.DEFAULT indicates that no minimum width or
	 * no minimum height is specified.
	 *
	 * @param width a minimum width for the control
	 * @param height a minimum height for the control
	 */
	WRowData(int width, int height) {
		Init(width, height);
	}

	/**
	 * Constructs a new instance of RowData according to the parameter.
	 * A value of SWT.DEFAULT indicates that no minimum width or
	 * no minimum height is specified.
	 *
	 * @param point a point whose x coordinate specifies a minimum width for the control
	 * and y coordinate specifies a minimum height for the control
	 */
	WRowData(const WSize &size) {
		Init(size.width, size.height);
	}
};
/**
 * Instances of this class determine the size and position of the
 * children of a <code>Composite</code> by placing them either in
 * horizontal rows or vertical columns within the parent <code>Composite</code>.
 * <p>
 * <code>RowLayout</code> aligns all controls in one row if the
 * <code>type</code> is set to horizontal, and one column if it is
 * set to vertical. It has the ability to wrap, and provides configurable
 * margins and spacing. <code>RowLayout</code> has a number of configuration
 * fields. In addition, the height and width of each control in a
 * <code>RowLayout</code> can be specified by setting a <code>RowData</code>
 * object into the control using <code>setLayoutData ()</code>.
 * </p>
 * <p>
 * The following example code creates a <code>RowLayout</code>, sets all
 * of its fields to non-default values, and then sets it into a
 * <code>Shell</code>.</p>
 * <pre>
 * 		RowLayout rowLayout = new RowLayout();
 * 		rowLayout.wrap = false;
 * 		rowLayout.pack = false;
 * 		rowLayout.justify = true;
 * 		rowLayout.type = SWT.VERTICAL;
 * 		rowLayout.marginLeft = 5;
 * 		rowLayout.marginTop = 5;
 * 		rowLayout.marginRight = 5;
 * 		rowLayout.marginBottom = 5;
 * 		rowLayout.spacing = 0;
 * 		shell.setLayout(rowLayout);
 * </pre>
 * If you are using the default field values, you only need one line of code:
 * <pre>
 * 		shell.setLayout(new RowLayout());
 * </pre>
 *
 * @see RowData
 * @see <a href="http://www.eclipse.org/swt/snippets/#rowlayout">RowLayout snippets</a>
 * @see <a href="http://www.eclipse.org/swt/examples.php">SWT Example: LayoutExample</a>
 * @see <a href="http://www.eclipse.org/swt/">Sample code and further information</a>
 */
class SWTP_PUBLIC WRowLayout: public WLayout {

	/**
	 * type specifies whether the layout places controls in rows or
	 * columns.
	 *
	 * The default value is HORIZONTAL.
	 *
	 * Possible values are: <ul>
	 *    <li>HORIZONTAL: Position the controls horizontally from left to right</li>
	 *    <li>VERTICAL: Position the controls vertically from top to bottom</li>
	 * </ul>
	 *
	 * @since 2.0
	 */
	int type;
	union {
		struct {
			/**
			 * wrap specifies whether a control will be wrapped to the next
			 * row if there is insufficient space on the current row.
			 *
			 * The default value is true.
			 */
			unsigned wrap :1;
			/**
			 * pack specifies whether all controls in the layout take
			 * their preferred size.  If pack is false, all controls will
			 * have the same size which is the size required to accommodate the
			 * largest preferred height and the largest preferred width of all
			 * the controls in the layout.
			 *
			 * The default value is true.
			 */
			unsigned pack :1;
			/**
			 * fill specifies whether the controls in a row should be
			 * all the same height for horizontal layouts, or the same
			 * width for vertical layouts.
			 *
			 * The default value is false.
			 *
			 * @since 3.0
			 */
			unsigned fill :1;
			/**
			 * center specifies whether the controls in a row should be
			 * centered vertically in each cell for horizontal layouts,
			 * or centered horizontally in each cell for vertical layouts.
			 *
			 * The default value is false.
			 *
			 * @since 3.4
			 */
			unsigned center :1;
			/**
			 * justify specifies whether the controls in a row should be
			 * fully justified, with any extra space placed between the controls.
			 *
			 * The default value is false.
			 */
			unsigned justify :1;
		};
		int flags;
	};

	/**
	 * marginWidth specifies the number of points of horizontal margin
	 * that will be placed along the left and right edges of the layout.
	 *
	 * The default value is 0.
	 *
	 * @since 3.0
	 */
	int marginWidth;

	/**
	 * marginHeight specifies the number of points of vertical margin
	 * that will be placed along the top and bottom edges of the layout.
	 *
	 * The default value is 0.
	 *
	 * @since 3.0
	 */
	int marginHeight;

	/**
	 * spacing specifies the number of points between the edge of one cell
	 * and the edge of its neighbouring cell.
	 *
	 * The default value is 3.
	 */
	int spacing;

	/**
	 * marginLeft specifies the number of points of horizontal margin
	 * that will be placed along the left edge of the layout.
	 *
	 * The default value is 3.
	 */
	int marginLeft;

	/**
	 * marginTop specifies the number of points of vertical margin
	 * that will be placed along the top edge of the layout.
	 *
	 * The default value is 3.
	 */
	int marginTop;

	/**
	 * marginRight specifies the number of points of horizontal margin
	 * that will be placed along the right edge of the layout.
	 *
	 * The default value is 3.
	 */
	int marginRight;

	/**
	 * marginBottom specifies the number of points of vertical margin
	 * that will be placed along the bottom edge of the layout.
	 *
	 * The default value is 3.
	 */
	int marginBottom;
public:
	void Init(int type) {
		w_layout_row_init((w_layout_row*) this, type);
	}
	/**
	 * Constructs a new instance of this class with type HORIZONTAL.
	 */
	WRowLayout() {
		Init(W_HORIZONTAL);
	}

	/**
	 * Constructs a new instance of this class given the type.
	 *
	 * @param type the type of row layout
	 *
	 * @since 2.0
	 */
	WRowLayout(int type) {
		Init(type);
	}
	void ComputeSize(WComposite *composite, WSize &result, int wHint, int hHint,
			bool flushCache);
	void DoLayout(WComposite *composite, bool flushCache);
	void SetData(WControl *control, const WLayoutData &data);
	void GetData(WControl *control, WLayoutData &data);
};
#endif /* SWTP_LAYOUTS_ROWLAYOUT_H_ */
