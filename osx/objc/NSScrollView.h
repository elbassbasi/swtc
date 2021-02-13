/*
 * NSScrollView.h
 *
 *  Created on: Jan 1, 2021
 *      Author: azeddine
 */

#ifndef OSX_OBJC_NSSCROLLVIEW_H_
#define OSX_OBJC_NSSCROLLVIEW_H_
#include "NSView.h"
typedef struct NSScrollView NSScrollView;
#define NSSCROLLVIEW(x) ((NSScrollView*)x)
NSScrollView* SWTScrollView_new(void* userdata);
int NSScrollView_isKindOfClass(NSView* view);
void NSScrollView_setDrawsBackground(NSScrollView* scrollView,int draw);
void NSScrollView_setHasHorizontalScroller(NSScrollView* scrollView,int scroller);
void NSScrollView_setHasVerticalScroller(NSScrollView* scrollView,int scroller);
void NSScrollView_setAutohidesScrollers(NSScrollView* scrollView,int autohide);
void NSScrollView_setBorderType(NSScrollView* scrollView,int border);
NSView* NSScrollView_contentView(NSScrollView* scrollView);
void NSScrollView_setDocumentView(NSScrollView* scrollView,NSView* view);
void NSScrollView_contentSizeForFrameSize(NSScrollView* scrollView,w_size* size,int hFlag,int vFlag,int type);
NSView* NSScrollView_documentView(NSScrollView* scrollView);
#endif /* OSX_OBJC_NSSCROLLVIEW_H_ */
