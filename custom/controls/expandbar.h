/*
 * expandbar.h
 *
 *  Created on: 28 juin 2021
 *      Author: Azeddine
 */

#ifndef CUSTOM_CONTROLS_EXPANDBAR_H_
#define CUSTOM_CONTROLS_EXPANDBAR_H_
#include "../widgets/canvas.h"
typedef struct cw_expanditem {
	unsigned expanded :1;
	unsigned hover :1;
	int image;
	w_rect rect;
	char *text;
	void *userdata;
	w_control *control;
} cw_expanditem;
typedef struct cw_expandbar_priv {
	w_array *items;
	w_imagelist *imagelist;
	int band_height;
	int spacing;
	int focusItem;
	int yCurrentScroll;
} cw_expandbar_priv;
void cw_expandbar_show_item(w_expandbar *expandbar, int index,
		cw_expanditem *_item);
void cw_expanditem_redraw(w_expandbar *expandbar, cw_expanditem *_item,
		int all);
void cw_expandbar_layout_items(w_expandbar *expandbar, int index);
void cw_expandbar_set_scrollbar(w_expandbar *expandbar);
void cw_expandbar_update_band_heigth(w_widget *expandbar);
void cw_expandbar_class_init(w_toolkit *toolkit, wushort classId,struct _w_expandbar_class *clazz);
#endif /* CUSTOM_CONTROLS_EXPANDBAR_H_ */
