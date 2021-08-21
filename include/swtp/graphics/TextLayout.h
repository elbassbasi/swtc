/*
 * TextLayout.h
 *
 *  Created on: 17 févr. 2020
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_GRAPHICS_TEXTLAYOUT_H_
#define SWTP_GRAPHICS_TEXTLAYOUT_H_
#include "GC.h"
/**
 * Instances of this class represent glyph metrics.
 * <p>
 * The hashCode() method in this class uses the values of the public
 * fields to compute the hash value. When storing instances of the
 * class in hashed collections, do not modify these fields after the
 * object has been inserted.
 * </p>
 * <p>
 * Application code does <em>not</em> need to explicitly release the
 * resources managed by each instance when those instances are no longer
 * required, and thus no <code>dispose()</code> method is provided.
 * </p>
 *
 * @see TextStyle
 * @see TextLayout
 */
class WGlyphMetrics {
public:
	/**
	 * the ascent of the GlyphMetrics
	 */
	wushort ascent;
	/**
	 * the descent of the GlyphMetrics
	 */
	wushort descent;
	/**
	 * the width of the GlyphMetrics
	 */
	wushort width;
	/**
	 * Constructs an instance of this class with the given
	 * ascent, descent and width values.
	 *
	 * @param ascent the GlyphMetrics ascent
	 * @param descent the GlyphMetrics descent
	 * @param width the GlyphMetrics width
	 */
	void Set(wushort ascent, wushort descent, wushort width) {
		this->ascent = ascent;
		this->descent = descent;
		this->width = width;
	}
	bool Equals(const WGlyphMetrics &metrics) {
		return metrics.ascent == ascent && metrics.descent == descent
				&& metrics.width == width;
	}
};
/**
 * <code>TextStyle</code> defines a set of styles that can be applied
 * to a range of text.
 * <p>
 * The hashCode() method in this class uses the values of the public
 * fields to compute the hash value. When storing instances of the
 * class in hashed collections, do not modify these fields after the
 * object has been inserted.
 * </p>
 * <p>
 * Application code does <em>not</em> need to explicitly release the
 * resources managed by each instance when those instances are no longer
 * required, and thus no <code>dispose()</code> method is provided.
 * </p>
 *
 * @see TextLayout
 * @see Font
 * @see Color
 */
class WTextStyle: public w_textstyle {
public:
	void Init() {
		this->flags = 0;
	}
	/**
	 * Create an empty text style.
	 */
	WTextStyle() {
		Init();
	}
	/**
	 * Create a new text style with the specified font, foreground
	 * and background.
	 *
	 * @param font the font of the style, <code>null</code> if none
	 * @param foreground the foreground color of the style, <code>null</code> if none
	 * @param background the background color of the style, <code>null</code> if none
	 */
	WTextStyle(WFont *font, WColor foreground, WColor background) {
		Init();
		SetFont(font);
		SetForeground(foreground);
		SetBackground(background);
	}
	/**
	 * Create a new text style from an existing text style.
	 *
	 * @param style the style to copy
	 */
	WTextStyle(const WTextStyle &style) {
		memcpy(this, &style, sizeof(WTextStyle));
	}
	WTextStyle& SetFont(WFont *font) {
		this->mask_font = 1;
		this->font = (w_font*) font;
		return *this;
	}
	WTextStyle& SetForeground(WColor foreground) {
		this->mask_foreground = 1;
		this->foreground = foreground;
		return *this;
	}
	WTextStyle& SetForeground(int foreground) {
		this->mask_foreground = 1;
		this->foreground = foreground;
		return *this;
	}
	WTextStyle& SetBackground(WColor background) {
		this->mask_background = 1;
		this->background = background;
		return *this;
	}
	WTextStyle& SetBackground(int background) {
		this->mask_background = 1;
		this->background = background;
		return *this;
	}
	WTextStyle& SetStrikeout(int color) {
		this->mask_strikeout = 1;
		this->strikeout = 1;
		this->strikeoutColor = color;
		return *this;
	}
	WTextStyle& SetUnderline(int style, int color) {
		this->mask_underline = 1;
		this->underline = 1;
		this->underlineStyle = style;
		this->underlineColor = color;
		return *this;
	}
	WTextStyle& SetUnderlineSingle(int color) {
		return SetUnderline(W_UNDERLINE_SINGLE, color);
	}
	WTextStyle& SetUnderlineDouble(int color) {
		return SetUnderline(W_UNDERLINE_DOUBLE, color);
	}
	WTextStyle& SetUnderlineError(int color) {
		return SetUnderline(W_UNDERLINE_ERROR, color);
	}
	WTextStyle& SetUnderlineLink(int color) {
		return SetUnderline(W_UNDERLINE_LINK, color);
	}
	WTextStyle& SetUnderlineSquiggle(int color) {
		return SetUnderline(W_UNDERLINE_SQUIGGLE, color);
	}
	WTextStyle& SetRise(int rise) {
		this->mask_rise = 1;
		this->rise = rise;
		return *this;
	}
	/**
	 * Constructs an instance of this class with the given
	 * ascent, descent and width values.
	 *
	 * @param ascent the GlyphMetrics ascent
	 * @param descent the GlyphMetrics descent
	 * @param width the GlyphMetrics width
	 */
	WTextStyle& SetGlyph(wushort ascent, wushort descent, wushort width) {
		this->mask_metrics = 1;
		this->metrics.ascent = ascent;
		this->metrics.descent = descent;
		this->metrics.width = width;
		return *this;
	}
	WTextStyle& SetMetrics(const WGlyphMetrics &metrics) {
		return SetGlyph(metrics.ascent, metrics.descent, metrics.width);
	}
	WGlyphMetrics& GetMetrics() {
		return reinterpret_cast<WGlyphMetrics&>(this->metrics);
	}
};
class WTextStyleRange: public WTextStyle {
public:
	WRange range;
};
class WTextLayout: public WResource {
public:
	/**
	 * Constructs a new instance of this class on the given device.
	 * <p>
	 * You must dispose the text layout when it is no longer required.
	 * </p>
	 *
	 * @param device the device on which to allocate the text layout
	 *
	 * @see #dispose()
	 */
	WTextLayout() {
		w_textlayout_init(W_TEXTLAYOUT(this));
	}
	~WTextLayout() {
		w_textlayout_dispose(W_TEXTLAYOUT(this));
	}
	void Dispose() {
		w_textlayout_dispose(W_TEXTLAYOUT(this));
	}
	bool IsOk() {
		return w_textlayout_isok(W_TEXTLAYOUT(this)) > 0;
	}
	bool Create() {
		return w_textlayout_create(W_TEXTLAYOUT(this)) > 0;
	}
	/**
	 * Draws the receiver's text using the specified GC at the specified
	 * point.
	 *
	 * @param gc the GC to draw
	 * @param x the x coordinate of the top left corner of the rectangular area where the text is to be drawn
	 * @param y the y coordinate of the top left corner of the rectangular area where the text is to be drawn
	 */
	void Draw(WGraphics &gc, int x, int y) {
		Draw(gc, WRect(x, y, -1, -1));
	}
	void Draw(WGraphics &gc, const WRect &rect) {
		w_textlayout_draw(W_TEXTLAYOUT(this), W_GRAPHICS(&gc), (w_rect*) &rect);
	}
	/**
	 * Returns the receiver's horizontal text alignment, which will be one
	 * of <code>W_LEFT</code>, <code>W_CENTER</code> or
	 * <code>W_RIGHT</code>.
	 *
	 * @return the alignment used to positioned text horizontally
	 */
	int GetAlignment() {
		return w_textlayout_get_alignment(W_TEXTLAYOUT(this));
	}
	/**
	 * Returns the ascent of the receiver.
	 *
	 * @return the ascent
	 *
	 * @see #getDescent()
	 * @see #setDescent(int)
	 * @see #setAscent(int)
	 * @see #getLineMetrics(int)
	 */
	int GetAscent() {
		return w_textlayout_get_ascent(W_TEXTLAYOUT(this));
	}
	/**
	 * Returns the bounds of the receiver. The width returned is either the
	 * width of the longest line or the width set using {@link TextLayout#setWidth(int)}.
	 * To obtain the text bounds of a line use {@link TextLayout#getLineBounds(int)}.
	 *
	 * @return the bounds of the receiver
	 *
	 * @see #setWidth(int)
	 * @see #getLineBounds(int)
	 */
	void GetBounds(WRect &bounds) {
		GetBounds(0, -1, bounds);
	}
	/**
	 * Returns the bounds for the specified range of characters. The
	 * bounds is the smallest rectangle that encompasses all characters
	 * in the range. The start and end offsets are inclusive and will be
	 * clamped if out of range.
	 *
	 * @param start the start offset
	 * @param end the end offset
	 * @return the bounds of the character range
	 */
	void GetBounds(int start, int end, WRect &bounds) {
		return w_textlayout_get_bounds(W_TEXTLAYOUT(this), start, end,
				(w_rect*) &bounds, W_ENCODING_UTF8);
	}
	/**
	 * Returns the descent of the receiver.
	 *
	 * @return the descent
	 *
	 * @see #getAscent()
	 * @see #setAscent(int)
	 * @see #setDescent(int)
	 * @see #getLineMetrics(int)
	 */
	int GetDescent() {
		return w_textlayout_get_descent(W_TEXTLAYOUT(this));
	}
	/**
	 * Returns the default font currently being used by the receiver
	 * to draw and measure text.
	 *
	 * @return the receiver's font
	 */
	WFont* GetFont() {
		return (WFont*) w_textlayout_get_font(W_TEXTLAYOUT(this));
	}

	/**
	 * Returns the receiver's indent.
	 *
	 * @return the receiver's indent
	 */
	int GetIndent() {
		return w_textlayout_get_indent(W_TEXTLAYOUT(this));
	}
	/**
	 * Returns the receiver's justification.
	 *
	 * @return the receiver's justification
	 */
	bool GetJustify() {
		return w_textlayout_get_justify(W_TEXTLAYOUT(this));
	}
	/**
	 * Returns the embedding level for the specified character offset. The
	 * embedding level is usually used to determine the directionality of a
	 * character in bidirectional text.
	 *
	 * @param offset the character offset
	 * @return the embedding level
	 */
	int GetLevel(int offset) {
		return w_textlayout_get_level(W_TEXTLAYOUT(this), offset,
				W_ENCODING_UTF8);
	}
	/**
	 * Returns the bounds of the line for the specified line index.
	 *
	 * @param lineIndex the line index
	 * @return the line bounds
	 */
	void GetLineBounds(int lineIndex, WRect &bounds) {
		return w_textlayout_get_line_bounds(W_TEXTLAYOUT(this), lineIndex,
				(w_rect*) &bounds);
	}
	/**
	 * Returns the receiver's line count. This includes lines caused
	 * by wrapping.
	 *
	 * @return the line count
	 */
	int GetLineCount() {
		return w_textlayout_get_line_count(W_TEXTLAYOUT(this));
	}
	/**
	 * Returns the index of the line that contains the specified
	 * character offset.
	 *
	 * @param offset the character offset
	 * @return the line index
	 */
	int GetLineIndex(int offset) {
		return w_textlayout_get_line_index(W_TEXTLAYOUT(this), offset,
				W_ENCODING_UTF8);
	}
	/**
	 * Returns the font metrics for the specified line index.
	 *
	 * @param lineIndex the line index
	 * @return the font metrics
	 */
	bool GetLineMetrics(int lineIndex, WFontMetrics &fontMetrics) {
		return w_textlayout_get_line_metrics(W_TEXTLAYOUT(this), lineIndex,
				W_FONTMETRICS(&fontMetrics)) > 0;
	}
	/**
	 * Returns the line offsets.  Each value in the array is the
	 * offset for the first character in a line except for the last
	 * value, which contains the length of the text.
	 *
	 * @return the line offsets
	 */
	int GetLineOffset(int lineIndex) {
		return w_textlayout_get_line_offset(W_TEXTLAYOUT(this), lineIndex,
				W_ENCODING_UTF8);
	}
	/**
	 * Returns the location for the specified character offset. The
	 * <code>trailing</code> argument indicates whether the offset
	 * corresponds to the leading or trailing edge of the cluster.
	 *
	 * @param offset the character offset
	 * @param trailing the trailing flag
	 * @return the location of the character offset
	 *
	 * @see #getOffset(Point, int[])
	 * @see #getOffset(int, int, int[])
	 */
	bool GetLocation(int offset, bool trailing, WPoint &location) {
		return w_textlayout_get_location(W_TEXTLAYOUT(this), offset, trailing,
				(w_point*) &location, W_ENCODING_UTF8) > 0;
	}
	/**
	 * Returns the next offset for the specified offset and movement
	 * type.  The movement is one of <code>W_MOVEMENT_CHAR</code>,
	 * <code>W_MOVEMENT_CLUSTER</code>, <code>W_MOVEMENT_WORD</code>,
	 * <code>W_MOVEMENT_WORD_END</code> or <code>W_MOVEMENT_WORD_START</code>.
	 *
	 * @param offset the start offset
	 * @param movement the movement type
	 * @return the next offset
	 *
	 * @see #getPreviousOffset(int, int)
	 */
	int GetNextOffset(int offset, int movement) {
		return w_textlayout_get_next_offset(W_TEXTLAYOUT(this), offset,
				movement, W_ENCODING_UTF8);
	}
	/**
	 * Returns the character offset for the specified point.
	 * For a typical character, the trailing argument will be filled in to
	 * indicate whether the point is closer to the leading edge (0) or
	 * the trailing edge (1).  When the point is over a cluster composed
	 * of multiple characters, the trailing argument will be filled with the
	 * position of the character in the cluster that is closest to
	 * the point.
	 *
	 * @param point the point
	 * @param trailing the trailing buffer
	 * @return the character offset
	 *
	 * @see #getLocation(int, boolean)
	 */
	int GetOffset(WPoint &point, int *trailing, int count) {
		return w_textlayout_get_offset(W_TEXTLAYOUT(this), (w_point*) &point,
				trailing, count, W_ENCODING_UTF8);
	}
	/**
	 * Returns the character offset for the specified point.
	 * For a typical character, the trailing argument will be filled in to
	 * indicate whether the point is closer to the leading edge (0) or
	 * the trailing edge (1).  When the point is over a cluster composed
	 * of multiple characters, the trailing argument will be filled with the
	 * position of the character in the cluster that is closest to
	 * the point.
	 *
	 * @param x the x coordinate of the point
	 * @param y the y coordinate of the point
	 * @param trailing the trailing buffer
	 * @return the character offset
	 *
	 * @see #getLocation(int, boolean)
	 */
	int GetOffset(int x, int y, int *trailing, int count) {
		WPoint point(x, y);
		return GetOffset(point, trailing, count);
	}
	/**
	 * Returns the orientation of the receiver.
	 *
	 * @return the orientation style
	 */
	int GetOrientation() {
		return w_textlayout_get_orientation(W_TEXTLAYOUT(this));
	}
	/**
	 * Returns the previous offset for the specified offset and movement
	 * type.  The movement is one of <code>W_MOVEMENT_CHAR</code>,
	 * <code>W_MOVEMENT_CLUSTER</code> or <code>W_MOVEMENT_WORD</code>,
	 * <code>W_MOVEMENT_WORD_END</code> or <code>W_MOVEMENT_WORD_START</code>.
	 *
	 * @param offset the start offset
	 * @param movement the movement type
	 * @return the previous offset
	 *
	 * @see #getNextOffset(int, int)
	 */
	int GetPreviousOffset(int offset, int movement) {
		return w_textlayout_get_previous_offset(W_TEXTLAYOUT(this), offset,
				movement, W_ENCODING_UTF8);
	}
	/**
	 * Gets the ranges of text that are associated with a <code>TextStyle</code>.
	 *
	 * @return the ranges, an array of offsets representing the start and end of each
	 * text style.
	 *
	 * @see #getStyles()
	 */
	WResult GetRanges(WIterator<WTextStyleRange> &ranges) {
		return w_textlayout_get_ranges(W_TEXTLAYOUT(this),
				(w_iterator*) &ranges);
	}
	/**
	 * Returns the line spacing of the receiver.
	 *
	 * @return the line spacing
	 */
	int GetSpacing() {
		return w_textlayout_get_spacing(W_TEXTLAYOUT(this));
	}
	/**
	 * Gets the style of the receiver at the specified character offset.
	 *
	 * @param offset the text offset
	 * @return the style or <code>null</code> if not set
	 */
	bool GetStyle(int offset, WTextStyle &style) {
		return w_textlayout_get_style(W_TEXTLAYOUT(this), offset,
				(w_textstyle*) &style, W_ENCODING_UTF8) > 0;
	}
	/**
	 * Returns the tab list of the receiver.
	 *
	 * @return the tab list
	 */
	int GetTabs(int *tabs, int count) {
		return w_textlayout_get_tabs(W_TEXTLAYOUT(this), tabs, count);
	}
	/**
	 * Gets the receiver's text, which will be an empty
	 * string if it has never been set.
	 *
	 * @return the receiver's text
	 */
	WString GetText() {
		WString str;
		w_textlayout_get_text(W_TEXTLAYOUT(this), w_alloc_string_ref, &str.ref,
				W_ENCODING_UTF8);
		return str;
	}
	/**
	 * Returns the text direction of the receiver.
	 *
	 * @return the text direction value
	 */
	int GetTextDirection() {
		return w_textlayout_get_text_direction(W_TEXTLAYOUT(this));
	}
	/**
	 * Returns the width of the receiver.
	 *
	 * @return the width
	 */
	int GetWidth() {
		return w_textlayout_get_width(W_TEXTLAYOUT(this));
	}
	/**
	 * Returns the receiver's wrap indent.
	 *
	 * @return the receiver's wrap indent
	 */
	int GetWrapIndent() {
		return w_textlayout_get_wrap_indent(W_TEXTLAYOUT(this));
	}
	/**
	 * Sets the text alignment for the receiver. The alignment controls
	 * how a line of text is positioned horizontally. The argument should
	 * be one of <code>W_LEFT</code>, <code>W_RIGHT</code> or <code>W_CENTER</code>.
	 * <p>
	 * The default alignment is <code>W_LEFT</code>.  Note that the receiver's
	 * width must be set in order to use <code>W_RIGHT</code> or <code>W_CENTER</code>
	 * alignment.
	 * </p>
	 *
	 * @param alignment the new alignment
	 *
	 * @see #setWidth(int)
	 */
	bool SetAlignment(int alignment) {
		return w_textlayout_set_alignment(W_TEXTLAYOUT(this), alignment);
	}
	/**
	 * Sets the ascent of the receiver. The ascent is distance in pixels
	 * from the baseline to the top of the line and it is applied to all
	 * lines. The default value is <code>-1</code> which means that the
	 * ascent is calculated from the line fonts.
	 *
	 * @param ascent the new ascent
	 *
	 * @see #setDescent(int)
	 * @see #getLineMetrics(int)
	 */
	bool SetAscent(int ascent) {
		return w_textlayout_set_ascent(W_TEXTLAYOUT(this), ascent);
	}
	/**
	 * Sets the descent of the receiver. The descent is distance in pixels
	 * from the baseline to the bottom of the line and it is applied to all
	 * lines. The default value is <code>-1</code> which means that the
	 * descent is calculated from the line fonts.
	 *
	 * @param descent the new descent
	 *
	 * @see #setAscent(int)
	 * @see #getLineMetrics(int)
	 */
	bool SetDescent(int descent) {
		return w_textlayout_set_descent(W_TEXTLAYOUT(this), descent);
	}
	/**
	 * Sets the default font which will be used by the receiver
	 * to draw and measure text. If the
	 * argument is null, then a default font appropriate
	 * for the platform will be used instead. Note that a text
	 * style can override the default font.
	 *
	 * @param font the new font for the receiver, or null to indicate a default font
	 */
	bool SetFont(WFont *font) {
		return w_textlayout_set_font(W_TEXTLAYOUT(this), (w_font*) font);
	}
	/**
	 * Sets the indent of the receiver. This indent is applied to the first line of
	 * each paragraph.
	 *
	 * @param indent new indent
	 *
	 * @see #setWrapIndent(int)
	 */
	bool SetIndent(int indent) {
		return w_textlayout_set_indent(W_TEXTLAYOUT(this), indent);
	}
	/**
	 * Sets the justification of the receiver. Note that the receiver's
	 * width must be set in order to use justification.
	 *
	 * @param justify new justify
	 */
	bool SetJustify(bool justify) {
		return w_textlayout_set_justify(W_TEXTLAYOUT(this), justify);
	}
	/**
	 * Sets the orientation of the receiver, which must be one
	 * of <code>W_LEFT_TO_RIGHT</code> or <code>W_RIGHT_TO_LEFT</code>.
	 *
	 * @param orientation new orientation style
	 */
	bool SetOrientation(int orientation) {
		return w_textlayout_set_orientation(W_TEXTLAYOUT(this), orientation);
	}
	/**
	 * Sets the line spacing of the receiver.  The line spacing
	 * is the space left between lines.
	 *
	 * @param spacing the new line spacing
	 */
	bool SetSpacing(int spacing) {
		return w_textlayout_set_spacing(W_TEXTLAYOUT(this), spacing);
	}
	/**
	 * Sets the style of the receiver for the specified range.  Styles previously
	 * set for that range will be overwritten.  The start and end offsets are
	 * inclusive and will be clamped if out of range.
	 *
	 * @param style the style
	 * @param start the start offset
	 * @param end the end offset
	 */
	bool SetStyle(WTextStyle &style, int start, int end) {
		return w_textlayout_set_style(W_TEXTLAYOUT(this), (w_textstyle*) &style,
				start, end, W_ENCODING_UTF8) > 0;
	}
	bool SetForeground(int start, int end, int foreground) {
		WTextStyle style;
		style.SetForeground(foreground);
		return SetStyle(style, start, end);
	}
	bool SetBackground(int start, int end, int background) {
		WTextStyle style;
		style.SetBackground(background);
		return SetStyle(style, start, end);
	}
	/**
	 * Sets the receiver's tab list. Each value in the tab list specifies
	 * the space in points from the origin of the text layout to the respective
	 * tab stop.  The last tab stop width is repeated continuously.
	 *
	 * @param tabs the new tab list
	 */
	bool SetTabs(int *tabs, int count) {
		return w_textlayout_set_tabs(W_TEXTLAYOUT(this), tabs, count);
	}
	/**
	 * Sets the receiver's text.
	 *<p>
	 * Note: Setting the text also clears all the styles. This method
	 * returns without doing anything if the new text is the same as
	 * the current text.
	 * </p>
	 *
	 * @param text the new text
	 */
	bool SetText(const char *text, int length) {
		return w_textlayout_set_text(W_TEXTLAYOUT(this), text, length) > 0;
	}
	/**
	 * Sets the receiver's text.
	 *<p>
	 * Note: Setting the text also clears all the styles. This method
	 * returns without doing anything if the new text is the same as
	 * the current text.
	 * </p>
	 *
	 * @param text the new text
	 */
	bool SetText(const char *text) {
		return SetText(text, -1);
	}
	/**
	 * Sets the text direction of the receiver, which must be one
	 * of <code>W_LEFT_TO_RIGHT</code>, <code>W_RIGHT_TO_LEFT</code>
	 * or <code>W_AUTO_TEXT_DIRECTION</code>.
	 *
	 * <p>
	 * <b>Warning</b>: This API is currently only implemented on Windows.
	 * It doesn't set the base text direction on GTK and Cocoa.
	 * </p>
	 *
	 * @param textDirection the new text direction
	 */
	bool SetTextDirection(int textDirection) {
		return w_textlayout_set_text_direction(W_TEXTLAYOUT(this),
				textDirection);
	}
	/**
	 * Sets the line width of the receiver, which determines how
	 * text should be wrapped and aligned. The default value is
	 * <code>-1</code> which means wrapping is disabled.
	 *
	 * @param width the new width
	 *
	 * @see #setAlignment(int)
	 */
	bool SetWidth(int width) {
		return w_textlayout_set_width(W_TEXTLAYOUT(this), width) > 0;
	}
	/**
	 * Sets the wrap indent of the receiver. This indent is applied to all lines
	 * in the paragraph except the first line.
	 *
	 * @param wrapIndent new wrap indent
	 *
	 * @see #setIndent(int)
	 */
	bool SetWrapIndent(int wrapIndent) {
		return w_textlayout_set_wrap_indent(W_TEXTLAYOUT(this), wrapIndent);
	}
private:
	void *handles[sizeof(w_textlayout) / sizeof(void*)];
};

#endif /* SWTP_GRAPHICS_TEXTLAYOUT_H_ */
