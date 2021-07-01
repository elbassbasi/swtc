/*
 * ExpandBarDemo.cpp
 *
 *  Created on: 13 mai 2020
 *      Author: Azeddine El Bassbasi
 */

#include "TExpandBarDemo.h"

void TExpandBarDemo::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Demo", new TExpandBarDemo());
}

WControl* TExpandBarDemo::GetControl(WComposite *parent) {
	if (!this->IsOk()) {
		this->CreateControl(parent);
	}
	return this;
}
const char *TExpandBarDemo::names[length] = { //
		"What is your favorite button", //
				"What is your favorite icon", //
				"What is your favorite range widget" //
		};
void TExpandBarDemo::CreateControl(WComposite *parent) {
	WExpandItem item;
	WSize size;
	this->Create(parent, W_VSCROLL);
	WFont *font = GetFont();
	WFontData fontData;
	font->GetFontData(fontData);
	fontData.SetBold();
	fontData.SetHeight(fontData.GetHeight() - 4);
	this->boldFont.Create(fontData);
	this->SetFont(boldFont);
	MFrame *frame = (MFrame*) GetFrame();
	this->SetImageList(frame->GetImageList16());
	for (int i = 0; i < length; i++) {
		composite[i].Create(this, W_NONE);
		composite[i].SetLayout(layout[i]);
		CreateWidgets(i);
		this->AppendItem(item, names[i]);
		item.SetImage(i % length);
		composite[i].ComputeSize(size, W_DEFAULT, W_DEFAULT);
		item.SetControl(&composite[i]);
		item.SetHeight(size.height);
		item.SetExpanded(true);
	}
}

void TExpandBarDemo::CreateWidgets(int index) {
	switch (index) {
	case 0:
		Create_0(&this->composite[index]);
		break;
	case 1:
		Create_1(&this->composite[index]);
		break;
	case 2:
		Create_2(&this->composite[index]);
		break;
	}
}
int TExpandBarDemo::styles_0[length_0] = { W_PUSH, W_RADIO, W_CHECK, W_TOGGLE };
const char *TExpandBarDemo::names_0[length_0] = { "PUSH", "RADIO", "CHECK",
		"TOGGLE" };
void TExpandBarDemo::Create_0(WComposite *composite) {
	layout[0].marginLeft = layout[0].marginTop = layout[0].marginRight =
			layout[0].marginBottom = 10;
	layout[0].verticalSpacing = 10;
	for (int i = 0; i < length_0; i++) {
		btn_0[i].Create(composite, styles_0[i]);
		btn_0[i].SetText(names_0[i]);
	}
}
int TExpandBarDemo::images_1[length_1] = { W_ICON_ERROR, W_ICON_INFORMATION,
		W_ICON_WARNING, W_ICON_QUESTION };
const char *TExpandBarDemo::names_1[length_1] = { "SWT.ICON_ERROR",
		"SWT.ICON_INFORMATION", "SWT.ICON_WARNING", "SWT.ICON_QUESTION" };
void TExpandBarDemo::Create_1(WComposite *composite) {
	WImage *image;
	layout[1].numColumns = 2;
	layout[1].marginLeft = layout[1].marginTop = layout[1].marginRight =
			layout[1].marginBottom = 10;
	layout[1].verticalSpacing = 10;
	MFrame *frame = (MFrame*) GetFrame();
	for (int i = 0; i < length_1; i++) {
		label_1[2 * i].Create(composite, W_NONE);
		image = frame->GetImage16(images_1[i]);
		label_1[2 * i].SetImage(image);
		label_1[2 * i + 1].Create(composite, W_NONE);
		label_1[2 * i + 1].SetText(names_1[i]);
	}
}

void TExpandBarDemo::Create_2(WComposite *composite) {
	layout[2].numColumns = 2;
	layout[2].marginLeft = layout[2].marginTop = layout[2].marginRight =
			layout[2].marginBottom = 10;
	layout[2].verticalSpacing = 10;
	label_2[0].CreateLabel(composite, "Scale");
	scale_2.CreateScale(composite);
	label_2[1].CreateLabel(composite, "Spinner");
	spinner_2.Create(composite, W_BORDER);
	label_2[2].CreateLabel(composite, "Slider");
	slider_2.CreateSlider(composite);
}
