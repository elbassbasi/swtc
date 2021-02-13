/*
 * row.c
 *
 *  Created on: 28 sept. 2020
 *      Author: azeddine
 */
#include <swtc.h>

void w_layout_row_data_init(w_layout_row_data *rowdata){

}
void w_layout_row_init(w_layout_row *layout, int type){

}
void w_layout_row_compute_size(w_layout *layout, struct w_composite *composite,
		w_size *size, int wHint, int hHint, int flushCache){

}
void w_layout_row_do_layout(w_layout *layout, struct w_composite *composite, int flushCache){

}
void w_layout_row_set_data(w_layout *layout, w_control *control,
		const w_layout_data *_data){

}
void w_layout_row_get_data(w_layout *layout, w_control *control,
		w_layout_data *_data){

}
_w_layout_class w_layout_row_class = { w_layout_row_compute_size, //
		w_layout_flush_cache0, //
		w_layout_row_do_layout, //
		w_layout_row_set_data, //
		w_layout_row_get_data //
		};
