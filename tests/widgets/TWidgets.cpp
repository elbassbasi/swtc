/*
 * TWidgets.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "TWidgets.h"
#include "Canvas/TCanvass.h"
#include "Caret/TCarets.h"
#include "Composite/TComposites.h"
#include "Control/TControlss.h"
#include "Dialogs/TDialogs.h"
#include "Dnd/TDnd.h"
#include "GLCanvas/TGLCanvass.h"
#include "Menu/TMenus.h"
#include "Scrollable/TScrollables.h"
#include "Shell/TShells.h"
#include "TaskBar/TTaskBars.h"
#include "Theme/TThemes.h"
#include "Toolkit/TToolkits.h"
#include "ToolTip/TTooltips.h"
#include "Tracker/TTrackers.h"
#include "Tray/TTrays.h"

void TWidgets::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Widgets", new TWidgets());
}

bool TWidgets::Expand(WTreeItem &item) {
	TCanvass::Registre(item);
	TCarets::Registre(item);
	TComposites::Registre(item);
	TControlss::Registre(item);
	TDialogs::Registre(item);
	TDnd::Registre(item);
	TGLCanvass::Registre(item);
	TMenus::Registre(item);
	TScrollables::Registre(item);
	TShells::Registre(item);
	TTaskBars::Registre(item);
	TThemes::Registre(item);
	TToolkits::Registre(item);
	TTooltips::Registre(item);
	TTrackers::Registre(item);
	TTrays::Registre(item);
	return true;
}
