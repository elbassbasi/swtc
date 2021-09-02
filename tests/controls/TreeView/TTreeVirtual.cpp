/*
 * TTreeCustom.cpp
 *
 *  Created on: 13 mai 2020
 *      Author: Azeddine El Bassbasi
 */

#include "TTreeVirtual.h"

void TTreeVirtual::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "virtual", new TTreeVirtual());
}
WMenuItems TTreeVirtual::menuitems[] = { //
		{ W_PUSH, 0, -1, "Insert", 0 },	//
				{ W_PUSH, 0, -1, "Delete", W_ACTION(TTreeVirtual::Delete) },//
				{ 0, 0, 0, 0 },	//
		};
WControl* TTreeVirtual::GetControl(WComposite *parent) {
	if (!this->IsOk()) {
		this->CreateControl(parent);
	}
	return this;
}

TTreeVirtual::TTreeVirtual() {
	this->fontBold = 0;
}

TTreeVirtual::~TTreeVirtual() {
	this->fontBold->Dispose();
}

void TTreeVirtual::CreateControl(WComposite *parent) {
	WTreeItem item, tmp, root;
	WColumnItem column;
	this->Create(parent,
			W_VIRTUAL | W_HSCROLL | W_VSCROLL | W_FULL_SELECTION | W_CHECK
					| W_CUSTOMDRAW);
	menu.CreatePopUp(this);
	menu.CreateItems(this, menuitems);
	SetMenu(&menu);
	/* create text edit */
	text.Create(this, W_NONE);
	text.SetVisible(false);
	/* init tree view */
	MFrame *frame = (MFrame*) GetFrame();
	this->SetImageList(frame->GetImageList32());
	this->GetColumn(0, column).SetText("id");
	this->AppendColumn(column, "progress").SetResizable(false);
	column.SetWidth(60);
	this->AppendColumn(column, "int");
	column.SetAlignment(W_CENTER);
	this->SetHeaderVisible(true);
	this->SetLinesVisible(true);
	this->GetRootItem(root);
	int imagelistcount = this->GetImageList()->GetCount();
	if (imagelistcount == 0)
		imagelistcount = 1;
	for (int i = 0; i < 50; i++) {
		if (root.AppendItem(item).IsOk()) {
			item.SetImage(i % imagelistcount);
			item.SetData(new Person(i, -1));
			for (int j = 0; j < 5; j++) {
				item.AppendItem(tmp, 0);
				if (tmp.IsOk()) {
					tmp.SetImage(j);
					tmp.SetData(new Person(i, j));
				}
			}
		}
	}
	WFontData fontdata;
	this->GetFont()->GetFontData(fontdata);
	fontdata.SetStyle(W_BOLD);
	this->fontBold = WFont::Create(fontdata);
}

Person::Person(int i, int j) {
	this->i = i;
	this->j = j;
	if (j < 0) {
		sprintf(this->name, "person %d", i);
		this->progress = i % 5;
	} else {
		sprintf(this->name, "_person %d,%d", i, j);
		this->progress = j;
	}
}
#define NUM_PIXEL 1
bool TTreeVirtual::OnItemMeasure(WTreeEvent &e) {
	e.rect->height = 40;
	return true;
}

bool TTreeVirtual::OnItemErase(WTreeEvent &e) {
	e.detail &= ~W_HOT;
	if (!(e.detail & W_SELECTED))
		return false; /* item not selected */
	WRect rect;
	GetClientArea(rect);
	int clientWidth = rect.width;
	int oldForeground = e.gc->GetForeground();
	int oldBackground = e.gc->GetBackground();
	e.gc->SetForeground(W_COLOR_RED);
	e.gc->SetBackground(W_COLOR_YELLOW);
	e.gc->FillGradientRectangle(0, e.rect->y, clientWidth, e.rect->height,
			false);
	e.gc->SetForeground(oldForeground);
	e.gc->SetBackground(oldBackground);
	e.detail &= ~W_SELECTED;
	return true;
}

bool TTreeVirtual::OnItemPaint(WTreeEvent &e) {
	Person *p = e.GetItemData<Person>();
	char txt[20];
	if (e.column->GetIndex() == 1) {
		WRect clip;
		e.gc->GetClipping(clip);
		p = (Person*) e.item->GetData();
		WRect r(*e.rect);
		r.x += NUM_PIXEL;
		r.y += NUM_PIXEL;
		r.width -= 2 * NUM_PIXEL;
		r.height -= 2 * NUM_PIXEL;
		WTheme *theme = GetTheme();
		WProgressBarThemeData data;
		if (p != 0) {
			data.maximum = 5;
			data.selection = p->progress;
			sprintf(txt, "%d%%", p->progress);
		} else {
			sprintf(txt, "unknown");
			data.maximum = 5;
			data.selection = 0;
		}
		e.gc->FillRectangle(r);
		data.SetText(txt);
		data.DrawAll(theme);
		return true;
	}
	return WTreeView::OnItemPaint(e);
}

bool TTreeVirtual::OnItemGetText(WTreeEvent &e) {
	Person *p = e.GetItemData<Person>();
	if (p != 0) {
		switch (e.GetColumnIndex()) {
		case 0:
			e.SetAttrText(p->name);
			break;
		case 2:
			e.SetAttrTextV("n:%s p:%d", p->name, p->progress);
			break;
		default:
			return WTreeView::OnItemGetText(e);
			break;
		}
	}
	return true;
}
bool TTreeVirtual::OnItemGetAttr(WListEvent &e) {
	Person *p = e.GetItemData<Person>();
	if (p != 0) {
		switch (e.GetColumnIndex()) {
		case 2:
			if (e.item->GetChecked()) {
				e.SetAttrBackground(W_COLOR_MAGENTA);
				e.SetAttrForeground(W_COLOR_RED);
				e.SetAttrFont(fontBold);
			} else {
				e.SetAttrForeground(W_COLOR_BLUE);
			}
			break;
		default:
			return WTreeView::OnItemGetAttr(e);
			break;
		}
	}
	return true;
}
bool TTreeVirtual::OnItemDefaultSelection(WTreeEvent &e) {
	if (e.item->IsOk()) {
		WRect bounds;
		e.item->GetBounds(bounds);
		text.item = *e.GetTreeItem();
		WString str = e.item->GetText();
		text.SetText(str);
		text.SetBounds(bounds);
		text.SetVisible(true);
	}
	return false;
}

bool TTreeVirtual::OnItemDispose(WTreeEvent &e) {
	Person *p = e.GetItemData<Person>();
	if (p != 0) {
		e.item->SetData(0);
		delete p;
	}
	return false;
}

bool TTreeVirtual::OnItemCollapse(WListEvent &e) {
	e.GetTreeItem()->RemoveAll();
	return true;
}

bool TTreeVirtual::Delete(WEvent &e) {
	WIterator<WTreeItem> selection;
	if (GetSelection(selection)) {
		WTreeItem item;
		while (selection.Next(item)) {
			item.RemoveAll();
			item.SetHasChildren();
		}
	}
	return true;
}

bool TTreeVirtualEdit::OnFocusOut(WEvent &e) {
	bool ret = WTextEdit::OnFocusOut(e);
	SetTextAndHide();
	return ret;
}

bool TTreeVirtualEdit::OnTraverse(WKeyEvent &e) {
	bool ret = WTextEdit::OnTraverse(e);
	return ret;
}

bool TTreeVirtualEdit::OnDefaultSelection(WTextEditEvent &e) {
	bool ret = WTextEdit::OnDefaultSelection(e);
	SetTextAndHide();
	return ret;
}

void TTreeVirtualEdit::SetTextAndHide() {
	WString text = this->GetText();
	item.SetText(text);
	SetVisible(false);
}
