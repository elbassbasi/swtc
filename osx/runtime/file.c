/*
 * file.c
 *
 *  Created on: 9 oct. 2019
 *      Author: azeddine El Bassbasi
 */
#include <swtc.h>
#include <dirent.h>
#include <stdio.h>
FILE* w_fopen(const char *file, const char *mode) {
	FILE *f = fopen(file, mode);
	return f;
}
typedef struct w_iterator_dir {
	w_basic_iterator base;
	DIR *dr;
	size_t count;
} w_iterator_dir;
wresult _w_iterator_dir_next(w_iterator *it, void *obj) {
	struct w_iterator_dir *mem = (w_iterator_dir*) it;
	struct dirent *de;
	w_finddir *find = (w_finddir*) obj;
	if ((de = readdir(mem->dr)) != 0) {
		find->name = de->d_name;
		find->attribute = 0;
		if (de->d_type == DT_DIR) {
			find->attribute |= W_ATTRIBUTE_DIR;
		}
		find->creationTime =0 /*de->d_off*/;
		return W_TRUE;
	} else {
		return W_FALSE;
	}
}
wresult _w_iterator_dir_reset(w_iterator *it) {
	struct w_iterator_dir *mem = (w_iterator_dir*) it;
	rewinddir(mem->dr);
	return W_TRUE;
}
wresult _w_iterator_dir_close(w_iterator *it) {
	struct w_iterator_dir *mem = (w_iterator_dir*) it;
	closedir(mem->dr);
	return W_TRUE;
}
wresult _w_iterator_dir_remove(w_iterator *it) {
	return W_ERROR_NOT_IMPLEMENTED;
}
size_t _w_iterator_dir_get_count(w_iterator *it) {
	struct w_iterator_dir *mem = (w_iterator_dir*) it;
	if (mem->count < 0) {
		long int pos = telldir(mem->dr);
		rewinddir(mem->dr);
		size_t count = 0;
		struct dirent *de;
		if ((de = readdir(mem->dr)) != 0) {
			count++;
		}
		seekdir(mem->dr, pos);
		mem->count = count;
	}
	return mem->count;
}
_w_iterator_class _w_iterator_dir_class = { //
		_w_iterator_dir_close, //
				_w_iterator_dir_next, //
				_w_iterator_dir_reset, //
				_w_iterator_dir_remove, //
				_w_iterator_dir_get_count //
		};
wresult w_dir_list(const char *dir, w_iterator *files) {
	w_iterator_close(files);
	if (dir == 0)
		return W_ERROR_INVALID_ARGUMENT;
	((w_iterator_dir*) files)->dr = opendir(dir);
	if (((w_iterator_dir*) files)->dr == 0) {
		return W_ERROR_NO_HANDLES;
	}
	((w_iterator_dir*) files)->count = -1;
	((w_iterator_dir*) files)->base.clazz = &_w_iterator_dir_class;
	return W_TRUE;
}
