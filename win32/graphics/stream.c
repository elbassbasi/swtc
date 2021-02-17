/*
 * Name:        stream.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "gc.h"
#include "objidl.h"
/*** IUnknown methods ***/
HRESULT STDMETHODCALLTYPE swt_stream_QueryInterface(IStream *This,
REFIID riid, void **ppvObject) {
	if (ppvObject == 0)
		return E_NOINTERFACE;
	if (IsEqualGUID(riid, &IID_IStream)) {
		*ppvObject = This;
		return S_OK;
	}
	if (IsEqualGUID(riid, &IID_ISequentialStream)) {
		*ppvObject = This;
		return S_OK;
	}
	if (IsEqualGUID(riid, &IID_IUnknown)) {
		*ppvObject = This;
		return S_OK;
	}
	return E_NOINTERFACE;
}

ULONG STDMETHODCALLTYPE swt_stream_AddRef(IStream *This) {
	return 1;
}

ULONG STDMETHODCALLTYPE swt_stream_Release(IStream *This) {
	return 1;
}

/*** ISequentialStream methods ***/
HRESULT STDMETHODCALLTYPE swt_stream_Read(IStream *This, void *pv, ULONG cb,
		ULONG *pcbRead) {
	swt_stream *s = (swt_stream*) This;
	w_stream *stream = s->s;
	size_t _pcbRead = 0;
	wresult result = stream->clazz->read(stream, pv, cb, &_pcbRead);
	*pcbRead = _pcbRead;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE swt_stream_Write(IStream *This, const void *pv,
		ULONG cb, ULONG *pcbWritten) {
	swt_stream *s = (swt_stream*) This;
	w_stream *stream = s->s;
	size_t _pcbWritten = 0;
	wresult result = stream->clazz->write(stream, pv, cb, &_pcbWritten);
	*pcbWritten = _pcbWritten;
	return S_OK;
}

/*** IStream methods ***/
HRESULT STDMETHODCALLTYPE swt_stream_Seek(IStream *This, LARGE_INTEGER dlibMove,
		DWORD dwOrigin, ULARGE_INTEGER *plibNewPosition) {
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE swt_stream_SetSize(IStream *This,
		ULARGE_INTEGER libNewSize) {
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE swt_stream_CopyTo(IStream *This, IStream *pstm,
		ULARGE_INTEGER cb, ULARGE_INTEGER *pcbRead,
		ULARGE_INTEGER *pcbWritten) {
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE swt_stream_Commit(IStream *This,
		DWORD grfCommitFlags) {
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE swt_stream_Revert(IStream *This) {
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE swt_stream_LockRegion(IStream *This,
		ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType) {
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE swt_stream_UnlockRegion(IStream *This,
		ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType) {
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE swt_stream_Stat(IStream *This, STATSTG *pstatstg,
		DWORD grfStatFlag) {
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE swt_stream_Clone(IStream *This, IStream **ppstm) {
	return E_NOTIMPL;
}
IStreamVtbl swt_stream_vtbl = { swt_stream_QueryInterface, swt_stream_AddRef,
		swt_stream_Release, swt_stream_Read, swt_stream_Write, swt_stream_Seek,
		swt_stream_SetSize, swt_stream_CopyTo, swt_stream_Commit,
		swt_stream_Revert, swt_stream_LockRegion, swt_stream_UnlockRegion,
		swt_stream_Stat, swt_stream_Clone };
void swt_stream_init(swt_stream *stream, w_stream *s) {
	stream->lpVtbl = &swt_stream_vtbl;
	stream->s = s;
}
