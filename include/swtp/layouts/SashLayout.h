/*
 * SashLayout.h
 *
 *  Created on: 8 mai 2020
 *      Author: azeddine
 */

#ifndef SWTP_LAYOUTS_SASHLAYOUT_H_
#define SWTP_LAYOUTS_SASHLAYOUT_H_
#include "Layout.h"
class WSash;
class WSashLayoutData {
private:
	wushort size;
	union {
		wushort flags;
	};
public:
	size_t weight;
};
class SWTP_PUBLIC WSashLayoutBase: public WLayout {
protected:
	WSash *sashes;
	wuint64 style;
	size_t count;
public:
	WSashLayoutBase() {
		Init(W_HORIZONTAL);
	}
	~WSashLayoutBase();
	void Init(int style) {
		w_layout_sash_init((w_layout_sash*) this, style);
	}
	void ComputeSize(WComposite *composite, WSize &result, int wHint, int hHint,
			bool flushCache);
	void DoLayout(WComposite *composite, bool flushCache);
	void SetSashes(WSash *sashes, size_t count) {
		w_layout_sash_set_sashes((w_layout_sash*) this, (w_sash*) sashes,
				count);
	}
	void SetWeights(WComposite *composite, int *weights, size_t count) {
		w_layout_sash_set_weights((w_layout_sash*) this, W_COMPOSITE(composite),
				weights, count);
	}
	void GetData(WControl *control, WLayoutData &data);
	void SetData(WControl *control, const WLayoutData &data);
};

template<size_t n = 0>
class WSashLayout: public WSashLayoutBase {
public:
	void Init(int style) {
		WSashLayoutBase::Init(style);
		SetSashes(sashes_tmp, n);
	}
	void InitVertical() {
		Init(W_VERTICAL);
	}
	void InitHorizental() {
		Init(W_HORIZONTAL);
	}
	WSashLayout() {
		InitHorizental();
	}
	WSashLayout(int style) {
		Init(style);
	}
	WSash sashes_tmp[n];
};
template<size_t n = 0>
class WSashLayoutHorizental: public WSashLayout<n> {
public:
	WSashLayoutHorizental() :
			WSashLayout<n>(W_HORIZONTAL) {
	}
};
template<size_t n = 0>
class WSashLayoutVertical: public WSashLayout<n> {
public:
	WSashLayoutVertical() :
			WSashLayout<n>(W_VERTICAL) {
	}
};

#endif /* SWTP_LAYOUTS_SASHLAYOUT_H_ */
