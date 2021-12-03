/*
 * Toolkit.h
 *
 *  Created on: 25 juil. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_WIDGETS_TOOLKIT_H_
#define SWTP_WIDGETS_TOOLKIT_H_
#include "Frame.h"
/**
 * Instances of this class are responsible for managing the
 * connection between SWT and the underlying operating
 * system. Their most important function is to implement
 * the SWT event loop in terms of the platform event model.
 * They also provide various methods for accessing information
 * about the operating system, and have overall control over
 * the operating system resources which SWT allocates.
 * <p>
 * Applications which are built with SWT will <em>almost always</em>
 * require only a single display. In particular, some platforms
 * which SWT supports will not allow more than one <em>active</em>
 * display. In other words, some platforms do not support
 * creating a new display if one already exists that has not been
 * sent the <code>dispose()</code> message.
 * <p>
 * In SWT, the thread which creates a <code>Display</code>
 * instance is distinguished as the <em>user-interface thread</em>
 * for that display.
 * </p>
 * The user-interface thread for a particular display has the
 * following special attributes:
 * <ul>
 * <li>
 * The event loop for that display must be run from the thread.
 * </li>
 * <li>
 * Some SWT API methods (notably, most of the public methods in
 * <code>Widget</code> and its subclasses), may only be called
 * from the thread. (To support multi-threaded user-interface
 * applications, class <code>Display</code> provides inter-thread
 * communication methods which allow threads other than the
 * user-interface thread to request that it perform operations
 * on their behalf.)
 * </li>
 * <li>
 * The thread is not allowed to construct other
 * <code>Display</code>s until that display has been disposed.
 * (Note that, this is in addition to the restriction mentioned
 * above concerning platform support for multiple displays. Thus,
 * the only way to have multiple simultaneously active displays,
 * even on platforms which support it, is to have multiple threads.)
 * </li>
 * </ul>
 * Enforcing these attributes allows SWT to be implemented directly
 * on the underlying operating system's event model. This has
 * numerous benefits including smaller footprint, better use of
 * resources, safer memory management, clearer program logic,
 * better performance, and fewer overall operating system threads
 * required. The down side however, is that care must be taken
 * (only) when constructing multi-threaded applications to use the
 * inter-thread communication mechanisms which this class provides
 * when required.
 * </p><p>
 * All SWT API methods which may only be called from the user-interface
 * thread are distinguished in their documentation by indicating that
 * they throw the "<code>ERROR_THREAD_INVALID_ACCESS</code>"
 * SWT exception.
 * </p>
 * <dl>
 * <dt><b>Styles:</b></dt>
 * <dd>(none)</dd>
 * <dt><b>Events:</b></dt>
 * <dd>Close, Dispose, OpenDocument, Settings, Skin</dd>
 * </dl>
 * <p>
 * IMPORTANT: This class is <em>not</em> intended to be subclassed.
 * </p>
 * @see #syncExec
 * @see #asyncExec
 * @see #wake
 * @see #readAndDispatch
 * @see #sleep
 * @see #dispose
 * @noextend This class is not intended to be subclassed by clients.
 */
class SWTP_PUBLIC WToolkit {
protected:
	/**
	 * Constructs a new instance of this class.
	 * <p>
	 * Note: The resulting display is marked as the <em>current</em>
	 * display. If this is the first display which has been
	 * constructed since the application started, it is also
	 * marked as the <em>default</em> display.
	 * </p>

	 *
	 * @see #getCurrent
	 * @see #getDefault
	 * @see Shell
	 */
	WToolkit() {

	}
public:
	/**
	 * Causes the <code>run()</code> method of the runnable to
	 * be invoked by the user-interface thread at the next
	 * reasonable opportunity. The caller of this method continues
	 * to run in parallel, and is not notified when the
	 * runnable has completed.  Specifying <code>null</code> as the
	 * runnable simply wakes the user-interface thread when run.
	 * <p>
	 * Note that at the time the runnable is invoked, widgets
	 * that have the receiver as their display may have been
	 * disposed. Therefore, it is necessary to check for this
	 * case inside the runnable before accessing the widget.
	 * </p>
	 *
	 * @param runnable code to run on the user-interface thread or <code>null</code>
	 *
	 * @see #syncExec
	 */
	bool AsyncExec(w_thread_start function, void *args) {
		return w_toolkit_async_exec(W_TOOLKIT(this), function, args) > 0;
	}
	/**
	 * Causes the system hardware to emit a short sound
	 * (if it supports this capability).
	 */
	bool Beep() {
		return w_toolkit_beep(W_TOOLKIT(this)) > 0;
	}
	/**
	 * Returns the currently active <code>Shell</code>, or null
	 * if no shell belonging to the currently running application
	 * is active.
	 *
	 * @return the active shell or null
	 */
	WFrame* GetActiveShell() {
		return (WFrame*) w_toolkit_get_active_shell(W_TOOLKIT(this));
	}
	/**
	 * Returns the single instance of the application menu bar, or
	 * <code>null</code> if there is no application menu bar for the platform.
	 *
	 * @return the application menu bar, or <code>null</code>
	 */
	WMenu* GetMenuBar() {
		return (WMenu*) w_toolkit_get_menubar(W_TOOLKIT(this));
	}
	/**
	 * Returns a rectangle describing the receiver's size and location. Note that
	 * on multi-monitor systems the origin can be negative.
	 *
	 * @return the bounding rectangle
	 */
	WRect& GetBounds(WRect &bounds) {
		w_toolkit_get_bounds(W_TOOLKIT(this), (w_rect*) &bounds);
		return bounds;
	}
	/**
	 * Returns a rectangle which describes the area of the
	 * receiver which is capable of displaying data.
	 *
	 * @return the client area
	 *
	 * @see #getBounds
	 */
	WRect& GetClientArea(WRect &clientArea) {
		w_toolkit_get_client_area(W_TOOLKIT(this), (w_rect*) &clientArea);
		return clientArea;
	}
	/**
	 * Returns the control which the on-screen pointer is currently
	 * over top of, or null if it is not currently over one of the
	 * controls built by the currently running application.
	 *
	 * @return the control under the cursor or <code>null</code>
	 */
	WControl* GetCursorControl() {
		return (WControl*) w_toolkit_get_cursor_control(W_TOOLKIT(this));
	}
	/**
	 * Returns the location of the on-screen pointer relative
	 * to the top left corner of the screen.
	 *
	 * @return the cursor location
	 */
	bool GetCursorLocation(WPoint &location) {
		return w_toolkit_get_cursor_location(W_TOOLKIT(this),
				(w_point*) &location) > 0;
	}
	/**
	 * Returns an array containing the recommended cursor sizes.
	 *
	 * @return the array of cursor sizes
	 */
	size_t GetCursorSizes(WSize *sizes, size_t length) {
		return w_toolkit_get_cursor_sizes(W_TOOLKIT(this), (w_size*) sizes,
				length);
	}
	/**
	 * Returns the button dismissal alignment, one of <code>LEFT</code> or <code>RIGHT</code>.
	 * The button dismissal alignment is the ordering that should be used when positioning the
	 * default dismissal button for a dialog.  For example, in a dialog that contains an OK and
	 * CANCEL button, on platforms where the button dismissal alignment is <code>LEFT</code>, the
	 * button ordering should be OK/CANCEL.  When button dismissal alignment is <code>RIGHT</code>,
	 * the button ordering should be CANCEL/OK.
	 *
	 * @return the button dismissal order
	 */
	int GetDismissalAlignment() {
		return w_toolkit_get_dismissal_alignment(W_TOOLKIT(this));
	}
	/**
	 * Returns the longest duration, in milliseconds, between
	 * two mouse button clicks that will be considered a
	 * <em>double click</em> by the underlying operating system.
	 *
	 * @return the double click time
	 */
	int GetDoubleClickTime() {
		return w_toolkit_get_double_click_time(W_TOOLKIT(this));
	}
	/**
	 * Returns the control which currently has keyboard focus,
	 * or null if keyboard events are not currently going to
	 * any of the controls built by the currently running
	 * application.
	 *
	 * @return the focus control or <code>null</code>
	 */
	WControl* GetFocusControl() {
		return (WControl*) w_toolkit_get_focus_control(W_TOOLKIT(this));
	}
	/**
	 * Returns true when the high contrast mode is enabled.
	 * Otherwise, false is returned.
	 * <p>
	 * Note: This operation is a hint and is not supported on
	 * platforms that do not have this concept.
	 * </p>
	 *
	 * @return the high contrast mode
	 */
	bool GetHighContrast() {
		return w_toolkit_get_high_contrast(W_TOOLKIT(this));
	}
	/**
	 * Returns the maximum allowed depth of icons on this display, in bits per pixel.
	 * On some platforms, this may be different than the actual depth of the display.
	 *
	 * @return the maximum icon depth
	 */
	int GetIconDepth() {
		return w_toolkit_get_icon_depth(W_TOOLKIT(this));
	}
	/**
	 * Returns an array containing the recommended icon sizes.
	 *
	 * @return the array of icon sizes
	 */
	size_t GetIconSizes(WSize *sizes, size_t length) {
		return w_toolkit_get_icon_sizes(W_TOOLKIT(this), (w_size*) sizes,
				length);
	}
	/**
	 * Returns a (possibly empty) array containing all shells which have
	 * not been disposed and have the receiver as their display.
	 *
	 * @return the receiver's shells
	 */
	void GetShells(WIterator<WFrame*> &shells) {
		w_toolkit_get_shells(W_TOOLKIT(this), (w_iterator*) &shells);
	}
	/**
	 * Returns the matching standard color for the given
	 * constant, which should be one of the color constants
	 * specified in class <code>SWT</code>. Any value other
	 * than one of the SWT color constants which is passed
	 * in will result in the color black. This color should
	 * not be free'd because it was allocated by the system,
	 * not the application.
	 *
	 * @param id the color constant
	 * @return the matching color
	 *
	 * @see SWT
	 */
	w_color GetSystemColor(int id) {
		return w_toolkit_get_system_color(W_TOOLKIT(this), id);
	}
	/**
	 * Returns the matching standard platform cursor for the given
	 * constant, which should be one of the cursor constants
	 * specified in class <code>SWT</code>. This cursor should
	 * not be free'd because it was allocated by the system,
	 * not the application.  A value of <code>null</code> will
	 * be returned if the supplied constant is not an SWT cursor
	 * constant.
	 *
	 * @param id the SWT cursor constant
	 * @return the corresponding cursor or <code>null</code>
	 *
	 * @see W_CURSOR_ARROW
	 * @see W_CURSOR_WAIT
	 * @see W_CURSOR_CROSS
	 * @see W_CURSOR_APPSTARTING
	 * @see W_CURSOR_HELP
	 * @see W_CURSOR_SIZEALL
	 * @see W_CURSOR_SIZENESW
	 * @see W_CURSOR_SIZENS
	 * @see W_CURSOR_SIZENWSE
	 * @see W_CURSOR_SIZEWE
	 * @see W_CURSOR_SIZEN
	 * @see W_CURSOR_SIZES
	 * @see W_CURSOR_SIZEE
	 * @see W_CURSOR_SIZEW
	 * @see W_CURSOR_SIZENE
	 * @see W_CURSOR_SIZESE
	 * @see W_CURSOR_SIZESW
	 * @see W_CURSOR_SIZENW
	 * @see W_CURSOR_UPARROW
	 * @see W_CURSOR_IBEAM
	 * @see W_CURSOR_NO
	 * @see W_CURSOR_HAND
	 */
	WCursor* GetSystemCursor(int id) {
		return (WCursor*) w_toolkit_get_system_cursor(W_TOOLKIT(this), id);
	}
	/**
	 * Returns a reasonable font for applications to use.
	 * On some platforms, this will match the "default font"
	 * or "system font" if such can be found.  This font
	 * should not be free'd because it was allocated by the
	 * system, not the application.
	 * <p>
	 * Typically, applications which want the default look
	 * should simply not set the font on the widgets they
	 * create. Widgets are always created with the correct
	 * default font for the class of user-interface component
	 * they represent.
	 * </p>
	 *
	 * @return a font
	 */
	WFont* GetSystemFont() {
		return (WFont*) w_toolkit_get_system_font(W_TOOLKIT(this));
	}
	/**
	 * Returns the matching standard platform image for the given
	 * constant, which should be one of the icon constants
	 * specified in class <code>SWT</code>. This image should
	 * not be free'd because it was allocated by the system,
	 * not the application.  A value of <code>null</code> will
	 * be returned either if the supplied constant is not an
	 * SWT icon constant or if the platform does not define an
	 * image that corresponds to the constant.
	 *
	 * @param id the SWT icon constant
	 * @return the corresponding image or <code>null</code>
	 *
	 * @see W_ICON_ERROR
	 * @see W_ICON_INFORMATION
	 * @see W_ICON_QUESTION
	 * @see W_ICON_WARNING
	 * @see W_ICON_WORKING
	 */
	WImage* GetSystemImage(wuint id) {
		w_image *image;
		w_toolkit_get_system_image(W_TOOLKIT(this), id, &image);
		return (WImage*) image;
	}
	/**
	 * Returns the single instance of the system-provided menu for the application, or
	 * <code>null</code> on platforms where no menu is provided for the application.
	 *
	 * @return the system menu, or <code>null</code>
	 */
	WMenu* GetSystemMenu() {
		return (WMenu*) w_toolkit_get_system_menu(W_TOOLKIT(this));
	}
	/**
	 * Returns the single instance of the system taskBar or null
	 * when there is no system taskBar available for the platform.
	 *
	 * @return the system taskBar or <code>null</code>
	 */
	WTaskBar* GetSystemTaskBar() {
		return (WTaskBar*) w_toolkit_get_system_taskbar(W_TOOLKIT(this));
	}
	/**
	 * Returns the single instance of the system tray or null
	 * when there is no system tray available for the platform.
	 *
	 * @return the system tray or <code>null</code>
	 */
	WTray* GetSystemTray() {
		return (WTray*) w_toolkit_get_system_tray(W_TOOLKIT(this));
	}
	WTheme* GetTheme() {
		return (WTheme*) w_toolkit_get_theme((w_toolkit*) this);
	}
	/**
	 * Returns the user-interface thread for the receiver.
	 *
	 * @return the receiver's user-interface thread
	 */
	WThread* GetThread() {
		return (WThread*) w_toolkit_get_thread(W_TOOLKIT(this));
	}
	/**
	 * Returns a boolean indicating whether a touch-aware input device is
	 * attached to the system and is ready for use.
	 *
	 * @return <code>true</code> if a touch-aware input device is detected, or <code>false</code> otherwise
	 */
	bool GetTouchEnabled() {
		return w_toolkit_get_touch_enabled(W_TOOLKIT(this));
	}
	/**
	 * Maps a point from one coordinate system to another.
	 * When the control is null, coordinates are mapped to
	 * the display.
	 * <p>
	 * NOTE: On right-to-left platforms where the coordinate
	 * systems are mirrored, special care needs to be taken
	 * when mapping coordinates from one control to another
	 * to ensure the result is correctly mirrored.
	 *
	 * Mapping a point that is the origin of a rectangle and
	 * then adding the width and height is not equivalent to
	 * mapping the rectangle.  When one control is mirrored
	 * and the other is not, adding the width and height to a
	 * point that was mapped causes the rectangle to extend
	 * in the wrong direction.  Mapping the entire rectangle
	 * instead of just one point causes both the origin and
	 * the corner of the rectangle to be mapped.
	 * </p>
	 *
	 * @param from the source <code>Control</code> or <code>null</code>
	 * @param to the destination <code>Control</code> or <code>null</code>
	 * @param point to be mapped
	 * @return point with mapped coordinates
	 */
	bool Map(WControl *from, WControl *to, WPoint &result,
			const WPoint &point) {
		return w_toolkit_map_0(W_TOOLKIT(this), W_CONTROL(from), W_CONTROL(to),
				(w_point*) &result, (w_point*) &point) > 0;
	}
	/**
	 * Maps a point from one coordinate system to another.
	 * When the control is null, coordinates are mapped to
	 * the display.
	 * <p>
	 * NOTE: On right-to-left platforms where the coordinate
	 * systems are mirrored, special care needs to be taken
	 * when mapping coordinates from one control to another
	 * to ensure the result is correctly mirrored.
	 *
	 * Mapping a point that is the origin of a rectangle and
	 * then adding the width and height is not equivalent to
	 * mapping the rectangle.  When one control is mirrored
	 * and the other is not, adding the width and height to a
	 * point that was mapped causes the rectangle to extend
	 * in the wrong direction.  Mapping the entire rectangle
	 * instead of just one point causes both the origin and
	 * the corner of the rectangle to be mapped.
	 * </p>
	 *
	 * @param from the source <code>Control</code> or <code>null</code>
	 * @param to the destination <code>Control</code> or <code>null</code>
	 * @param x coordinates to be mapped
	 * @param y coordinates to be mapped
	 * @return point with mapped coordinates
	 */
	bool Map(WControl *from, WControl *to, WPoint &result, int x, int y) {
		return Map(from, to, result, WPoint(x, y));
	}
	/**
	 * Maps a point from one coordinate system to another.
	 * When the control is null, coordinates are mapped to
	 * the display.
	 * <p>
	 * NOTE: On right-to-left platforms where the coordinate
	 * systems are mirrored, special care needs to be taken
	 * when mapping coordinates from one control to another
	 * to ensure the result is correctly mirrored.
	 *
	 * Mapping a point that is the origin of a rectangle and
	 * then adding the width and height is not equivalent to
	 * mapping the rectangle.  When one control is mirrored
	 * and the other is not, adding the width and height to a
	 * point that was mapped causes the rectangle to extend
	 * in the wrong direction.  Mapping the entire rectangle
	 * instead of just one point causes both the origin and
	 * the corner of the rectangle to be mapped.
	 * </p>
	 *
	 * @param from the source <code>Control</code> or <code>null</code>
	 * @param to the destination <code>Control</code> or <code>null</code>
	 * @param rectangle to be mapped
	 * @return rectangle with mapped coordinates
	 */
	bool Map(WControl *from, WControl *to, WRect &result,
			const WRect &rectangle) {
		return w_toolkit_map_1(W_TOOLKIT(this), W_CONTROL(from), W_CONTROL(to),
				(w_rect*) &result, (w_rect*) &rectangle) > 0;
	}
	/**
	 * Maps a point from one coordinate system to another.
	 * When the control is null, coordinates are mapped to
	 * the display.
	 * <p>
	 * NOTE: On right-to-left platforms where the coordinate
	 * systems are mirrored, special care needs to be taken
	 * when mapping coordinates from one control to another
	 * to ensure the result is correctly mirrored.
	 *
	 * Mapping a point that is the origin of a rectangle and
	 * then adding the width and height is not equivalent to
	 * mapping the rectangle.  When one control is mirrored
	 * and the other is not, adding the width and height to a
	 * point that was mapped causes the rectangle to extend
	 * in the wrong direction.  Mapping the entire rectangle
	 * instead of just one point causes both the origin and
	 * the corner of the rectangle to be mapped.
	 * </p>
	 *
	 * @param from the source <code>Control</code> or <code>null</code>
	 * @param to the destination <code>Control</code> or <code>null</code>
	 * @param x coordinates to be mapped
	 * @param y coordinates to be mapped
	 * @param width coordinates to be mapped
	 * @param height coordinates to be mapped
	 * @return rectangle with mapped coordinates
	 */
	bool Map(WControl *from, WControl *to, WRect &result, int x, int y,
			int width, int height) {
		return Map(from, to, result, WRect(x, y, width, height));
	}
	/**
	 * Generate a low level system event.
	 *
	 * <code>post</code> is used to generate low level keyboard
	 * and mouse events. The intent is to enable automated UI
	 * testing by simulating the input from the user.  Most
	 * SWT applications should never need to call this method.
	 * <p>
	 * Note that this operation can fail when the operating system
	 * fails to generate the event for any reason.  For example,
	 * this can happen when there is no such key or mouse button
	 * or when the system event queue is full.
	 * </p>
	 * <p>
	 * <b>Event Types:</b>
	 * <p>KeyDown, KeyUp
	 * <p>The following fields in the <code>Event</code> apply:
	 * <ul>
	 * <li>(in) type KeyDown or KeyUp</li>
	 * <p> Either one of:
	 * <li>(in) character a character that corresponds to a keyboard key</li>
	 * <li>(in) keyCode the key code of the key that was typed,
	 *          as defined by the key code constants in class <code>SWT</code></li>
	 * </ul>
	 * <p>MouseDown, MouseUp</p>
	 * <p>The following fields in the <code>Event</code> apply:
	 * <ul>
	 * <li>(in) type MouseDown or MouseUp
	 * <li>(in) button the button that is pressed or released
	 * </ul>
	 * <p>MouseMove</p>
	 * <p>The following fields in the <code>Event</code> apply:
	 * <ul>
	 * <li>(in) type MouseMove
	 * <li>(in) x the x coordinate to move the mouse pointer to in screen coordinates
	 * <li>(in) y the y coordinate to move the mouse pointer to in screen coordinates
	 * </ul>
	 * <p>MouseWheel</p>
	 * <p>The following fields in the <code>Event</code> apply:
	 * <ul>
	 * <li>(in) type MouseWheel
	 * <li>(in) detail either SWT.SCROLL_LINE or SWT.SCROLL_PAGE
	 * <li>(in) count the number of lines or pages to scroll
	 * </ul>
	 * </dl>
	 *
	 * @param event the event to be generated
	 *
	 * @return true if the event was generated or false otherwise
	 *
	 */
	bool Post(WEvent &event) {
		return w_toolkit_post(W_TOOLKIT(this), W_EVENT(&event)) > 0;
	}
	bool PostQuit(int quit) {
		return w_toolkit_post_quit(W_TOOLKIT(this), quit);
	}
	bool PostQuit() {
		return PostQuit(EXIT_SUCCESS);
	}
	/**
	 * Reads an event from the operating system's event queue,
	 * dispatches it appropriately, and returns <code>true</code>
	 * if there is potentially more work to do, or <code>false</code>
	 * if the caller can sleep until another event is placed on
	 * the event queue.
	 * <p>
	 * In addition to checking the system event queue, this method also
	 * checks if any inter-thread messages (created by <code>syncExec()</code>
	 * or <code>asyncExec()</code>) are waiting to be processed, and if
	 * so handles them before returning.
	 * </p>
	 *
	 * @return <code>false</code> if the caller can sleep upon return from this method
	 *
	 * @see #sleep
	 * @see #wake
	 */
	bool ReadAndDispatch() {
		return w_toolkit_read_and_dispatch(W_TOOLKIT(this)) > 0;
	}
	/**
	 * Sets the location of the on-screen pointer relative to the top left corner
	 * of the screen.  <b>Note: It is typically considered bad practice for a
	 * program to move the on-screen pointer location.</b>
	 *
	 * @param x the new x coordinate for the cursor
	 * @param y the new y coordinate for the cursor
	 */
	bool SetCursorLocation(int x, int y) {
		return SetCursorLocation(WPoint(x, y));
	}
	/**
	 * Sets the location of the on-screen pointer relative to the top left corner
	 * of the screen.  <b>Note: It is typically considered bad practice for a
	 * program to move the on-screen pointer location.</b>
	 *
	 * @param point new position
	 */
	bool SetCursorLocation(const WPoint &point) {
		return w_toolkit_set_cursor_location(W_TOOLKIT(this), (w_point*) &point)
				> 0;
	}
	bool SetTheme(WTheme *theme) {
		return w_toolkit_set_theme(W_TOOLKIT(this), (w_theme*) theme) > 0;
	}
	/**
	 * Causes the user-interface thread to <em>sleep</em> (that is,
	 * to be put in a state where it does not consume CPU cycles)
	 * until an event is received or it is otherwise awakened.
	 *
	 * @return <code>true</code> if an event requiring dispatching was placed on the queue.
	 *
	 * @see #wake
	 */
	bool Sleep() {
		return w_toolkit_sleep(W_TOOLKIT(this));
	}
	int Run(WFrame *frame) {
		return w_toolkit_run(W_TOOLKIT(this), W_SHELL(frame));
	}
	int Run() {
		return Run(0);
	}
	/**
	 * Causes the <code>run()</code> method of the runnable to
	 * be invoked by the user-interface thread at the next
	 * reasonable opportunity. The thread which calls this method
	 * is suspended until the runnable completes.  Specifying <code>null</code>
	 * as the runnable simply wakes the user-interface thread.
	 * <p>
	 * Note that at the time the runnable is invoked, widgets
	 * that have the receiver as their display may have been
	 * disposed. Therefore, it is necessary to check for this
	 * case inside the runnable before accessing the widget.
	 * </p>
	 *
	 * @param runnable code to run on the user-interface thread or <code>null</code>
	 *
	 * @see #asyncExec
	 */
	bool SyncExec(w_thread_start function, void *args) {
		return w_toolkit_sync_exec(W_TOOLKIT(this), function, args) > 0;
	}
	/**
	 * Causes the <code>run()</code> method of the runnable to
	 * be invoked by the user-interface thread after the specified
	 * number of milliseconds have elapsed. If milliseconds is less
	 * than zero, the runnable is not executed.
	 * <p>
	 * Note that at the time the runnable is invoked, widgets
	 * that have the receiver as their display may have been
	 * disposed. Therefore, it is necessary to check for this
	 * case inside the runnable before accessing the widget.
	 * </p>
	 *
	 * @param milliseconds the delay before running the runnable
	 * @param runnable code to run on the user-interface thread
	 *
	 * @see #asyncExec
	 */
	bool TimerExec(int milliseconds, w_thread_start function, void *args) {
		return w_toolkit_timer_exec(W_TOOLKIT(this), milliseconds, function,
				args) > 0;
	}
	/**
	 * Forces all outstanding paint requests for the display
	 * to be processed before this method returns.
	 *
	 * @see Control#update()
	 */
	bool Update() {
		return w_toolkit_update(W_TOOLKIT(this)) > 0;
	}
	/**
	 * If the receiver's user-interface thread was <code>sleep</code>ing,
	 * causes it to be awakened and start running again. Note that this
	 * method may be called from any thread.
	 *
	 * @see #sleep
	 */
	bool Wake() {
		return w_toolkit_wake(W_TOOLKIT(this)) > 0;
	}
};

namespace swt {
typedef ::WToolkit Toolkit;
}  // namespace swt

#endif /* SWTP_WIDGETS_TOOLKIT_H_ */
