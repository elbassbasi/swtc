/*
 * Value.h
 *
 *  Created on: 12 aout 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_CORE_VALUE_H_
#define SWTP_CORE_VALUE_H_
#include "String.h"

class SWTP_PUBLIC WValue: public w_value {
public:
	WValue() {
		w_value_init(this);
	}
	WValue(bool _bool) {
		w_value_init_bool(this, _bool);
	}
	WValue(char _char) {
		w_value_init_char(this, _char);
	}
	WValue(short _short) {
		w_value_init_short(this, _short);
	}
	WValue(int _int) {
		w_value_init_int(this, _int);
	}
	WValue(wint64 _int) {
		w_value_init_int64(this, _int);
	}
	WValue(float _float) {
		w_value_init_float(this, _float);
	}
	WValue(double _double) {
		w_value_init_double(this, _double);
	}
	WValue(const char *string) {
		w_value_init_string(this, string, -1, 0);
	}
	WValue(const char *string, bool clone) {
		w_value_init_string(this, string, -1, clone ? W_VALUE_FREE_MEMORY : 0);
	}
	template<typename T>
	WValue(T *v) {
		w_value_init(this);
		this->clazz = &T::value_class;
		this->pointer = v;
	}
	~WValue() {
		w_value_free(this);
	}
	void Free() {
		w_value_free(this);
	}
	wuint GetClassId() {
		return this->clazz->type;
	}
	/*
	 * @param temp
	 */
	const char* GetString(char *temp, size_t length, int flags) {
		return w_value_get_string(this, temp, length, flags);
	}
	const char* GetString(char *temp, size_t length) {
		return GetString(temp, length, W_VALUE_STRING_COPY_IS_NEED);
	}
	void SetInt(wint64 int_) {
		w_value_set_int(this, int_);
	}
	bool IsInt() {
		return GetClassId() == W_VALUE_INT;
	}
	void SetFloat(double float_) {
		w_value_set_float(this, float_);
	}
	bool SetString(const char *string, size_t length, int flags) {
		return w_value_set_string(this, string, length, flags);
	}
	bool SetUserString(const char *string, size_t length, bool free) {
		return SetString(string, length,
				free ? W_VALUE_FREE_MEMORY | W_VALUE_USER_MEMORY : W_VALUE_USER_MEMORY);
	}
	void SetPointer(void *pointer, size_t length, int flags) {
		w_value_set_pointer(this, pointer, length, flags);
	}
	void SetPointer(void *pointer, size_t length) {
		w_value_set_pointer(this, pointer, length, 0);
	}
	void SetUserPointer(void *pointer, size_t length) {
		SetPointer(pointer, length, W_VALUE_USER_MEMORY);
	}
	bool SetString(const char *string, bool clone) {
		return SetString(string, -1, clone ? W_VALUE_FREE_MEMORY : 0);
	}
	void SetString(const char *string) {
		SetString(string, false);
	}
	bool CopyString(const char *string) {
		return SetString(string, true);
	}
	bool CopyString(const char *string, size_t length) {
		return SetString(string, length, W_VALUE_STRING_COPY);
	}
	int Print(const char *format, ...) {
		va_list args;
		va_start(args, format);
		int ret = Print(format, args);
		va_end(args);
		return ret;
	}
	int Sprint(const char *format, ...) {
		va_list args;
		va_start(args, format);
		int ret = Print(format, args);
		va_end(args);
		return ret;
	}
	int Print(const char *format, va_list args) {
		return Print(0, 0, format, args);
	}
	int Print(w_alloc alloc, void *user_data, const char *format,
			va_list args) {
		return w_value_vprint(this, alloc, user_data, format, args);
	}
};

#endif /* SWTP_CORE_VALUE_H_ */
