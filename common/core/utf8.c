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

#define UTF8_COMPUTE(Char, Mask, Len)					      \
  if (Char < 128)							      \
    {									      \
      Len = 1;								      \
      Mask = 0x7f;							      \
    }									      \
  else if ((Char & 0xe0) == 0xc0)					      \
    {									      \
      Len = 2;								      \
      Mask = 0x1f;							      \
    }									      \
  else if ((Char & 0xf0) == 0xe0)					      \
    {									      \
      Len = 3;								      \
      Mask = 0x0f;							      \
    }									      \
  else if ((Char & 0xf8) == 0xf0)					      \
    {									      \
      Len = 4;								      \
      Mask = 0x07;							      \
    }									      \
  else if ((Char & 0xfc) == 0xf8)					      \
    {									      \
      Len = 5;								      \
      Mask = 0x03;							      \
    }									      \
  else if ((Char & 0xfe) == 0xfc)					      \
    {									      \
      Len = 6;								      \
      Mask = 0x01;							      \
    }									      \
  else									      \
    Len = 1;

#define UTF8_LENGTH(Char)              \
  ((Char) < 0x80 ? 1 :                 \
   ((Char) < 0x800 ? 2 :               \
    ((Char) < 0x10000 ? 3 :            \
     ((Char) < 0x200000 ? 4 :          \
      ((Char) < 0x4000000 ? 5 : 6)))))

#define UTF8_GET(Result, Chars, Count, Mask, Len)			      \
  (Result) = (Chars)[0] & (Mask);					      \
  for ((Count) = 1; (Count) < (Len); ++(Count))				      \
    {									      \
      if (((Chars)[(Count)] & 0xc0) != 0x80)				      \
	{								      \
	  (Result) = -1;						      \
	  break;							      \
	}								      \
      (Result) <<= 6;							      \
      (Result) |= ((Chars)[(Count)] & 0x3f);				      \
    }

#define UNICODE_VALID(Char)                   \
    ((Char) < 0x110000 &&                     \
     (((Char) & 0xFFFFF800) != 0xD800))

#define CONT_BYTE_FAST(p) ((unsigned char)*p++ & 0x3f)

const char w_utf8_skip_data[256] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3,
		3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6,
		1, 1 };
void w_utf8_iter_init(w_utf8_iter *iter, const char *p) {
	iter->p = p;
	iter->flags = 0;
}
wunichar w_utf8_iter_next_utf32(w_utf8_iter *iter) {
	/*int i, mask = 0, len;
	 int result;
	 unsigned char c = (unsigned char) *p;
	 UTF8_COMPUTE(c, mask, len);
	 UTF8_GET(result, p, i, mask, len);
	 return result;*/
}
wchar w_utf8_iter_next_utf16(w_utf8_iter *iter) {

}
wunichar w_utf8_get_unichar(const char *p) {
	int i, mask = 0, len;
	int result;
	unsigned char c = (unsigned char) *p;
	UTF8_COMPUTE(c, mask, len);
	UTF8_GET(result, p, i, mask, len);
	return result;
}
wunichar w_utf8_get_next_char(const char *p, const char **next) {
	int i, mask = 0, len;
	int result;
	unsigned char c = (unsigned char) *p;
	UTF8_COMPUTE(c, mask, len);
	UTF8_GET(result, p, i, mask, len);
	*next = p + len;
	return result;
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
w_utf8_strncpy(char *dest, const char *src, int n) {
	const char *s = src;
	while (n && *s) {
		s = w_utf8_next(s);
		n--;
	}
	strncpy(dest, src, s - src);
	dest[s - src] = 0;
	return dest;
}
int w_utf8_from_unichar(int c, char *outbuf) {
	int len = 0;
	int first;
	int i;
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
	if (outbuf) {
		for (i = len - 1; i > 0; --i) {
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
		int charlen = w_utf8_from_unichar(c, ch);
		ch[charlen] = '\0';
		return strstr(p, ch);
	}
}

int w_utf8_from_ucs4(const wuint *str, int len, char *result, int n) {
	int result_length;
	char *p;
	int i;
	if (result == 0) {
		result_length = 0;
		for (i = 0; len < 0 || i < len; i++) {
			if (!str[i])
				break;

			result_length += UTF8_LENGTH(str[i]);
		}
		return result_length;
	}

	p = result;

	i = 0;
	while (p < result + n) {
		p += w_utf8_from_unichar(str[i++], p);
	}

	*p = '\0';

	return (int) (p - result);

}
int w_utf8_to_ucs4(const char *str, int len, wunichar *result, int n) {
	/*	gunichar *result = NULL;
	 gint n_chars, i;
	 const gchar *in;

	 in = str;
	 n_chars = 0;
	 while ((len < 0 || str + len - in > 0) && *in) {
	 gunichar wc = g_utf8_get_char_extended(in,
	 len < 0 ? 6 : str + len - in);
	 if (wc & 0x80000000) {
	 if (wc == (gunichar) - 2) {
	 if (items_read)
	 break;
	 else
	 g_set_error_literal(error, G_CONVERT_ERROR,
	 G_CONVERT_ERROR_PARTIAL_INPUT,
	 _("Partial character sequence at end of input"));
	 } else
	 g_set_error_literal(error, G_CONVERT_ERROR,
	 G_CONVERT_ERROR_ILLEGAL_SEQUENCE,
	 _("Invalid byte sequence in conversion input"));

	 goto err_out;
	 }

	 n_chars++;

	 in = g_utf8_next_char(in);
	 }

	 result = try_malloc_n(n_chars + 1, sizeof(gunichar), error);
	 if (result == NULL)
	 goto err_out;

	 in = str;
	 for (i = 0; i < n_chars; i++) {
	 result[i] = g_utf8_get_char(in);
	 in = g_utf8_next_char(in);
	 }
	 result[i] = 0;

	 if (items_written)
	 *items_written = n_chars;

	 err_out: if (items_read)
	 *items_read = in - str;

	 return result;
	 */
}
wunichar w_utf8_get_char_extended(const char *p, int max_len) {
	int i, len;
	wunichar min_code;
	wunichar wc = (wuchar) *p;

	const wunichar partial_sequence = (wunichar) -2;
	const wunichar malformed_sequence = (wunichar) -1;

	if (wc < 0x80) {
		return wc;
	} else if (wc < 0xc0) {
		return malformed_sequence;
	} else if (wc < 0xe0) {
		len = 2;
		wc &= 0x1f;
		min_code = 1 << 7;
	} else if (wc < 0xf0) {
		len = 3;
		wc &= 0x0f;
		min_code = 1 << 11;
	} else if (wc < 0xf8) {
		len = 4;
		wc &= 0x07;
		min_code = 1 << 16;
	} else if (wc < 0xfc) {
		len = 5;
		wc &= 0x03;
		min_code = 1 << 21;
	} else if (wc < 0xfe) {
		len = 6;
		wc &= 0x01;
		min_code = 1 << 26;
	} else {
		return malformed_sequence;
	}

	if (max_len >= 0 && len > max_len) {
		for (i = 1; i < max_len; i++) {
			if ((((wuchar*) p)[i] & 0xc0) != 0x80)
				return malformed_sequence;
		}
		return partial_sequence;
	}

	for (i = 1; i < len; ++i) {
		wunichar ch = ((wuchar*) p)[i];

		if ((ch & 0xc0) != 0x80) {
			if (ch)
				return malformed_sequence;
			else
				return partial_sequence;
		}

		wc <<= 6;
		wc |= (ch & 0x3f);
	}

	if (wc < min_code)
		return malformed_sequence;

	return wc;
}
#define SURROGATE_VALUE(h,l) (((h) - 0xd800) * 0x400 + (l) - 0xdc00 + 0x10000)
int w_utf8_from_utf16(const wchar *str, int len, char *result, int n) {
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
	in = str;
	while (out < result + n) {
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
		out += w_utf8_from_unichar(wc, out);

		next2: in++;
	}

	/********** DIFFERENT for UTF8/UCS4 **********/
	*out = '\0';

	return (int) (in - str);
}
int w_utf8_to_utf16(const char *str, int len, wchar *result, int n) {
	int n16;
	const char *in;
	int i, m;
	in = str;
	n16 = 0;
	if (result == 0) {
		while ((len < 0 || (m = str + len - in) > 0) && *in) {
			wunichar wc = w_utf8_get_char_extended(in, len < 0 ? 6 : m);
			if (wc < 0x10000) {
				n16 += 1;
			} else {
				n16 += 2;
			}
			in = w_utf8_next(in);
		}
		return n16;
	}
	in = str;
	for (i = 0; i < n;) {
		wunichar wc;
		if (len < 0) {
			if (*in == 0)
				break;
			wc = w_utf8_get_char_extended(in, 6);
		} else {
			m = str + len - in;
			if (m < 0)
				break;
			wc = w_utf8_get_char_extended(in, m);
		}
		if (wc < 0x10000) {
			result[i++] = wc;
		} else {
			result[i++] = (wc - 0x10000) / 0x400 + 0xd800;
			result[i++] = (wc - 0x10000) % 0x400 + 0xdc00;
		}
		in = w_utf8_next(in);
	}
	if (n > i)
		result[i] = 0;
	return i;
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
