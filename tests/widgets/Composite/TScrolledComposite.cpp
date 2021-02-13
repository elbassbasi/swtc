/*
 * TScrolledComposite.cpp
 *
 *  Created on: 6 sept. 2020
 *      Author: azeddine
 */

#include "TScrolledComposite.h"
void TScrolledComposite::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "ScrolledComposite", new TScrolledComposite());
}
TScrolledComposite::TScrolledComposite() {
	this->content = 0;
	this->minWidth = this->minHeight = 0;
	this->alwaysShowScroll = false;
	this->expandHorizontal = false;
	this->expandVertical = false;
	this->showFocusedControl = false;
	this->showNextFocusedControl = true;
}

TScrolledComposite::~TScrolledComposite() {
}

void TScrolledComposite::CreateControl(WComposite *parent) {
	this->Create(parent, W_HSCROLL | W_VSCROLL);
	composite.Create(this, 0);
	char txt[50];
	WRect bounds;
	composite.ComputeTrim(bounds, 0, 0, 500,
	NUM_OF_BUTTON * 100);
	composite.SetBounds(bounds);
	for (size_t i = 0; i < NUM_OF_BUTTON; i++) {
		btn[i].Create(&composite, W_FLAT | W_CENTER);
		btn[i].SetBounds(0, i * 100, 490, 50);
		sprintf(txt, "btn %d", (int) i);
		btn[i].SetText(txt);
	}
	this->content = &composite;
}

bool TScrolledComposite::OnLayout(WEvent &e) {
	if (this->content == 0)
		return false;
	WScrollBar hBar, vBar;
	GetHorizontalBar(hBar);
	GetVerticalBar(vBar);
	bool create_graphics = false;
	if (hBar.IsOk() || vBar.IsOk()) {
		create_graphics = true;
	}
	WRect contentRect;
	this->content->GetBounds(contentRect);
	if (!this->alwaysShowScroll) {
		bool hVisible = NeedHScroll(hBar, vBar, contentRect, false);
		bool vVisible = NeedVScroll(hBar, vBar, contentRect, hVisible);
		if (!hVisible && vVisible)
			hVisible = NeedHScroll(hBar, vBar, contentRect, vVisible);
		if (hBar.IsOk())
			hBar.SetVisible(hVisible);
		if (vBar.IsOk())
			vBar.SetVisible(vVisible);
	}
	WRect hostRect;
	GetClientArea(hostRect);
	if (this->expandHorizontal) {
		contentRect.width = WMAX(this->minWidth, hostRect.width);
	}
	if (this->expandVertical) {
		contentRect.height = WMAX(this->minHeight, hostRect.height);
	}
	WGraphics gc;
	WFontMetrics metrics;
	if (create_graphics) {
		content->GetGraphics(gc);
		gc.GetFontMetrics(metrics);
	}
	if (hBar.IsOk()) {
		hBar.SetMaximum(contentRect.width - hostRect.width);
		hBar.SetThumb(WMIN(contentRect.width, hostRect.width));
		hBar.SetIncrement(metrics.getAverageCharWidth());
		hBar.SetPageIncrement(hostRect.width);
		int hPage = contentRect.width - hostRect.width;
		int hSelection = hBar.GetSelection();
		if (hSelection >= hPage) {
			if (hPage <= 0) {
				hSelection = 0;
				hBar.SetSelection(0);
			}
			contentRect.x = -hSelection;
		}
	}

	if (vBar.IsOk()) {
		vBar.SetMaximum(contentRect.height - hostRect.height);
		vBar.SetThumb(WMIN(contentRect.height, hostRect.height));
		vBar.SetIncrement(metrics.getHeight());
		vBar.SetPageIncrement(hostRect.height);
		int vPage = contentRect.height - hostRect.height;
		int vSelection = vBar.GetSelection();
		if (vSelection >= vPage) {
			if (vPage <= 0) {
				vSelection = 0;
				vBar.SetSelection(0);
			}
			contentRect.y = -vSelection;
		}
	}

	this->content->SetBounds(contentRect);
	return true;
}

bool TScrolledComposite::OnHScroll(WScrollBarEvent &e) {
	if (content == 0)
		return false;
	WRect location;
	content->GetBounds(location);
	int hSelection = e.scrollbar->GetSelection();
	content->SetLocation(-hSelection, location.y);
	return true;
}

bool TScrolledComposite::OnVScroll(WScrollBarEvent &e) {
	if (content == 0)
		return false;
	WRect location;
	content->GetBounds(location);
	int vSelection = e.scrollbar->GetSelection();
	content->SetLocation(location.x, -vSelection);
	return true;
}

bool TScrolledComposite::NeedHScroll(WScrollBar &hBar, WScrollBar &vBar,
		WRect &contentRect, bool vVisible) {
	WRect hostRect;
	WSize sz;
	GetBounds(hostRect);
	int border = this->GetBorderWidth();
	hostRect.width -= 2 * border;
	if (vVisible && vBar.IsOk())
		hostRect.width -= vBar.GetSize(sz).width;

	if (!expandHorizontal && contentRect.width > hostRect.width)
		return true;
	if (expandHorizontal && minWidth > hostRect.width)
		return true;
	return false;
}

bool TScrolledComposite::NeedVScroll(WScrollBar &hBar, WScrollBar &vBar,
		WRect &contentRect, bool hVisible) {
	WRect hostRect;
	WSize sz;
	GetBounds(hostRect);
	int border = this->GetBorderWidth();
	hostRect.height -= 2 * border;
	if (hVisible && hBar.IsOk())
		hostRect.height -= hBar.GetSize(sz).height;

	if (!expandVertical && contentRect.height > hostRect.height)
		return true;
	if (expandVertical && minHeight > hostRect.height)
		return true;
	return false;
}
