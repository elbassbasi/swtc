/*
 * TBrowsers.h
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#ifndef TESTS_CONTROLS_WEBVIEW_TWEBVIEWS_H_
#define TESTS_CONTROLS_WEBVIEW_TWEBVIEWS_H_
#include "../TControls.h"
class TWebViews: public FolderTreeItem {
public:
	static void Registre(WTreeItem &parent);
	bool Expand(WTreeItem &item);
};

#endif /* TESTS_CONTROLS_WEBVIEW_TWEBVIEWS_H_ */
