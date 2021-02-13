/*
 * Name:        imagelist.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "gc.h"
void w_imagelist_init(w_imagelist *imagelist) {
}
void w_imagelist_dispose(w_imagelist *imagelist) {
}
wresult w_imagelist_create(w_imagelist *imagelist, w_size *size,
		int initialCount) {
}
int w_imagelist_add(w_imagelist *imagelist, w_image *image) {
}
wresult w_imagelist_is_ok(w_imagelist *imagelist) {
}
wresult w_imagelist_replace(w_imagelist *imagelist, int index, w_image *image) {
}
wresult w_imagelist_remove(w_imagelist *imagelist, int index) {
}
wresult w_imagelist_remove_all(w_imagelist *imagelist) {
}
wresult w_imagelist_get_image(w_imagelist *imagelist, int index, int copy,
		w_image *image) {
}
wresult w_imagelist_get_size(w_imagelist *imagelist, w_size *size) {
}
int w_imagelist_get_count(w_imagelist *imagelist) {
}
wresult w_imagelist_draw(w_imagelist *imagelist, w_graphics *graphics,
		int index, w_point *pt, int state) {
}
wresult w_imagelist_draw_ex(w_imagelist *imagelist, w_graphics *graphics,
		int index, w_point *pt, int state, w_color bg, w_color fg) {
}
