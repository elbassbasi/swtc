/*
 * Name:        value.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef SWTC_CORE_VALUE_H_
#define SWTC_CORE_VALUE_H_
#include "utf8.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef enum {
	W_VALUE_UNKNOWN = 0, //
	W_VALUE_POINTER,
	W_VALUE_BOOL,
	W_VALUE_INT,
	W_VALUE_FLOAT,
	W_VALUE_DOUBLE,
	//string
	W_VALUE_STRING_UTF8,
	W_VALUE_STRING_REF,
} w_value_type;
typedef struct w_value w_value;
typedef struct w_value_class {
	wushort type;
	wushort flags;
	const char *name;
	wresult (*free)(w_value *value);
	wresult (*convert_to)(w_value *value, w_value *to, w_alloc alloc,
			void *user_data);
} w_value_class;

enum {
	//flags
	/*
	 * free memory when use w_value_free
	 * @see W_VALUE_STRING ,W_VALUE_POINTER
	 */
	W_VALUE_FREE_MEMORY = 1 << 0,
	/*
	 * used when user need to copy memory to pointer
	 * @see W_VALUE_STRING ,W_VALUE_POINTER
	 */
	W_VALUE_USER_MEMORY = 1 << 1,
	W_VALUE_STRING_COPY = 1 << 2,
	W_VALUE_STRING_COPY_IS_NEED = 1 << 3,
	W_VALUE_CHAR = 1,
	W_VLAUE_SHORT = 2,
	W_VALUE_INT32 = 3,
	W_VALUE_INT64 = 4,
	W_VALUE_UNSIGNED = 1 << 5,
};
struct w_value {
	w_value_class *clazz;
	union {
		wint64 INT64;
		double DOUBLE;
		float FLOAT;
		void *pointer;
		char *string;
	};
	wuint flags;
	wuint size;
};
//init
SWT_PUBLIC void w_value_registre_class(w_value_class *clazz);
SWT_PUBLIC void w_value_init_bool(w_value *value, int _int);
SWT_PUBLIC void w_value_init_char(w_value *value, char _char);
SWT_PUBLIC void w_value_init_short(w_value *value, short _short);
SWT_PUBLIC void w_value_init_int(w_value *value, int _int);
SWT_PUBLIC void w_value_init_int64(w_value *value, wint64 _i);
SWT_PUBLIC void w_value_init_float(w_value *value, float _float);
SWT_PUBLIC void w_value_init_double(w_value *value, double _double);
SWT_PUBLIC wresult w_value_init_string(w_value *value, const char *string,
		size_t length, int flags);
SWT_PUBLIC void w_value_init_pointer(w_value *value, void *pointer,
		size_t length, int flags);
//set
SWT_PUBLIC void w_value_set_int(w_value *value, int _int);
SWT_PUBLIC void w_value_set_float(w_value *value, float _float);
SWT_PUBLIC wresult w_value_set_string(w_value *value, const char *string,
		size_t length, int flags);
SWT_PUBLIC void w_value_set_pointer(w_value *value, void *pointer,
		size_t length, int flags);

SWT_PUBLIC void w_value_init(w_value *value);
SWT_PUBLIC void w_value_free(w_value *value);

SWT_PUBLIC wint64 w_value_get_int(w_value *value);
SWT_PUBLIC float w_value_get_float(w_value *value);
SWT_PUBLIC double w_value_get_double(w_value *value);

SWT_PUBLIC const char* w_value_get_string_0(w_value *value, w_alloc alloc,
		void *user_data, int flags);
SWT_PUBLIC const char* w_value_get_string(w_value *value, char *text,
		size_t length, int flags);
SWT_PUBLIC char* w_value_string_create(w_value *value, size_t length);
SWT_PUBLIC void w_value_string_copy(w_value *value, char *text, size_t length);
SWT_PUBLIC int w_value_print(w_value *value, w_alloc alloc, void *user_data,
		const char *Template, ...);
SWT_PUBLIC int w_value_vprint(w_value *value, w_alloc alloc, void *user_data,
		const char *Template, va_list args);
#ifdef __cplusplus
}
#endif
#endif /* SWTC_CORE_VALUE_H_ */
