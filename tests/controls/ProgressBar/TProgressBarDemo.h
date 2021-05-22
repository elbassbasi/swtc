/*
 * TProgressBarDemo.h
 *
 *  Created on: 13 mai 2020
 *      Author: Azeddine El Bassbasi
 */

#ifndef TESTS_CONTROLS_PROGRESSBAR_TPROGRESSBARDEMO_H_
#define TESTS_CONTROLS_PROGRESSBAR_TPROGRESSBARDEMO_H_
#include "TProgressBars.h"
class TProgressBarDemo : public CompositeTreeItem {
public:
	static void Registre(WTreeItem &parent);
	void CreateControl(WComposite *parent);
	bool OnTimer(WTimerEvent& e);
	bool OnNotifySelection(WEvent &e);
	WGridLayout layout;
	WProgressBar progress;
	WProgressBar vertical;
	WButton start;
	WButton stop;
};

#endif /* TESTS_CONTROLS_PROGRESSBAR_TPROGRESSBARDEMO_H_ */
