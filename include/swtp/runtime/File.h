/*
 * WFile.h
 *
 *  Created on: 30 oct. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_RUNTIME_FILE_H_
#define SWTP_RUNTIME_FILE_H_
#include "../core/Resource.h"

class WFile: public WResource {
public:
	FILE *file;
	WFile() {
		this->file = 0;
	}
	~WFile() {
		Close();
	}
	bool Open(const char *file, const char *mode = "r") {
		return (this->file = w_fopen(file, mode)) != 0;
	}
	bool OpenForRead(const char *file) {
		return Open(file, "r");
	}
	bool OpenForWrite(const char *file) {
		return Open(file, "w");
	}
	bool OpenForAppend(const char *file) {
		return Open(file, "a");
	}
	void Close() {
		if (this->file) {
			fclose(this->file);
		}
	}
	// is file opened?
	bool IsOpened() const {
		return this->file != 0;
	}
	// is end of file reached?
	bool Eof() const {
		return feof(this->file);
	}
	size_t Read(void *data, size_t length) {
		return fread(data, 1, length, this->file);
	}
	size_t Write(const void *data, size_t length) {
		return fwrite(data, 1, length, this->file);
	}
	int Flush() {
		return fflush(this->file);
	}
	int SeekBegin(wint64 Offset) {
		return fseek(this->file, Offset, SEEK_SET);
	}
	int SeekBegin() {
		return SeekBegin(0);
	}
	int SeekEnd(wint64 Offset) {
		return fseek(this->file, Offset, SEEK_END);
	}
	int SeekEnd() {
		return SeekEnd(0);
	}
	int SeekCurrent(wint64 Offset) {
		return fseek(this->file, Offset, SEEK_CUR);
	}
	int Seek(wint64 Offset) {
		return SeekBegin(Offset);
	}
	wint64 Tell() const {
		return ftell(this->file);
	}
	wint64 GetSize() {
		wint64 pos = Tell();
		SeekEnd();
		wint64 size = Tell();
		SeekBegin(pos);
		return size;
	}
	int Printf(const char *format, ...) {
		va_list args;
		va_start(args, format);
		int ret = vfprintf(this->file, format, args);
		va_end(args);
		return ret;
	}
	int Printf(const char *format, va_list args) {
		return vfprintf(this->file, format, args);
	}

};
class WFindDir {
public:
	const char *name;
	int attribute;
	wint64 creationTime;
	wint64 filesize;
	bool IsDir() {
		return (this->attribute & W_ATTRIBUTE_DIR) != 0;
	}
};
inline bool WDirList(const char *dir, WIterator<WFindDir> &list) {
	return w_dir_list(dir, (w_iterator*) &list) > 0;
}

#endif /* SWTP_RUNTIME_FILE_H_ */
