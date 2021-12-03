/*
 * TProgressBarThread.h
 *
 *  Created on: 19 juin 2021
 *      Author: azeddine
 */

#ifndef TESTS_CONTROLS_PROGRESSBAR_TPROGRESSBARTHREAD_H_
#define TESTS_CONTROLS_PROGRESSBAR_TPROGRESSBARTHREAD_H_
#include "TProgressBars.h"
class TProgressBarThread: public CompositeTreeItem, public IWRunnable {
public:
	static const int thread_length = 1;
	static void Registre(WTreeItem &parent);
	TProgressBarThread();
	~TProgressBarThread();
	void CreateControl(WComposite *parent);
	void OnNotifyExec();
	bool OnStart(WEvent &e);
	void Run();
	unsigned is_started :1;
	int selection;
	int maximum;
	WThread threads[thread_length];
	WGridLayout layout;
	WProgressBar progress;
	WLabel label;
	WButton start;
};

#endif /* TESTS_CONTROLS_PROGRESSBAR_TPROGRESSBARTHREAD_H_ */
