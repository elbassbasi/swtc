/*
 * ExpandBarDemo.cpp
 *
 *  Created on: 13 mai 2020
 *      Author: Azeddine El Bassbasi
 */

#include "TExpandBarDemo.h"

void TExpandBarDemo::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Demonstrates", new TExpandBarDemo());
}

WControl* TExpandBarDemo::GetControl(WComposite *parent) {
	if (!this->IsOk()) {
		this->CreateControl(parent);
	}
	return this;
}

void TExpandBarDemo::CreateControl(WComposite *parent) {
	WExpandItem item;
	WSize size;
	WImage *image;
	WToolkit *toolkit;
	this->Create(parent, W_VSCROLL);
	toolkit = this->GetToolkit();
	// First item
	composite[0].Create(this, W_NONE);
	layout[0].marginLeft = layout[0].marginTop = layout[0].marginRight =
			layout[0].marginBottom = 10;
	layout[0].verticalSpacing = 10;
	composite[0].SetLayout(layout[0]);
	button[0].Create(composite[0], W_PUSH);
	button[0].SetText("PUSH");
	button[1].Create(composite[0], W_RADIO);
	button[1].SetText("RADIO");
	button[2].Create(composite[0], W_CHECK);
	button[2].SetText("CHECK");
	button[3].Create(composite[0], W_TOGGLE);
	button[3].SetText("TOGGLE");
	this->AppendItem(item, "What is your favorite button");
	composite[0].ComputeSize(size, W_DEFAULT, W_DEFAULT);
	item.SetHeight(size.height);
	item.SetControl(&composite[0]);
	//item.SetImage(MShell::GetImage16_(this,1));
	// Second item
	composite[1].Create(this, W_NONE);
	layout[1].numColumns = 2;
	layout[1].marginLeft = layout[1].marginTop = layout[1].marginRight =
			layout[1].marginBottom = 10;
	layout[1].verticalSpacing = 10;
	composite[1].SetLayout(layout[1]);
	label[0].Create(composite[1], W_NONE);
	image = toolkit->GetSystemImage(W_ICON_ERROR);
	label[0].SetImage(image);
	label[1].Create(composite[1], W_NONE);
	label[1].SetText("SWT.ICON_ERROR");
	label[2].Create(composite[1], W_NONE);
	image = toolkit->GetSystemImage(W_ICON_INFORMATION);
	label[2].SetImage(image);
	label[3].Create(composite[1], W_NONE);
	label[3].SetText("SWT.ICON_INFORMATION");
	label[4].Create(composite[1], W_NONE);
	image = toolkit->GetSystemImage(W_ICON_WARNING);
	label[4].SetImage(image);
	label[5].Create(composite[1], W_NONE);
	label[5].SetText("SWT.ICON_WARNING");
	label[6].Create(composite[1], W_NONE);
	image = toolkit->GetSystemImage(W_ICON_QUESTION);
	label[6].SetImage(image);
	label[7].Create(composite[1], W_NONE);
	label[7].SetText("SWT.ICON_QUESTION");
	this->AppendItem(item, "What is your favorite icon");
	item.SetHeight(composite[1].ComputeSize(size, W_DEFAULT, W_DEFAULT).height);
	item.SetControl(&composite[1]);
	//item.SetImage(image);

	// Third item
	composite[2].Create(this, W_NONE);
	layout[2].numColumns = 2;
	layout[2].marginLeft = layout[2].marginTop = layout[2].marginRight =
			layout[2].marginBottom = 10;
	layout[2].verticalSpacing = 10;
	composite[2].SetLayout(layout[2]);
	label[8].Create(composite[2], W_NONE);
	label[8].SetText("Scale");
	scale.Create(&composite[2], W_NONE);
	label[9].Create(composite[2], W_NONE);
	label[0].SetText("Spinner");
	spinner.Create(&composite[2], W_BORDER);
	label[10].Create(composite[2], W_NONE);
	label[10].SetText("Slider");
	slider.Create(&composite[2], W_NONE);
	this->AppendItem(item, "What is your favorite range widget");
	item.SetHeight(composite[2].ComputeSize(size, W_DEFAULT, W_DEFAULT).height);
	item.SetControl(&composite[2]);
	//item.setImage(image);

}
