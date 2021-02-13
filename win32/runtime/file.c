/*
 * Name:        file.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "core.h"
#include "../widgets/toolkit.h"
FILE* w_fopen(const char *file, const char *mode) {
	int wfile_length;
	int wmode_length;
	int alloc;
	WCHAR *wfile, *wmode;
	FILE *f;
	wfile_length = w_utf8_to_utf16(file, -1, 0, 0);
	wmode_length = w_utf8_to_utf16(mode, -1, 0, 0);
	alloc = wfile_length + wmode_length + 2;
	if (alloc > 0x200) {
		alloc = 0;
	}
	wmode = _w_toolkit_malloc(alloc);
	if (wmode == 0) return 0;
	wfile = &wmode[wmode_length + 1];
	w_utf8_to_utf16(file, -1, wfile, wfile_length + 1);
	w_utf8_to_utf16(mode, -1, wmode, wmode_length);
	f = _wfopen(wfile, wmode);
	_w_toolkit_free(wmode, alloc);
	return f;
}
struct w_iterator_dir_mem {
	HANDLE handle;
	char *file;
	WCHAR *dir;
	WIN32_FIND_DATAW data;
	unsigned has_next :1;
	size_t count;
};
typedef struct w_iterator_dir {
	struct _w_iterator_class *clazz;
	struct w_iterator_dir_mem *mem;
} w_iterator_dir;
wresult _w_iterator_dir_next(w_iterator *it, void *obj) {
	struct w_iterator_dir_mem *mem = ((w_iterator_dir*) it)->mem;
	w_finddir *find = (w_finddir*) obj;
	if (mem->has_next) {
		w_utf8_from_utf16(mem->data.cFileName, -1, mem->file, MAX_PATH * 3);
		find->name = mem->file;
		find->attribute = mem->data.dwFileAttributes;
		find->creationTime = /*(mem->data.ftCreationTime.dwHighDateTime << 32)
		 +*/mem->data.ftCreationTime.dwLowDateTime;
		find->filesize = /* (mem->data.nFileSizeHigh << 32)
		 +*/mem->data.nFileSizeLow;
		if (!FindNextFileW(mem->handle, &mem->data)) {
			mem->has_next = 0;
		}
		return W_TRUE;
	} else {
		return W_FALSE;
	}
}
wresult _w_iterator_dir_reset(w_iterator *it) {
	struct w_iterator_dir_mem *mem = ((w_iterator_dir*) it)->mem;
	FindClose(mem->handle);
	mem->handle = FindFirstFileW(mem->dir, &mem->data);
	if (mem->handle == INVALID_HANDLE_VALUE) {
		w_iterator_close(it);
		return W_ERROR_NO_HANDLES;
	}
	return W_TRUE;
}
wresult _w_iterator_dir_close(w_iterator *it) {
	struct w_iterator_dir_mem *mem = ((w_iterator_dir*) it)->mem;
	FindClose(mem->handle);
	free(mem);
	return W_TRUE;
}
wresult _w_iterator_dir_remove(w_iterator *it) {
	return W_FALSE;
}
size_t _w_iterator_dir_get_count(w_iterator *it) {
	struct w_iterator_dir_mem *mem = ((w_iterator_dir*) it)->mem;
	size_t count;
	if (mem->count == -1) {
		WIN32_FIND_DATAW data;
		HANDLE handle = FindFirstFileW(mem->dir, &data);
		if (handle == INVALID_HANDLE_VALUE) {
			return 0;
		}
		count = 0;
		while (FindNextFileW(handle, &data)) {
			count++;
		}
		FindClose(handle);
		mem->count = count;
	}
	return mem->count;
}
_w_iterator_class _w_iterator_dir_class = { _w_iterator_dir_close,
		_w_iterator_dir_next, _w_iterator_dir_reset, _w_iterator_dir_remove,
		_w_iterator_dir_get_count };
wresult w_dir_list(const char *dir, w_iterator *files) {
	w_iterator_close(files);
	if (dir == 0)
		return W_ERROR_INVALID_ARGUMENT;
	int l = w_utf8_to_utf16(dir, -1, 0, 0);
	struct w_iterator_dir_mem *mem = malloc(
			sizeof(struct w_iterator_dir_mem) + (l + 5) * sizeof(WCHAR)
					+ MAX_PATH * 3);
	if (mem == 0) {
		return W_ERROR_NO_MEMORY;
	}
	mem->file = &((char*) mem)[sizeof(struct w_iterator_dir_mem)];
	mem->dir = (WCHAR*) &((char*) mem)[sizeof(struct w_iterator_dir_mem)
			+ MAX_PATH * 3];
	w_utf8_to_utf16(dir, -1, mem->dir, l + 1);
	WCHAR *_d = mem->dir;
	while (*_d != 0) {
		if (*_d == '/')
			*_d = '\\';
		_d++;
	}
	if (mem->dir[l] != '\\') {
		mem->dir[l] = '\\';
	}
	mem->dir[l + 1] = '*';
	mem->dir[l + 2] = 0;
	mem->handle = FindFirstFileW(mem->dir, &mem->data);
	if (mem->handle == INVALID_HANDLE_VALUE) {
		free(mem);
		return W_ERROR_NO_HANDLES;
	}
	mem->count = -1;
	mem->has_next = 1;
	((w_iterator_dir*) files)->mem = mem;
	((w_iterator_dir*) files)->clazz = &_w_iterator_dir_class;
	return W_TRUE;
}
