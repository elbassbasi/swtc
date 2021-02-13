/*
 * Canvas.h
 *
 *  Created on: 25 juil. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_WIDGETS_CANVAS_H_
#define SWTP_WIDGETS_CANVAS_H_
#include "Composite.h"
class WCaret;
class WIME;
class WImeEvent: public WEvent {
public:
	int detail;
	WRange range;
	const char *text;
};
/**
 * Instances of this class provide a surface for drawing
 * arbitrary graphics.
 * <dl>
 * <dt><b>Styles:</b></dt>
 * <dd>(none)</dd>
 * <dt><b>Events:</b></dt>
 * <dd>(none)</dd>
 * </dl>
 * <p>
 * This class may be subclassed by custom control implementors
 * who are building controls that are <em>not</em> constructed
 * from aggregates of other controls. That is, they are either
 * painted using SWT graphics calls or are handled by native
 * methods.
 * </p>
 *
 * @see Composite
 */
class SWTP_PUBLIC WCanvas: public WComposite {
public:
	WCanvas() {
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
	 * </p>
	 *
	 * @param parent a composite control which will be the parent of the new instance (cannot be null)
	 * @param style the style of control to construct
	 * @see Widget#checkSubclass
	 * @see Widget#getStyle
	 */
	WCanvas(WComposite *parent, wuint64 style) {
		Create(parent, style);
	}
	using WComposite::DrawBackground;
	/**
	 * Fills the interior of the rectangle specified by the arguments,
	 * with the receiver's background.
	 *
	 * @param gc the gc where the rectangle is to be filled
	 * @param x the x coordinate of the rectangle to be filled
	 * @param y the y coordinate of the rectangle to be filled
	 * @param width the width of the rectangle to be filled
	 * @param height the height of the rectangle to be filled
	 */
	void DrawBackground(WGraphics &gc, int x, int y, int width, int height) {
		DrawBackground(gc, x, y, width, height, 0, 0);
	}
	void DrawBackground(WGraphics &gc, const WRect &rect) {
		DrawBackground(gc, rect, WPoint(0, 0));
	}
	/**
	 * Returns the caret.
	 * <p>
	 * The caret for the control is automatically hidden
	 * and shown when the control is painted or resized,
	 * when focus is gained or lost and when an the control
	 * is scrolled.  To avoid drawing on top of the caret,
	 * the programmer must hide and show the caret when
	 * drawing in the window any other time.
	 * </p>
	 *
	 * @return the caret for the receiver, may be null
	 */
	WCaret* GetCaret() {
		WCaret *caret;
		_get_caret(&caret);
		return caret;
	}
	/**
	 * Returns the IME.
	 *
	 * @return the IME
	 */
	WIME* GetIME() {
		WIME *ime;
		_get_ime(&ime);
		return ime;
	}
	/**
	 * Scrolls a rectangular area of the receiver by first copying
	 * the source area to the destination and then causing the area
	 * of the source which is not covered by the destination to
	 * be repainted. Children that intersect the rectangle are
	 * optionally moved during the operation. In addition, all outstanding
	 * paint events are flushed before the source area is copied to
	 * ensure that the contents of the canvas are drawn correctly.
	 *
	 * @param destX the x coordinate of the destination
	 * @param destY the y coordinate of the destination
	 * @param x the x coordinate of the source
	 * @param y the y coordinate of the source
	 * @param width the width of the area
	 * @param height the height of the area
	 * @param all <code>true</code>if children should be scrolled, and <code>false</code> otherwise
	 */
	bool Scroll(int destX, int destY, int x, int y, int width, int height,
			bool all) {
		return Scroll(WPoint(destX, destY), WRect(x, y, width, height), all);
	}
	bool Scroll(const WPoint &dest, const WRect &rect, bool all) {
		return _scroll(dest, rect, all) > 0;
	}
	/**
	 * Sets the receiver's caret.
	 * <p>
	 * The caret for the control is automatically hidden
	 * and shown when the control is painted or resized,
	 * when focus is gained or lost and when an the control
	 * is scrolled.  To avoid drawing on top of the caret,
	 * the programmer must hide and show the caret when
	 * drawing in the window any other time.
	 * </p>
	 * @param caret the new caret for the receiver, may be null
	 */
	bool SetCaret(WCaret *caret) {
		return _set_caret(caret) > 0;
	}
	/**
	 * Sets the receiver's IME.
	 *
	 * @param ime the new IME for the receiver, may be 0
	 */
	bool SetIME(WIME *ime) {
		return _set_ime(ime) > 0;
	}
public:
	static bool IsCanvas(WWidget *widget) {
		return widget->GetClassId() >= _W_CLASS_CANVAS;
	}
	static WCanvas* ToCanvas(WWidget *widget) {
		if (IsCanvas(widget))
			return (WCanvas*) widget;
		else
			return 0;
	}
protected:
	w_class_id _GetClassID();
	bool PostEvent(WEvent *e);
	virtual bool OnImeComposition(WImeEvent &e);
public:
	WResult _create_caret(WCaret &caret) {
		return w_canvas_create_caret(W_CANVAS(this), W_CARET(&caret));
	}
	WResult _create_ime(WIME &ime) {
		return w_canvas_create_ime(W_CANVAS(this), W_IME(&ime));
	}
	WResult _scroll(const WPoint &dest, const WRect &rect, int all) {
		return w_canvas_scroll(W_CANVAS(this), (w_point*) &dest,
				(w_rect*) &rect, all);
	}
	WResult _get_caret(WCaret **caret) {
		return w_canvas_get_caret(W_CANVAS(this), (w_caret**) caret);
	}
	WResult _set_caret(WCaret *caret) {
		return w_canvas_set_caret(W_CANVAS(this), W_CARET(caret));
	}
	WResult _set_ime(WIME *ime) {
		return w_canvas_set_ime(W_CANVAS(this), W_IME(ime));
	}
	WResult _get_ime(WIME **ime) {
		return w_canvas_get_ime(W_CANVAS(this), (w_ime**) ime);
	}
private:
	//void *handles[(sizeof(w_canvas) - sizeof(w_composite)) / sizeof(void*)];
};
/**
 * Instances of this class provide an i-beam that is typically used
 * as the insertion point for text.
 * <dl>
 * <dt><b>Styles:</b></dt>
 * <dd>(none)</dd>
 * <dt><b>Events:</b></dt>
 * <dd>(none)</dd>
 * </dl>
 * <p>
 * IMPORTANT: This class is <em>not</em> intended to be subclassed.
 * </p>
 */
class SWTP_PUBLIC WCaret: public WWidgetData {
public:
	WCaret() {

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
	 * </p>
	 *
	 * @param parent a composite control which will be the parent of the new instance (cannot be null)
	 * @param style the style of control to construct
	 *
	 * @see SWT
	 * @see Widget#checkSubclass
	 * @see Widget#getStyle
	 */
	bool Create(WCanvas *parent) {
		return parent->_create_caret(*this) > 0;
	}
	/**
	 * Returns a rectangle describing the receiver's size and location
	 * relative to its parent (or its display if its parent is null).
	 *
	 * @return the receiver's bounding rectangle
	 */
	WRect& GetBounds(WRect &rect) {
		_get_bounds(&rect.GetOrigin(), &rect.GetSize());
		return rect;
	}
	/**
	 * Returns a rectangle describing the receiver's size and location
	 * relative to its parent (or its display if its parent is null).
	 *
	 * @return the receiver's bounding rectangle
	 */
	WRect GetBounds() {
		WRect rect;
		return GetBounds(rect);
	}
	/**
	 * Returns the font that the receiver will use to paint textual information.
	 *
	 * @return the receiver's font
	 */
	WFont* GetFont() {
		WFont *font;
		_get_font(font);
		return font;
	}
	/**
	 * Returns the image that the receiver will use to paint the caret.
	 *
	 * @return the receiver's image
	 */
	bool GetImage(WImage &image) {
		return _get_image(image) > 0;
	}
	/**
	 * Returns a point describing the receiver's location relative
	 * to its parent (or its display if its parent is null).
	 *
	 * @return the receiver's location
	 */
	WPoint& GetLocation(WPoint &location) {
		_get_bounds(&location, 0);
		return location;
	}
	/**
	 * Returns a point describing the receiver's location relative
	 * to its parent (or its display if its parent is null).
	 *
	 * @return the receiver's location
	 */
	WPoint GetLocation() {
		WPoint location;
		return GetLocation(location);
	}
	/**
	 * Returns the receiver's parent, which must be a <code>Canvas</code>.
	 *
	 * @return the receiver's parent
	 */
	WCanvas* GetParent() {
		WCanvas *parent;
		_get_parent(parent);
		return parent;
	}
	/**
	 * Returns a point describing the receiver's size.
	 *
	 * @return the receiver's size
	 */
	WSize& GetSize(WSize &size) {
		_get_bounds(0, &size);
		return size;
	}
	/**
	 * Returns a point describing the receiver's size.
	 *
	 * @return the receiver's size
	 */
	WSize GetSize() {
		WSize size;
		return GetSize(size);
	}
	/**
	 * Returns <code>true</code> if the receiver is visible, and
	 * <code>false</code> otherwise.
	 * <p>
	 * If one of the receiver's ancestors is not visible or some
	 * other condition makes the receiver not visible, this method
	 * may still indicate that it is considered visible even though
	 * it may not actually be showing.
	 * </p>
	 *
	 * @return the receiver's visibility state
	 */
	bool GetVisible() {
		return _get_visible() > 0;
	}
	/**
	 * Returns <code>true</code> if the receiver is visible and all
	 * of the receiver's ancestors are visible and <code>false</code>
	 * otherwise.
	 *
	 * @return the receiver's visibility state
	 *
	 * @see #getVisible
	 */
	bool IsVisible() {
		return _is_visible() > 0;
	}
	/**
	 * Sets the receiver's size and location to the rectangular
	 * area specified by the arguments. The <code>x</code> and
	 * <code>y</code> arguments are relative to the receiver's
	 * parent (or its display if its parent is null).
	 *
	 * @param x the new x coordinate for the receiver
	 * @param y the new y coordinate for the receiver
	 * @param width the new width for the receiver
	 * @param height the new height for the receiver
	 */
	bool SetBounds(int x, int y, int width, int height) {
		return SetBounds(WRect(x, y, width, height));
	}
	/**
	 * Sets the receiver's size and location to the rectangular
	 * area specified by the argument. The <code>x</code> and
	 * <code>y</code> fields of the rectangle are relative to
	 * the receiver's parent (or its display if its parent is null).
	 *
	 * @param rect the new bounds for the receiver
	 */
	bool SetBounds(const WRect &rect) {
		return _set_bounds(&rect.GetOrigin(), &rect.GetSize());
	}
	/**
	 * Sets the font that the receiver will use to paint textual information
	 * to the font specified by the argument, or to the default font for that
	 * kind of control if the argument is null.
	 *
	 * @param font the new font (or null)
	 */
	bool SetFont(WFont *font) {
		return _set_font(font) > 0;
	}
	/**
	 * Sets the image that the receiver will use to paint the caret
	 * to the image specified by the argument, or to the default
	 * which is a filled rectangle if the argument is null
	 *
	 * @param image the new image (or null)
	 */
	bool SetImage(const WImage &image) {
		return _set_image(image) > 0;
	}
	/**
	 * Sets the receiver's location to the point specified by
	 * the arguments which are relative to the receiver's
	 * parent (or its display if its parent is null).
	 *
	 * @param x the new x coordinate for the receiver
	 * @param y the new y coordinate for the receiver
	 */
	bool SetLocation(int x, int y) {
		return SetLocation(WPoint(x, y));
	}
	/**
	 * Sets the receiver's location to the point specified by
	 * the argument which is relative to the receiver's
	 * parent (or its display if its parent is null).
	 *
	 * @param location the new location for the receiver
	 */
	bool SetLocation(const WPoint &location) {
		return _set_bounds(&location, 0) > 0;
	}
	/**
	 * Sets the receiver's size to the point specified by the arguments.
	 *
	 * @param width the new width for the receiver
	 * @param height the new height for the receiver
	 */
	bool SetSize(int width, int height) {
		return SetSize(WSize(width, height));
	}
	/**
	 * Sets the receiver's size to the point specified by the argument.
	 *
	 * @param size the new extent for the receiver
	 */
	bool SetSize(const WSize &size) {
		return _set_bounds(0, (WSize*) &size);
	}
	/**
	 * Marks the receiver as visible if the argument is <code>true</code>,
	 * and marks it invisible otherwise.
	 * <p>
	 * If one of the receiver's ancestors is not visible or some
	 * other condition makes the receiver not visible, marking
	 * it visible may not actually cause it to be displayed.
	 * </p>
	 *
	 * @param visible the new visibility state
	 */
	bool SetVisible(bool visible) {
		return _set_visible(visible) > 0;
	}
public:
	WResult _get_bounds(WPoint *location, WSize *size) {
		return w_caret_get_bounds(W_CARET(this), (w_point*) location,
				(w_size*) size);
	}
	WResult _get_font(WFont *&font) {
		return w_caret_get_font(W_CARET(this), (w_font**) &font);
	}
	WResult _get_image(WImage &image) {
		return w_caret_get_image(W_CARET(this), W_IMAGE(&image));
	}
	WResult _get_parent(WCanvas *&parent) {
		return w_caret_get_parent(W_CARET(this), (w_canvas**) &parent);
	}
	WResult _get_visible() {
		return w_caret_get_visible(W_CARET(this));
	}
	WResult _is_visible() {
		return w_caret_is_visible(W_CARET(this));
	}
	WResult _set_bounds(const WPoint *location, const WSize *size) {
		return w_caret_set_bounds(W_CARET(this), (w_point*) location,
				(w_size*) size);
	}
	WResult _set_font(WFont *font) {
		return w_caret_set_font(W_CARET(this), W_FONT(font));
	}
	WResult _set_image(const WImage &image) {
		return w_caret_set_image(W_CARET(this), W_IMAGE(&image));
	}
	WResult _set_visible(bool visible) {
		return w_caret_set_visible(W_CARET(this), visible);
	}
private:
	void *handles[(sizeof(w_caret) - sizeof(w_widgetdata)) / sizeof(void*)];
};
/**
 * Instances of this class represent input method editors.
 * These are typically in-line pre-edit text areas that allow
 * the user to compose characters from Far Eastern languages
 * such as Japanese, Chinese or Korean.
 *
 * <dl>
 * <dt><b>Styles:</b></dt>
 * <dd>(none)</dd>
 * <dt><b>Events:</b></dt>
 * <dd>ImeComposition</dd>
 * </dl>
 * <p>
 * IMPORTANT: This class is <em>not</em> intended to be subclassed.
 * </p>
 */
class SWTP_PUBLIC WIME: public WWidgetData {
public:
	/**
	 * Prevents uninitialized instances from being created outside the package.
	 */
	WIME() {
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
	 * </p>
	 *
	 * @param parent a canvas control which will be the parent of the new instance (cannot be null)
	 * @param style the style of control to construct
	 */
	bool Create(WCanvas *parent) {
		return parent->_create_ime(*this) > 0;
	}
	/**
	 * Returns the offset of the caret from the start of the document.
	 * The caret is within the current composition.
	 *
	 * @return the caret offset
	 */
	int GetCaretOffset() {
		return _get_caret_offset();
	}
	/**
	 * Returns the commit count of the composition.  This is the
	 * number of characters that have been composed.  When the
	 * commit count is equal to the length of the composition
	 * text, then the in-line edit operation is complete.
	 *
	 * @return the commit count
	 *
	 * @see IME#getText
	 */
	int GetCommitCount() {
		return _get_commit_count();
	}
	/**
	 * Returns the offset of the composition from the start of the document.
	 * This is the start offset of the composition within the document and
	 * in not changed by the input method editor itself during the in-line edit
	 * session.
	 *
	 * @return the offset of the composition
	 */
	int GetCompositionOffset() {
		return _get_composition_offset();
	}
	/**
	 * Returns the ranges for the style that should be applied during the
	 * in-line edit session.
	 * <p>
	 * The ranges array contains start and end pairs.  Each pair refers to
	 * the corresponding style in the styles array.  For example, the pair
	 * that starts at ranges[n] and ends at ranges[n+1] uses the style
	 * at styles[n] returned in styles.
	 * </p>
	 * @return the ranges for the styles
	 */
	WResult GetRanges(WIterator<WTextStyleRange> &ranges) {
		return _get_ranges(ranges);
	}
	/**
	 * Returns the composition text.
	 * <p>
	 * The text for an IME is the characters in the widget that
	 * are in the current composition. When the commit count is
	 * equal to the length of the composition text, then the
	 * in-line edit operation is complete.
	 * </p>
	 *
	 * @return the widget text
	 */
	WString GetText() {
		WString str;
		_get_text(w_alloc_string_ref, &str.ref);
		return str;
	}
	/**
	 * Returns <code>true</code> if the caret should be wide, and
	 * <code>false</code> otherwise.  In some languages, for example
	 * Korean, the caret is typically widened to the width of the
	 * current character in the in-line edit session.
	 *
	 * @return the wide caret state
	 */
	bool GetWideCaret() {
		return _get_wide_caret() > 0;
	}
	/**
	 * Sets the offset of the composition from the start of the document.
	 * This is the start offset of the composition within the document and
	 * in not changed by the input method editor itself during the in-line edit
	 * session but may need to be changed by clients of the IME.  For example,
	 * if during an in-line edit operation, a text editor inserts characters
	 * above the IME, then the IME must be informed that the composition
	 * offset has changed.
	 *
	 * @param offset the offset of the composition
	 */
	bool SetCompositionOffset(int offset) {
		return _set_composition_offset(offset) > 0;
	}
public:
	int _get_caret_offset() {
		return w_ime_get_caret_offset(W_IME(this));
	}
	int _get_commit_count() {
		return w_ime_get_commit_count(W_IME(this));
	}
	int _get_composition_offset() {
		return w_ime_get_composition_offset(W_IME(this));
	}
	WResult _get_ranges(WIterator<WTextStyleRange> &ranges) {
		return w_ime_get_ranges(W_IME(this), (w_iterator*) &ranges);
	}
	wresult _get_text(w_alloc alloc, void *user_data) {
		return w_ime_get_text(W_IME(this), alloc, user_data);
	}
	wresult _get_wide_caret() {
		return w_ime_get_wide_caret(W_IME(this));
	}
	wresult _set_composition_offset(int offset) {
		return w_ime_set_composition_offset(W_IME(this), offset);
	}
private:
	void *handles[(sizeof(w_ime) - sizeof(w_widgetdata)) / sizeof(void*)];
};
namespace swt {
typedef ::WCanvas Canvas;
typedef ::WCaret Caret;
typedef ::WIME IME;
}  // namespace swt

#endif /* SWTP_WIDGETS_CANVAS_H_ */
