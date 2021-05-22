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
	this->layout.numColumns = 3;
	this->SetLayout(this->layout);
	WGridData data;
	for (int i = 0; i < length; i++) {
		wuint64 st = styles[i], j;
		slider[i].Create(this, st);
		if (st & W_VERTICAL) {
			data.Init(W_GRID_FILL_VERTICAL);
			data.verticalSpan = length;
		} else {
			data.Init(W_GRID_FILL_HORIZONTAL);
			data.horizontalSpan = layout.numColumns;
		}
		slider[i].SetLayoutData(data);
		slider[i].SetMaximum(100);
		slider[i].SetId(i);
	}
	for (int i = 0; i < length; i++) {
		label[i].Create(this, W_NONE);
		data.Init(W_GRID_FILL_BOTH);
		label[i].SetLayoutData(data);
		SetLabelText(i);
	}
}

void TSliderDemo::SetLabelText(wuint index) {
	if (index >= length)
		return;
	char txt[30];
	WSlider *slider = &this->slider[index];
	int maximum = slider->GetMaximum();
	int selection = slider->GetSelection();
	snprintf(txt, sizeof(txt), "slider %d : %d/%d", index, selection, maximum);
	label[index].SetText(txt);
}

bool TSliderDemo::OnNotifySelection(WEvent &e) {
	if (WSlider::IsSlider(e.widget)) {
		WSlider *slider = static_cast<WSlider*>(e.widget);
		SetLabelText(slider->GetId());
		return true;
	}
	return false;
}
