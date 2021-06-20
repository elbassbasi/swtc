/*
 * value.c
 *
 *  Created on: 20 sept. 2019
 *      Author: azeddine El Bassbasi
 */
#include <swtc.h>
wresult w_value_unknown_free(w_value *value) {
	return W_FALSE;
}
wresult w_value_unknown_convert(w_value *value, w_value *v2, w_alloc alloc,
		void *user_data) {
	return W_FALSE;
}
w_value_class w_value_unknown_class = { //
		W_VALUE_POINTER, //
				0, //
				"unknown", //
				w_value_unknown_free, //
				w_value_unknown_convert, //
		};
/*
 * pointer type
 */
wresult w_value_pointer_free(w_value *value) {
	if (value->flags & W_VALUE_FREE_MEMORY) {
		free(value->pointer);
	}
	return W_TRUE;
}
wresult w_value_pointer_convert(w_value *value, w_value *v2, w_alloc alloc,
		void *user_data) {
	switch (v2->clazz->type) {
	case W_VALUE_POINTER:
		value->pointer = v2->pointer;
		break;
	case W_VALUE_STRING_UTF8:
		value->pointer = v2->string;
		break;
	default:
		value->pointer = 0;
		return W_FALSE;
	}
	return W_TRUE;
}
w_value_class w_value_pointer_class = { //
		W_VALUE_POINTER, //
				0, //
				"pointer", //
				w_value_pointer_free, //
				w_value_pointer_convert, //
		};
/*
 * Int type
 */
wresult w_value_int_free(w_value *value) {
	return W_TRUE;
}
wresult w_value_int_convert(w_value *value, w_value *v2, w_alloc alloc,
		void *user_data) {
	switch (v2->clazz->type) {
	case W_VALUE_POINTER:
		v2->pointer = 0;
		break;
	case W_VALUE_BOOL:
	case W_VALUE_INT:
		v2->INT64 = value->INT64;
		break;
	case W_VALUE_FLOAT:
		v2->FLOAT = value->INT64;
		break;
	case W_VALUE_DOUBLE:
		v2->DOUBLE = value->INT64;
		break;
	case W_VALUE_STRING_UTF8: {
		w_value_print(v2, alloc, user_data, "%ld", value->INT64);
	}
		break;
	default:
		return W_FALSE;
	}
	return W_TRUE;
}
/*
 * Int type
 */
wresult w_value_bool_convert(w_value *value, w_value *v2, w_alloc alloc,
		void *user_data) {
	if (v2->clazz->type == W_VALUE_STRING_UTF8) {
		if (value->INT64) {
			w_value_set_string(v2, "true", -1, 0);
		} else {
			w_value_set_string(v2, "false", -1, 0);
		}
		return W_TRUE;
	}
	return w_value_int_convert(value, v2, alloc, user_data);
}
w_value_class w_value_bool_class = { //
		W_VALUE_BOOL, //
				0, //
				"bool", //
				w_value_int_free, //
				w_value_bool_convert, //
		};
w_value_class w_value_int_class = { //
		W_VALUE_INT, //
				0, //
				"int", //
				w_value_int_free, //
				w_value_int_convert, //
		};
/*
 * float type
 */
wresult w_value_float_free(w_value *value) {
	return W_TRUE;
}
wresult w_value_float_convert(w_value *value, w_value *v2, w_alloc alloc,
		void *user_data) {
	switch (v2->clazz->type) {
	case W_VALUE_POINTER:
		v2->pointer = 0;
		break;
	case W_VALUE_BOOL:
	case W_VALUE_INT:
		v2->INT64 = value->INT64;
		break;
	case W_VALUE_FLOAT:
		v2->FLOAT = value->INT64;
		break;
	case W_VALUE_DOUBLE:
		v2->DOUBLE = value->INT64;
		break;
	case W_VALUE_STRING_UTF8:
		w_value_print(v2, alloc, user_data, "%f", value->FLOAT);
		break;
	default:
		return W_FALSE;
	}
	return W_TRUE;
}
w_value_class w_value_float_class = { //
		W_VALUE_FLOAT, //
				0, //
				"float", //
				w_value_float_free, //
				w_value_float_convert, //
		};
/*
 * double type
 */
wresult w_value_double_free(w_value *value) {
	return W_TRUE;
}
wresult w_value_double_convert(w_value *value, w_value *v2, w_alloc alloc,
		void *user_data) {
	switch (v2->clazz->type) {
	case W_VALUE_POINTER:
		v2->pointer = 0;
		break;
	case W_VALUE_BOOL:
	case W_VALUE_INT:
		v2->INT64 = value->INT64;
		break;
	case W_VALUE_FLOAT:
		v2->FLOAT = value->INT64;
		break;
	case W_VALUE_DOUBLE:
		v2->DOUBLE = value->INT64;
		break;
	case W_VALUE_STRING_UTF8: {
		w_value_print(v2, alloc, user_data, "%lf", value->DOUBLE);
	}
		break;
	default:
		return W_FALSE;
	}
	return W_TRUE;
}
w_value_class w_value_double_class = { //
		W_VALUE_DOUBLE, //
				0, //
				"double", //
				w_value_double_free, //
				w_value_double_convert, //
		};
/*
 * float type
 */
wresult w_value_utf8_free(w_value *value) {
	return W_TRUE;
}
wresult w_value_utf8_convert(w_value *value, w_value *v2, w_alloc alloc,
		void *user_data) {
	switch (v2->clazz->type) {
	case W_VALUE_POINTER:
		value->INT64 = ((wint64*) v2->pointer)[0];
		break;
	case W_VALUE_BOOL:
	case W_VALUE_INT:
		value->INT64 = v2->INT64;
		break;
	case W_VALUE_FLOAT:
		value->INT64 = v2->FLOAT;
		break;
	case W_VALUE_DOUBLE:
		value->INT64 = v2->DOUBLE;
		break;
	case W_VALUE_STRING_UTF8: {
		//need modified
		value->string = v2->string;
	}
		break;
	default:
		return W_FALSE;
	}
	return W_TRUE;
}
w_value_class w_value_utf8_class = { //
		W_VALUE_STRING_UTF8, //
				0, //
				"utf8", //
				w_value_utf8_free, //
				w_value_utf8_convert, //
		};
/*
 * unicode
 */
wresult w_value_unicode_free(w_value *value) {
	return W_TRUE;
}
wresult w_value_unicode_convert(w_value *value, w_value *v2, w_alloc alloc,
		void *user_data) {
	switch (v2->clazz->type) {
	case W_VALUE_POINTER:
		value->INT64 = ((wint64*) v2->pointer)[0];
		break;
	case W_VALUE_BOOL:
	case W_VALUE_INT:
		value->INT64 = v2->INT64;
		break;
	case W_VALUE_FLOAT:
		value->INT64 = v2->FLOAT;
		break;
	case W_VALUE_DOUBLE:
		value->INT64 = v2->DOUBLE;
		break;
	case W_VALUE_STRING_UTF8: {
		//need modified
		value->string = v2->string;
	}
		break;
	default:
		return W_FALSE;
	}
	return W_TRUE;
}
w_value_class w_value_unicode_class = { //
		W_VALUE_STRING_UTF8, //
				0, //
				"utf8", //
				w_value_unicode_free, //
				w_value_unicode_convert, //
		};
wushort id = 0x100;
void w_value_registre_class(w_value_class *clazz) {
	clazz->type = id;
	id++;
}
void w_value_init_pointer(w_value *value, void *pointer, size_t length,
		int flags) {
	value->clazz = &w_value_pointer_class;
	value->flags = flags;
	value->size = length;
	value->pointer = pointer;
}
void w_value_init_bool(w_value *value, int _int) {
	value->clazz = &w_value_bool_class;
	value->INT64 = _int;
	value->flags = 0;
	value->size = 0;
}
void w_value_init_char(w_value *value, char _char) {
	value->clazz = &w_value_int_class;
	value->INT64 = _char;
	value->flags = W_VALUE_CHAR;
	value->size = 0;
}
void w_value_init_short(w_value *value, short _short) {
	value->clazz = &w_value_int_class;
	value->INT64 = _short;
	value->flags = W_VLAUE_SHORT;
	value->size = 0;
}
void w_value_init_int(w_value *value, int _int) {
	value->clazz = &w_value_int_class;
	value->INT64 = _int;
	value->flags = W_VALUE_INT32;
	value->size = 0;
}
void w_value_init_int64(w_value *value, wint64 _int) {
	value->clazz = &w_value_int_class;
	value->INT64 = _int;
	value->flags = W_VALUE_INT64;
	value->size = 0;
}
void w_value_init_double(w_value *value, double _double) {
	value->clazz = &w_value_double_class;
	value->DOUBLE = _double;
	value->flags = 0;
	value->size = 0;
}
void w_value_init_float(w_value *value, float _float) {
	value->clazz = &w_value_float_class;
	value->FLOAT = _float;
	value->flags = 0;
	value->size = 0;
}
wresult w_value_init_string(w_value *value, const char *string, int length,
		int flags) {
	value->clazz = &w_value_utf8_class;
	value->flags = flags;
	value->size = length;
	if (flags & W_VALUE_FREE_MEMORY) {
		value->string = strdup(string);
	} else {
		value->string = (char*) string;
	}
	return W_TRUE;
}
void w_value_set_int(w_value *value, int _int) {
	w_value_free(value);
	w_value_init_int(value, _int);
}
void w_value_set_float(w_value *value, float _float) {
	w_value_free(value);
	w_value_init_float(value, _float);
}
wresult w_value_set_string(w_value *value, const char *string, size_t length,
		int flags) {
	w_value_free(value);
	return w_value_init_string(value, string, length, flags);
}
void w_value_set_pointer(w_value *value, void *pointer, size_t length,
		int flags) {
	w_value_free(value);
	return w_value_init_pointer(value, pointer, length, flags);
}

void w_value_init(w_value *value) {
	value->clazz = &w_value_unknown_class;
	value->pointer = 0;
	value->flags = 0;
	value->size = 0;
}
void w_value_free(w_value *value) {
	value->clazz->free(value);
	value->clazz = &w_value_unknown_class;
}

wint64 w_value_get_int(w_value *value) {
	w_value v2;
	w_value_init_int64(&v2, 0);
	value->clazz->convert_to(value, &v2, 0, 0);
	return v2.INT64;
}
float w_value_get_float(w_value *value) {
	w_value v2;
	w_value_init_float(&v2, 0);
	value->clazz->convert_to(value, &v2, 0, 0);
	return v2.FLOAT;
}
double w_value_get_double(w_value *value) {
	w_value v2;
	w_value_init_double(&v2, 0);
	value->clazz->convert_to(value, &v2, 0, 0);
	return v2.DOUBLE;
}
const char* w_value_get_string_0(w_value *value, w_alloc alloc, void *user_data,
		int flags) {
	w_value v2;
	v2.clazz = &w_value_utf8_class;
	v2.flags = flags;
	v2.size = 0;
	v2.string = 0;
	value->clazz->convert_to(value, &v2, alloc, user_data);
	w_value_free(&v2);
	return v2.string;
}
const char* w_value_get_string(w_value *value, char *text, int length,
		int flags) {
	if (value->clazz->type == W_VALUE_STRING_UTF8) {
		return value->string;
	} else {
		w_alloc_buffer buffer;
		buffer.buffer = text;
		buffer.total_size = 0;
		buffer.size = length;
		return w_value_get_string_0(value, w_alloc_buffer_copy, &buffer, flags);
	}
}
void w_value_string_copy(w_value *value, char *text, size_t length) {
}
char* w_value_string_create(w_value *value, size_t length) {
	if (value->clazz == &w_value_utf8_class
			&& (value->flags & W_VALUE_USER_MEMORY) && value->string != 0
			&& value->size >= length) {
		return value->string;
	} else {
		w_value_free(value);
		value->clazz = &w_value_utf8_class;
		value->string = (char*) malloc(length);
		value->flags = W_VALUE_FREE_MEMORY;
		value->size = length;
		return value->string;
	}
}
int w_value_vprint(w_value *value, w_alloc alloc, void *user_data,
		const char *_template, va_list args) {
	va_list args2;
	va_copy(args2, args);
	size_t sz = vsnprintf(0, 0, _template, args2);
	va_end(args2);
	char *s = w_value_string_create(value, sz + 1);
	if (s != 0) {
		size_t sz = vsprintf(s, _template, args);
	}
	return sz;
}
int w_value_print(w_value *value, w_alloc alloc, void *user_data,
		const char *Template, ...) {
	va_list args;
	va_start(args, Template);
	int ret = w_value_vprint(value, alloc, user_data, Template, args);
	va_end(args);
	return ret;
}
