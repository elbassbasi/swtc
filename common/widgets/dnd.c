/*
 * dnd.c
 *
 *  Created on: 7 oct. 2020
 *      Author: azeddine El Bassbasi
 */
#include <swtc.h>

wresult w_dragsource_create(w_dragsource *dragsource, w_toolkit *toolkit,
		w_control *control, wuint64 style,
		w_widget_post_event_proc post_event) {
	return _w_widget_create(W_WIDGET(dragsource), toolkit, W_WIDGET(control),
			style, _W_CLASS_DRAGSOURCE, post_event);
}
w_control* w_dragsource_get_control(w_dragsource *dragsource) {
	int result = W_WIDGET_CHECK0(dragsource);
	if (result > 0) {
		return W_DRAGSOURCE_GET_CLASS(dragsource)->get_control(dragsource);
	} else
		return 0;
}
w_transfer** w_dragsource_get_transfer(w_dragsource *dragsource,
		size_t *length) {
	int result = W_WIDGET_CHECK0(dragsource);
	if (result > 0) {
		return W_DRAGSOURCE_GET_CLASS(dragsource)->get_transfer(dragsource,
				length);
	} else {
		*length = 0;
		return 0;
	}
}
wresult w_dragsource_set_transfer(w_dragsource *dragsource,
		w_transfer **transferAgents, size_t length) {
	int result = W_WIDGET_CHECK0(dragsource);
	if (result > 0) {
		return W_DRAGSOURCE_GET_CLASS(dragsource)->set_transfer(dragsource,
				transferAgents, length);
	} else
		return result;
}
/*
 *
 */
wresult w_droptarget_create(w_droptarget *droptarget, w_toolkit *toolkit,
		w_control *control, wuint64 style,
		w_widget_post_event_proc post_event) {
	return _w_widget_create(W_WIDGET(droptarget), toolkit, W_WIDGET(control),
			style, _W_CLASS_DROPTARGET, post_event);
}
w_control* w_droptarget_get_control(w_droptarget *droptarget) {
	int result = W_WIDGET_CHECK0(droptarget);
	if (result > 0) {
		return W_DROPTARGET_GET_CLASS(droptarget)->get_control(droptarget);
	} else
		return 0;
}
w_transfer** w_droptarget_get_transfer(w_droptarget *droptarget,
		size_t *length) {
	int result = W_WIDGET_CHECK0(droptarget);
	if (result > 0) {
		return W_DROPTARGET_GET_CLASS(droptarget)->get_transfer(droptarget,
				length);
	} else {
		*length = 0;
		return 0;
	}
}
wresult w_droptarget_set_transfer(w_droptarget *droptarget,
		w_transfer **transferAgents, size_t length) {
	int result = W_WIDGET_CHECK0(droptarget);
	if (result > 0) {
		return W_DROPTARGET_GET_CLASS(droptarget)->set_transfer(droptarget,
				transferAgents, length);
	} else
		return result;
}
