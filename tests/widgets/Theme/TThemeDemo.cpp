/*
 * TThemeDemo.cpp
 *
 *  Created on: 11 mai 2020
 *      Author: azeddine
 */

#include "TThemeDemo.h"

void TThemeDemo::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Button_PUSH",
			new TThemeDemo(W_THEME_CLASS_BUTTON, 0, W_PUSH));
	ITreeItem::Regitre(parent, "Button_CHECK",
			new TThemeDemo(W_THEME_CLASS_BUTTON, 0, W_CHECK));
	ITreeItem::Regitre(parent, "Button_RADIO",
			new TThemeDemo(W_THEME_CLASS_BUTTON, 0, W_RADIO));
	ITreeItem::Regitre(parent, "ProgressBar_VERTICAL",
			new TThemeDemo(W_THEME_CLASS_PROGRESSBAR, 0, W_VERTICAL));
	ITreeItem::Regitre(parent, "ProgressBar_HORIZONTAL",
			new TThemeDemo(W_THEME_CLASS_PROGRESSBAR, 0, W_HORIZONTAL));
	ITreeItem::Regitre(parent, "ComboBox",
			new TThemeDemo(W_THEME_CLASS_COMBOBOX, 0, 0));
	ITreeItem::Regitre(parent, "TabItem",
			new TThemeDemo(W_THEME_CLASS_TABVIEW, W_THEME_TABITEM, 0));
}

TThemeDemo::TThemeDemo(wushort clazz, wushort part, int style) {
	this->clazz = clazz;
	this->part = part;
	this->style = style;
}
int TThemeDemo::states[] = { W_THEME_SELECTED, W_THEME_FOCUSED, W_THEME_HOT,
		W_THEME_PRESSED, W_THEME_ACTIVE, W_THEME_DISABLED, W_THEME_DEFAULTED,
		W_THEME_GRAYED, };
const char *TThemeDemo::texts[] = { "selected", "focused", "hot", "pressed",
		"active", "disabled", "defaulted", "grayed" };
bool TThemeDemo::OnPaint(WPaintEvent &e) {
	WTheme *theme = GetToolkit()->GetTheme();
	WThemeData data;
	WRect area, rect;
	GetClientArea(area);
	const int states_length = sizeof(states) / sizeof(states[0]);
	int height = area.height * 2 / states_length - 5;
	int width = area.width / 2 - 5;
	rect.x = area.x;
	rect.y = area.y;
	rect.width = width;
	rect.height = height;
	data.style = this->style;
	data.clazz = this->clazz;
	data.part = this->part;
	for (int i = 0; i < states_length; i++) {
		data.state = states[i];
		theme->DrawBackground(data, *e.gc, rect);
		theme->DrawTextCenter(data, *e.gc, rect, texts[i]);
		if (i == 3) {
			rect.x += width + 5;
			rect.y = area.y;
		} else {
			rect.y += height + 5;
		}
	}
	return true;
}
