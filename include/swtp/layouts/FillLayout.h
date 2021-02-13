/*
 * FillLayout.h
 *
 *  Created on: 8 mai 2020
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_LAYOUTS_FILLLAYOUT_H_
#define SWTP_LAYOUTS_FILLLAYOUT_H_
#include "Layout.h"
class SWTP_PUBLIC WFillLayout: public WLayout {
public:
	/**
	 * type specifies how controls will be positioned
	 * within the layout.
	 *
	 * The default value is HORIZONTAL.
	 *
	 * Possible values are: <ul>
	 *    <li>HORIZONTAL: Position the controls horizontally from left to right</li>
	 *    <li>VERTICAL: Position the controls vertically from top to bottom</li>
	 * </ul>
	 */
	int type;

	/**
	 * marginWidth specifies the number of pixels of horizontal margin
	 * that will be placed along the left and right edges of the layout.
	 *
	 * The default value is 0.
	 */
	int marginWidth;

	/**
	 * marginHeight specifies the number of pixels of vertical margin
	 * that will be placed along the top and bottom edges of the layout.
	 *
	 * The default value is 0.
	 */
	int marginHeight;

	/**
	 * spacing specifies the number of pixels between the edge of one cell
	 * and the edge of its neighbouring cell.
	 *
	 * The default value is 0.
	 */
	int spacing;
	void Init(int type);

	/**
	 * Constructs a new instance of this class.
	 */
	WFillLayout();

	/**
	 * Constructs a new instance of this class given the type.
	 *
	 * @param type the type of fill layout
	 */
	WFillLayout(int type);
	void ComputeSize(WComposite *composite, WSize &result, int wHint,
			int hHint, bool flushCache);
	void DoLayout(WComposite *composite, bool flushCache);
};
class SWTP_PUBLIC WFillLayoutHorizontal: public WFillLayout {
public:
	WFillLayoutHorizontal() :
			WFillLayout(W_HORIZONTAL) {
	}
};
class SWTP_PUBLIC WFillLayoutVertical: public WFillLayout {
public:
	WFillLayoutVertical() :
			WFillLayout(W_VERTICAL) {
	}
};
#endif /* SWTP_LAYOUTS_FILLLAYOUT_H_ */
