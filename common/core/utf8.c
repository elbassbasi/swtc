/*
 * utf8.c
 *
 *  Created on: 2 oct. 2020
 *      Author: azeddine El Bassbasi
 */
#include <swtc.h>
#include <wchar.h>
/*
 * utf8
 */
#define UTF8_LENGTH(Char)              \
  ((Char) < 0x80 ? 1 :                 \
   ((Char) < 0x800 ? 2 :               \
    ((Char) < 0x10000 ? 3 :            \
     ((Char) < 0x200000 ? 4 :          \
      ((Char) < 0x4000000 ? 5 : 6)))))

#define UNICODE_VALID(Char)                   \
    ((Char) < 0x110000 &&                     \
     (((Char) & 0xFFFFF800) != 0xD800))

#define CONT_BYTE_FAST(p) ((unsigned char)*p++ & 0x3f)

wuchar w_utf8_skip_data[256] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3,
		3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 1, 1 };
wuchar w_utf8_mask[256] = { 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f,
		0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f,
		0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f,
		0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f,
		0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f,
		0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f,
		0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f,
		0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f,
		0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f,
		0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f,
		0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f,
		0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f,
		0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f,
		0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f,
		0x0f, 0x0f, 0x0f, 0x0f, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
		0x03, 0x03, 0x03, 0x03, 0x01, 0x01, 0x00, 0x00 };
wuchar w_utf8_bits_first[32] = { 0, 0, 0, 0, 0, 0, 0, 0xc0, 0xc0, 0xc0, 0xc0,
		0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf8, 0xf8,
		0xf8, 0xf8, 0xf8, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc };
wuchar w_utf8_bits_length[32] = { 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3,
		3, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6 };
wunichar w_utf8_get_unichar(const char *p, wintptr max_len) {
	if (max_len <= 0)
		return 0;
	int i, mask, len;
	unsigned char c = (unsigned char) *p;
	len = WMIN((size_t )w_utf8_skip_data[c], max_len);
	mask = w_utf8_mask[c];
	wunichar result = p[0] & mask;
	for (int i = 1; i < len; i++) {
		if ((p[i] & 0xc0) != 0x80) {
			result = -1;
			break;
		}
		result <<= 6;
		result |= (p[i] & 0x3f);
	}
	return result;
}
wunichar w_utf16_get_unichar(const wchar_t *p, wintptr max_len) {
	if (max_len <= 0)
		return 0;
	wchar_t ch0 = p[0];
	if (ch0 < 0xD800) // [0x0000‥0xD7FF]
		return ch0;
	if (ch0 < 0xDC00) { // [0xD800‥0xDBFF] [0xDC00‥0xDFFF]
		if (max_len <= 1)
			return 0;
		wchar_t ch1 = p[1];
		if (ch1 >> 10 != 0x37) {
			return -1;
		}
		return (ch0 << 10) + ch1 - 0x35FDC00;
	}
	if (ch0 < 0xE000)
		return -1;
	// [0xE000‥0xFFFF]
	return ch0;
}
const wchar_t*
w_utf16_next(const wchar_t *p) {
	wchar_t ch0 = p[0];
	if (ch0 < 0xD800) // [0x0000‥0xD7FF]
		return p + 1;
	if (ch0 < 0xDC00) { // [0xD800‥0xDBFF] [0xDC00‥0xDFFF]
		return p + 2;
	}
	return p + 1;
}
const char*
w_utf8_find_prev(const char *str, const char *p) {
	for (--p; p >= str; --p) {
		if ((*p & 0xc0) != 0x80)
			return p;
	}
	return NULL;
}
const char*
w_utf8_find_next(const char *p, const char *end) {
	if (end) {
		for (++p; p < end && (*p & 0xc0) == 0x80; ++p)
			;
		return (p >= end) ? NULL : p;
	} else {
		for (++p; (*p & 0xc0) == 0x80; ++p)
			;
		return p;
	}
}
const char*
w_utf8_prev(const char *p) {
	while (1) {
		p--;
		if ((*p & 0xc0) != 0x80)
			return p;
	}
}
const char*
w_utf8_next(const char *p) {
	return p + w_utf8_skip_data[(unsigned char) *p];
}
size_t w_utf8_strlen(const char *p, size_t max) {
	size_t len = 0;
	const char *start = p;
	if (max < 0) {
		while (*p) {
			p = w_utf8_next(p);
			++len;
		}
	} else {
		if (max == 0 || !*p)
			return 0;

		p = w_utf8_next(p);

		while (p - start < max && *p) {
			++len;
			p = w_utf8_next(p);
		}

		/* only do the last len increment if we got a complete
		 * char (don't count partial chars)
		 */
		if (p - start <= max)
			++len;
	}
	return len;
}
char*
w_utf8_offset_to_pointer(const char *str, int offset) {
	const char *s = str;

	if (offset > 0)
		while (offset--)
			s = w_utf8_next(s);
	else {
		const char *s1;

		/* This nice technique for fast backwards stepping
		 * through a UTF-8 string was dubbed "stutter stepping"
		 * by its inventor, Larry Ewing.
		 */
		while (offset) {
			s1 = s;
			s += offset;
			while ((*s & 0xc0) == 0x80)
				s--;

			offset += w_utf8_pointer_to_offset(s, s1);
		}
	}

	return (char*) s;
}
int w_utf8_pointer_to_offset(const char *str, const char *pos) {
	const char *s = str;
	int offset = 0;
	if (pos < str)
		offset = -w_utf8_pointer_to_offset(pos, str);
	else
		while (s < pos) {
			s = w_utf8_next(s);
			offset++;
		}
	return offset;
}
char*
w_utf8_strncpy(char *dest, const char *src, size_t n) {
	const char *s = src;
	while (n && *s) {
		s = w_utf8_next(s);
		n--;
	}
	strncpy(dest, src, s - src);
	dest[s - src] = 0;
	return dest;
}
size_t w_utf8_unichar_length(wunichar c) {
#if defined(__GNUC__) || defined(_MSC_VER)
	int _i;
	if (c == 0) {
		_i = 0;
	} else {
#ifdef __GNUC__
		_i = __builtin_ctz(c);
#else
		_BitScanForward(&_i,c);
#endif
	}
	return w_utf8_bits_length[_i];
#else
	return UTF8_LENGTH(c);
#endif
}
size_t w_utf8_add_unichar(char *outbuf, size_t sz, wunichar c) {
	int len, first;
#if defined(__GNUC__) || defined(_MSC_VER)
	int _i;
	if (c == 0) {
		_i = 0;
	} else {
#ifdef __GNUC__
		_i = __builtin_ctz(c);
#else
		_BitScanForward(&_i,c);
#endif
	}
	len = w_utf8_bits_length[_i];
	if (sz == 0)
		return len;
	first = w_utf8_bits_first[_i];
#else
	if (c < 0x80) {
		first = 0;
		len = 1;
	} else if (c < 0x800) {
		first = 0xc0;
		len = 2;
	} else if (c < 0x10000) {
		first = 0xe0;
		len = 3;
	} else if (c < 0x200000) {
		first = 0xf0;
		len = 4;
	} else if (c < 0x4000000) {
		first = 0xf8;
		len = 5;
	} else {
		first = 0xfc;
		len = 6;
	}
#endif
	if (sz >= len) {
		int i;
		for (i = len - 1; i > 0; --i) {
			outbuf[i] = (c & 0x3f) | 0x80;
			c >>= 6;
		}
		outbuf[0] = c | first;
	} else {
		int i;
		for (i = len - 1; i > 0; --i) {
			if (i < sz)
				outbuf[i] = (c & 0x3f) | 0x80;
			c >>= 6;
		}
		outbuf[0] = c | first;
	}
	return len;
}
char*
w_utf8_strchr(const char *p, int c) {
	if (c < 0x80) {
		return strchr(p, c);
	} else {
		char ch[10];
		int charlen = w_utf8_add_unichar(ch, sizeof(ch), c);
		ch[charlen] = '\0';
		return strstr(p, ch);
	}
}

int w_utf8_from_ucs4(const wunichar *str, size_t len, char *result, size_t n) {
	size_t i = 0;
	if (result == 0) {
		int _n = 0;
		while (1) {
			wunichar c = str[i];
			if (c == 0 || i >= len)
				break;
			_n += w_utf8_unichar_length(c);
			i++;
		}
		return _n;
	} else {
		int _n = 0;
		while (1) {
			wunichar c = str[i];
			if (c == 0 || i >= len)
				break;
			_n += w_utf8_add_unichar(&result[_n], n - _n, c);
			i++;
		}
		if (_n < n) {
			result[_n] = 0;
		}
		return _n;
	}

}
int w_utf8_to_ucs4(const char *str, size_t len, wunichar *result, size_t n) {
	const char *s = str, *end = str + len;
	size_t i = 0, l;
	wunichar wc;
	if (result == 0) {
		while (1) {
			l = len >= 0 ? end - s : 6;
			wc = w_utf8_get_unichar(s, l);
			if (wc == 0)
				break;
			if (wc < 0x10000) {
				i += 1;
			} else {
				i += 2;
			}
			s = w_utf8_next(s);
		}
		return i;
	} else {
		while (1) {
			l = len >= 0 ? end - s : 6;
			wc = w_utf8_get_unichar(s, l);
			if (wc == 0)
				break;
			result[i++] = wc;
			if (i >= n)
				break;
			s = w_utf8_next(s);
		}
		if (n > i)
			result[i] = 0;
		return i;
	}
}
#define SURROGATE_VALUE(h,l) (((h) - 0xd800) * 0x400 + (l) - 0xdc00 + 0x10000)
int w_utf8_from_utf16(const wchar *str, size_t len, char *result, size_t n) {
	/* This function and g_utf16_to_ucs4 are almost exactly identical -
	 * The lines that differ are marked.
	 */
	const wchar *in;
	char *out;
	int n_bytes;
	wunichar high_surrogate;

	n_bytes = 0;
	in = str;
	high_surrogate = 0;
	if (result == 0) {
		while ((len < 0 || in - str < len) && *in) {
			wchar c = *in;
			wuint wc;
			if (c >= 0xdc00 && c < 0xe000) /* low surrogate */
			{
				wc = SURROGATE_VALUE(high_surrogate, c);
				high_surrogate = 0;
			} else {
				if (c >= 0xd800 && c < 0xdc00) /* high surrogate */
				{
					high_surrogate = c;
					goto next1;
				} else
					wc = c;
			}

			/********** DIFFERENT for UTF8/UCS4 **********/
			n_bytes += UTF8_LENGTH(wc);

			next1: in++;
		}
		return n_bytes;
	}

	/* At this point, everything is valid, and we just need to convert
	 */
	/********** DIFFERENT for UTF8/UCS4 **********/

	high_surrogate = 0;
	out = result;
	const char *end = result + n;
	in = str;
	while (out < end) {
		wchar c = *in;
		wunichar wc;

		if (c >= 0xdc00 && c < 0xe000) /* low surrogate */
		{
			wc = SURROGATE_VALUE(high_surrogate, c);
			high_surrogate = 0;
		} else if (c >= 0xd800 && c < 0xdc00) /* high surrogate */
		{
			high_surrogate = c;
			goto next2;
		} else
			wc = c;

		/********** DIFFERENT for UTF8/UCS4 **********/
		out += w_utf8_add_unichar(out, end - out, wc);

		next2: in++;
	}

	/********** DIFFERENT for UTF8/UCS4 **********/
	result[n - 1] = '\0';

	return (int) (in - str);
}
int w_utf8_to_utf16(const char *str, size_t len, wchar *result, size_t n) {
	const char *s = str, *end = str + len;
	int i = 0, l;
	wunichar wc;
	if (result == 0) {
		while (1) {
			l = len >= 0 ? end - s : 6;
			wc = w_utf8_get_unichar(s, l);
			if (wc == 0)
				break;
			if (wc < 0x10000) {
				i += 1;
			} else {
				i += 2;
			}
			s = w_utf8_next(s);
		}
		return i;
	} else {
		int _n = n - 1;
		while (1) {
			l = len >= 0 ? end - s : 6;
			wc = w_utf8_get_unichar(s, l);
			if (wc == 0)
				break;
			if (wc < 0x10000) {
				if (i >= n)
					break;
				result[i++] = wc;
			} else {
				if (i >= _n)
					break;
				result[i++] = (wc - 0x10000) / 0x400 + 0xd800;
				result[i++] = (wc - 0x10000) % 0x400 + 0xdc00;
			}
			s = w_utf8_next(s);
		}
		if (n > i)
			result[i] = 0;
		return i;
	}
}
char*
w_strndup(const char *s, size_t n) {
	char *result;
	size_t len = strlen(s);

	if (n < len)
		len = n;

	result = (char*) malloc(len + 1);
	if (!result)
		return 0;

	result[len] = '\0';
	return (char*) memcpy(result, s, len);
}
