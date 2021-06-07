/*
 * TrayDemo.h
 *
 *  Created on: 1 juin 2021
 *      Author: Azeddine
 */

#ifndef TESTS_WIDGETS_TRAY_TTRAYDEMO_H_
#define TESTS_WIDGETS_TRAY_TTRAYDEMO_H_
#include "TTrays.h"
class TTrayDemo;
class TTrayItem: public WTrayItem {
public:
	TTrayDemo *parent;
	bool Create(TTrayDemo *parent);
protected:
	bool OnSelection(WEvent &e);
	bool OnDefaultSelection(WEvent &e);
	bool OnMenuDetect(WEvent &e);
	bool OnShow(WEvent &e);
	bool OnHide(WEvent &e);
};
class TTrayDemo: public CompositeTreeItem {
public:
	static void Registre(WTreeItem &parent);
	TTrayDemo();
	~TTrayDemo();
	void CreateControl(WComposite *parent);
protected:
	bool OnShowTray(WEvent &e);
	bool OnShowTooltip(WEvent &e);
public:
	TTrayItem trayItem;
	WGridLayout layout;
	WMenu menu;
	WButton show;
	WButton showtooltip;
	WTextEdit tooltipText;
};

#endif /* TESTS_WIDGETS_TRAY_TTRAYDEMO_H_ */
