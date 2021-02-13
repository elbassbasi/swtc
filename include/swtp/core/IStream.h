/*
 * IStream.h
 *
 *  Created on: 23 oct. 2020
 *      Author: azeddine
 */

#ifndef SWTP_CORE_ISTREAM_H_
#define SWTP_CORE_ISTREAM_H_
#include "Resource.h"
class IWBasicStream: public IDestruct {
public:
	virtual WResult Close()=0;
	virtual WResult Read(void *pv, size_t cb, size_t *pcbRead)=0;
	virtual WResult Write(const void *pv, size_t cb, size_t *pcbWritten)=0;
	virtual WResult Seek(wuint64 dlibMove, int dwOrigin)=0;
	virtual WResult Tell(wuint64 *pos)=0;
	virtual WResult Rewind()=0;
	virtual WResult SetSize(wuint64 libNewSize)=0;
	virtual WResult GetSize(wuint64 *libNewSize)=0;
public:
	size_t Read(void *pv, size_t cb) {
		size_t pcbRead = 0;
		this->Read(pv, cb, &pcbRead);
		return pcbRead;
	}
	size_t Write(const void *pv, size_t cb) {
		size_t pcbWritten = 0;
		this->Write(pv, cb, &pcbWritten);
		return pcbWritten;
	}

};
class IWStream: public IWBasicStream {
protected:
	struct _w_stream_class *clazz;
public:
	IWStream();
	~IWStream();
};

namespace swt {
typedef IWBasicStream IBasicStream;
typedef IWStream IStream;
}  // namespace swt

#endif /* SWTP_CORE_ISTREAM_H_ */
