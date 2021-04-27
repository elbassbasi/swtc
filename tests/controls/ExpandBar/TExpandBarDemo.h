/*
 * ExpandBarDemo.h
 *
 *  Created on: 13 mai 2020
 *      Author: Azeddine El Bassbasi
 */

#ifndef TESTS_CONTROLS_EXPANDBAR_TEXPANDBARDEMO_H_
#define TESTS_CONTROLS_EXPANDBAR_TEXPANDBARDEMO_H_
#include "TExpandBars.h"
class TExpandBarDemo: public WExpandBar, public ITreeItem {
public:
	static const int length = 3;
	static const char *names[length];
	static void Registre(WTreeItem &parent);
	WControl* GetControl(WComposite *parent);
	void CreateControl(WComposite *parent);
	void CreateWidgets(int index);
	WComposite composite[length];
	WGridLayout layout[length];
	/* item 0 */
	static const int length_0 = 4;
	static int styles_0[length_0];
	static const char *names_0[length_0];
	WButton btn_0[length_0];
	void Create_0(WComposite *composite);
	/* item 1 */
	static const int length_1 = 4;
	static int images_1[length_1];
	static const char *names_1[length_1];
	WLabel label_1[length_1 * 2];
	void Create_1(WComposite *composite);
	/* item 2 */
	WLabel label_2[3];
	WSlider scale_2;
	WSpinner spinner_2;
	WSlider slider_2;
	void Create_2(WComposite *composite);
};

#endif /* TESTS_CONTROLS_EXPANDBAR_TEXPANDBARDEMO_H_ */
