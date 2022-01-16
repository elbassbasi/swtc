/*
 * NonRectangularWindow.cpp
 *
 *  Created on: 16 janv. 2022
 *      Author: azeddine
 */

#include "TShellWithRegion.h"

TShellWithRegion::TShellWithRegion() {

}

TShellWithRegion::~TShellWithRegion() {
}

void TShellWithRegion::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "ShellWithRegion", new TShellWithRegion());
}
void TShellWithRegion::CreateControl(WComposite *parent) {
	WShell *shell = parent->GetShell();
	this->Create(shell, W_NO_TRIM | W_ON_TOP);
	WRegion region;
	region.CreateElliptic(0,0,67*2,67*2);
	/*count = Circle(&points, 20, 67, 50);
	region.Subtract(points, count);
	delete[] points;
	int _p[] = { 67, 50, 55, 105, 79, 105 };
	region.Subtract((WPoint*) _p, sizeof(_p) / (2 * sizeof(_p[0])));*/
	this->SetRegion(region);
	WRect bounds;
	region.GetBounds(bounds);
	this->SetSize(bounds);
	btn.Create(this, W_PUSH);
	//b.setBackground(shell.getBackground());
	btn.SetText("close");
	btn.Pack();
	btn.SetLocation(10, 68);
	this->Open();
}

WControl* TShellWithRegion::GetControl(WComposite *parent) {
	if (!this->IsOk()) {
		this->CreateControl(parent);
	}
	return this;
}

bool TShellWithRegion::OnPaint(WPaintEvent &e) {
	WRect bounds;
	GetBounds(bounds);
	e->SetBackground(W_COLOR_BLUE);
	e->FillRectangle(bounds);
	return true;
}
