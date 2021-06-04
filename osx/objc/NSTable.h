/*
 * NSTable.h
 *
 *  Created on: Jan 2, 2021
 *      Author: azeddine
 */

#ifndef OSX_OBJC_NSTABLE_H_
#define OSX_OBJC_NSTABLE_H_
#include "NSView.h"
typedef struct NSTableView NSTableView;
typedef struct NSTableHeaderView NSTableHeaderView;
typedef struct NSTableColumn NSTableColumn;
void NSTableView_setHeaderView(NSTableView* tableView,NSTableHeaderView *headerView);
void NSTableView_addTableColumn(NSTableView* tableView,NSTableColumn* column);
NSUInteger NSTableView_selectedRow(NSTableView* tableView);
/*

 */
NSTableHeaderView* NSTableHeaderView_new(w_widget* widget);

NSTableColumn* SWTTableColumn_new(NSString* identifier);
void NSTableColumn_setMinWidth(NSTableColumn* tablecolumn,int minWidth);
void NSTableColumn_setWidth(NSTableColumn* tablecolumn,int width);
void NSTableColumn_setResizingMask(NSTableColumn* tablecolumn,long ops);
void NSTableColumn_setDataCell(NSTableColumn* tablecolumn,NSCell* cell);
NSCell* NSTableColumn_headerCell(NSTableColumn* tablecolumn);
NSCell* SWTImageTextCell_new(NSString* identifier);


#endif /* OSX_OBJC_NSTABLE_H_ */
