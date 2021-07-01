/*
 * GridLayout.h
 *
 *  Created on: 8 mai 2020
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_LAYOUTS_GRIDLAYOUT_H_
#define SWTP_LAYOUTS_GRIDLAYOUT_H_
#include "Layout.h"
/**
 * <code>WGridData</code> is the layout data object associated with
 * <code>WGridLayout</code>. To set a <code>WGridData</code> object into a
 * control, you use the <code>WControl.SetLayoutData(WGridData)</code> method.
 * <p>
 * There are two ways to create a <code>WGridData</code> object with certain
 * fields set. The first is to set the fields directly, like this:
 * <pre>
 * 		WGridData gridData;
 * 		gridData.horizontalAlignment = WGridData::FILL;
 * 		gridData.grabExcessHorizontalSpace = true;
 * 		button1.SetLayoutData(gridData);
 *
 * 		WGridData gridData();
 * 		gridData.horizontalAlignment = WGridData::FILL;
 * 		gridData.verticalAlignment = WGridData::FILL;
 * 		gridData.grabExcessHorizontalSpace = true;
 * 		gridData.grabExcessVerticalSpace = true;
 * 		gridData.horizontalSpan = 2;
 * 		button2.SetLayoutData(gridData);
 * </pre>
 * The second is to take advantage of <code>WGridData</code> convenience constructors, for example:
 * <pre>
 *      button1.SetLayoutData(WGridData (WGridData::FILL, WGridData::CENTER, true, false));
 *      button2.SetLayoutData(WGridData (WGridData::FILL, WGridData::FILL, true, true, 2, 1));
 * </pre>
 * </p>
 * <p>
 * NOTE: Do not reuse <code>WGridData</code> objects. Every control in a
 * <code>Composite</code> that is managed by a <code>WGridLayout</code>
 * must have a unique <code>WGridData</code> object. If the layout data
 * for a control in a <code>WGridLayout</code> is null at layout time,
 * a unique <code>WGridData</code> object is created for it.
 * </p>
 *
 * @see WGridLayout
 * @see WControl#setLayoutData
 */
class SWTP_PUBLIC WGridData: public WLayoutData {
public:
	enum {
		/**
		 * Value for horizontalAlignment or verticalAlignment.
		 * Position the control at the top or left of the cell.
		 * Not recommended. Use WGridData::BEGINNING, WGridData::TOP or WGridData::LEFT instead.
		 */
		BEGINNING = W_GRID_BEGINNING,
		/**
		 * Value for horizontalAlignment or verticalAlignment.
		 * Position the control in the vertical or horizontal center of the cell
		 * Not recommended. Use WGridData::CENTER instead.
		 */
		CENTER = W_GRID_CENTER,
		/**
		 * Value for horizontalAlignment or verticalAlignment.
		 * Position the control at the bottom or right of the cell
		 * Not recommended. Use WGridData::END, WGridData::BOTTOM or WGridData::RIGHT instead.
		 */
		END = W_GRID_END,
		/**
		 * Value for horizontalAlignment or verticalAlignment.
		 * Resize the control to fill the cell horizontally or vertically.
		 * Not recommended. Use WGridData::FILL instead.
		 */
		FILL = W_GRID_FILL,
		VBEGINNING = W_GRID_VBEGINNING,
		VCENTER = W_GRID_VCENTER,
		VEND = W_GRID_VEND,
		VFILL = W_GRID_VFILL,
		HBEGINNING = W_GRID_HBEGINNING,
		HCENTER = W_GRID_HCENTER,
		HEND = W_GRID_HEND,
		HFILL = W_GRID_HFILL,
		EXCLUDE = W_GRID_EXCLUDE,
		/**
		 * Style bit for <code>WGridData(int)</code>.
		 * Position the control at the top of the cell.
		 * Not recommended. Use
		 * <code>WGridData(int, WGridData::BEGINNING, bool, bool)</code>
		 * instead.
		 */
		VERTICAL_ALIGN_BEGINNING = W_GRID_VERTICAL_ALIGN_BEGINNING,
		/**
		 * Style bit for <code>WGridData(int)</code> to position the
		 * control in the vertical center of the cell.
		 * Not recommended. Use
		 * <code>WGridData(int, WGridData::CENTER, bool, bool)</code>
		 * instead.
		 */
		VERTICAL_ALIGN_CENTER = W_GRID_VERTICAL_ALIGN_CENTER,
		/**
		 * Style bit for <code>WGridData(int)</code> to position the
		 * control at the bottom of the cell.
		 * Not recommended. Use
		 * <code>WGridData(int, WGridData::END, bool, bool)</code>
		 * instead.
		 */
		VERTICAL_ALIGN_END = W_GRID_VERTICAL_ALIGN_END,
		/**
		 * Style bit for <code>WGridData(int)</code> to resize the
		 * control to fill the cell vertically.
		 * Not recommended. Use
		 * <code>WGridData(int, WGridData::FILL, bool, bool)</code>
		 * instead
		 */
		VERTICAL_ALIGN_FILL = W_GRID_VERTICAL_ALIGN_FILL,
		/**
		 * Style bit for <code>WGridData(int)</code> to position the
		 * control at the left of the cell.
		 * Not recommended. Use
		 * <code>WGridData(WGridData::BEGINNING, int, bool, bool)</code>
		 * instead.
		 */
		HORIZONTAL_ALIGN_BEGINNING = W_GRID_HORIZONTAL_ALIGN_BEGINNING,
		/**
		 * Style bit for <code>WGridData(int)</code> to position the
		 * control in the horizontal center of the cell.
		 * Not recommended. Use
		 * <code>WGridData(WGridData::CENTER, int, bool, bool)</code>
		 * instead.
		 */
		HORIZONTAL_ALIGN_CENTER = W_GRID_HORIZONTAL_ALIGN_CENTER,
		/**
		 * Style bit for <code>WGridData(int)</code> to position the
		 * control at the right of the cell.
		 * Not recommended. Use
		 * <code>WGridData(WGridData::END, int, bool, bool)</code>
		 * instead.
		 */
		HORIZONTAL_ALIGN_END = W_GRID_HORIZONTAL_ALIGN_END,
		/**
		 * Style bit for <code>WGridData(int)</code> to resize the
		 * control to fill the cell horizontally.
		 * Not recommended. Use
		 * <code>WGridData(WGridData::FILL, int, bool, bool)</code>
		 * instead.
		 */
		HORIZONTAL_ALIGN_FILL = W_GRID_HORIZONTAL_ALIGN_FILL,
		/**
		 * Style bit for <code>WGridData(int)</code> to resize the
		 * control to fit the remaining horizontal space.
		 * Not recommended. Use
		 * <code>WGridData(int, int, true, bool)</code>
		 * instead.
		 */
		GRAB_HORIZONTAL = W_GRID_GRAB_HORIZONTAL,
		/**
		 * Style bit for <code>WGridData(int)</code> to resize the
		 * control to fit the remaining vertical space.
		 * Not recommended. Use
		 * <code>WGridData(int, int, bool, true)</code>
		 * instead.
		 */
		GRAB_VERTICAL = W_GRID_GRAB_VERTICAL,
		/**
		 * Style bit for <code>WGridData(int)</code> to resize the
		 * control to fill the cell vertically and to fit the remaining
		 * vertical space.
		 * FILL_VERTICAL = VERTICAL_ALIGN_FILL | GRAB_VERTICAL
		 * Not recommended. Use
		 * <code>WGridData(int, WGridData::FILL, bool, true)</code>
		 * instead.
		 */
		FILL_VERTICAL = W_GRID_FILL_VERTICAL,
		/**
		 * Style bit for <code>WGridData(int)</code> to resize the
		 * control to fill the cell horizontally and to fit the remaining
		 * horizontal space.
		 * FILL_HORIZONTAL = HORIZONTAL_ALIGN_FILL | GRAB_HORIZONTAL
		 * Not recommended. Use
		 * <code>WGridData(WGridData::FILL, int, true, bool)</code>
		 * instead.
		 */
		FILL_HORIZONTAL = W_GRID_FILL_HORIZONTAL,
		/**
		 * Style bit for <code>WGridData(int)</code> to resize the
		 * control to fill the cell horizontally and vertically and
		 * to fit the remaining horizontal and vertical space.
		 * FILL_BOTH = FILL_VERTICAL | FILL_HORIZONTAL
		 * Not recommended. Use
		 * <code>WGridData(WGridData::FILL, WGridData::FILL, true, true)</code>
		 * instead.
		 */
		FILL_BOTH = W_GRID_FILL_BOTH,
	};
	void Init(int style) {
		w_layout_grid_data_init((w_layout_grid_data*) this);
		this->flags = style;
	}
	void Init() {
		Init(W_GRID_HORIZONTAL_ALIGN_BEGINNING);
	}
	void Init(int horizontalAlignment, int verticalAlignment,
			bool grabExcessHorizontalSpace, bool grabExcessVerticalSpace,
			int horizontalSpan, int verticalSpan) {
		w_layout_grid_data_init((w_layout_grid_data*) this);
		this->horizontalAlignment = horizontalAlignment;
		this->verticalAlignment = verticalAlignment;
		this->grabExcessHorizontalSpace = grabExcessHorizontalSpace;
		this->grabExcessVerticalSpace = grabExcessVerticalSpace;
		this->horizontalSpan = horizontalSpan;
		this->verticalSpan = verticalSpan;
	}
	void Init(int style, int horizontalSpan, int verticalSpan) {
		w_layout_grid_data_init((w_layout_grid_data*) this);
		this->flags = style;
		this->horizontalSpan = horizontalSpan;
		this->verticalSpan = verticalSpan;
	}
	void Init(int width, int height) {
		w_layout_grid_data_init((w_layout_grid_data*) this);
		this->widthHint = width;
		this->heightHint = height;
	}
	/**
	 * Constructs a new instance of WGridData using
	 * default values.
	 */
	WGridData() {
		Init();
	}
	/**
	 * Constructs a new instance based on the WGridData style.
	 * This constructor is not recommended.
	 *
	 * @param style the WGridData style
	 */
	WGridData(wushort style) {
		Init(style);
	}

	/**
	 * Constructs a new instance of GridData according to the parameters.
	 *
	 * @param horizontalAlignment how control will be positioned horizontally within a cell,
	 * 		one of: WGridData::BEGINNING (or LEFT), WGridData::CENTER, WGridData::END (or RIGHT), or WGridData::FILL
	 * @param verticalAlignment how control will be positioned vertically within a cell,
	 * 		one of: WGridData::BEGINNING (or TOP), WGridData::CENTER, WGridData::END (or BOTTOM), or WGridData::FILL
	 * @param grabExcessHorizontalSpace whether cell will be made wide enough to fit the remaining horizontal space
	 * @param grabExcessVerticalSpace whether cell will be made high enough to fit the remaining vertical space
	 */
	WGridData(int horizontalAlignment, int verticalAlignment,
			bool grabExcessHorizontalSpace, bool grabExcessVerticalSpace) {
		Init(horizontalAlignment, verticalAlignment, grabExcessHorizontalSpace,
				grabExcessVerticalSpace, 1, 1);
	}

	/**
	 * Constructs a new instance of WGridData according to the parameters.
	 *
	 * @param horizontalAlignment how control will be positioned horizontally within a cell,
	 * 		one of: W_BEGINNING (or W_LEFT), W_CENTER, W_END (or W_RIGHT), or W_FILL
	 * @param verticalAlignment how control will be positioned vertically within a cell,
	 * 		one of: W_BEGINNING (or W_TOP), W_CENTER, W_END (or W_BOTTOM), or W_FILL
	 * @param grabExcessHorizontalSpace whether cell will be made wide enough to fit the remaining horizontal space
	 * @param grabExcessVerticalSpace whether cell will be made high enough to fit the remaining vertical space
	 * @param horizontalSpan the number of column cells that the control will take up
	 * @param verticalSpan the number of row cells that the control will take up
	 */
	WGridData(int horizontalAlignment, int verticalAlignment,
			bool grabExcessHorizontalSpace, bool grabExcessVerticalSpace,
			int horizontalSpan, int verticalSpan) {
		Init(horizontalAlignment, verticalAlignment, grabExcessHorizontalSpace,
				grabExcessVerticalSpace, horizontalSpan, verticalSpan);
	}
	/**
	 * Constructs a new instance based on the WGridData style.
	 * This constructor is not recommended.
	 *
	 * @param style the WGridData style
	 * @param horizontalSpan the number of column cells that the control will take up
	 * @param verticalSpan the number of row cells that the control will take up
	 */
	WGridData(int style, int horizontalSpan, int verticalSpan) {
		Init(style, horizontalSpan, verticalSpan);
	}

	/**
	 * Constructs a new instance of WGridData according to the parameters.
	 * A value of W_DEFAULT indicates that no minimum width or
	 * no minimum height is specified.
	 *
	 * @param width a minimum width for the column
	 * @param height a minimum height for the row
	 *
	 * @since 3.0
	 */
	WGridData(int width, int height) {
		Init(width, height);
	}
public:
	union {
		struct {
			/**
			 * verticalAlignment specifies how controls will be positioned
			 * vertically within a cell.
			 *
			 * The default value is CENTER.
			 *
			 * Possible values are: <ul>
			 *    <li>WGridData::BEGINNING (or W_TOP): Position the control at the top of the cell</li>
			 *    <li>WGridData::CENTER: Position the control in the vertical center of the cell</li>
			 *    <li>WGridData::END (or W_BOTTOM): Position the control at the bottom of the cell</li>
			 *    <li>WGridData::FILL: Resize the control to fill the cell vertically</li>
			 * </ul>
			 */
			unsigned verticalAlignment :2;

			/**
			 * horizontalAlignment specifies how controls will be positioned
			 * horizontally within a cell.
			 *
			 * The default value is BEGINNING.
			 *
			 * Possible values are: <ul>
			 *    <li>WGridData::BEGINNING (or W_LEFT): Position the control at the left of the cell</li>
			 *    <li>WGridData::CENTER: Position the control in the horizontal center of the cell</li>
			 *    <li>WGridData::END (or W_RIGHT): Position the control at the right of the cell</li>
			 *    <li>WGridData::FILL: Resize the control to fill the cell horizontally</li>
			 * </ul>
			 */
			unsigned horizontalAlignment :2;

			/**
			 * <p>grabExcessHorizontalSpace specifies whether the width of the cell
			 * changes depending on the size of the parent Composite.  If
			 * grabExcessHorizontalSpace is <code>true</code>, the following rules
			 * apply to the width of the cell:</p>
			 * <ul>
			 * <li>If extra horizontal space is available in the parent, the cell will
			 * grow to be wider than its preferred width.  The new width
			 * will be "preferred width + delta" where delta is the extra
			 * horizontal space divided by the number of grabbing columns.</li>
			 * <li>If there is not enough horizontal space available in the parent, the
			 * cell will shrink until it reaches its minimum width as specified by
			 * GridData.minimumWidth. The new width will be the maximum of
			 * "minimumWidth" and "preferred width - delta", where delta is
			 * the amount of space missing divided by the number of grabbing columns.</li>
			 * <li>If the parent is packed, the cell will be its preferred width
			 * as specified by GridData.widthHint.</li>
			 * <li>If the control spans multiple columns and there are no other grabbing
			 * controls in any of the spanned columns, the last column in the span will
			 * grab the extra space.  If there is at least one other grabbing control
			 * in the span, the grabbing will be spread over the columns already
			 * marked as grabExcessHorizontalSpace.</li>
			 * </ul>
			 *
			 * <p>The default value is false.</p>
			 *
			 * @see GridData#minimumWidth
			 * @see GridData#widthHint
			 */
			unsigned grabExcessHorizontalSpace :1;

			/**
			 * <p>grabExcessVerticalSpace specifies whether the height of the cell
			 * changes depending on the size of the parent Composite.  If
			 * grabExcessVerticalSpace is <code>true</code>, the following rules
			 * apply to the height of the cell:</p>
			 * <ul>
			 * <li>If extra vertical space is available in the parent, the cell will
			 * grow to be taller than its preferred height.  The new height
			 * will be "preferred height + delta" where delta is the extra
			 * vertical space divided by the number of grabbing rows.</li>
			 * <li>If there is not enough vertical space available in the parent, the
			 * cell will shrink until it reaches its minimum height as specified by
			 * GridData.minimumHeight. The new height will be the maximum of
			 * "minimumHeight" and "preferred height - delta", where delta is
			 * the amount of space missing divided by the number of grabbing rows.</li>
			 * <li>If the parent is packed, the cell will be its preferred height
			 * as specified by GridData.heightHint.</li>
			 * <li>If the control spans multiple rows and there are no other grabbing
			 * controls in any of the spanned rows, the last row in the span will
			 * grab the extra space.  If there is at least one other grabbing control
			 * in the span, the grabbing will be spread over the rows already
			 * marked as grabExcessVerticalSpace.</li>
			 * </ul>
			 *
			 * <p>The default value is false.</p>
			 *
			 * @see GridData#minimumHeight
			 * @see GridData#heightHint
			 */
			unsigned grabExcessVerticalSpace :1;

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

	/**
	 * widthHint specifies the preferred width in pixels. This value
	 * is the wHint passed into Control.computeSize(int, int, bool)
	 * to determine the preferred size of the control.
	 *
	 * The default value is W_DEFAULT.
	 *
	 * @see Control#computeSize(int, int, bool)
	 */
	int widthHint;

	/**
	 * heightHint specifies the preferred height in pixels. This value
	 * is the hHint passed into Control.computeSize(int, int, bool)
	 * to determine the preferred size of the control.
	 *
	 * The default value is W_DEFAULT.
	 *
	 * @see Control#computeSize(int, int, bool)
	 */
	int heightHint;

	/**
	 * horizontalIndent specifies the number of pixels of indentation
	 * that will be placed along the left side of the cell.
	 *
	 * The default value is 0.
	 */
	int horizontalIndent;

	/**
	 * verticalIndent specifies the number of pixels of indentation
	 * that will be placed along the top side of the cell.
	 *
	 * The default value is 0.
	 *
	 */
	int verticalIndent;

	/**
	 * horizontalSpan specifies the number of column cells that the control
	 * will take up.
	 *
	 * The default value is 1.
	 */
	int horizontalSpan;

	/**
	 * verticalSpan specifies the number of row cells that the control
	 * will take up.
	 *
	 * The default value is 1.
	 */
	int verticalSpan;

	/**
	 * minimumWidth specifies the minimum width in pixels.  This value
	 * applies only if grabExcessHorizontalSpace is true. A value of
	 * W_DEFAULT means that the minimum width will be the result
	 * of Control.computeSize(int, int, bool) where wHint is
	 * determined by GridData.widthHint.
	 *
	 * The default value is 0.
	 *
	 * @see Control#computeSize(int, int, bool)
	 * @see GridData#widthHint
	 */
	int minimumWidth;

	/**
	 * minimumHeight specifies the minimum height in pixels.  This value
	 * applies only if grabExcessVerticalSpace is true.  A value of
	 * W_DEFAULT means that the minimum height will be the result
	 * of Control.computeSize(int, int, bool) where hHint is
	 * determined by GridData.heightHint.
	 *
	 * The default value is 0.
	 *
	 * @see Control#computeSize(int, int, bool)
	 * @see GridData#heightHint
	 */
	int minimumHeight;
private:
	/*
	 * caches
	 */
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
};

/**
 * Instances of this class lay out the control children of a
 * <code>WComposite</code> in a grid.
 * <p>
 * <code>WGridLayout</code> has a number of configuration fields, and the
 * controls it lays out can have an associated layout data object, called
 * <code>WGridData</code>. The power of <code>WGridLayout</code> lies in the
 * ability to configure <code>WGridData</code> for each control in the layout.
 * </p>
 * <p>
 * The following code creates a shell managed by a <code>WGridLayout</code>
 * with 3 columns:
 * <pre>
 * class MyShell : public WShell{
 * private:
 * WGridLayout gridLayout;
 * public:
 * MyShell(){
 * 		Create();
 * 		gridLayout.numColumns = 3;
 * 		SetLayout(gridLayout);
 * 	}
 * };
 * </pre>
 * The <code>numColumns</code> field is the most important field in a
 * <code>WGridLayout</code>. Widgets are laid out in columns from left
 * to right, and a new row is created when <code>numColumns</code> + 1
 * controls are added to the <code>WComposite<code>.
 * </p>
 *
 * @see WGridData
 */
class SWTP_PUBLIC WGridLayout: public WLayout {
public:

	/**
	 * numColumns specifies the number of cell columns in the layout.
	 * If numColumns has a value less than 1, the layout will not
	 * set the size and position of any controls.
	 *
	 * The default value is 1.
	 */
	int numColumns;

	/**
	 * makeColumnsEqualWidth specifies whether all columns in the layout
	 * will be forced to have the same width.
	 *
	 * The default value is false.
	 */
	unsigned makeColumnsEqualWidth :1;

	/**
	 * marginWidth specifies the number of pixels of horizontal margin
	 * that will be placed along the left and right edges of the layout.
	 *
	 * The default value is 5.
	 */
	int marginWidth;

	/**
	 * marginHeight specifies the number of pixels of vertical margin
	 * that will be placed along the top and bottom edges of the layout.
	 *
	 * The default value is 5.
	 */
	int marginHeight;

	/**
	 * marginLeft specifies the number of pixels of horizontal margin
	 * that will be placed along the left edge of the layout.
	 *
	 * The default value is 0.
	 */
	int marginLeft;

	/**
	 * marginTop specifies the number of pixels of vertical margin
	 * that will be placed along the top edge of the layout.
	 *
	 * The default value is 0.
	 */
	int marginTop;

	/**
	 * marginRight specifies the number of pixels of horizontal margin
	 * that will be placed along the right edge of the layout.
	 *
	 * The default value is 0.
	 */
	int marginRight;

	/**
	 * marginBottom specifies the number of pixels of vertical margin
	 * that will be placed along the bottom edge of the layout.
	 *
	 * The default value is 0.
	 */
	int marginBottom;

	/**
	 * horizontalSpacing specifies the number of pixels between the right
	 * edge of one cell and the left edge of its neighbouring cell to
	 * the right.
	 *
	 * The default value is 5.
	 */
	int horizontalSpacing;

	/**
	 * verticalSpacing specifies the number of pixels between the bottom
	 * edge of one cell and the top edge of its neighbouring cell underneath.
	 *
	 * The default value is 5.
	 */
	int verticalSpacing;
private:
	int reserved[5];
public:
	void Init() {
		Init(1, false);
	}
	void Init(int numColumns, bool makeColumnsEqualWidth);
	/**
	 * Constructs a new instance of this class
	 * with a single column.
	 */
	WGridLayout();

	/**
	 * Constructs a new instance of this class given the
	 * number of columns, and whether or not the columns
	 * should be forced to have the same width.
	 * If numColumns has a value less than 1, the layout will not
	 * set the size and position of any controls.
	 *
	 * @param numColumns the number of columns in the grid
	 * @param makeColumnsEqualWidth whether or not the columns will have equal width
	 */
	WGridLayout(int numColumns, bool makeColumnsEqualWidth);

	void ComputeSize(WComposite *composite, WSize &result, int wHint, int hHint,
			bool flushCache);
	void DoLayout(WComposite *composite, bool flushCache);
	void SetData(WControl *control, const WLayoutData &data);
	void SetData(WControl &control, const WLayoutData &data) {
		SetData(&control, data);
	}
	void GetData(WControl *control, WLayoutData &data);
	void GetData(WControl &control, WLayoutData &data) {
		GetData(&control, data);
	}
};

#endif /* SWTP_LAYOUTS_GRIDLAYOUT_H_ */
