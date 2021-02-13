#import "swt.h"
@interface SWTTableView : NSTableView{
@public void* widget;
}

@end

@implementation SWTTableView


@end

void NSTableView_setHeaderView(NSTableView* tableView,NSTableHeaderView *headerView){
    tableView.headerView = headerView;
}
void NSTableView_addTableColumn(NSTableView* tableView,NSTableColumn* column){
    [tableView addTableColumn:column];
}
/*
 
 */
@interface SWTTableHeaderView : NSTableHeaderView{
@public w_widget* widget;
}

@end

@implementation SWTTableHeaderView


@end

NSTableHeaderView* NSTableHeaderView_new(w_widget* widget){
    SWTTableHeaderView* tableHeaderView = [[SWTTableHeaderView alloc] init];
    tableHeaderView->widget = widget;
    return tableHeaderView;
}
w_widget* NSTableView_getWidget(NSView* view){
    if( [view isKindOfClass:[SWTTableView class]])
    return ((SWTTableView*)view)->widget;
    else return 0;
}
NSUInteger NSTableView_selectedRow(NSTableView* tableView){
    return [tableView selectedRow];
}
/*
 SWTImageTextCell
 */
@interface SWTImageTextCell : NSTextFieldCell{
@public void* userdata;
}

@end

@implementation SWTImageTextCell


@end
NSCell* SWTImageTextCell_new(NSString* identifier){
    SWTImageTextCell* cell = [[SWTImageTextCell alloc] init];
    return cell;
}
void NSCell_setTitle(NSCell* cell,NSString* str){
    [cell setTitle:str];
}
void NSCell_setLineBreakMode(NSCell* cell,int lineBreak){
    [cell setLineBreakMode:lineBreak];
}
/*
 
 */
@interface SWTTableColumn : NSTableColumn{
@public w_widget* widget;
}

@end

@implementation SWTTableColumn


@end
NSTableColumn* SWTTableColumn_new(NSString* identifier){
    SWTTableColumn* tableColumn = [[SWTTableColumn alloc] init];
    [tableColumn setIdentifier:identifier];
    return tableColumn;
}
void NSTableColumn_setMinWidth(NSTableColumn* tablecolumn,int minWidth){
    [tablecolumn setMinWidth:minWidth];
}
void NSTableColumn_setWidth(NSTableColumn* tablecolumn,int width){
    [tablecolumn setWidth:width];
}
void NSTableColumn_setResizingMask(NSTableColumn* tablecolumn,NSTableColumnResizingOptions ops){
    [tablecolumn setResizingMask:ops];
}
void NSTableColumn_setDataCell(NSTableColumn* tablecolumn,NSCell* cell){
    [tablecolumn setDataCell:cell];
}
NSCell* NSTableColumn_headerCell(NSTableColumn* tablecolumn){
    return [tablecolumn headerCell];
}

