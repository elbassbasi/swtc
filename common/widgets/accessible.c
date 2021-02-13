/*
 * accessible.c
 *
 *  Created on: 26 août 2019
 *      Author: azeddine El Bassbasi
 */
#include <swtc.h>
void w_accessible_add_relation(w_accessible *accessible, int type,
		w_accessible *target) {
}
struct w_control* w_accessible_get_control(w_accessible *accessible) {
	return 0;
}
void w_accessible_remove_relation(w_accessible *accessible, int type,
		w_accessible *target) {
}
void w_accessible_send_event(w_accessible *accessible, int event,
		void *eventData) {
}
void w_accessible_selection_changed(w_accessible *accessible) {
}
void w_accessible_set_focus(w_accessible *accessible, int childID) {
}
void w_accessible_text_caret_moved(w_accessible *accessible, int index) {
}
void w_accessible_text_changed(w_accessible *accessible, int type,
		int startIndex, int length) {
}
void w_accessible_text_selection_changed(w_accessible *accessible) {
}
