/*
 * Name:        alloc.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef SWTC_CORE_ALLOC_H_
#define SWTC_CORE_ALLOC_H_
#include "defs.h"
#include "atomic.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef size_t (*w_alloc)(void *user_data, size_t size, void **buf);
SWT_PUBLIC wresult w_alloc_set_text(w_alloc alloc, void *user_data, int toenc,
		const char *text, int length, int enc);
SWT_PUBLIC wresult w_alloc_printf(w_alloc alloc, void *user_data, int toenc,
		int enc, const char *format, va_list args);

typedef struct w_alloc_buffer {
	void *buffer;
	size_t size;
	size_t total_size;
} w_alloc_buffer;
size_t w_alloc_buffer_new(void *user_data, size_t size, void **buffer);
size_t w_alloc_buffer_copy(void *user_data, size_t size, void **buffer);
/*
 * array
 */
typedef struct w_array {
	int alloc;
	int count;
	char data[0];
} w_array;
typedef void (*w_array_free_element)(w_array *array, void *element, int index);
SWT_PUBLIC int w_array_get_count(w_array *array, void **firstElement);
SWT_PUBLIC void* w_array_get(w_array *array, int index, int element_size);
SWT_PUBLIC void* w_array_set(w_array **array, int index, int element_size);
SWT_PUBLIC int w_array_alloc(w_array **array, int newalloc, int element_size);
SWT_PUBLIC void* w_array_add(w_array **array, int index, int element_size,
		int *newIndex);
SWT_PUBLIC void w_array_remove(w_array *array, int index, int element_size,
		w_array_free_element free_element);
SWT_PUBLIC void w_array_free(w_array *array, int element_size,
		w_array_free_element free_element);
/*
 * link
 */
typedef struct w_link_0 {
	void *next;
	void *prev;
} w_link_0;
SWT_PUBLIC void w_link_linkfirst_0(w_link_0 *e, void *e_p, void **first);
SWT_PUBLIC void w_link_linklast_0(w_link_0 *e, void *e_p, void **first);
SWT_PUBLIC void w_link_linkbefore_0(w_link_0 *e, void *e_p, void *succ,
		void **first);
SWT_PUBLIC void w_link_linkafter_0(w_link_0 *e, void *e_p, void *succ,
		void **first);
SWT_PUBLIC void w_link_unlink_0(w_link_0 *e, void *e_p, void **first);
SWT_PUBLIC void w_link_replace_0(w_link_0 *x, void *x_p, void *e, void **first);

typedef struct w_link w_link;
struct w_link {
	w_link *next;
	w_link *prev;
};
SWT_PUBLIC void w_link_linkfirst(w_link *e, w_link **first);
SWT_PUBLIC void w_link_linklast(w_link *e, w_link **first);
SWT_PUBLIC void w_link_linkbefore(w_link *e, w_link *succ, w_link **first);
SWT_PUBLIC void w_link_linkafter(w_link *e, w_link *succ, w_link **first);
SWT_PUBLIC void w_link_unlink(w_link *e, w_link **first);
SWT_PUBLIC void w_link_replace(w_link *x, w_link *e, w_link **first);

#ifdef __cplusplus
}
#endif
#endif /* SWTC_CORE_ALLOC_H_ */
