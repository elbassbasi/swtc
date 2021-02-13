/*
 * String.h
 *
 *  Created on: 24 juil. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_CORE_STRING_H_
#define SWTP_CORE_STRING_H_
#include "Utf8.h"
extern "C" {
typedef struct w_string_ref {
	unsigned sign :8; //must be 0
	unsigned length :24;
	wuint ref;
	char data[0];
} w_string_ref;

SWTP_PUBLIC extern struct w_string_ref *empty_string;
SWTP_PUBLIC const char* w_string_get_chars(w_string_ref *ref);
SWTP_PUBLIC const char* w_string_get_chars_not_null(struct w_string_ref *ref);
SWTP_PUBLIC size_t w_string_get_length(w_string_ref *ref);
SWTP_PUBLIC w_string_ref* w_string_new(size_t size);
SWTP_PUBLIC void w_string_init_from_chars(struct w_string_ref **ref,
		const char *chars, int copy);
SWTP_PUBLIC void w_string_set_chars(w_string_ref **ref, const char *chars,
		int copy);
SWTP_PUBLIC void w_string_init_from_wchars(struct w_string_ref **ref,
		const wchar_t *wchars);
SWTP_PUBLIC void w_string_set_wchars(w_string_ref **ref, const wchar_t *chars);
SWTP_PUBLIC void w_string_init_from_ref(w_string_ref **ref, w_string_ref *s,
		int clone);
SWTP_PUBLIC void w_string_move(w_string_ref **ref, w_string_ref *s,
		int clone);
SWTP_PUBLIC void w_string_copy(w_string_ref **ref, w_string_ref *s);
SWTP_PUBLIC void w_string_copy_chars(w_string_ref **ref, const char *str,
		size_t size);
SWTP_PUBLIC void w_string_ref_inc(w_string_ref *ref);
SWTP_PUBLIC w_string_ref* w_string_ref_dec(w_string_ref *ref);
SWTP_PUBLIC void w_string_sprintf_0(w_string_ref **ref, const char *format,
		va_list args);
SWTP_PUBLIC void w_string_sprintf(w_string_ref **ref, const char *format, ...);
SWTP_PUBLIC void w_string_concat(w_string_ref **ref, w_string_ref *s1,
		w_string_ref *s2);
/*
 *	w_alloc_string_ref
 */
SWTP_PUBLIC size_t w_alloc_string_ref(void* user_data,
		size_t size, void **string);
}
/*
 *
 */
class SWTP_PUBLIC WString {
public:
	w_string_ref *ref;
	WString() {
		this->ref = 0;
	}
	WString(const char *str) {
		w_string_init_from_chars(&this->ref, str, false);
	}
	WString(const char *str, bool copy) {
		w_string_init_from_chars(&this->ref, str, copy);
	}
	WString(const wchar_t *wchar) {
		w_string_init_from_wchars(&this->ref, wchar);
	}
	WString(const WString &str) {
		w_string_init_from_ref(&this->ref, str.ref, false);
	}
	WString(w_string_ref *ref) {
		w_string_init_from_ref(&this->ref, ref, false);
	}
	~WString() {
		w_string_ref_dec(this->ref);
	}
	WString& operator=(const WString &str) {
		w_string_move(&this->ref, str.ref, false);
		return *this;
	}
	WString& operator=(w_string_ref *ref) {
		w_string_move(&this->ref, ref, false);
		return *this;
	}
	WString& operator=(const char *str) {
		w_string_set_chars(&this->ref, str, false);
		return *this;
	}
	operator const char*() {
		return GetChars();
	}
public:
	/**
	 * Returns the length of this string.
	 * The length is equal to the number of <a href="Character.html#unicode">Unicode
	 * code units</a> in the string.
	 *
	 * @return  the length of the sequence of characters represented by this
	 *          object.
	 */
	size_t GetLength() const {
		return w_string_get_length(this->ref);
	}
	/**
	 * Returns the length of this string.
	 * The length is equal to the number of <a href="Character.html#unicode">Unicode
	 * code units</a> in the string.
	 *
	 * @return  the length of the sequence of characters represented by this
	 *          object.
	 */
	size_t Length() const {
		return w_string_get_length(this->ref);
	}
	/**
	 * Returns {@code true} if, and only if, {@link #length()} is {@code 0}.
	 *
	 * @return {@code true} if {@link #length()} is {@code 0}, otherwise
	 * {@code false}
	 */
	bool IsEmpty() {
		return GetLength() == 0;
	}
	const char* GetChars() const {
		return w_string_get_chars(this->ref);
	}
	const char* GetCharsNotNull() const {
		return w_string_get_chars_not_null(this->ref);
	}
	char CharAt(int index) const {
		return GetChars()[index];
	}
	WString& Copy(const WString &str) {
		w_string_copy(&this->ref, str.ref);
		return *this;
	}
	WString& Copy(w_string_ref *ref) {
		w_string_copy(&this->ref, ref);
		return *this;
	}
	WString& Copy(const char *str) {
		w_string_set_chars(&this->ref, str, true);
		return *this;
	}
	WString& Copy(const char *str, size_t size) {
		w_string_copy_chars(&this->ref, str, size);
		return *this;
	}
	WString Clone() {
		WString s;
		s.Copy(*this);
		return s;
	}
	static WString Sprintf(const char *format, ...) {
		WString ret;
		va_list args;
		va_start(args, format);
		w_string_sprintf_0(&ret.ref, format, args);
		return ret;
	}
	static WString Concat(const WString &a, const WString &b) {
		WString s;
		w_string_concat(&s.ref, a.ref, b.ref);
		return s;
	}
};

inline WString operator+(const WString &a, const char *b) {
	return WString::Concat(a, WString(b, false));
}
inline WString operator+(const WString &a, const WString &b) {
	return WString::Concat(a, b);
}
inline WString operator+(const char *a, const WString &b) {
	return WString::Concat(WString(a, false), b);
}

namespace swt {
typedef ::WString String;
}  // namespace swt

#endif /* SWTP_CORE_STRING_H_ */
