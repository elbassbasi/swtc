/*
 * NonRectangularWindow.h
 *
 *  Created on: 16 janv. 2022
 *      Author: azeddine
 */

#ifndef TESTS_WIDGETS_SHELL_TSHELLWITHREGION_H_
#define TESTS_WIDGETS_SHELL_TSHELLWITHREGION_H_
#include "TShells.h"
class TShellWithRegion : public WFrame , public ITreeItem {
public:
	TShellWithRegion();
	~TShellWithRegion();
	static void Registre(WTreeItem &parent);
	void CreateControl(WComposite *parent);
	WControl* GetControl(WComposite *parent);
	WButton btn;
protected:
	bool OnPaint(WPaintEvent &e);
};

#endif /* TESTS_WIDGETS_SHELL_TSHELLWITHREGION_H_ */
