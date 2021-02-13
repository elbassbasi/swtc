/*
 * TThemeDemo.cpp
 *
 *  Created on: 11 mai 2020
 *      Author: azeddine
 */

#include "TThemeDemo.h"

void TThemeDemo::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Button_PUSH",
			new TThemeDemo(W_THEME_CLASS_BUTTON, W_PUSH));
	ITreeItem::Regitre(parent, "Button_CHECK",
			new TThemeDemo(W_THEME_CLASS_BUTTON, W_CHECK));
	ITreeItem::Regitre(parent, "Button_RADIO",
			new TThemeDemo(W_THEME_CLASS_BUTTON, W_RADIO));
	ITreeItem::Regitre(parent, "ProgressBar_VERTICAL",
			new TThemeDemo(W_THEME_CLASS_PROGRESSBAR, W_VERTICAL));
	ITreeItem::Regitre(parent, "ProgressBar_HORIZONTAL",
			new TThemeDemo(W_THEME_CLASS_PROGRESSBAR, W_HORIZONTAL));
}

TThemeDemo::TThemeDemo(int clazz, int style) {
	this->clazz = clazz;
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
	int height = area.height / (sizeof(states) / sizeof(states[0])) - 5;
	int width = area.width / 2 - 5;
	rect.x = area.x;
	rect.y = area.y;
	rect.width = width;
	rect.height = height;
	data.style = this->style;
	data.clazz = this->clazz;
	for (int i = 0; i < (sizeof(states) / sizeof(states[0])); i++) {
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
