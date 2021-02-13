/*
 * NSOutlineView.h
 *
 *  Created on: Jan 2, 2021
 *      Author: azeddine
 */

#ifndef OSX_OBJC_NSOUTLINEVIEW_H_
#define OSX_OBJC_NSOUTLINEVIEW_H_
#include "NSTable.h"
typedef struct NSOutlineView NSOutlineView;
typedef struct SWTTreeItem SWTTreeItem;

NSOutlineView* SWTOutlineView_new(void* userdata);
void NSOutlineView_setAllowsMultipleSelection(NSOutlineView* outlineView,int op);
void NSOutlineView_setAutoresizesOutlineColumn(NSOutlineView* outlineView,int op);
void NSOutlineView_setAutosaveExpandedItems(NSOutlineView* outlineView,int op);
void NSOutlineView_setDataSource(NSOutlineView* outlineView,int op);
void NSOutlineView_setDelegate(NSOutlineView* outlineView,int op);
void NSOutlineView_setColumnAutoresizingStyle(NSOutlineView* outlineView,long op);
void NSOutlineView_setIntercellSpacing(NSOutlineView* outlineView,int width,int heigth);
void NSOutlineView_reloadItem(NSOutlineView* outlineView,SWTTreeItem* obj,int reloadChildren);
void NSOutlineView_reloadData(NSOutlineView* outlineView);
void NSOutlineView_setOutlineTableColumn(NSOutlineView* tableView,NSTableColumn* column);
NSObject* NSOutlineView_itemAtRow(NSOutlineView* tableView,NSUInteger index);

/*
 */

SWTTreeItem *SWTTreeItem_new();
SWTTreeItem *SWTTreeItem_create(NSOutlineView* outline,SWTTreeItem* parent,int index);
void SWTTreeItem_setText(SWTTreeItem *item,NSString* text);
void SWTTreeItem_setHasChildren(SWTTreeItem *item);
void SWTTreeItem_setUserData(SWTTreeItem *item,void* userdata);
void* SWTTreeItem_getUserData(SWTTreeItem *item);


#endif /* OSX_OBJC_NSOUTLINEVIEW_H_ */
