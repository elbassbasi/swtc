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
		} else {
			data.Init(W_GRID_FILL_HORIZONTAL);
			data.horizontalSpan = layout.numColumns;
		}
		slider[i].SetLayoutData(data);
		slider[i].SetMaximum(100);
		slider[i].SetId(i);
	}
	label.Create(this, W_NONE);
	data.Init(W_GRID_FILL_VERTICAL);
	label.SetLayoutData(data);
	SetLabelText(&slider[0]);
}

void TSliderDemo::SetLabelText(WSlider *slider) {
	char txt[30];
	int maximum = slider->GetMaximum();
	int selection = slider->GetSelection();
	snprintf(txt, sizeof(txt), "slider %d : %d/%d", slider->GetId(),
			selection, maximum);
	label.SetText(txt);
}

bool TSliderDemo::OnNotify(WEvent &e) {
	if (WSlider::IsSlider(e.widget)) {
		WSlider *slider = static_cast<WSlider*>(e.widget);
		SetLabelText(slider);
		return true;
	}
	return false;
}
