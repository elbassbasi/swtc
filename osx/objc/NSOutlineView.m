#import "swt.h"
/*
 SWTTreeItem
 */
@interface SWTTreeItem : NSObject{
@public SWTTreeItem* parent;
@public NSMutableArray<SWTTreeItem*>* children;
@public NSString* text;
@public void* userdata;
@public int imageIndex;
@public BOOL hasChildren;
}

@end
@implementation SWTTreeItem


@end
/*
 SWTOutlineView
 */
@interface SWTOutlineView : NSOutlineView <NSOutlineViewDataSource,NSOutlineViewDelegate>{
@public void* widget;
@public SWTTreeItem* root;
}
@end

@implementation SWTOutlineView
/*
 NSOutlineViewDataSource
 */
- (NSInteger)outlineView:(NSOutlineView *)outlineView numberOfChildrenOfItem:(nullable id)item{
    SWTTreeItem* i;
    if(item == nil){
        i= self->root;
    }else{
        i = item;
    }
    NSMutableArray<SWTTreeItem*>* childrens= i->children;
    if(childrens == 0) {
        return 0;
    }else{
        return [childrens count];
    }
}
- (id)outlineView:(NSOutlineView *)outlineView child:(NSInteger)index ofItem:(nullable id)item{
    SWTTreeItem* i;
    if(item == nil){
        i = self->root;
    }else{
        i = item;
    }
    NSMutableArray<SWTTreeItem*>* childrens= i->children;
    if(childrens==0) {
        return nil;
    }else{
        int count = [childrens count];
        if(index < count) return [childrens objectAtIndex:index];
        else return nil;
    }
}
- (BOOL)outlineView:(NSOutlineView *)outlineView isItemExpandable:(id)item{
    SWTTreeItem* i;
    if(item == nil){
        i= self->root;
    }else{
        i = item;
    }
    return i->hasChildren;
}
- (nullable id)outlineView:(NSOutlineView *)outlineView objectValueForTableColumn:(nullable NSTableColumn *)tableColumn byItem:(nullable id)item{
    return ((SWTTreeItem*)item)->text;
}
/*
 NSOutlineViewDelegate
 */
/*- (nullable NSView *)outlineView:(NSOutlineView *)outlineView viewForTableColumn:(nullable NSTableColumn *)tableColumn item:(id)item{
    NSTableCellView *cellView = [outlineView makeViewWithIdentifier:tableColumn.identifier owner:self];
    if(cellView != nil){
        cellView.textField.stringValue = ((SWTTreeItem*)item)->text;
    }
    return cellView;
}
- (nullable NSTableRowView *)outlineView:(NSOutlineView *)outlineView rowViewForItem:(id)item{
    return nil;
}
- (void)outlineView:(NSOutlineView *)outlineView didAddRowView:(NSTableRowView *)rowView forRow:(NSInteger)row{
    
}
- (void)outlineView:(NSOutlineView *)outlineView didRemoveRowView:(NSTableRowView *)rowView forRow:(NSInteger)row{
}
- (nullable NSString *)outlineView:(NSOutlineView *)outlineView typeSelectStringForTableColumn:(nullable NSTableColumn *)tableColumn item:(id)item {
     return ((SWTTreeItem*)item)->text;
}
- (void)outlineView:(NSOutlineView *)outlineView willDisplayCell:(id)cell forTableColumn:(nullable NSTableColumn *)tableColumn item:(id)item{
    NSTextFieldCell* textCell = (NSTextFieldCell*) cell;
    //[textCell setTitle:((SWTTreeItem*)item)->text];
}*/
- (void)outlineViewSelectionDidChange:(NSNotification *)notification{
    NSObject * item = [notification.userInfo objectForKey:@"NSObject"];
    _w_event_platform e;
    _w_event_platform_init(&e, widget, self, _NS_outlineViewSelectionDidChange);
    e.args[0]._ptr =(void*) item;
    _w_widget_send_event(widget,W_EVENT(&e));
}
- (void)outlineViewColumnDidMove:(NSNotification *)notification{
    _w_event_platform e;
    _w_event_platform_init(&e, widget, self, _NS_outlineViewColumnDidMove);
    e.args[0]._ptr =(void*) notification;
    _w_widget_send_event(widget,W_EVENT(&e));
}
- (void)outlineViewColumnDidResize:(NSNotification *)notification{
    _w_event_platform e;
    _w_event_platform_init(&e, widget, self, _NS_outlineViewColumnDidResize);
    e.args[0]._ptr =(void*) notification;
    _w_widget_send_event(widget,W_EVENT(&e));
}
- (void)outlineViewSelectionIsChanging:(NSNotification *)notification{
    _w_event_platform e;
    _w_event_platform_init(&e, widget, self, _NS_outlineViewSelectionIsChanging);
    e.args[0]._ptr =(void*) notification;
    _w_widget_send_event(widget,W_EVENT(&e));
}
- (void)outlineViewItemWillExpand:(NSNotification *)notification{
    NSObject * item = [notification.userInfo objectForKey:@"NSObject"];
    _w_event_platform e;
    _w_event_platform_init(&e, widget, self, _NS_outlineViewItemWillExpand);
    e.args[0]._ptr =(void*) item;
    _w_widget_send_event(widget,W_EVENT(&e));
}
- (void)outlineViewItemDidExpand:(NSNotification *)notification{
    SWTTreeItem * item =(SWTTreeItem *) [notification.userInfo objectForKey:@"NSObject"];
    _w_event_platform e;
    _w_event_platform_init(&e, widget, self, _NS_outlineViewItemDidExpand);
    e.args[0]._ptr =(void*) item;
    _w_widget_send_event(widget,W_EVENT(&e));
    if(item != nil){
        if(item->hasChildren==YES && item->children==nil){
            item->hasChildren = NO;
            [self reloadItem:item];
        }
    }
}
- (void)outlineViewItemWillCollapse:(NSNotification *)notification{
    _w_event_platform e;
    _w_event_platform_init(&e, widget, self, _NS_outlineViewItemWillCollapse);
    e.args[0]._ptr =(void*) notification;
    _w_widget_send_event(widget,W_EVENT(&e));
}
- (void)outlineViewItemDidCollapse:(NSNotification *)notification{
    _w_event_platform e;
    _w_event_platform_init(&e, widget, self, _NS_outlineViewItemDidCollapse);
    e.args[0]._ptr =(void*) notification;
    _w_widget_send_event(widget,W_EVENT(&e));
}
@end
w_widget* SWTOutlineView_get_widget(NSView* view){
    if( [view isKindOfClass:[SWTOutlineView class]])
    return ((SWTOutlineView*)view)->widget;
    else return 0;
}
NSOutlineView* SWTOutlineView_new(void* userdata){
    NSRect frame=NSMakeRect(0, 0, 0, 0);
    SWTOutlineView* outlineView = [[SWTOutlineView alloc] initWithFrame:frame];
    _w_toolkit_registre_class(_NS_CLASS_OUTLINEVIEW,outlineView.class,SWTOutlineView_get_widget);
    outlineView->widget = userdata;
    outlineView->root = [[SWTTreeItem alloc] init];
    outlineView.delegate = outlineView;
    outlineView.dataSource = outlineView;
    return outlineView;
}
void NSOutlineView_setAllowsMultipleSelection(NSOutlineView* outlineView,int op){
    [outlineView setAllowsMultipleSelection:op];
}
void NSOutlineView_setAutoresizesOutlineColumn(NSOutlineView* outlineView,int op){
    [outlineView setAutoresizesOutlineColumn:op];
}
void NSOutlineView_setAutosaveExpandedItems(NSOutlineView* outlineView,int op){
    [outlineView setAutosaveExpandedItems:op];
}
void NSOutlineView_setDataSource(NSOutlineView* outlineView,id object){
    [outlineView setDataSource:object];
}
void NSOutlineView_setDelegate(NSOutlineView* outlineView,id object){
    [outlineView setDelegate:object];
}
void NSOutlineView_setColumnAutoresizingStyle(NSOutlineView* outlineView,NSTableViewColumnAutoresizingStyle op){
    [outlineView setColumnAutoresizingStyle:op];
}
void NSOutlineView_setIntercellSpacing(NSOutlineView* outlineView,int width,int heigth){
    NSSize size=NSMakeSize(width, heigth);
    outlineView.intercellSpacing= size;
}
void NSOutlineView_reloadItem(NSOutlineView* outlineView,SWTTreeItem* obj,int reloadChildren){
    [outlineView reloadItem:obj reloadChildren:reloadChildren];
}
void NSOutlineView_reloadData(NSOutlineView* outlineView){
    [outlineView reloadData];
}
void NSOutlineView_setOutlineTableColumn(NSOutlineView* tableView,NSTableColumn* column){
    [tableView setOutlineTableColumn:column];
}
NSObject* NSOutlineView_itemAtRow(NSOutlineView* tableView,NSUInteger index){
    return [tableView itemAtRow:index];
}

/*
 
 */


SWTTreeItem *SWTTreeItem_new(){
    return [[SWTTreeItem alloc] init];
}
SWTTreeItem *SWTTreeItem_create(SWTOutlineView* outline,SWTTreeItem* parent,int index){
    SWTTreeItem *obj = [[SWTTreeItem alloc] init];
    if(parent==nil){
        parent = outline->root;
    }
    if(parent->children == nil){
        parent->children =[[NSMutableArray alloc] init];
        parent->hasChildren = YES;
    }
    if(index < 0){
        [parent->children addObject:obj];
    }else{
        [parent->children insertObject:obj atIndex:index];
    }
    obj->parent = parent;
    return obj;
}
void SWTTreeItem_setText(SWTTreeItem *item,NSString* text){
    item->text = text;
}
void SWTTreeItem_setHasChildren(SWTTreeItem *item){
    item->hasChildren = YES;
}
void SWTTreeItem_setUserData(SWTTreeItem *item,void* userdata){
    item->userdata = userdata;
}
void* SWTTreeItem_getUserData(SWTTreeItem *item){
    return item->userdata;
}

