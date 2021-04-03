/*
 * TSliderDemo.cpp
 *
 *  Created on: 24 mai 2020
 *      Author: Azeddine El Bassbasi
 */

#include "TSliderDemo.h"

void TSliderDemo::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Demo", new TSliderDemo());
}
int TSliderDemo::styles[length] = { W_HORIZONTAL, W_SCALE | W_HORIZONTAL,
		W_VERTICAL, W_SCALE | W_VERTICAL };
void TSliderDemo::CreateControl(WComposite *parent) {
	this->Create(parent, W_NONE);
	this->layout.numColumns = 2;
	this->SetLayout(this->layout);
	WGridData data;
	for (int i = 0; i < length; i++) {
		wuint64 st = styles[i], j;
		slider[i].Create(this, st);
		if (st & W_VERTICAL) {
			data.Init(W_GRID_FILL_VERTICAL);
		} else {
			data.Init(W_GRID_FILL_HORIZONTAL);
			data.horizontalSpan = layout.numColumns;
		}
		slider[i].SetLayoutData(data);
	}
}
