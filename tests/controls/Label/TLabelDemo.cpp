/*
 * TLabelDemo.cpp
 *
 *  Created on: 10 mai 2020
 *      Author: azeddine
 */

#include "TLabelDemo.h"

void TLabelDemo::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent,"Demonstrates",new TLabelDemo());
}

void TLabelDemo::CreateControl(WComposite *parent) {
	this->Create(parent,W_NONE);
	this->layout.numColumns = 2;
	WGridData data;
	data.verticalAlignment = W_GRID_FILL;
	data.horizontalAlignment = W_GRID_FILL;
	data.grabExcessVerticalSpace = true;
	this->SetLayout(this->layout);
	// Create a label
	label[0].Create(this,W_NONE);
	label[0].SetText("This is a plain label.");
	// Create a label with a border
	label[1].Create(this,W_BORDER);
	label[1].SetText("This is a plain label.");
	// Create a vertical separator
	label[2].Create(this,W_SEPARATOR);
	this->layout.SetData(&label[2],data);
	// Create a horizontal separator
	label[3].Create(this,W_HORIZONTAL | W_SEPARATOR);
	this->layout.SetData(&label[3],data);
	// Create a label with an image
	label[4].Create(this,W_NONE);
	//label[4].SetImage(0);
}
