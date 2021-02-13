/*
 * TBrowserDemo.h
 *
 *  Created on: 13 mai 2020
 *      Author: Azeddine El Bassbasi
 */

#ifndef TESTS_CONTROLS_WEBVIEW_TWEBVIEWDEMO_H_
#define TESTS_CONTROLS_WEBVIEW_TWEBVIEWDEMO_H_
#include "TWebViews.h"
class TWebViewDemo : public WWebView , public ITreeItem {
public:
	static void Registre(WTreeItem &parent);
	WControl* GetControl(WComposite *parent);
	void CreateControl(WComposite *parent);

};

#endif /* TESTS_CONTROLS_WEBVIEW_TWEBVIEWDEMO_H_ */
