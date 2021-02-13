/*
 * Shell.h
 *
 *  Created on: 25 juil. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_WIDGETS_SHELL_H_
#define SWTP_WIDGETS_SHELL_H_
#include "Canvas.h"
class WButton;
class WToolBar;
/**
 * Instances of this class represent the "windows"
 * which the desktop or "window manager" is managing.
 * Instances that do not have a parent (that is, they
 * are built using the constructor, which takes a
 * <code>WToolkit</code> as the argument) are described
 * as <em>top level</em> shells. Instances that do have
 * a parent are described as <em>secondary</em> or
 * <em>dialog</em> shells.
 * <p>
 * Instances are always displayed in one of the maximized,
 * minimized or normal states:
 * <ul>
 * <li>
 * When an instance is marked as <em>maximized</em>, the
 * window manager will typically resize it to fill the
 * entire visible area of the display, and the instance
 * is usually put in a state where it can not be resized
 * (even if it has style <code>RESIZE</code>) until it is
 * no longer maximized.
 * </li><li>
 * When an instance is in the <em>normal</em> state (neither
 * maximized or minimized), its appearance is controlled by
 * the style constants which were specified when it was created
 * and the restrictions of the window manager (see below).
 * </li><li>
 * When an instance has been marked as <em>minimized</em>,
 * its contents (client area) will usually not be visible,
 * and depending on the window manager, it may be
 * "iconified" (that is, replaced on the desktop by a small
 * simplified representation of itself), relocated to a
 * distinguished area of the screen, or hidden. Combinations
 * of these changes are also possible.
 * </li>
 * </ul>
 * </p><p>
 * The <em>modality</em> of an instance may be specified using
 * style bits. The modality style bits are used to determine
 * whether input is blocked for other shells on the display.
 * The <code>PRIMARY_MODAL</code> style allows an instance to block
 * input to its parent. The <code>APPLICATION_MODAL</code> style
 * allows an instance to block input to every other shell in the
 * display. The <code>SYSTEM_MODAL</code> style allows an instance
 * to block input to all shells, including shells belonging to
 * different applications.
 * </p><p>
 * Note: The styles supported by this class are treated
 * as <em>HINT</em>s, since the window manager for the
 * desktop on which the instance is visible has ultimate
 * control over the appearance and behavior of decorations
 * and modality. For example, some window managers only
 * support resizable windows and will always assume the
 * RESIZE style, even if it is not set. In addition, if a
 * modality style is not supported, it is "upgraded" to a
 * more restrictive modality style that is supported. For
 * example, if <code>PRIMARY_MODAL</code> is not supported,
 * it would be upgraded to <code>APPLICATION_MODAL</code>.
 * A modality style may also be "downgraded" to a less
 * restrictive style. For example, most operating systems
 * no longer support <code>SYSTEM_MODAL</code> because
 * it can freeze up the desktop, so this is typically
 * downgraded to <code>APPLICATION_MODAL</code>.
 * <dl>
 * <dt><b>Styles:</b></dt>
 * <dd>BORDER, CLOSE, MIN, MAX, NO_MOVE, NO_TRIM, RESIZE, TITLE, ON_TOP, TOOL, SHEET</dd>
 * <dd>APPLICATION_MODAL, MODELESS, PRIMARY_MODAL, SYSTEM_MODAL</dd>
 * <dt><b>Events:</b></dt>
 * <dd>Activate, Close, Deactivate, Deiconify, Iconify</dd>
 * </dl>
 * Class <code>SWT</code> provides two "convenience constants"
 * for the most commonly required style combinations:
 * <dl>
 * <dt><code>SHELL_TRIM</code></dt>
 * <dd>
 * the result of combining the constants which are required
 * to produce a typical application top level shell: (that
 * is, <code>CLOSE | TITLE | MIN | MAX | RESIZE</code>)
 * </dd>
 * <dt><code>DIALOG_TRIM</code></dt>
 * <dd>
 * the result of combining the constants which are required
 * to produce a typical application dialog shell: (that
 * is, <code>TITLE | CLOSE | BORDER</code>)
 * </dd>
 * </dl>
 * </p>
 * <p>
 * Note: Only one of the styles APPLICATION_MODAL, MODELESS,
 * PRIMARY_MODAL and SYSTEM_MODAL may be specified.
 * </p><p>
 * IMPORTANT: This class is <em>not</em> intended to be subclassed.
 * </p>
 */
class SWTP_PUBLIC WShell: public WCanvas {
public:
	WShell() {

	}
	/**
	 * Constructs a new instance of this class. This is equivalent
	 * to calling <code>Shell((WToolkit*) 0)</code>.
	 */
	bool Create() {
		return Create((WToolkit*) 0);
	}
	/**
	 * Constructs a new instance of this class given only the style
	 * value describing its behavior and appearance. This is equivalent
	 * to calling <code>Shell((Display) null, style)</code>.
	 * <p>
	 * The style value is either one of the style constants defined in
	 * class <code>SWT</code> which is applicable to instances of this
	 * class, or must be built by <em>bitwise OR</em>'ing together
	 * (that is, using the <code>int</code> "|" operator) two or more
	 * of those <code>SWT</code> style constants. The class description
	 * lists the style constants that are applicable to the class.
	 * Style bits are also inherited from superclasses.
	 * </p>
	 *
	 * @param style the style of control to construct
	 *
	 * @see SWT#BORDER
	 * @see SWT#CLOSE
	 * @see SWT#MIN
	 * @see SWT#MAX
	 * @see SWT#RESIZE
	 * @see SWT#TITLE
	 * @see SWT#TOOL
	 * @see SWT#NO_TRIM
	 * @see SWT#NO_MOVE
	 * @see SWT#SHELL_TRIM
	 * @see SWT#DIALOG_TRIM
	 * @see SWT#ON_TOP
	 * @see SWT#MODELESS
	 * @see SWT#PRIMARY_MODAL
	 * @see SWT#APPLICATION_MODAL
	 * @see SWT#SYSTEM_MODAL
	 * @see SWT#SHEET
	 */
	bool Create(wuint64 style) {
		return WControl::Create((WToolkit*) 0, 0, style);
	}
	/**
	 * Constructs a new instance of this class given only the style
	 * value describing its behavior and appearance. This is equivalent
	 * to calling <code>Shell((Display) null, style)</code>.
	 * <p>
	 * The style value is either one of the style constants defined in
	 * class <code>SWT</code> which is applicable to instances of this
	 * class, or must be built by <em>bitwise OR</em>'ing together
	 * (that is, using the <code>int</code> "|" operator) two or more
	 * of those <code>SWT</code> style constants. The class description
	 * lists the style constants that are applicable to the class.
	 * Style bits are also inherited from superclasses.
	 * </p>
	 *
	 * @param style the style of control to construct
	 *
	 * @see SWT#BORDER
	 * @see SWT#CLOSE
	 * @see SWT#MIN
	 * @see SWT#MAX
	 * @see SWT#RESIZE
	 * @see SWT#TITLE
	 * @see SWT#TOOL
	 * @see SWT#NO_TRIM
	 * @see SWT#NO_MOVE
	 * @see SWT#SHELL_TRIM
	 * @see SWT#DIALOG_TRIM
	 * @see SWT#ON_TOP
	 * @see SWT#MODELESS
	 * @see SWT#PRIMARY_MODAL
	 * @see SWT#APPLICATION_MODAL
	 * @see SWT#SYSTEM_MODAL
	 * @see SWT#SHEET
	 */
	WShell(wuint64 style) :
			WCanvas() {
		Create(style | W_FREE_MEMORY);
	}
	/**
	 * Constructs a new instance of this class given only the display
	 * to create it on. It is created with style <code>SWT.SHELL_TRIM</code>.
	 * <p>
	 * Note: Currently, null can be passed in for the display argument.
	 * This has the effect of creating the shell on the currently active
	 * display if there is one. If there is no current display, the
	 * shell is created on a "default" display. <b>Passing in null as
	 * the display argument is not considered to be good coding style,
	 * and may not be supported in a future release of SWT.</b>
	 * </p>
	 *
	 * @param display the display to create the shell on
	 */
	bool Create(WToolkit *toolkit) {
		return Create(toolkit, W_SHELL_TRIM);
	}
	/**
	 * Constructs a new instance of this class given only the display
	 * to create it on. It is created with style <code>SWT.SHELL_TRIM</code>.
	 * <p>
	 * Note: Currently, null can be passed in for the display argument.
	 * This has the effect of creating the shell on the currently active
	 * display if there is one. If there is no current display, the
	 * shell is created on a "default" display. <b>Passing in null as
	 * the display argument is not considered to be good coding style,
	 * and may not be supported in a future release of SWT.</b>
	 * </p>
	 *
	 * @param display the display to create the shell on
	 */
	WShell(WToolkit *toolkit) :
			WCanvas() {
		Create(toolkit, W_SHELL_TRIM | W_FREE_MEMORY);
	}
	/**
	 * Constructs a new instance of this class given the display
	 * to create it on and a style value describing its behavior
	 * and appearance.
	 * <p>
	 * The style value is either one of the style constants defined in
	 * class <code>SWT</code> which is applicable to instances of this
	 * class, or must be built by <em>bitwise OR</em>'ing together
	 * (that is, using the <code>int</code> "|" operator) two or more
	 * of those <code>SWT</code> style constants. The class description
	 * lists the style constants that are applicable to the class.
	 * Style bits are also inherited from superclasses.
	 * </p><p>
	 * Note: Currently, null can be passed in for the display argument.
	 * This has the effect of creating the shell on the currently active
	 * display if there is one. If there is no current display, the
	 * shell is created on a "default" display. <b>Passing in null as
	 * the display argument is not considered to be good coding style,
	 * and may not be supported in a future release of SWT.</b>
	 * </p>
	 *
	 * @param display the display to create the shell on
	 * @param style the style of control to construct
	 *
	 * @see SWT#BORDER
	 * @see SWT#CLOSE
	 * @see SWT#MIN
	 * @see SWT#MAX
	 * @see SWT#RESIZE
	 * @see SWT#TITLE
	 * @see SWT#TOOL
	 * @see SWT#NO_TRIM
	 * @see SWT#NO_MOVE
	 * @see SWT#SHELL_TRIM
	 * @see SWT#DIALOG_TRIM
	 * @see SWT#ON_TOP
	 * @see SWT#MODELESS
	 * @see SWT#PRIMARY_MODAL
	 * @see SWT#APPLICATION_MODAL
	 * @see SWT#SYSTEM_MODAL
	 * @see SWT#SHEET
	 */
	bool Create(WToolkit *toolkit, wuint64 style) {
		return WControl::Create(toolkit, 0, style);
	}
	/**
	 * Constructs a new instance of this class given the display
	 * to create it on and a style value describing its behavior
	 * and appearance.
	 * <p>
	 * The style value is either one of the style constants defined in
	 * class <code>SWT</code> which is applicable to instances of this
	 * class, or must be built by <em>bitwise OR</em>'ing together
	 * (that is, using the <code>int</code> "|" operator) two or more
	 * of those <code>SWT</code> style constants. The class description
	 * lists the style constants that are applicable to the class.
	 * Style bits are also inherited from superclasses.
	 * </p><p>
	 * Note: Currently, null can be passed in for the display argument.
	 * This has the effect of creating the shell on the currently active
	 * display if there is one. If there is no current display, the
	 * shell is created on a "default" display. <b>Passing in null as
	 * the display argument is not considered to be good coding style,
	 * and may not be supported in a future release of SWT.</b>
	 * </p>
	 *
	 * @param display the display to create the shell on
	 * @param style the style of control to construct
	 *
	 * @see SWT#BORDER
	 * @see SWT#CLOSE
	 * @see SWT#MIN
	 * @see SWT#MAX
	 * @see SWT#RESIZE
	 * @see SWT#TITLE
	 * @see SWT#TOOL
	 * @see SWT#NO_TRIM
	 * @see SWT#NO_MOVE
	 * @see SWT#SHELL_TRIM
	 * @see SWT#DIALOG_TRIM
	 * @see SWT#ON_TOP
	 * @see SWT#MODELESS
	 * @see SWT#PRIMARY_MODAL
	 * @see SWT#APPLICATION_MODAL
	 * @see SWT#SYSTEM_MODAL
	 * @see SWT#SHEET
	 */
	WShell(WToolkit *toolkit, wuint64 style) :
			WCanvas() {
		Create(toolkit, style);
	}
	/**
	 * Constructs a new instance of this class given only its
	 * parent. It is created with style <code>SWT.DIALOG_TRIM</code>.
	 * <p>
	 * Note: Currently, null can be passed in for the parent.
	 * This has the effect of creating the shell on the currently active
	 * display if there is one. If there is no current display, the
	 * shell is created on a "default" display. <b>Passing in null as
	 * the parent is not considered to be good coding style,
	 * and may not be supported in a future release of SWT.</b>
	 * </p>
	 *
	 * @param parent a shell which will be the parent of the new instance
	 *
	 */
	bool Create(WShell *parent) {
		return Create(parent, W_DIALOG_TRIM);
	}
	/**
	 * Constructs a new instance of this class given only its
	 * parent. It is created with style <code>SWT.DIALOG_TRIM</code>.
	 * <p>
	 * Note: Currently, null can be passed in for the parent.
	 * This has the effect of creating the shell on the currently active
	 * display if there is one. If there is no current display, the
	 * shell is created on a "default" display. <b>Passing in null as
	 * the parent is not considered to be good coding style,
	 * and may not be supported in a future release of SWT.</b>
	 * </p>
	 *
	 * @param parent a shell which will be the parent of the new instance
	 */
	WShell(WShell *parent) :
			WCanvas() {
		Create(parent, W_DIALOG_TRIM);
	}
	/**
	 * Constructs a new instance of this class given its parent
	 * and a style value describing its behavior and appearance.
	 * <p>
	 * The style value is either one of the style constants defined in
	 * class <code>SWT</code> which is applicable to instances of this
	 * class, or must be built by <em>bitwise OR</em>'ing together
	 * (that is, using the <code>int</code> "|" operator) two or more
	 * of those <code>SWT</code> style constants. The class description
	 * lists the style constants that are applicable to the class.
	 * Style bits are also inherited from superclasses.
	 * </p><p>
	 * Note: Currently, null can be passed in for the parent.
	 * This has the effect of creating the shell on the currently active
	 * display if there is one. If there is no current display, the
	 * shell is created on a "default" display. <b>Passing in null as
	 * the parent is not considered to be good coding style,
	 * and may not be supported in a future release of SWT.</b>
	 * </p>
	 *
	 * @param parent a shell which will be the parent of the new instance
	 * @param style the style of control to construct
	 *
	 * @see SWT#BORDER
	 * @see SWT#CLOSE
	 * @see SWT#MIN
	 * @see SWT#MAX
	 * @see SWT#RESIZE
	 * @see SWT#TITLE
	 * @see SWT#NO_TRIM
	 * @see SWT#NO_MOVE
	 * @see SWT#SHELL_TRIM
	 * @see SWT#DIALOG_TRIM
	 * @see SWT#ON_TOP
	 * @see SWT#TOOL
	 * @see SWT#MODELESS
	 * @see SWT#PRIMARY_MODAL
	 * @see SWT#APPLICATION_MODAL
	 * @see SWT#SYSTEM_MODAL
	 * @see SWT#SHEET
	 */
	bool Create(WShell *parent, wuint64 style) {
		return WControl::Create(0, parent, style);
	}
	/**
	 * Constructs a new instance of this class given its parent
	 * and a style value describing its behavior and appearance.
	 * <p>
	 * The style value is either one of the style constants defined in
	 * class <code>SWT</code> which is applicable to instances of this
	 * class, or must be built by <em>bitwise OR</em>'ing together
	 * (that is, using the <code>int</code> "|" operator) two or more
	 * of those <code>SWT</code> style constants. The class description
	 * lists the style constants that are applicable to the class.
	 * Style bits are also inherited from superclasses.
	 * </p><p>
	 * Note: Currently, null can be passed in for the parent.
	 * This has the effect of creating the shell on the currently active
	 * display if there is one. If there is no current display, the
	 * shell is created on a "default" display. <b>Passing in null as
	 * the parent is not considered to be good coding style,
	 * and may not be supported in a future release of SWT.</b>
	 * </p>
	 *
	 * @param parent a shell which will be the parent of the new instance
	 * @param style the style of control to construct
	 *
	 * @see SWT#BORDER
	 * @see SWT#CLOSE
	 * @see SWT#MIN
	 * @see SWT#MAX
	 * @see SWT#RESIZE
	 * @see SWT#TITLE
	 * @see SWT#NO_TRIM
	 * @see SWT#NO_MOVE
	 * @see SWT#SHELL_TRIM
	 * @see SWT#DIALOG_TRIM
	 * @see SWT#ON_TOP
	 * @see SWT#TOOL
	 * @see SWT#MODELESS
	 * @see SWT#PRIMARY_MODAL
	 * @see SWT#APPLICATION_MODAL
	 * @see SWT#SYSTEM_MODAL
	 * @see SWT#SHEET
	 */
	WShell(WShell *parent, wuint64 style) :
			WCanvas() {
		Create(parent, style);
	}
	/**
	 * Requests that the window manager close the receiver in
	 * the same way it would be closed when the user clicks on
	 * the "close box" or performs some other platform specific
	 * key or mouse combination that indicates the window
	 * should be removed.
	 *
	 * @see SWT#Close
	 * @see #dispose
	 */
	void Close() {
		_close();
	}
	/**
	 * Returns a ToolBar object representing the tool bar that can be shown in the receiver's
	 * trim. This will return <code>null</code> if the platform does not support tool bars that
	 * are not part of the content area of the shell, or if the Shell's style does not support
	 * having a tool bar.
	 * <p>
	 *
	 * @return a ToolBar object representing the Shell's tool bar, or <code>null</code>.
	 *
	 */
	WToolBar* GetToolBar() {
		WToolBar *toolbar;
		_get_toolbar(&toolbar);
		return toolbar;
	}

	/**
	 * Returns the receiver's alpha value. The alpha value
	 * is between 0 (transparent) and 255 (opaque).
	 *
	 * @return the alpha value
	 *
	 */
	int GetAlpha() {
		return _WReturnInt(_get_alpha());
	}

	/**
	 * Returns the receiver's default button if one had
	 * previously been set, otherwise returns null.
	 *
	 * @return the default button or null
	 *
	 * @see #setDefaultButton(Button)
	 */
	WButton* GetDefaultButton() {
		WButton *button;
		_get_default_button(&button);
		return button;
	}

	/**
	 * Returns <code>true</code> if the receiver is currently
	 * in fullscreen state, and false otherwise.
	 * <p>
	 *
	 * @return the fullscreen state
	 *
	 */
	bool GetFullScreen() {
		return w_shell_get_full_screen(W_SHELL(this)) > 0;
	}
	/**
	 * Returns the receiver's image if it had previously been
	 * set using <code>setImage()</code>. The image is typically
	 * displayed by the window manager when the instance is
	 * marked as iconified, and may also be displayed somewhere
	 * in the trim when the instance is in normal or maximized
	 * states.
	 * <p>
	 * Note: This method will return null if called before
	 * <code>setImage()</code> is called. It does not provide
	 * access to a window manager provided, "default" image
	 * even if one exists.
	 * </p>
	 *
	 * @return the image
	 */
	bool GetImage(WImage &image) {
		return GetImages(&image, 1) > 1;
	}
	/**
	 * Returns the receiver's images if they had previously been
	 * set using <code>setImages()</code>. Images are typically
	 * displayed by the window manager when the instance is
	 * marked as iconified, and may also be displayed somewhere
	 * in the trim when the instance is in normal or maximized
	 * states. Depending where the icon is displayed, the platform
	 * chooses the icon with the "best" attributes.  It is expected
	 * that the array will contain the same icon rendered at different
	 * sizes, with different depth and transparency attributes.
	 *
	 * <p>
	 * Note: This method will return an empty array if called before
	 * <code>setImages()</code> is called. It does not provide
	 * access to a window manager provided, "default" image
	 * even if one exists.
	 * </p>
	 *
	 * @return the images
	 */
	WResult GetImages(WImage *images, size_t length) {
		return _get_images(images, length);
	}
	/**
	 * Returns <code>true</code> if the receiver is currently
	 * maximized, and false otherwise.
	 * <p>
	 *
	 * @return the maximized state
	 *
	 * @see #setMaximized
	 */
	bool GetMaximized() {
		return _get_maximized() > 0;
	}
	/**
	 * Returns the receiver's menu bar if one had previously
	 * been set, otherwise returns null.
	 *
	 * @return the menu bar or null
	 */
	WMenu* GetMenuBar() {
		WMenu *menu;
		_get_menu_bar(&menu);
		return menu;
	}
	/**
	 * Returns <code>true</code> if the receiver is currently
	 * minimized, and false otherwise.
	 * <p>
	 *
	 * @return the minimized state
	 *
	 * @see #setMinimized
	 */
	bool GetMinimized() {
		return _get_minimized() > 0;
	}
	/**
	 * Returns a point describing the minimum receiver's size. The
	 * x coordinate of the result is the minimum width of the receiver.
	 * The y coordinate of the result is the minimum height of the
	 * receiver.
	 *
	 * @return the receiver's size
	 *
	 */
	void GetMinimumSize(WSize &size) {
		_get_minimum_size(size);
	}
	/**
	 * Returns a point describing the minimum receiver's size. The
	 * x coordinate of the result is the minimum width of the receiver.
	 * The y coordinate of the result is the minimum height of the
	 * receiver.
	 *
	 * @return the receiver's size
	 *
	 */
	WSize GetMinimumSize() {
		WSize sz;
		GetMinimumSize(sz);
		return sz;
	}
	/**
	 * Gets the receiver's modified state.
	 *
	 * @return <code>true</code> if the receiver is marked as modified, or <code>false</code> otherwise
	 *
	 */
	bool GetModified() {
		return _get_modified() > 0;
	}
	/**
	 * Returns the receiver's input method editor mode. This
	 * will be the result of bitwise OR'ing together one or
	 * more of the following constants defined in class
	 * <code>SWT</code>:
	 * <code>NONE</code>, <code>ROMAN</code>, <code>DBCS</code>,
	 * <code>PHONETIC</code>, <code>NATIVE</code>, <code>ALPHA</code>.
	 *
	 * @return the IME mode
	 *
	 * @see SWT
	 */
	int GetImeInputMode() {
		return _WReturnInt(_get_ime_input_mode());
	}
	/**
	 * Returns an array containing all shells which are
	 * descendants of the receiver.
	 * <p>
	 * @return the dialog shells
	 */
	bool GetShells(WIterator<WShell*> &shells) {
		return _get_shells(shells) > 0;
	}
	/**
	 * Returns the receiver's text, which is the string that the
	 * window manager will typically display as the receiver's
	 * <em>title</em>. If the text has not previously been set,
	 * returns an empty string.
	 *
	 * @return the text
	 */
	WString GetText() {
		WString str;
		_get_text(w_alloc_string_ref, &str.ref, W_ENCODING_UTF8);
		return str;
	}
	/**
	 * Returns the receiver's text, which is the string that the
	 * window manager will typically display as the receiver's
	 * <em>title</em>. If the text has not previously been set,
	 * returns an empty string.
	 *
	 * @return the text
	 */
	std::string GetTextStd() {
		std::string str;
		_get_text(w_alloc_std_string, &str, W_ENCODING_UTF8);
		return std::move(str);
	}
	/**
	 * Moves the receiver to the top of the drawing order for
	 * the display on which it was created (so that all other
	 * shells on that display, which are not the receiver's
	 * children will be drawn behind it), marks it visible,
	 * sets the focus and asks the window manager to make the
	 * shell active.
	 *
	 * @see Control#moveAbove
	 * @see Control#setFocus
	 * @see Control#setVisible
	 * @see Display#getActiveShell
	 * @see Decorations#setDefaultButton(Button)
	 * @see Shell#setActive
	 * @see Shell#forceActive
	 */
	bool Open() {
		return _open() > 0;
	}
	/**
	 * If the receiver is visible, moves it to the top of the
	 * drawing order for the display on which it was created
	 * (so that all other shells on that display, which are not
	 * the receiver's children will be drawn behind it) and asks
	 * the window manager to make the shell active
	 *
	 * @see Control#moveAbove
	 * @see Control#setFocus
	 * @see Control#setVisible
	 * @see Display#getActiveShell
	 * @see Decorations#setDefaultButton(Button)
	 * @see Shell#open
	 * @see Shell#setActive
	 */
	bool SetActive() {
		return _set_active() > 0;
	}
	/**
	 * Sets the receiver's alpha value which must be
	 * between 0 (transparent) and 255 (opaque).
	 * <p>
	 * This operation requires the operating system's advanced
	 * widgets subsystem which may not be available on some
	 * platforms.
	 * </p>
	 * @param alpha the alpha value
	 */
	bool SetAlpha(int alpha) {
		return _set_alpha(alpha) > 0;
	}
	/**
	 * If the argument is not null, sets the receiver's default
	 * button to the argument, and if the argument is null, sets
	 * the receiver's default button to the first button which
	 * was set as the receiver's default button (called the
	 * <em>saved default button</em>). If no default button had
	 * previously been set, or the saved default button was
	 * disposed, the receiver's default button will be set to
	 * null.
	 * <p>
	 * The default button is the button that is selected when
	 * the receiver is active and the user presses ENTER.
	 * </p>
	 *
	 * @param button the new default button
	 */
	bool SetDefaultButton(WButton *button) {
		return _set_default_button(button) > 0;
	}
	/**
	 * Sets the full screen state of the receiver.
	 * If the argument is <code>true</code> causes the receiver
	 * to switch to the full screen state, and if the argument is
	 * <code>false</code> and the receiver was previously switched
	 * into full screen state, causes the receiver to switch back
	 * to either the maximized or normal states.
	 * <p>
	 * Note: The result of intermixing calls to <code>setFullScreen(true)</code>,
	 * <code>setMaximized(true)</code> and <code>setMinimized(true)</code> will
	 * vary by platform. Typically, the behavior will match the platform user's
	 * expectations, but not always. This should be avoided if possible.
	 * </p>
	 *
	 * @param fullScreen the new fullscreen state
	 */
	bool SetFullScreen(bool fullScreen) {
		return _set_full_screen(fullScreen) > 0;
	}
	/**
	 * Sets the receiver's image to the argument, which may
	 * be null. The image is typically displayed by the window
	 * manager when the instance is marked as iconified, and
	 * may also be displayed somewhere in the trim when the
	 * instance is in normal or maximized states.
	 *
	 * @param image the new image (or null)
	 */
	bool SetImage(WImage &image) {
		return setImages(&image, 1);
	}
	/**
	 * Sets the receiver's images to the argument, which may
	 * be an empty array. Images are typically displayed by the
	 * window manager when the instance is marked as iconified,
	 * and may also be displayed somewhere in the trim when the
	 * instance is in normal or maximized states. Depending where
	 * the icon is displayed, the platform chooses the icon with
	 * the "best" attributes. It is expected that the array will
	 * contain the same icon rendered at different sizes, with
	 * different depth and transparency attributes.
	 *
	 * @param images the new image array
	 */
	WResult setImages(WImage *images, size_t length) {
		return _set_images(images, length);
	}
	/**
	 * Sets the input method editor mode to the argument which
	 * should be the result of bitwise OR'ing together one or more
	 * of the following constants defined in class <code>SWT</code>:
	 * <code>NONE</code>, <code>ROMAN</code>, <code>DBCS</code>,
	 * <code>PHONETIC</code>, <code>NATIVE</code>, <code>ALPHA</code>.
	 *
	 * @param mode the new IME mode
	 *
	 * @see SWT
	 */
	bool SetImeInputMode(int mode) {
		return _set_ime_input_mode(mode) > 0;
	}
	/**
	 * Sets the maximized state of the receiver.
	 * If the argument is <code>true</code> causes the receiver
	 * to switch to the maximized state, and if the argument is
	 * <code>false</code> and the receiver was previously maximized,
	 * causes the receiver to switch back to either the minimized
	 * or normal states.
	 * <p>
	 * Note: The result of intermixing calls to <code>setMaximized(true)</code>
	 * and <code>setMinimized(true)</code> will vary by platform. Typically,
	 * the behavior will match the platform user's expectations, but not
	 * always. This should be avoided if possible.
	 * </p>
	 *
	 * @param maximized the new maximized state
	 *
	 * @see #setMinimized
	 */
	bool SetMaximized(bool maximized) {
		return _set_maximized(maximized) > 0;
	}
	/**
	 * Sets the receiver's menu bar to the argument, which
	 * may be null.
	 *
	 * @param menu the new menu bar
	 */
	bool SetMenuBar(WMenuBase *menu) {
		return _set_menu_bar(menu) > 0;
	}
	/**
	 * Sets the minimized stated of the receiver.
	 * If the argument is <code>true</code> causes the receiver
	 * to switch to the minimized state, and if the argument is
	 * <code>false</code> and the receiver was previously minimized,
	 * causes the receiver to switch back to either the maximized
	 * or normal states.
	 * <p>
	 * Note: The result of intermixing calls to <code>setMaximized(true)</code>
	 * and <code>setMinimized(true)</code> will vary by platform. Typically,
	 * the behavior will match the platform user's expectations, but not
	 * always. This should be avoided if possible.
	 * </p>
	 *
	 * @param minimized the new maximized state
	 *
	 * @see #setMaximized
	 */
	bool SetMinimized(bool minimized) {
		return _set_minimized(minimized) > 0;
	}
	/**
	 * Sets the receiver's minimum size to the size specified by the arguments.
	 * If the new minimum size is larger than the current size of the receiver,
	 * the receiver is resized to the new minimum size.
	 *
	 * @param width the new minimum width for the receiver
	 * @param height the new minimum height for the receiver
	 */
	bool SetMinimumSize(int width, int height) {
		return SetMinimumSize(WSize(width, height));
	}
	/**
	 * Sets the receiver's minimum size to the size specified by the argument.
	 * If the new minimum size is larger than the current size of the receiver,
	 * the receiver is resized to the new minimum size.
	 *
	 * @param size the new minimum size for the receiver
	 */
	bool SetMinimumSize(const WSize &size) {
		return _set_minimum_size(size) > 0;
	}
	/**
	 * Sets the receiver's modified state as specified by the argument.
	 *
	 * @param modified the new modified state for the receiver
	 *
	 */
	bool SetModified(bool modified) {
		return _set_modified(modified) > 0;
	}
	/**
	 * Sets the receiver's text, which is the string that the
	 * window manager will typically display as the receiver's
	 * <em>title</em>, to the argument, which must not be null.
	 * <p>
	 * Note: If control characters like '\n', '\t' etc. are used
	 * in the string, then the behavior is platform dependent.
	 * </p>
	 *
	 * @param string the new text
	 */
	bool SetText(const char *text, size_t length) {
		return _set_text(text, -1, W_ENCODING_UTF8) > 0;
	}
	/**
	 * Sets the receiver's text, which is the string that the
	 * window manager will typically display as the receiver's
	 * <em>title</em>, to the argument, which must not be null.
	 * <p>
	 * Note: If control characters like '\n', '\t' etc. are used
	 * in the string, then the behavior is platform dependent.
	 * </p>
	 *
	 * @param string the new text
	 */
	bool SetText(const char *text) {
		return SetText(text, -1);
	}
	/**
	 * If the receiver is visible, moves it to the top of the
	 * drawing order for the display on which it was created
	 * (so that all other shells on that display, which are not
	 * the receiver's children will be drawn behind it) and forces
	 * the window manager to make the shell active.
	 *
	 * @see Control#moveAbove
	 * @see Control#setFocus
	 * @see Control#setVisible
	 * @see Display#getActiveShell
	 * @see Decorations#setDefaultButton(Button)
	 * @see Shell#open
	 * @see Shell#setActive
	 */
	bool ForceActive() {
		return _force_active() > 0;
	}
public:
	static bool IsShell(WWidget *widget) {
		return widget->GetClassId() >= _W_CLASS_SHELL;
	}
	static WShell* ToShell(WWidget *widget) {
		if (IsShell(widget))
			return (WShell*) widget;
		else
			return 0;
	}
protected:
	w_class_id _GetClassID();
	bool PostEvent(WEvent *e);
	virtual bool OnClose(WEvent &e);
	virtual bool OnIconify(WEvent &e);
	virtual bool OnDeiconify(WEvent &e);
	virtual bool OnActivate(WEvent &e);
	virtual bool OnDeactivate(WEvent &e);
	virtual bool OnNotify(WEvent &e);
public:
	WResult _create(WToolkit *toolkit, WShell *parent, wuint64 style,
			w_widget_post_event_proc post_event) {
		return w_shell_create(W_SHELL(this), (w_toolkit*) toolkit,
				W_SHELL(parent), style, post_event_proc);
	}
	WResult _create(WToolkit *toolkit, WShell *parent, wuint64 style) {
		return _create(toolkit, parent, style, WWidget::post_event_proc);
	}
	WResult _close() {
		return w_shell_close(W_SHELL(this));
	}
	WResult _get_toolbar(WToolBar **toolbar) {
		return w_shell_get_toolbar(W_SHELL(this), (w_toolbar**) toolbar);
	}
	WResult _get_alpha() {
		return w_shell_get_alpha(W_SHELL(this));
	}
	WResult _get_default_button(WButton **button) {
		return w_shell_get_default_button(W_SHELL(this), (w_button**) button);
	}
	WResult _get_full_screen() {
		return w_shell_get_full_screen(W_SHELL(this));
	}
	WResult _get_images(WImage *images, size_t length) {
		return w_shell_get_images(W_SHELL(this), (w_image*) images, length);
	}
	WResult _get_maximized() {
		return w_shell_get_maximized(W_SHELL(this));
	}
	WResult _get_menu_bar(WMenu **menu) {
		return w_shell_get_menu_bar(W_SHELL(this), (w_menu**) menu);
	}
	WResult _get_minimized() {
		return w_shell_get_minimized(W_SHELL(this));
	}
	WResult _get_minimum_size(WSize &size) {
		return w_shell_get_minimum_size(W_SHELL(this), (w_size*) &size);
	}
	WResult _get_modified() {
		return w_shell_get_modified(W_SHELL(this));
	}
	WResult _get_ime_input_mode() {
		return w_shell_get_ime_input_mode(W_SHELL(this));
	}
	WResult _get_shells(WIterator<WShell*> &shells) {
		return w_shell_get_shells(W_SHELL(this), (w_iterator*) &shells);
	}
	WResult _get_text(w_alloc alloc, void *user_data, int enc) {
		return w_shell_get_text(W_SHELL(this), alloc, user_data, enc);
	}
	WResult _open() {
		return w_shell_open(W_SHELL(this));
	}

	WResult _set_active() {
		return w_shell_set_active(W_SHELL(this));
	}
	WResult _set_alpha(int alpha) {
		return w_shell_set_alpha(W_SHELL(this), alpha);
	}
	WResult _set_default_button(WButton *button) {
		return w_shell_set_default_button(W_SHELL(this), W_BUTTON(button));
	}
	WResult _set_full_screen(bool fullScreen) {
		return w_shell_set_full_screen(W_SHELL(this), fullScreen);
	}
	WResult _set_images(WImage *images, size_t length) {
		return w_shell_set_images(W_SHELL(this), W_IMAGE(images), length);
	}
	WResult _set_ime_input_mode(int mode) {
		return w_shell_set_ime_input_mode(W_SHELL(this), mode);
	}
	WResult _set_maximized(bool maximized) {
		return w_shell_set_maximized(W_SHELL(this), maximized);
	}
	WResult _set_menu_bar(WMenuBase *menu) {
		return w_shell_set_menu_bar(W_SHELL(this), W_MENU(menu));
	}
	WResult _set_minimized(bool minimized) {
		return w_shell_set_minimized(W_SHELL(this), minimized);
	}
	WResult _set_minimum_size(const WSize &size) {
		return w_shell_set_minimum_size(W_SHELL(this), (w_size*) &size);
	}
	WResult _set_modified(bool modified) {
		return w_shell_set_modified(W_SHELL(this), modified);
	}
	WResult _set_text(const char *text, size_t length, int enc) {
		return w_shell_set_text(W_SHELL(this), text, length, enc);
	}
	WResult _force_active() {
		return w_shell_force_active(W_SHELL(this));
	}
private:
	void *handles[(sizeof(w_shell) - sizeof(w_canvas)) / sizeof(void*)];
};

namespace swt {
typedef ::WShell Shell;
}  // namespace swt

#endif /* SWTP_WIDGETS_SHELL_H_ */
