/*
 * TButtonCircle.cpp
 *
 *  Created on: 3 avr. 2021
 *      Author: azeddine
 */

#include "TButtonCircle.h"
void TButtonCircle::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Circle", new TButtonCircle());
}
TButtonCircle::TButtonCircle() {
}
TButtonCircle::~TButtonCircle() {
}

void TButtonCircle::CreateCircleRegion(WRegion &region, int r, int offsetX,
		int offsetY) {
	const int count = 8 * r + 4;
	int *polygon = new int[8 * r + 4];
	// x^2 + y^2 = r^2
	for (int i = 0; i < 2 * r + 1; i++) {
		int x = i - r;
		int y = (int) sqrt(r * r - x * x);
		polygon[2 * i] = offsetX + x;
		polygon[2 * i + 1] = offsetY + y;
		polygon[8 * r - 2 * i - 2] = offsetX + x;
		polygon[8 * r - 2 * i - 1] = offsetY - y;
	}
	region.Create();
	region.Add((WPoint*) polygon, count / 2);
	delete[] polygon;
}
void TButtonCircle::CreateControl(WComposite *parent) {
	char txt[30];
	this->Create(parent, W_FREE_MEMORY);
	//this->layout.numColumns = 2;
	this->SetLayout(this->layout);
	WRegion r;
	CreateCircleRegion(r, 67, 87, 77);
	for (size_t i = 0; i < length; i++) {
		btn[i].Create(this, W_PUSH);
		sprintf(txt, "button %d", (int) i + 1);
		btn[i].SetText(txt);
		btn[i].SetRegion(r);
	}
}

bool TButtonCircle::OnNotifySelection(WEvent &e) {
	if (WButton::IsButton(e.widget)) {
		WButton *b = static_cast<WButton*>(e.widget);
		WString text = b->GetText();
		WMessageBox(GetFrame(), W_OK, "button selected",
				text.GetCharsNotNull());
		return true;
	}
	return false;
}
