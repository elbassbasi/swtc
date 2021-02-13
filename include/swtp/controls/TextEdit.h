/*
 * Text.h
 *
 *  Created on: 9 ao�t 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_CONTROLS_TEXT_H_
#define SWTP_CONTROLS_TEXT_H_
#include "../widgets/Scrollable.h"
/**
 * Instances of this class are selectable user interface
 * objects that allow the user to enter and modify text.
 * Text controls can be either single or multi-line.
 * When a text control is created with a border, the
 * operating system includes a platform specific inset
 * around the contents of the control.  When created
 * without a border, an effort is made to remove the
 * inset such that the preferred size of the control
 * is the same size as the contents.
 * <p>
 * <dl>
 * <dt><b>Styles:</b></dt>
 * <dd>CENTER, ICON_CANCEL, ICON_SEARCH, LEFT, MULTI, PASSWORD, SEARCH, SINGLE, RIGHT, READ_ONLY, WRAP</dd>
 * <dt><b>Events:</b></dt>
 * <dd>DefaultSelection, Modify, Verify, OrientationChange</dd>
 * </dl>
 * <p>
 * Note: Only one of the styles MULTI and SINGLE may be specified,
 * and only one of the styles LEFT, CENTER, and RIGHT may be specified.
 * </p>
 * <p>
 * Note: The styles ICON_CANCEL and ICON_SEARCH are hints used in combination with SEARCH.
 * When the platform supports the hint, the text control shows these icons.  When an icon
 * is selected, a default selection event is sent with the detail field set to one of
 * ICON_CANCEL or ICON_SEARCH.  Normally, application code does not need to check the
 * detail.  In the case of ICON_CANCEL, the text is cleared before the default selection
 * event is sent causing the application to search for an empty string.
 * </p>
 * <p>
 * IMPORTANT: This class is <em>not</em> intended to be subclassed.
 * </p>
 */
class SWTP_PUBLIC WTextEdit: public WScrollable {
public:
	WTextEdit() {

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
	 * @see SWT#SINGLE
	 * @see SWT#MULTI
	 * @see SWT#READ_ONLY
	 * @see SWT#WRAP
	 * @see SWT#LEFT
	 * @see SWT#RIGHT
	 * @see SWT#CENTER
	 * @see SWT#PASSWORD
	 * @see SWT#SEARCH
	 * @see SWT#ICON_SEARCH
	 * @see SWT#ICON_CANCEL
	 * @see Widget#checkSubclass
	 * @see Widget#getStyle
	 */
	WTextEdit(WComposite *parent, wuint64 style) {
		Create(parent, style);
	}
	/**
	 * Appends a string.
	 * <p>
	 * The new text is appended to the text at
	 * the end of the widget.
	 * </p>
	 *
	 * @param string the string to be appended
	 */
	bool Append(const char *string, size_t length) {
		return _WReturnBool(_append(string, length,W_ENCODING_UTF8));
	}
	/**
	 * Appends a string.
	 * <p>
	 * The new text is appended to the text at
	 * the end of the widget.
	 * </p>
	 *
	 * @param string the string to be appended
	 */
	bool Append(const char *string) {
		return Append(string, -1);
	}
	/**
	 * Appends a string.
	 * <p>
	 * The new text is appended to the text at
	 * the end of the widget.
	 * </p>
	 *
	 * @param string the string to be appended
	 */
	bool AppendUnicode(const wchar_t *string, size_t length) {
		return _WReturnBool(_append((const char*)string, length,W_ENCODING_UNICODE));
	}
	/**
	 * Appends a string.
	 * <p>
	 * The new text is appended to the text at
	 * the end of the widget.
	 * </p>
	 *
	 * @param string the string to be appended
	 */
	bool AppendUnicode(const wchar_t *string) {
		return AppendUnicode(string, -1);
	}
	/**
	 * Clears the selection.
	 */
	bool ClearSelection() {
		return _WReturnBool(_clear_selection());
	}
	/**
	 * Copies the selected text.
	 * <p>
	 * The current selection is copied to the clipboard.
	 * </p>
	 */
	bool Copy() {
		return _WReturnBool(_copy());
	}
	/**
	 * Cuts the selected text.
	 * <p>
	 * The current selection is first copied to the
	 * clipboard and then deleted from the widget.
	 * </p>
	 */
	bool Cut() {
		return _WReturnBool(_cut());
	}
	/**
	 * Returns the line number of the caret.
	 * <p>
	 * The line number of the caret is returned.
	 * </p>
	 *
	 * @return the line number
	 */
	int GetCaretLineNumber() {
		return _WReturnInt(_get_caret_line_number());
	}
	/**
	 * Returns a point describing the receiver's location relative
	 * to its parent (or its display if its parent is null).
	 * <p>
	 * The location of the caret is returned.
	 * </p>
	 *
	 * @return a point, the location of the caret
	 */
	WPoint& GetCaretLocation(WPoint &location) {
		_get_caret_location(&location);
		return location;
	}
	/**
	 * Returns a point describing the receiver's location relative
	 * to its parent (or its display if its parent is null).
	 * <p>
	 * The location of the caret is returned.
	 * </p>
	 *
	 * @return a point, the location of the caret
	 */
	WPoint GetCaretLocation() {
		WPoint location;
		return GetCaretLocation(location);
	}
	/**
	 * Returns the character position of the caret.
	 * <p>
	 * Indexing is zero based.
	 * </p>
	 *
	 * @return the position of the caret
	 */
	int GetCaretPosition() {
		return _WReturnInt(_get_caret_position(W_ENCODING_UTF8));
	}
	/**
	 * Returns the character position of the caret.
	 * <p>
	 * Indexing is zero based.
	 * </p>
	 *
	 * @return the position of the caret
	 */
	int GetCaretPositionUnicode() {
		return _WReturnInt(_get_caret_position(W_ENCODING_UNICODE));
	}
	/**
	 * Returns the number of characters.
	 *
	 * @return number of characters in the widget
	 */
	int GetCharCount() {
		return _WReturnInt(_get_char_count(W_ENCODING_UTF8));
	}
	/**
	 * Returns the number of characters.
	 *
	 * @return number of characters in the widget
	 */
	int GetCharCountUnicode() {
		return _WReturnInt(_get_char_count(W_ENCODING_UNICODE));
	}
	/**
	 * Returns the double click enabled flag.
	 * <p>
	 * The double click flag enables or disables the
	 * default action of the text widget when the user
	 * double clicks.
	 * </p>
	 *
	 * @return whether or not double click is enabled
	 */
	bool GetDoubleClickEnabled() {
		return _WReturnBool(_get_doubleclick_enabled());
	}
	/**
	 * Returns the echo character.
	 * <p>
	 * The echo character is the character that is
	 * displayed when the user enters text or the
	 * text is changed by the programmer.
	 * </p>
	 *
	 * @return the echo character
	 *
	 * @see #setEchoChar
	 */
	char GetEchoChar() {
		return _WReturnInt(_get_echo_char());
	}
	/**
	 * Returns the editable state.
	 *
	 * @return whether or not the receiver is editable
	 */
	bool GetEditable() {
		return _WReturnBool(_get_editable());
	}
	/**
	 * Returns the number of lines.
	 *
	 * @return the number of lines in the widget
	 */
	int GetLineCount() {
		return _WReturnInt(_get_line_count());
	}
	/**
	 * Returns the line delimiter.
	 *
	 * @return a string that is the line delimiter
	 *
	 * @see #DELIMITER
	 */
	int GetLineDelimiter() {
		return _WReturnInt(_get_line_delimiter());
	}
	/**
	 * Returns the height of a line.
	 *
	 * @return the height of a row of text
	 */
	int GetLineHeight() {
		return _WReturnInt(_get_line_height());
	}
	/**
	 * Returns the widget message.  The message text is displayed
	 * as a hint for the user, indicating the purpose of the field.
	 * <p>
	 * Typically this is used in conjunction with <code>SWT.SEARCH</code>.
	 * </p>
	 *
	 * @return the widget message
	 */
	WString GetMessage() {
		w_string_ref *ref = 0;
		_get_message(w_alloc_string_ref, &ref,W_ENCODING_UTF8);
		return ref;
	}
	/**
	 * Returns the character position at the given point in the receiver
	 * or -1 if no such position exists. The point is in the coordinate
	 * system of the receiver.
	 * <p>
	 * Indexing is zero based.
	 * </p>
	 *
	 * @return the position of the caret
	 */
	int GetPosition(const WPoint &point) {
		return _WReturnInt(_get_position(&point,W_ENCODING_UTF8));
	}
	/**
	 * Returns the character position at the given point in the receiver
	 * or -1 if no such position exists. The point is in the coordinate
	 * system of the receiver.
	 * <p>
	 * Indexing is zero based.
	 * </p>
	 *
	 * @return the position of the caret
	 */
	int GetPositionUnicode(const WPoint &point) {
		return _WReturnInt(_get_position(&point,W_ENCODING_UNICODE));
	}
	/**
	 * Returns a <code>Point</code> whose x coordinate is the
	 * character position representing the start of the selected
	 * text, and whose y coordinate is the character position
	 * representing the end of the selection. An "empty" selection
	 * is indicated by the x and y coordinates having the same value.
	 * <p>
	 * Indexing is zero based.  The range of a selection is from
	 * 0..N where N is the number of characters in the widget.
	 * </p>
	 *
	 * @return a point representing the selection start and end
	 */
	WRange& GetSelection(WRange &range) {
		_get_selection(&range,W_ENCODING_UTF8);
		return range;
	}
	/**
	 * Returns a <code>Point</code> whose x coordinate is the
	 * character position representing the start of the selected
	 * text, and whose y coordinate is the character position
	 * representing the end of the selection. An "empty" selection
	 * is indicated by the x and y coordinates having the same value.
	 * <p>
	 * Indexing is zero based.  The range of a selection is from
	 * 0..N where N is the number of characters in the widget.
	 * </p>
	 *
	 * @return a point representing the selection start and end
	 */
	WRange GetSelection() {
		WRange range;
		return GetSelection(range);
	}
	/**
	 * Returns a <code>Point</code> whose x coordinate is the
	 * character position representing the start of the selected
	 * text, and whose y coordinate is the character position
	 * representing the end of the selection. An "empty" selection
	 * is indicated by the x and y coordinates having the same value.
	 * <p>
	 * Indexing is zero based.  The range of a selection is from
	 * 0..N where N is the number of characters in the widget.
	 * </p>
	 *
	 * @return a point representing the selection start and end
	 */
	WRange& GetSelectionUnicode(WRange &range) {
		_get_selection(&range,W_ENCODING_UNICODE);
		return range;
	}
	/**
	 * Returns a <code>Point</code> whose x coordinate is the
	 * character position representing the start of the selected
	 * text, and whose y coordinate is the character position
	 * representing the end of the selection. An "empty" selection
	 * is indicated by the x and y coordinates having the same value.
	 * <p>
	 * Indexing is zero based.  The range of a selection is from
	 * 0..N where N is the number of characters in the widget.
	 * </p>
	 *
	 * @return a point representing the selection start and end
	 */
	WRange GetSelectionUnicode() {
		WRange range;
		return GetSelectionUnicode(range);
	}
	/**
	 * Returns the number of selected characters.
	 *
	 * @return the number of selected characters.
	 */
	int GetSelectionCount() {
		return _WReturnInt(_get_selection_count(W_ENCODING_UTF8));
	}
	/**
	 * Gets the selected text, or an empty string if there is no current selection.
	 *
	 * @return the selected text
	 */
	WString GetSelectionText() {
		w_string_ref *ref = 0;
		_get_selection_text(w_alloc_string_ref, &ref,W_ENCODING_UTF8);
		return ref;
	}
	/**
	 * Returns the number of tabs.
	 * <p>
	 * Tab stop spacing is specified in terms of the
	 * space (' ') character.  The width of a single
	 * tab stop is the pixel width of the spaces.
	 * </p>
	 *
	 * @return the number of tab characters
	 */
	int GetTabs() {
		return _WReturnInt(_get_tabs());
	}
	/**
	 * Returns the widget text.
	 * <p>
	 * The text for a text widget is the characters in the widget, or
	 * an empty string if this has never been set.
	 * </p>
	 *
	 * @return the widget text
	 */
	WString GetText() {
		w_string_ref *ref = 0;
		_get_text(w_alloc_string_ref, &ref,W_ENCODING_UTF8);
		return ref;
	}
	std::string GetTextStd() {
		std::string str;
		_get_text(w_alloc_std_string, &str,W_ENCODING_UTF8);
		return std::move(str);
	}
	/**
	 * Returns a range of text.  Returns an empty string if the
	 * start of the range is greater than the end.
	 * <p>
	 * Indexing is zero based.  The range of
	 * a selection is from 0..N-1 where N is
	 * the number of characters in the widget.
	 * </p>
	 *
	 * @param start the start of the range
	 * @param end the end of the range
	 * @return the range of text
	 */
	WString GetText(int start, int end) {
		w_string_ref *ref = 0;
		_get_text_range(WRange(start, end), w_alloc_string_ref, &ref,W_ENCODING_UTF8);
		return ref;
	}
	/**
	 * Returns the maximum number of characters that the receiver is capable of holding.
	 * <p>
	 * If this has not been changed by <code>setTextLimit()</code>,
	 * it will be the constant <code>Text.LIMIT</code>.
	 * </p>
	 *
	 * @return the text limit
	 *
	 * @see #LIMIT
	 */
	int GetTextLimit() {
		return _WReturnInt(_get_text_limit(W_ENCODING_UTF8));
	}
	/**
	 * Returns the zero-relative index of the line which is currently
	 * at the top of the receiver.
	 * <p>
	 * This index can change when lines are scrolled or new lines are added or removed.
	 * </p>
	 *
	 * @return the index of the top line
	 */
	int GetTopIndex() {
		return _WReturnInt(_get_top_index());
	}
	/**
	 * Returns the top pixel.
	 * <p>
	 * The top pixel is the pixel position of the line
	 * that is currently at the top of the widget.  On
	 * some platforms, a text widget can be scrolled by
	 * pixels instead of lines so that a partial line
	 * is displayed at the top of the widget.
	 * </p><p>
	 * The top pixel changes when the widget is scrolled.
	 * The top pixel does not include the widget trimming.
	 * </p>
	 *
	 * @return the pixel position of the top line
	 */
	int GetTopPixel() {
		return _WReturnInt(_get_top_pixel());
	}
	/**
	 * Inserts a string.
	 * <p>
	 * The old selection is replaced with the new text.
	 * </p>
	 *
	 * @param string the string
	 */
	bool Insert(const char *text, size_t length) {
		return _WReturnBool(_insert(text, length,W_ENCODING_UTF8));
	}
	/**
	 * Inserts a string.
	 * <p>
	 * The old selection is replaced with the new text.
	 * </p>
	 *
	 * @param string the string
	 */
	bool Insert(const char *text) {
		return Insert(text, -1);
	}
	/**
	 * Pastes text from clipboard.
	 * <p>
	 * The selected text is deleted from the widget
	 * and new text inserted from the clipboard.
	 * </p>
	 */
	bool Paste() {
		return _WReturnBool(_paste());
	}
	/**
	 * Selects all the text in the receiver.
	 */
	bool SelectAll() {
		return _WReturnBool(_select_all());
	}
	/**
	 * Sets the double click enabled flag.
	 * <p>
	 * The double click flag enables or disables the
	 * default action of the text widget when the user
	 * double clicks.
	 * </p><p>
	 * Note: This operation is a hint and is not supported on
	 * platforms that do not have this concept.
	 * </p>
	 *
	 * @param doubleClick the new double click flag
	 */
	bool SetDoubleClickEnabled(bool doubleClick) {
		return _WReturnBool(_set_doubleclick_enabled(doubleClick));
	}
	/**
	 * Sets the echo character.
	 * <p>
	 * The echo character is the character that is
	 * displayed when the user enters text or the
	 * text is changed by the programmer. Setting
	 * the echo character to '\0' clears the echo
	 * character and redraws the original text.
	 * If for any reason the echo character is invalid,
	 * or if the platform does not allow modification
	 * of the echo character, the default echo character
	 * for the platform is used.
	 * </p>
	 *
	 * @param echo the new echo character
	 */
	bool SetEchoChar(char echo) {
		return _WReturnBool(_set_echochar(echo));
	}
	/**
	 * Sets the editable state.
	 *
	 * @param editable the new editable state
	 */
	bool SetEditable(bool editable) {
		return _WReturnBool(_set_editable(editable));
	}
	/**
	 * Sets the widget message. The message text is displayed
	 * as a hint for the user, indicating the purpose of the field.
	 * <p>
	 * Typically this is used in conjunction with <code>SWT.SEARCH</code>.
	 * </p>
	 *
	 * @param message the new message
	 */
	bool SetMessage(const char *message, size_t length) {
		return _WReturnBool(_set_message(message, length,W_ENCODING_UTF8));
	}
	/**
	 * Sets the widget message. The message text is displayed
	 * as a hint for the user, indicating the purpose of the field.
	 * <p>
	 * Typically this is used in conjunction with <code>SWT.SEARCH</code>.
	 * </p>
	 *
	 * @param message the new message
	 */
	bool SetMessage(const char *message) {
		return SetMessage(message, -1);
	}
	/**
	 * Sets the selection.
	 * <p>
	 * Indexing is zero based.  The range of
	 * a selection is from 0..N where N is
	 * the number of characters in the widget.
	 * </p><p>
	 * Text selections are specified in terms of
	 * caret positions.  In a text widget that
	 * contains N characters, there are N+1 caret
	 * positions, ranging from 0..N.  This differs
	 * from other functions that address character
	 * position such as getText () that use the
	 * regular array indexing rules.
	 * </p>
	 *
	 * @param start new caret position
	 */
	bool SetSelection(int start) {
		return _WReturnBool(_set_selection(start,W_ENCODING_UTF8));
	}
	/**
	 * Sets the selection to the range specified
	 * by the given start and end indices.
	 * <p>
	 * Indexing is zero based.  The range of
	 * a selection is from 0..N where N is
	 * the number of characters in the widget.
	 * </p><p>
	 * Text selections are specified in terms of
	 * caret positions.  In a text widget that
	 * contains N characters, there are N+1 caret
	 * positions, ranging from 0..N.  This differs
	 * from other functions that address character
	 * position such as getText () that use the
	 * usual array indexing rules.
	 * </p>
	 *
	 * @param start the start of the range
	 * @param end the end of the range
	 */
	bool SetSelection(int start, int end) {
		return SetSelection(WRange(start, end));
	}
	/**
	 * Sets the selection to the range specified
	 * by the given point, where the x coordinate
	 * represents the start index and the y coordinate
	 * represents the end index.
	 * <p>
	 * Indexing is zero based.  The range of
	 * a selection is from 0..N where N is
	 * the number of characters in the widget.
	 * </p><p>
	 * Text selections are specified in terms of
	 * caret positions.  In a text widget that
	 * contains N characters, there are N+1 caret
	 * positions, ranging from 0..N.  This differs
	 * from other functions that address character
	 * position such as getText () that use the
	 * usual array indexing rules.
	 * </p>
	 *
	 * @param selection the point
	 */
	bool SetSelection(const WRange &selection) {
		return _WReturnBool(_set_selection_range(&selection,W_ENCODING_UTF8));
	}
	/**
	 * Sets the number of tabs.
	 * <p>
	 * Tab stop spacing is specified in terms of the
	 * space (' ') character.  The width of a single
	 * tab stop is the pixel width of the spaces.
	 * </p>
	 *
	 * @param tabs the number of tabs
	 */
	bool SetTabs(int tabs) {
		return _WReturnBool(_set_tabs(tabs));
	}
	/**
	 * Sets the contents of the receiver to the given string. If the receiver has style
	 * SINGLE and the argument contains multiple lines of text, the result of this
	 * operation is undefined and may vary from platform to platform.
	 *
	 * @param string the new text
	 */
	bool SetText(const char *string, size_t length) {
		return _WReturnBool(_set_text(string, length,W_ENCODING_UTF8));
	}
	/**
	 * Sets the contents of the receiver to the given string. If the receiver has style
	 * SINGLE and the argument contains multiple lines of text, the result of this
	 * operation is undefined and may vary from platform to platform.
	 *
	 * @param string the new text
	 */
	bool SetText(const char *string) {
		return SetText(string, -1);
	}
	/**
	 * Sets the maximum number of characters that the receiver
	 * is capable of holding to be the argument.
	 * <p>
	 * Instead of trying to set the text limit to zero, consider
	 * creating a read-only text widget.
	 * </p><p>
	 * To reset this value to the default, use <code>setTextLimit(Text.LIMIT)</code>.
	 * Specifying a limit value larger than <code>Text.LIMIT</code> sets the
	 * receiver's limit to <code>Text.LIMIT</code>.
	 * </p>
	 *
	 * @param limit new text limit
	 *
	 * @see #LIMIT
	 */
	bool SetTextLimit(int limit) {
		return _WReturnBool(_set_text_limit(limit));
	}
	/**
	 * Sets the zero-relative index of the line which is currently
	 * at the top of the receiver. This index can change when lines
	 * are scrolled or new lines are added and removed.
	 *
	 * @param index the index of the top item
	 */
	bool SetTopIndex(int index) {
		return _WReturnBool(_set_top_index(index));
	}
	/**
	 * Shows the selection.
	 * <p>
	 * If the selection is already showing
	 * in the receiver, this method simply returns.  Otherwise,
	 * lines are scrolled until the selection is visible.
	 * </p>
	 */
	bool ShowSelection() {
		return _WReturnBool(_show_selection());
	}
protected:
	w_class_id _GetClassID();
public:
	wresult _append(const char *text, size_t length,int enc) {
		return w_textedit_append(W_TEXTEDIT(this), text, length,enc);
	}
	wresult _clear_selection() {
		return w_textedit_clear_selection(W_TEXTEDIT(this));
	}
	wresult _copy() {
		return w_textedit_copy(W_TEXTEDIT(this));
	}
	wresult _cut() {
		return w_textedit_cut(W_TEXTEDIT(this));
	}
	wresult _get_caret_line_number() {
		return w_textedit_get_caret_line_number(W_TEXTEDIT(this));
	}
	wresult _get_caret_location(WPoint *location) {
		return w_textedit_get_caret_location(W_TEXTEDIT(this), (w_point*) location);
	}
	wresult _get_caret_position(int enc) {
		return w_textedit_get_caret_position(W_TEXTEDIT(this),enc);
	}
	wresult _get_char_count(int enc) {
		return w_textedit_get_char_count(W_TEXTEDIT(this),enc);
	}
	wresult _get_doubleclick_enabled() {
		return w_textedit_get_doubleclick_enabled(W_TEXTEDIT(this));
	}
	wresult _get_echo_char() {
		return w_textedit_get_echo_char(W_TEXTEDIT(this));
	}
	wresult _get_editable() {
		return w_textedit_get_editable(W_TEXTEDIT(this));
	}
	wresult _get_line_count() {
		return w_textedit_get_line_count(W_TEXTEDIT(this));
	}
	wresult _get_line_delimiter() {
		return w_textedit_get_line_delimiter(W_TEXTEDIT(this));
	}
	wresult _get_line_height() {
		return w_textedit_get_line_height(W_TEXTEDIT(this));
	}
	wresult _get_message(w_alloc alloc, void *user_data,int enc) {
		return w_textedit_get_message(W_TEXTEDIT(this), alloc, user_data,enc);
	}
	wresult _get_position(const WPoint *point,int enc) {
		return w_textedit_get_position(W_TEXTEDIT(this), (w_point*) point,enc);
	}
	wresult _get_selection(WRange *range,int enc) {
		return w_textedit_get_selection(W_TEXTEDIT(this), (w_range*) range,enc);
	}
	wresult _get_selection_count(int enc) {
		return w_textedit_get_selection_count(W_TEXTEDIT(this),enc);
	}
	wresult _get_selection_text(w_alloc alloc, void *user_data,int enc) {
		return w_textedit_get_selection_text(W_TEXTEDIT(this), alloc, user_data,enc);
	}
	wresult _get_tabs() {
		return w_textedit_get_tabs(W_TEXTEDIT(this));
	}
	wresult _get_text(w_alloc alloc, void *user_data,int enc) {
		return w_textedit_get_text(W_TEXTEDIT(this), alloc, user_data,enc);
	}
	wresult _get_text_range(const WRange *range, w_alloc alloc,
			void *user_data,int enc) {
		return w_textedit_get_text_range(W_TEXTEDIT(this), (w_range*) range, alloc,
				user_data,enc);
	}
	wresult _get_text_range(const WRange &range, w_alloc alloc,
			void *user_data,int enc) {
		return _get_text_range(&range, alloc, user_data,enc);
	}
	wresult _get_text_limit(int enc) {
		return w_textedit_get_text_limit(W_TEXTEDIT(this),enc);
	}
	wresult _get_top_index() {
		return w_textedit_get_top_index(W_TEXTEDIT(this));
	}
	wresult _get_top_pixel() {
		return w_textedit_get_top_pixel(W_TEXTEDIT(this));
	}
	wresult _insert(const char *text, size_t length,int enc) {
		return w_textedit_insert(W_TEXTEDIT(this), text, length,enc);
	}
	wresult _paste() {
		return w_textedit_paste(W_TEXTEDIT(this));
	}
	wresult _select_all() {
		return w_textedit_select_all(W_TEXTEDIT(this));
	}
	wresult _set_doubleclick_enabled(bool doubleClick) {
		return w_textedit_set_doubleclick_enabled(W_TEXTEDIT(this), doubleClick);
	}
	wresult _set_echochar(int echo) {
		return w_textedit_set_echochar(W_TEXTEDIT(this), echo);
	}
	wresult _set_editable(bool editable) {
		return w_textedit_set_editable(W_TEXTEDIT(this), editable);
	}
	wresult _set_message(const char *message, size_t length,int enc) {
		return w_textedit_set_message(W_TEXTEDIT(this), message, length,enc);
	}
	wresult _set_selection(int start,int enc) {
		return w_textedit_set_selection(W_TEXTEDIT(this), start,enc);
	}
	wresult _set_selection_range(const WRange *selection,int enc) {
		return w_textedit_set_selection_range(W_TEXTEDIT(this), (w_range*) selection,enc);
	}
	wresult _set_tabs(int tabs) {
		return w_textedit_set_tabs(W_TEXTEDIT(this), tabs);
	}
	wresult _set_text(const char *text, size_t length,int enc) {
		return w_textedit_set_text(W_TEXTEDIT(this), text, length,enc);
	}
	wresult _set_text_limit(int limit) {
		return w_textedit_set_text_limit(W_TEXTEDIT(this), limit);
	}
	wresult _set_top_index(int index) {
		return w_textedit_set_top_index(W_TEXTEDIT(this), index);
	}
	wresult _show_selection() {
		return w_textedit_show_selection(W_TEXTEDIT(this));
	}
private:
	void *handles[(sizeof(w_textedit) - sizeof(w_scrollable)) / sizeof(void*)];
};

#endif /* SWTP_CONTROLS_TEXT_H_ */
