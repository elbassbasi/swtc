/*
 * TaskBar.h
 *
 *  Created on: 9 ao�t 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_WIDGETS_TASKBAR_H_
#define SWTP_WIDGETS_TASKBAR_H_
#include "Menu.h"
class WTaskBar;
/**
 * Instances of this class represent a task item.
 *
 * <dl>
 * <dt><b>Styles:</b></dt>
 * <dd>(none)</dd>
 * <dt><b>Events:</b></dt>
 * <dd>(none)</dd>
 * </dl>
 */
class SWTP_PUBLIC WTaskItem: public WItem {
public:
	/**
	 * Returns the receiver's pop up menu if it has one, or null
	 * if it does not.
	 *
	 * @return the receiver's menu
	 */
	WMenu* GetMenu() {
		WMenu* menu;
		w_taskitem_get_menu(W_TASKITEM(this),(w_menu**)&menu);
		return menu;
	}
	/**
	 * Returns the receiver's overlay image if it has one, or null
	 * if it does not.
	 *
	 * @return the receiver's overlay image
	 */
	WImage* GetOverlayImage() {
		WImage* image;
		w_taskitem_get_overlay_image(W_TASKITEM(this),(w_image**)&image);
		return image;
	}
	/**
	 * Returns the receiver's overlay text, which will be an empty
	 * string if it has never been set.
	 *
	 * @return the receiver's overlay text
	 */
	WString GetOverlayText() {
		WString str;
		w_taskitem_get_overlay_text(W_TASKITEM(this), w_alloc_string_ref,
				&str.ref);
		return str;
	}
	/**
	 * Returns the receiver's parent, which must be a <code>TaskBar</code>.
	 *
	 * @return the receiver's parent
	 *
	 */
	WTaskBar* GetParent() {
		return (WTaskBar*) WItem::GetParentWidget();
	}
	/**
	 * Returns the receiver's progress.
	 *
	 * @return the receiver's progress
	 */
	int GetProgress() {
		return w_taskitem_get_progress(W_TASKITEM(this));
	}
	/**
	 * Returns the receiver's progress state.
	 *
	 * @return the receiver's progress state
	 */
	int GetProgressState() {
		return w_taskitem_get_progress_state(W_TASKITEM(this));
	}
	/**
	 * Sets the receiver's pop up menu to the argument. The way the menu is
	 * shown is platform specific.
	 *
	 * <p>
	 * This feature might not be available for the receiver on all
	 * platforms. The application code can check if it is supported
	 * by calling the respective get method. When the feature is not
	 * available, the get method will always return the NULL.</p>
	 *
	 * <p>
	 * For better cross platform support, the application code should
	 * set this feature on the <code>TaskItem</code> for application.<br>
	 * On Windows, this feature will only work on RCP applications.</p>
	 *
	 * <p>
	 * The menu should be fully created before this method is called.
	 * Dynamic changes to the menu after the method is called will not be reflected
	 * in the native menu.</p>
	 *
	 * @param menu the new pop up menu
	 */
	bool SetMenu(WMenu *menu) {
		return w_taskitem_set_menu(W_TASKITEM(this), W_MENU(menu)) > 0;
	}
	/**
	 * Sets the receiver's overlay image, which may be null
	 * indicating that no image should be displayed. The bounds
	 * for the overlay image is determined by the platform and in
	 * general it should be a small image.
	 *
	 * <p>
	 * This feature might not be available for the receiver on all
	 * platforms. The application code can check if it is supported
	 * by calling the respective get method. When the feature is not
	 * available, the get method will always return the NULL.</p>
	 *
	 * <p>
	 * For better cross platform support, the application code should
	 * first try to set this feature on the <code>TaskItem</code> for the
	 * main shell then on the <code>TaskItem</code> for the application.</p>
	 *
	 * @param overlayImage the new overlay image (may be null)
	 */
	bool SetOverlayImage(WImage *overlayImage) {
		return w_taskitem_set_overlay_image(W_TASKITEM(this),
				W_IMAGE(overlayImage)) > 0;
	}
	/**
	 * Sets the receiver's overlay text. The space available to display the
	 * overlay text is platform dependent and in general it should be no longer
	 * than a few characters.
	 *
	 * <p>
	 * This feature might not be available for the receiver on all
	 * platforms. The application code can check if it is supported
	 * by calling the respective get method. When the feature is not
	 * available, the get method will always return an empty string.</p>
	 *
	 * <p>
	 * For better cross platform support, the application code should
	 * first try to set this feature on the <code>TaskItem</code> for the
	 * main shell then on the <code>TaskItem</code> for the application.</p>
	 *
	 * @param overlayText the new overlay text
	 */
	bool SetOverlayText(const char *overlayText) {
		return w_taskitem_set_overlay_text(W_TASKITEM(this), overlayText) > 0;
	}
	/**
	 * Sets the receiver's progress, the progress represents a percentage and
	 * should be in range from 0 to 100. The progress is only shown when the progress
	 * state is different than <code>SWT#DEFAULT</code>.
	 *
	 * <p>
	 * This feature might not be available for the receiver on all
	 * platforms. The application code can check if it is supported
	 * by calling the respective get method. When the feature is not
	 * available, the get method will always return zero.</p>
	 *
	 * <p>
	 * For better cross platform support, the application code should
	 * first try to set this feature on the <code>TaskItem</code> for the
	 * main shell then on the <code>TaskItem</code> for the application.</p>
	 *
	 * @param progress the new progress
	 *
	 * #see {@link #setProgressState(int)}
	 */
	bool SetProgress(int progress) {
		return w_taskitem_set_progress(W_TASKITEM(this), progress) > 0;
	}
	/**
	 * Sets the receiver's progress state, the state can be one of
	 * the following:
	 * <p><ul>
	 * <li>{@link SWT#DEFAULT}</li>
	 * <li>{@link SWT#NORMAL}</li>
	 * <li>{@link SWT#PAUSED}</li>
	 * <li>{@link SWT#ERROR}</li>
	 * <li>{@link SWT#INDETERMINATE}</li>
	 * </ul></p>
	 *
	 * The percentage of progress shown by the states <code>SWT#NORMAL</code>, <code>SWT#PAUSED</code>,
	 * <code>SWT#ERROR</code> is set with <code>setProgress()</code>. <br>
	 * The state <code>SWT#DEFAULT</code> indicates that no progress should be shown.
	 *
	 * <p>
	 * This feature might not be available for the receiver on all
	 * platforms. The application code can check if it is supported
	 * by calling the respective get method. When the feature is not
	 * available, the get method will always return <code>SWT#DEFAULT</code>.</p>
	 *
	 * <p>
	 * For better cross platform support, the application code should
	 * first try to set this feature on the <code>TaskItem</code> for the
	 * main shell then on the <code>TaskItem</code> for the application.</p>
	 *
	 * @param progressState the new progress state
	 *
	 * #see {@link #setProgress(int)}
	 */
	bool SetProgressState(int progressState) {
		return w_taskitem_set_progress_state(W_TASKITEM(this), progressState)
				> 0;
	}

};
/**
 * Instances of this class represent the system task bar.
 *
 * <dl>
 * <dt><b>Styles:</b></dt>
 * <dd>(none)</dd>
 * <dt><b>Events:</b></dt>
 * <dd>(none)</dd>
 * </dl>
 *
 * @see Display#getSystemTaskBar
 */
class SWTP_PUBLIC WTaskBar: public WWidget {
private:
	friend class WToolkit;
	WTaskBar() {

	}
public:
	/**
	 * Returns the item at the given, zero-relative index in the
	 * receiver. Throws an exception if the index is out of range.
	 *
	 * @param index the index of the item to return
	 * @return the item at the given index
	 */
	WTaskItem* GetItem(int index) {
		WTaskItem *item;
		w_taskbar_get_item(W_TASKBAR(this), index, (w_taskitem**) &item);
		return item;
	}
	/**
	 * Returns the <code>TaskItem</code> for the given <code>Shell</code> or the <code>TaskItem</code>
	 * for the application if the <code>Shell</code> parameter is <code>null</code>.
	 * If the requested item is not supported by the platform it returns <code>null</code>.
	 *
	 * @param shell the shell for which the task item is requested, or null to request the application item
	 * @return the task item for the given shell or the application
	 */
	WTaskItem* GetItem(WFrame *shell) {
		WTaskItem *item;
		w_taskbar_get_item_shell(W_TASKBAR(this), W_SHELL(shell),
				(w_taskitem**) &item);
		return item;
	}
	/**
	 * Returns the number of items contained in the receiver.
	 *
	 * @return the number of items
	 */
	int GetItemCount() {
		return w_taskbar_get_item_count(W_TASKBAR(this));
	}
	/**
	 * Returns an array of <code>TaskItem</code>s which are the items
	 * in the receiver.
	 * <p>
	 * Note: This is not the actual structure used by the receiver
	 * to maintain its list of items, so modifying the array will
	 * not affect the receiver.
	 * </p>
	 *
	 * @return the items in the receiver
	 */
	void GetItems(WIterator<WTaskItem> &items) {
		w_taskbar_get_items(W_TASKBAR(this), (w_iterator*) &items);
	}
private:
	void *handles[(sizeof(w_taskbar) - sizeof(w_widget)) / sizeof(void*)];
};

#endif /* SWTP_WIDGETS_TASKBAR_H_ */
