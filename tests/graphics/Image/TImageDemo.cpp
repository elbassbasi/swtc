/*
 * TImageDemo.cpp
 *
 *  Created on: 15 févr. 2021
 *      Author: azeddine
 */

#include "TImageDemo.h"
#include "../../main/MApp.h"
class _WFileStream: public IWStream {
public:
	FILE *file;
	bool Open(const char *file, const char *mode) {
		this->file = w_fopen(file, mode);
		return this->file != 0;
	}
	WResult Close() {
		if (this->file != 0) {
			fclose(this->file);
			return true;
		} else
			return false;
	}
	WResult Read(void *pv, size_t cb, size_t *pcbRead) {
		if (this->file != 0) {
			*pcbRead = fread(pv, 1, cb, this->file);
			return true;
		} else
			return false;
	}
	WResult Write(const void *pv, size_t cb, size_t *pcbWritten) {
		if (this->file != 0) {
			*pcbWritten = fwrite(pv, 1, cb, this->file);
			return true;
		} else
			return false;
	}
	WResult Seek(wuint64 dlibMove, int dwOrigin) {
		if (this->file != 0) {
			fseek(file, dlibMove, dwOrigin);
			return true;
		} else
			return false;
	}
	WResult Tell(wuint64 *pos) {
		if (this->file != 0) {
			*pos = ftell(file);
			return true;
		} else
			return false;
	}
	WResult Rewind() {
		if (this->file != 0) {
			rewind(file);
			return true;
		} else
			return false;
	}
	WResult SetSize(wuint64 libNewSize) {

	}
	WResult GetSize(wuint64 *libNewSize) {
		if (this->file != 0) {
			size_t current = ftell(this->file);
			fseek(this->file, 0L, SEEK_END);
			*libNewSize = ftell(this->file);
			fseek(this->file, current, SEEK_SET);
			return true;
		} else {
			*libNewSize = 0;
			return false;
		}
	}

};
void TImageDemo::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Demo", new TImageDemo());
}
TImageDemo::TImageDemo() {
}

TImageDemo::~TImageDemo() {
}

void TImageDemo::CreateControl(WComposite *parent) {
	CanvasTreeItem::CreateControl(parent);
	_WFileStream stream;
	WString _path = MApp::Get()->GetExecutablePath();
	int l = strlen(_path);
	char *path = (char*) malloc(_path.GetLength() + 30);
	if (path != 0) {
		sprintf(path, "%s/images/file.png", _path.GetCharsNotNull());
		stream.Open(path, "rb");
		free(path);
		streamImage.Create(&stream);
	}
}

bool TImageDemo::OnPaint(WPaintEvent &e) {
	MFrame *shell = (MFrame*) GetFrame();
	WImage *image = shell->GetImage32(IMAGELIST_ADD);
	e->DrawImage(*image, 0, 0);
	e->DrawImage(streamImage, 100, 100);
	return true;
}
