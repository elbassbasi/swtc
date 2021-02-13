/*
 * TControls.cpp
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#include "TControls.h"
#include "Button/TButtons.h"
#include "ComboBox/TComboBoxs.h"
#include "CoolBar/TCoolBars.h"
#include "DateTime/TDateTimes.h"
#include "ExpandBar/TExpandBars.h"
#include "GroupBox/TGroupBoxs.h"
#include "Label/TLabels.h"
#include "ListView/TListViews.h"
#include "ProgressBar/TProgressBars.h"
#include "Sash/TSashes.h"
#include "Slider/TSliders.h"
#include "Spinner/TSpinners.h"
#include "TabView/TTabViews.h"
#include "TextEdit/TTextEdits.h"
#include "ToolBar/TToolBars.h"
#include "TreeView/TTreeViews.h"
#include "WebView/TWebViews.h"
void TControls::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Controls", new TControls());
}
bool TControls::Expand(WTreeItem &item) {
	TWebViews::Registre(item);
	TButtons::Registre(item);
	TComboBoxs::Registre(item);
	TCoolBars::Registre(item);
	TDateTimes::Registre(item);
	TExpandBars::Registre(item);
	TGroupBoxs::Registre(item);
	TLabels::Registre(item);
	TProgressBars::Registre(item);
	TSashes::Registre(item);
	TSliders::Registre(item);
	TSpinners::Registre(item);
	TTabViews::Registre(item);
	TListViews::Registre(item);
	TTextEdits::Registre(item);
	TToolBars::Registre(item);
	TTreeViews::Registre(item);
	return true;
}

