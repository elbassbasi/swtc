/*
 * TreeItems.h
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */

#ifndef TESTS_MAIN_TREEITEMS_H_
#define TESTS_MAIN_TREEITEMS_H_
#include <swtp.h>
class ITreeItem {
protected:
	friend class MTreeView;
	static void RegitreRoot(WTreeItem &root);
public:
	static void Regitre(WTreeItem &parent, const char *text, ITreeItem *obj);
	virtual ~ITreeItem();
	virtual bool OnExpand(WTreeItem &item);
	virtual bool HasChildren();
	virtual WControl* GetControl(WComposite *parent)=0;
};
class FolderTreeItem: public ITreeItem {
protected:
	bool expand;
public:
	FolderTreeItem();
	virtual bool Expand(WTreeItem &item)=0;
	bool OnExpand(WTreeItem &item);
	bool HasChildren();
	WControl* GetControl(WComposite *parent);
};
class CompositeTreeItem: public WComposite, public ITreeItem {
public:
	bool OnExpand(WTreeItem &item);
	bool HasChildren();
	WControl* GetControl(WComposite *parent);
	virtual void CreateControl(WComposite *parent);
};
class CanvasTreeItem: public WCanvas, public ITreeItem {
public:
	bool OnExpand(WTreeItem &item);
	bool HasChildren();
	WControl* GetControl(WComposite *parent);
	virtual void CreateControl(WComposite *parent);
};
#endif /* TESTS_MAIN_TREEITEMS_H_ */
