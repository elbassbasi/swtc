/*
 * Event.h
 *
 *  Created on: 25 juil. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_WIDGETS_EVENT_H_
#define SWTP_WIDGETS_EVENT_H_
#include "../graphics/graphics.h"
class WItem;
class WPlatformEvent;
class WWidget;
class WMenu;
/**
 * This is the super class for all typed event classes provided
 * by SWT. Typed events contain particular information which is
 * applicable to the event occurrence.
 */
class SWTP_PUBLIC WEvent {
public:
	/**
	 * the type that the event occurred.
	 */
	unsigned int type;
	/**
	 * the time that the event occurred.
	 */
	unsigned int time;
	/**
	 * the platform event is not used for user
	 */
	WPlatformEvent *platform_event;
	/**
	 * the widget that issued the event
	 */
	WWidget *widget;
	/**
	 * a field for application use
	 */
	void *data;
};
/**
 * Instances of this class are sent whenever mouse
 * related actions occur. This includes mouse buttons
 * being pressed and released, the mouse pointer being
 * moved and the mouse pointer crossing widget boundaries.
 * <p>
 * Note: The <code>button</code> field is an integer that
 * represents the mouse button number.  This is not the same
 * as the <code>SWT</code> mask constants <code>BUTTONx</code>.
 * </p>
 */
class SWTP_PUBLIC WMouseEvent: public WEvent {
public:
	union {
		struct {
			unsigned doit :1;
		};
		int detail;
	};
	/**
	 * the number times the mouse has been clicked, as defined
	 * by the operating system; 1 for the first click, 2 for the
	 * second click and so on.
	 */
	wushort clickcount;
	/**
	 * the button that was pressed or released;
	 * <ul>
	 * <li>1 for the first button (usually 'left')</li>
	 * <li>2 for the second button (usually 'middle')</li>
	 * <li>3 for the third button (usually 'right')</li>
	 * <li>etc.</li>
	 */
	wushort button;
	/**
	 * the widget-relative, x coordinate of the pointer
	 * at the time the mouse button was pressed or released
	 */
	int x;
	/**
	 * the widget-relative, y coordinate of the pointer
	 * at the time the mouse button was pressed or released
	 */
	int y;
public:
	bool IsAlt() {
		return this->detail & W_ALT;
	}
	bool IsShift() {
		return this->detail & W_SHIFT;
	}
	bool IsCtrl() {
		return this->detail & W_CTRL;
	}
	bool IsCommand() {
		return this->detail & W_COMMAND;
	}
	int ToChars(char *text, size_t size, char separator, const char *label) {
		return snprintf(text, size,
				"%s%cdetail = %x%cclickcount = %d%cbutton = %d%cx=%d%cy=%d",
				label, separator, detail, separator, clickcount, separator,
				button, separator, x, separator, y);
	}
	int ToChars(char *text, size_t size, char separator) {
		return ToChars(text, size, separator, "");
	}
	int ToChars(char *text, size_t size, const char *label) {
		return ToChars(text, size, '\n', label);
	}
	int ToChars(char *text, size_t size) {
		return ToChars(text, size, '\n', "");
	}
};
/**
 * Instances of this class are sent as a result of
 * keys being pressed and released on the keyboard.
 * <p>
 * When a key listener is added to a control, the control
 * will take part in widget traversal.  By default, all
 * traversal keys (such as the tab key and so on) are
 * delivered to the control.  In order for a control to take
 * part in traversal, it should listen for traversal events.
 * Otherwise, the user can traverse into a control but not
 * out.  Note that native controls such as table and tree
 * implement key traversal in the operating system.  It is
 * not necessary to add traversal listeners for these controls,
 * unless you want to override the default traversal.
 * </p>
 */
class SWTP_PUBLIC WKeyEvent: public WEvent {
public:
	/**
	 * The traversal type.
	 * <p><ul>
	 * <li>{@link W_TRAVERSE_NONE}</li>
	 * <li>{@link W_TRAVERSE_ESCAPE}</li>
	 * <li>{@link W_TRAVERSE_RETURN}</li>
	 * <li>{@link W_TRAVERSE_TAB_NEXT}</li>
	 * <li>{@link W_TRAVERSE_TAB_PREVIOUS}</li>
	 * <li>{@link W_TRAVERSE_ARROW_NEXT}</li>
	 * <li>{@link W_TRAVERSE_ARROW_PREVIOUS}</li>
	 * <li>{@link W_TRAVERSE_MNEMONIC}</li>
	 * <li>{@link W_TRAVERSE_PAGE_NEXT}</li>
	 * <li>{@link W_TRAVERSE_PAGE_PREVIOUS}</li>
	 * </ul></p>
	 *
	 * Setting this field will change the type of traversal.
	 * For example, setting the detail to <code>TRAVERSE_NONE</code>
	 * causes no traversal action to be taken.
	 *
	 * When used in conjunction with the <code>doit</code> field, the
	 * traversal detail field can be useful when overriding the default
	 * traversal mechanism for a control. For example, setting the doit
	 * field to <code>false</code> will cancel the operation and allow
	 * the traversal key stroke to be delivered to the control. Setting
	 * the doit field to <code>true</code> indicates that the traversal
	 * described by the detail field is to be performed.
	 */
	union {
		struct {
			unsigned doit :1;
		};
		int detail;
	};
	/**
	 * the character represented by the key that was typed.
	 * This is the final character that results after all modifiers have been
	 * applied.  For example, when the user types Ctrl+A, the character value
	 * is 0x01.  It is important that applications do not attempt to modify the
	 * character value based on a stateMask (such as SWT.CTRL) or the resulting
	 * character will not be correct.
	 */
	int character;
	/**
	 * the key code of the key that was typed,
	 * as defined by the key code constants in class <code>SWT</code>.
	 * When the character field of the event is ambiguous, this field
	 * contains the unicode value of the original character.  For example,
	 * typing Ctrl+M or Return both result in the character '\r' but the
	 * keyCode field will also contain '\r' when Return was typed.
	 */
	int keycode;
	/**
	 * depending on the event, the location of key specified by the
	 * keyCode or character. The possible values for this field are
	 * <code>SWT.LEFT</code>, <code>SWT.RIGHT</code>, <code>SWT.KEYPAD</code>,
	 * or <code>SWT.NONE</code> representing the main keyboard area.
	 * <p>
	 * The location field can be used to differentiate key events that have
	 * the same key code and character but are generated by different keys
	 * on the keyboard. For example, a key down event with the key code equal
	 * to SWT.SHIFT can be generated by the left and the right shift keys on
	 * the keyboard.
	 * </p><p>
	 * The location field can only be used to determine the location of
	 * the key code or character in the current event. It does not include
	 * information about the location of modifiers in the state mask.
	 * </p>
	 *
	 * @see W_LEFT
	 * @see W_RIGHT
	 * @see W_KEYPAD
	 */
	int keylocation;
public:
	bool IsAlt() {
		return this->detail & W_ALT;
	}
	bool IsShift() {
		return this->detail & W_SHIFT;
	}
	bool IsCtrl() {
		return this->detail & W_CTRL;
	}
	bool IsCommand() {
		return this->detail & W_COMMAND;
	}
public:
	int ToChars(char *text, size_t size, char separator, const char *label) {
		return snprintf(text, size,
				"%s%cdetail = %x%ccharacter = %d%ckeycode = %d%ckeylocation=%d",
				label, separator, detail, separator, character, separator,
				keycode, separator, keylocation);
	}
	int ToChars(char *text, size_t size, char separator) {
		return ToChars(text, size, separator, "");
	}
	int ToChars(char *text, size_t size, const char *label) {
		return ToChars(text, size, '\n', label);
	}
	int ToChars(char *text, size_t size) {
		return ToChars(text, size, '\n', "");
	}
};
/**
 * Instances of this class are sent whenever the platform-
 * specific trigger for showing a context menu is detected.
 */
class SWTP_PUBLIC WMenuDetectEvent: public WEvent {
public:
	/**
	 * The context menu trigger type.
	 * <p><ul>
	 * <li>{@link W_MENU_MOUSE}</li>
	 * <li>{@link W_MENU_KEYBOARD}</li>
	 * </ul></p>
	 *
	 * A field indicating whether the context menu was triggered by a
	 * pointing device such as a mouse (indicated by <code>MENU_MOUSE</code>),
	 * or by a focus-based device such as a keyboard (<code>MENU_KEYBOARD</code>).
	 * If the trigger was <code>MENU_KEYBOARD</code>, then the application should
	 * provide new display-relative x and y coordinates based on the current
	 * selection or the current focus.
	 */
	union {
		struct {
			unsigned doit :1;
		};
		int detail;
	};
	/**
	 * The display-relative coordinate of the pointer
	 * at the time the context menu trigger occurred.
	 */
	WPoint location;
	WMenu *menu;
};

/**
 * Instances of this class are sent in response to
 * touch-based gestures that are triggered by the user.
 */
class SWTP_PUBLIC WGestureEvent: public WEvent {
public:
	/**
	 * The gesture type.
	 * <p><ul>
	 * <li>{@link W_GESTURE_BEGIN}</li>
	 * <li>{@link W_GESTURE_END}</li>
	 * <li>{@link W_GESTURE_MAGNIFY}</li>
	 * <li>{@link W_GESTURE_PAN}</li>
	 * <li>{@link W_GESTURE_ROTATE}</li>
	 * <li>{@link W_GESTURE_SWIPE}</li>
	 * </ul></p>
	 *
	 * This field determines the <code>GestureEvent</code> fields that contain valid data.
	 */
	int detail;
	/**
	 * The meaning of this field is dependent on the value of the <code>detail</code> field
	 * and the platform.  It can represent either the coordinate of the centroid of the
	 * touches that make up the gesture, or the coordinate of the cursor at the time the
	 * gesture was performed.
	 */
	WPoint location;
	/**
	 * This field is valid when the <code>detail</code> field is set to <code>GESTURE_SWIPE</code>
	 * or <code>GESTURE_PAN</code>.  Both <code>direction</code>
	 * can be valid for an individual gesture.  The meaning of this field is dependent on the value
	 * of the <code>detail</code> field.
	 * <p>
	 * If <code>detail</code> is <code>GESTURE_SWIPE</code> then a positive value indicates a swipe
	 * to the right and a negative value indicates a swipe to the left.
	 *
	 * If <code>detail</code> is <code>GESTURE_PAN</code> then a positive value indicates a pan to
	 * the right by this field's count of pixels and a negative value indicates a pan to the left
	 * by this field's count of pixels.
	 */
	WPoint direction;

	/**
	 * This field is valid when the <code>detail</code> field is set to <code>GESTURE_ROTATE</code>.
	 * It specifies the number of degrees rotated on the device since the gesture started. Positive
	 * values indicate counter-clockwise rotation, and negative values indicate clockwise rotation.
	 */
	double rotation;
	/**
	 * This field is valid when the <code>detail</code> field is set to <code>GESTURE_MAGNIFY</code>.
	 * This is the scale factor to be applied. This value will be 1.0 in the first received event with
	 * <code>GESTURE_MAGNIFY</code>, and will then fluctuate in subsequent events as the user moves
	 * their fingers.
	 */

	double magnification;
};
class SWTP_PUBLIC WTouch {
private:
	void *handle;
public:
	void *id;
	union {
		struct {
			unsigned doit :1;
		};
		int detail;
	};
	WRect bounds;
	WPoint location;
};
/**
 * Instances of this class are sent in response to
 * a touch-based input source being touched.
 */
class SWTP_PUBLIC WTouchEvent: public WEvent {
public:
	union {
		struct {
			unsigned doit :1;
		};
		int detail;
	};
private:
	/**
	 * The set of touches representing the state of all contacts with touch input
	 * device at the time the event was generated.
	 *
	 * @see WTouch
	 */
	void *touches;
public:
	bool GetTouches(WIterator<WTouch> &touches) {
		return w_event_touch_get_touches((w_event_touch*) this,
				(w_iterator*) &touches);
	}
	/**
	 * The widget-relative coordinate of the pointer
	 * at the time the touch occurred.
	 */
	WPoint location;
};
class SWTP_PUBLIC WPaintEvent: public WEvent {
public:
	WRect bounds;
	WGraphics *gc;
	WGraphics* operator ->() {
		return this->gc;
	}
};
class SWTP_PUBLIC WTimerEvent: public WEvent {
public:
	wushort id;
};
class SWTP_PUBLIC WSelectionEvent: public WEvent {
public:
	union {
		struct {
			unsigned doit :1;
		};
		int detail;
	};
};

class SWTP_PUBLIC WTooltipTextEvent: public WEvent {
public:
	union {
		struct {
			unsigned doit :1;
		};
		int detail;
	};
	WPoint location;
	WValue text;
	void SetText(const char *text) {
		this->text.SetString(text);
	}
	bool CopyText(const char *text, size_t length) {
		return this->text.CopyString(text, length);
	}
	bool CopyText(const char *text) {
		return this->text.CopyString(text);
	}
	int Sprint(const char *format, ...) {
		va_list args;
		va_start(args, format);
		int ret = this->text.Print(format, args);
		va_end(args);
		return ret;
	}
};

#endif /* SWTP_WIDGETS_EVENT_H_ */
