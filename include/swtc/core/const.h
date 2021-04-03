/*
 * Name:        const.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef SWTC_CORE_CONST_H_
#define SWTC_CORE_CONST_H_
#include "defs.h"
/**
 * Style constant
 */
enum {
	/*
	 * control
	 */
	W_BORDER = 1 << 0, //Control
	W_CLIP_CHILDREN = 1 << 1, //Control
	W_CLIP_SIBLINGS = 1 << 2, //Control
	W_LEFT_TO_RIGHT = 1 << 3, //Control,Menu,GC
	W_RIGHT_TO_LEFT = 1 << 4, //Control,Menu,GC
	W_MIRRORED = 1 << 5, //Control,Menu
	W_DOUBLE_BUFFERED = 1 << 6, //Control
	W_FLIP_TEXT_DIRECTION = 1 << 7, //Control
	W_FREE_MEMORY = 1 << 8, //
	W_DELETE_ON_DISPOSE = W_FREE_MEMORY,
	/*
	 * scrollable
	 */
	W_HSCROLL = 1 << 9, //Scrollable
	W_VSCROLL = 1 << 10, //Scrollable
	/*
	 * composite
	 */
	W_NO_BACKGROUND = 1 << 11, //Composite
	W_NO_FOCUS = 1 << 12, //Composite
	W_NO_REDRAW_RESIZE = 1 << 13, //Composite
	W_NO_MERGE_PAINTS = 1 << 14, //Composite
	W_NO_RADIO_GROUP = 1 << 15, //Composite,Menu
	W_TRANSPARENT = 1 << 16, //Composite
	/*
	 * shell style
	 */
	W_TOOL = 1 << 17, //Shell
	W_NO_TRIM = 1 << 18, //Shell
	W_RESIZE = 1 << 19, //Shell,Tracker
	W_CLOSE = 1 << 20, //Shell
	W_MENU = W_CLOSE, //Shell
	W_MIN = 1 << 21, //Shell
	W_MAX = 1 << 22, //Shell
	W_NO_MOVE = 1 << 23, //Shell
	W_ON_TOP = 1 << 24, //Shell
	W_SHEET = 1 << 25, //Dialog,Shell
	W_SHELL_TRIM = W_CLOSE | W_MIN | W_MAX | W_RESIZE,
	W_DIALOG_TRIM = W_CLOSE | W_BORDER,
	W_MODELESS = 26, //Dialog,Shell
	W_PRIMARY_MODAL = 1 << 27, //Dialog,Shell
	W_APPLICATION_MODAL = 1 << 28, //Dialog,Shell
	W_SYSTEM_MODAL = 1 << 29, //Dialog,Shell
	W_DISPOSE_ON_CLOSE = 1 << 30,
	W_TITLE = 1 << 31,

	/*
	 *tree
	 */
	W_MULTI = 1 << 18, //Text,List,Table,Tree,FileDialog
	W_CHECK = 1 << 19, //Button,MenuItem,ToolItem,Table,Tree
	W_VIRTUAL = 1 << 20, //Table,Tree
	W_SINGLE = 1 << 21, //Text,List,Table,Tree
	W_UP = 1 << 22, //Button,Tracker,Table,Tree
	W_DOWN = 1 << 23, //Button,Tracker,Table,Tree
	W_NO_SCROLL = 1 << 24, //Tree,Table
	W_HIDE_SELECTION = 1 << 25, //Table
	W_FULL_SELECTION = 1 << 26, //Table,Tree,StyledText,TextLayout

	W_HORIZONTAL = 1 << 18, //Label,ProgressBar,Sash,Scale,ScrollBar,Slider,ToolBar,FillLayout,RowLayout
	W_VERTICAL = 1 << 19, //Label,ProgressBar,Sash,Scale,ScrollBar,Slider,ToolBar,CoolBar,FillLayout,RowLayout
	W_DROP_DOWN = 1 << 18, //Menu,ToolItem,CoolItem,Combo,DateTime
	W_WRAP = 1 << 20, //Button,Label,Text,ToolBar,Spinner
	W_READ_ONLY = 1 << 19, //Combo,Text
	W_SIMPLE = 1 << 20, //Combo
	W_SHADOW_IN = 1 << 21, //Label,Group
	W_SHADOW_OUT = 1 << 22, //Label,Group,ToolBar
	W_SHADOW_ETCHED_IN = 1 << 18, //Group
	W_SHADOW_ETCHED_OUT = 1 << 19, //Group
	W_SHADOW_NONE = 1 << 23, //Label,Group

	W_FLAT = 1 << 21, //Button,ToolBar
	W_DATE = 1 << 19, //DateTime
	W_TIME = 1 << 20, //DateTime
	W_CALENDAR = 1 << 21, //DateTime
	W_SHORT = 1 << 22, //DateTime
	W_MEDIUM = 1 << 23, //DateTime
	W_LONG = 1 << 24, //DateTime
	W_WEBKIT = 1 << 18, //Browser
	/*
	 * text
	 */
	W_LEFT = 1 << 11, //Button,Label,Text,TableColumn,TreeColumn,Tracker
	W_RIGHT = 1 << 12, //Button,Label,Text,TableColumn,TreeColumn,Tracker
	W_SEARCH = 1 << 13, //Text
	W_PASSWORD = 1 << 14, //Text

	W_CENTER = 1 << 13, //Button,Label,TableColumn
	W_BAR = 1 << 11, //Menu
	W_POP_UP = 1 << 12, //Menu
	W_SEPARATOR = 1 << 14, //Label,MenuItem,ToolItem
	W_TOGGLE = 1 << 14, //Button
	W_ARROW = 1 << 15, //Button
	W_PUSH = 1 << 16, //Button,MenuItem,ToolItem
	W_RADIO = 1 << 17, //Button,MenuItem,ToolItem
	W_HYPERLINK = 1 << 20, //Button
	W_CASCADE = 1 << 15, //MenuItem
	W_CASCADE_END = 1 << 31,
	W_INDETERMINATE = 1 << 11, //ProgressBar
	W_SMOOTH = 1 << 12, //ProgressBar,Sash
	W_BALLOON = 1 << 13, //ToolTip
	W_SCALE = 1 << 21,

	W_TOP = W_UP,
	W_BOTTOM = W_DOWN, //
	W_LEAD = W_LEFT,
	W_TRAIL = W_RIGHT,

	W_CUSTOM_DRAW = 1 << 6,
	W_AUTO_TEXT_DIRECTION = W_LEFT_TO_RIGHT | W_RIGHT_TO_LEFT,
	W_UNDERLINE_SINGLE = 0, //TextStyle
	W_UNDERLINE_DOUBLE = 1, //TextStyle
	W_UNDERLINE_ERROR = 2, //TextStyle
	W_UNDERLINE_SQUIGGLE = 3, //TextStyle
	W_UNDERLINE_LINK = 4, //TextStyle
	W_BORDER_SOLID = 1, //TextStyle
	W_BORDER_DASH = 2, //TextStyle
	W_BORDER_DOT = 4, //TextStyle
	W_BEGINNING = 1, //GridLayout
	W_FILL = 4, //GridLayout
	/*
	 *
	 */
	W_SEPARATOR_FILL = -2,
	/**
	 * Input Method Editor style constant for native
	 * input behavior (value is 1&lt;&lt;3).
	 */
	W_DBCS = 1 << 1,
	W_ALPHA = 1 << 2,
	W_NATIVE = 1 << 3,
	W_PHONETIC = 1 << 4,
	W_ROMAN = 1 << 5,
};
/*
 *
 */
enum {
	W_BS = '\b',
	W_CR = '\r',
	W_DEL = 0x7F,
	W_ESC = 0x1B,
	W_LF = '\n',
	W_TAB = '\t',
	W_SPACE = ' ',
	W_ALT = 1 << 16,
	W_SHIFT = 1 << 17,
	W_CTRL = 1 << 18,
	W_COMMAND = 1 << 22,
	W_MODIFIER_MASK = W_ALT | W_SHIFT | W_CTRL | W_COMMAND,
	W_BUTTON1 = 1 << 19,
	W_BUTTON2 = 1 << 20,
	W_BUTTON3 = 1 << 21,
	W_BUTTON4 = 1 << 23,
	W_BUTTON5 = 1 << 25,
	W_BUTTON_MASK = W_BUTTON1 | W_BUTTON2 | W_BUTTON3 | W_BUTTON4 | W_BUTTON5,
	/*
	 #ifdef _cocoa
	 W_MOD1 = W_COMMAND,
	 W_MOD2 = W_SHIFT,
	 W_MOD3 = W_ALT,
	 W_MOD4 = W_CONTROL,
	 #else
	 W_MOD1 = W_CTRL,
	 W_MOD2 = W_SHIFT,
	 W_MOD3 = W_ALT,
	 W_MOD4 = 0,
	 #endif
	 */
	W_SCROLL_LINE = 1,
	W_SCROLL_PAGE = 2,
	W_KEYCODE_BIT = (1 << 24),
	W_KEY_MASK = W_KEYCODE_BIT + 0xFFFF,
	W_ARROW_UP = W_KEYCODE_BIT + 1,
	W_ARROW_DOWN = W_KEYCODE_BIT + 2,
	W_ARROW_LEFT = W_KEYCODE_BIT + 3,
	W_ARROW_RIGHT = W_KEYCODE_BIT + 4,
	W_PAGE_UP = W_KEYCODE_BIT + 5,
	W_PAGE_DOWN = W_KEYCODE_BIT + 6,
	W_HOME = W_KEYCODE_BIT + 7,
	W_END = W_KEYCODE_BIT + 8,
	W_INSERT = W_KEYCODE_BIT + 9,
	W_F1 = W_KEYCODE_BIT + 10,
	W_F2 = W_KEYCODE_BIT + 11,
	W_F3 = W_KEYCODE_BIT + 12,
	W_F4 = W_KEYCODE_BIT + 13,
	W_F5 = W_KEYCODE_BIT + 14,
	W_F6 = W_KEYCODE_BIT + 15,
	W_F7 = W_KEYCODE_BIT + 16,
	W_F8 = W_KEYCODE_BIT + 17,
	W_F9 = W_KEYCODE_BIT + 18,
	W_F10 = W_KEYCODE_BIT + 19,
	W_F11 = W_KEYCODE_BIT + 20,
	W_F12 = W_KEYCODE_BIT + 21,
	W_F13 = W_KEYCODE_BIT + 22,
	W_F14 = W_KEYCODE_BIT + 23,
	W_F15 = W_KEYCODE_BIT + 24,
	W_F16 = W_KEYCODE_BIT + 25,
	W_F17 = W_KEYCODE_BIT + 26,
	W_F18 = W_KEYCODE_BIT + 27,
	W_F19 = W_KEYCODE_BIT + 28,
	W_F20 = W_KEYCODE_BIT + 29,
	W_KEYPAD = 1 << 1,
	W_KEYPAD_MULTIPLY = W_KEYCODE_BIT + 42,
	W_KEYPAD_ADD = W_KEYCODE_BIT + 43,
	W_KEYPAD_SUBTRACT = W_KEYCODE_BIT + 45,
	W_KEYPAD_DECIMAL = W_KEYCODE_BIT + 46,
	W_KEYPAD_DIVIDE = W_KEYCODE_BIT + 47,
	W_KEYPAD_0 = W_KEYCODE_BIT + 48,
	W_KEYPAD_1 = W_KEYCODE_BIT + 49,
	W_KEYPAD_2 = W_KEYCODE_BIT + 50,
	W_KEYPAD_3 = W_KEYCODE_BIT + 51,
	W_KEYPAD_4 = W_KEYCODE_BIT + 52,
	W_KEYPAD_5 = W_KEYCODE_BIT + 53,
	W_KEYPAD_6 = W_KEYCODE_BIT + 54,
	W_KEYPAD_7 = W_KEYCODE_BIT + 55,
	W_KEYPAD_8 = W_KEYCODE_BIT + 56,
	W_KEYPAD_9 = W_KEYCODE_BIT + 57,
	W_KEYPAD_EQUAL = W_KEYCODE_BIT + 61,
	W_KEYPAD_CR = W_KEYCODE_BIT + 80,
	W_HELP = W_KEYCODE_BIT + 81,
	W_CAPS_LOCK = W_KEYCODE_BIT + 82,
	W_NUM_LOCK = W_KEYCODE_BIT + 83,
	W_SCROLL_LOCK = W_KEYCODE_BIT + 84,
	W_PAUSE = W_KEYCODE_BIT + 85,
	W_BREAK = W_KEYCODE_BIT + 86,
	W_PRINT_SCREEN = W_KEYCODE_BIT + 87,
};
SWT_PUBLIC int w_key_mod(wuint id);
#define	W_MOD1 w_key_mod(1)
#define	W_MOD2 w_key_mod(2)
#define	W_MOD3 w_key_mod(3)
#define	W_MOD4 w_key_mod(4)
/*
 * icon
 */
enum {
	W_ICON_ERROR = 1,
	W_ICON_INFORMATION = 1 << 1,
	W_ICON_QUESTION = 1 << 2,
	W_ICON_WARNING = 1 << 3,
	W_ICON_WORKING = 1 << 4,
	W_ICON_SEARCH = 1 << 9,
	W_ICON_CANCEL = 1 << 8,
};
/**
 * The <code>MessageBox</code> style constant,
 */
enum {
	W_OK = 1 << 5,
	W_YES = 1 << 6,
	W_NO = 1 << 7,
	W_CANCEL = 1 << 8,
	W_ABORT = 1 << 9,
	W_RETRY = 1 << 10,
	W_IGNORE = 1 << 11,
	W_OPEN = 1 << 12,
	W_SAVE = 1 << 13,
	W_INHERIT_NONE = 0,
	W_INHERIT_DEFAULT = 1,
	W_INHERIT_FORCE = 2,
};
/*
 * color
 */
enum {
	W_COLOR_WHITE = 0xFFFFFFFF,
	W_COLOR_BLACK = 0xFF000000,
	W_COLOR_RED = 0xFF0000FF,
	W_COLOR_DARK_RED = 0xFF800000,
	W_COLOR_GREEN = 0xFF00FF00,
	W_COLOR_DARK_GREEN = 0xFF008000,
	W_COLOR_YELLOW = 0xFF00FFFF,
	W_COLOR_DARK_YELLOW = 0xFF008080,
	W_COLOR_BLUE = 0xFFFF0000,
	W_COLOR_DARK_BLUE = 0xFF800000,
	W_COLOR_MAGENTA = 0xFFFF00FF,
	W_COLOR_DARK_MAGENTA = 0xFF800080,
	W_COLOR_CYAN = 0xFFFFFF00,
	W_COLOR_DARK_CYAN = 0xFF008080,
	W_COLOR_GRAY = 0xFF808080,
	W_COLOR_DARK_GRAY = 0xFF404040,
};
/**
 * System color used to paint dark shadow areas (value is 17).
 */
enum {
	W_COLOR_WIDGET_DARK_SHADOW = 1,
	W_COLOR_WIDGET_NORMAL_SHADOW = 2,
	W_COLOR_WIDGET_LIGHT_SHADOW = 3,
	W_COLOR_WIDGET_HIGHLIGHT_SHADOW = 4,
	W_COLOR_WIDGET_FOREGROUND = 5,
	W_COLOR_WIDGET_BACKGROUND = 6,
	W_COLOR_WIDGET_BORDER = 7,
	W_COLOR_LIST_FOREGROUND = 8,
	W_COLOR_LIST_BACKGROUND = 9,
	W_COLOR_LIST_SELECTION = 10,
	W_COLOR_LIST_SELECTION_TEXT = 11,
	W_COLOR_INFO_FOREGROUND = 12,
	W_COLOR_INFO_BACKGROUND = 13,
	W_COLOR_TITLE_FOREGROUND = 14,
	W_COLOR_TITLE_BACKGROUND = 15,
	W_COLOR_TITLE_BACKGROUND_GRADIENT = 16,
	W_COLOR_TITLE_INACTIVE_FOREGROUND = 17,
	W_COLOR_TITLE_INACTIVE_BACKGROUND = 18,
	W_COLOR_TITLE_INACTIVE_BACKGROUND_GRADIENT = 19,
	W_COLOR_LINK_FOREGROUND = 20,
	W_COLOR_TRANSPARENT = 21,
};

/**
 * Draw constant indicating whether the string drawing operation
 * should handle line-delimiters (value is 1&lt,&lt,1).
 */
enum {
	W_DRAW_TRANSPARENT = 1 << 0,
	W_DRAW_DELIMITER = 1 << 1,
	W_DRAW_TAB = 1 << 2,
	W_DRAW_MNEMONIC = 1 << 3,
	W_DRAW_LEFT = 1 << 4,
	W_DRAW_TOP = 1 << 5,
	W_DRAW_RIGHT = 1 << 6,
	W_DRAW_BOTTOM = 1 << 7,
	W_DRAW_HCENTER = 1 << 8,
	W_DRAW_VCENTER = 1 << 9,
	W_DRAW_CENTRE = W_DRAW_HCENTER | W_DRAW_VCENTER,
};
enum {
	W_CURSOR_ARROW = 0,
	W_CURSOR_WAIT = 1,
	W_CURSOR_CROSS = 2,
	W_CURSOR_APPSTARTING = 3,
	W_CURSOR_HELP = 4,
	W_CURSOR_SIZEALL = 5,
	W_CURSOR_SIZENESW = 6,
	W_CURSOR_SIZENS = 7,
	W_CURSOR_SIZENWSE = 8,
	W_CURSOR_SIZEWE = 9,
	W_CURSOR_SIZEN = 10,
	W_CURSOR_SIZES = 11,
	W_CURSOR_SIZEE = 12,
	W_CURSOR_SIZEW = 13,
	W_CURSOR_SIZENE = 14,
	W_CURSOR_SIZESE = 15,
	W_CURSOR_SIZESW = 16,
	W_CURSOR_SIZENW = 17,
	W_CURSOR_UPARROW = 18,
	W_CURSOR_IBEAM = 19,
	W_CURSOR_NO = 20,
	W_CURSOR_HAND = 21,
};
enum {
	W_DELIMITER_SELECTION = 1 << 17,
	W_LAST_LINE_SELECTION = 1 << 20,
	W_BITMAP = 0,
	W_ICON = 1,
	W_IMAGE_COPY = 0,
	W_IMAGE_DISABLE = 1,
	W_IMAGE_GRAY = 2,
	W_ERROR = 1,
	W_PAUSED = 1 << 2,
	W_NORMAL = 0,
	W_BOLD = 1 << 0,
	W_ITALIC = 1 << 1,
	W_CAP_FLAT = 1,
	W_CAP_ROUND = 2,
	W_CAP_SQUARE = 3,
	W_JOIN_MITER = 1,
	W_JOIN_ROUND = 2,
	W_JOIN_BEVEL = 3,
	W_LINE_SOLID = 1,
	W_LINE_DASH = 2,
	W_LINE_DOT = 3,
	W_LINE_DASHDOT = 4,
	W_LINE_DASHDOTDOT = 5,
	W_LINE_CUSTOM = 6,
	W_PATH_MOVE_TO = 1,
	W_PATH_LINE_TO = 2,
	W_PATH_QUAD_TO = 3,
	W_PATH_CUBIC_TO = 4,
	W_PATH_CLOSE = 5,
	W_FILL_EVEN_ODD = 1,
	W_FILL_WINDING = 2,
	W_IMAGE_UNDEFINED = -1,
	W_IMAGE_BMP = 0,
	W_IMAGE_BMP_RLE = 1,
	W_IMAGE_GIF = 2,
	W_IMAGE_ICO = 3,
	W_IMAGE_JPEG = 4,
	W_IMAGE_PNG = 5,
	W_IMAGE_TIFF = 6,
	W_IMAGE_OS2_BMP = 7,
	W_DM_UNSPECIFIED = 0x0,
	W_DM_FILL_NONE = 0x1,
	W_DM_FILL_BACKGROUND = 0x2,
	W_DM_FILL_PREVIOUS = 0x3,
	W_TRANSPARENCY_NONE = 0x0,
	W_TRANSPARENCY_ALPHA = 1 << 0,
	W_TRANSPARENCY_MASK = 1 << 1,
	W_TRANSPARENCY_PIXEL = 1 << 2,
	W_MOVEMENT_CHAR = 1 << 0,
	W_MOVEMENT_CLUSTER = 1 << 1,
	W_MOVEMENT_WORD = 1 << 2,
	W_MOVEMENT_WORD_END = 1 << 3,
	W_MOVEMENT_WORD_START = 1 << 4,
	W_ALL = 1 << 0,
	W_ID_ABOUT = -1,
	W_ID_HIDE = -3,
	W_ID_HIDE_OTHERS = -4,
	W_ID_SHOW_ALL = -5,
	W_ID_QUIT = -6,
	W_SCROLLBAR_OVERLAY = 1 << 1,
};
/*
 *
 */
enum {
	W_SELECTED = 1 << 1,
	W_FOCUSED = 1 << 2,
	W_BACKGROUND = 1 << 3,
	W_FOREGROUND = 1 << 4,
	W_HOT = 1 << 5,
	W_PRESSED = 1 << 6,
	W_ACTIVE = 1 << 7,
	W_DISABLED = 1 << 8,
	W_DEFAULTED = 1 << 9,
	W_GRAYED = 1 << 10,
};
/*
 * error
 */
enum {
	W_TRUE = 1,
	W_FALSE = 0,
	W_ERROR_NO_INDEX = -1,
	W_ERROR_NO_HANDLES = -2,
	W_ERROR_NO_MORE_CALLBACKS = -3,
	W_ERROR_NULL_ARGUMENT = -4,
	W_ERROR_INVALID_ARGUMENT = -5,
	W_ERROR_INVALID_RANGE = -6,
	W_ERROR_CANNOT_BE_ZERO = -7,
	W_ERROR_CANNOT_GET_ITEM = -8,
	W_ERROR_CANNOT_GET_SELECTION = -9,
	W_ERROR_CANNOT_INVERT_MATRIX = -10,
	W_ERROR_CANNOT_GET_ITEM_HEIGHT = -11,
	W_ERROR_CANNOT_GET_TEXT = -12,
	W_ERROR_CANNOT_SET_TEXT = -13,
	W_ERROR_ITEM_NOT_ADDED = -14,
	W_ERROR_ITEM_NOT_REMOVED = -15,
	W_ERROR_NO_GRAPHICS_LIBRARY = -16,
	W_ERROR_NOT_IMPLEMENTED = -20,
	W_ERROR_MENU_NOT_DROP_DOWN = -21,
	W_ERROR_THREAD_INVALID_ACCESS = -22,
	W_ERROR_WIDGET_DISPOSED = -24,
	W_ERROR_MENUITEM_NOT_CASCADE = -27,
	W_ERROR_CANNOT_SET_SELECTION = -28,
	W_ERROR_CANNOT_SET_MENU = -29,
	W_ERROR_CANNOT_SET_ENABLED = -30,
	W_ERROR_CANNOT_GET_ENABLED = -31,
	W_ERROR_INVALID_PARENT = -32,
	W_ERROR_MENU_NOT_BAR = -33,
	W_ERROR_CANNOT_GET_COUNT = -36,
	W_ERROR_MENU_NOT_POP_UP = -37,
	W_ERROR_UNSUPPORTED_DEPTH = -38,
	W_ERROR_IO = -39,
	W_ERROR_INVALID_IMAGE = -40,
	W_ERROR_UNSUPPORTED_FORMAT = -42,
	W_ERROR_INVALID_SUBCLASS = -43,
	W_ERROR_GRAPHIC_DISPOSED = -44,
	W_ERROR_DEVICE_DISPOSED = -45,
	W_ERROR_FAILED_EXEC = -46,
	W_ERROR_FAILED_LOAD_LIBRARY = -47,
	W_ERROR_INVALID_FONT = -48,
	W_ERROR_FUNCTION_DISPOSED = -49,
	W_ERROR_FAILED_EVALUATE = -50,
	W_ERROR_INVALID_RETURN_VALUE = -51,
	W_ERROR_NO_MEMORY = -52,
	W_ERROR_UNSPECIFIED = -53,
};
/*
 * classes
 */
typedef enum w_class_id {
	_W_CLASS_UNKNOWN = 0,
	//widget
	_W_CLASS_WIDGET,
	_W_CLASS_MENU = _W_CLASS_WIDGET,
	_W_CLASS_CARET,
	_W_CLASS_IME,
	_W_CLASS_TASKBAR,
	_W_CLASS_TOOLTIP,
	_W_CLASS_TRACKER,
	_W_CLASS_TRAY,
	_W_CLASS_DRAGSOURCE,
	_W_CLASS_DROPTARGET,
	//control
	_W_CLASS_CONTROL = _W_CLASS_WIDGET + 10,
	_W_CLASS_CCANVAS = _W_CLASS_CONTROL,
	_W_CLASS_BUTTON,
	_W_CLASS_LABEL,
	_W_CLASS_PROGRESSBAR,
	_W_CLASS_SASH,
	_W_CLASS_SLIDER,
	_W_CLASS_SCROLLBAR = _W_CLASS_SLIDER,

	//scrollable
	_W_CLASS_SCROLLABLE = _W_CLASS_CONTROL + 15,
	_W_CLASS_SCROLLABLE_CANVAS = _W_CLASS_SCROLLABLE,
	_W_CLASS_TEXTEDIT,

	//composite
	_W_CLASS_COMPOSITE = _W_CLASS_SCROLLABLE + 5,
	_W_CLASS_WEBVIEW,
	_W_CLASS_TREEVIEW,
	_W_CLASS_TABVIEW,
	_W_CLASS_COMBOBOX,
	_W_CLASS_COOLBAR,
	_W_CLASS_DATETIME,
	_W_CLASS_EXPANDBAR,
	_W_CLASS_GROUPBOX,
	_W_CLASS_SPINNER,
	_W_CLASS_LISTVIEW,
	_W_CLASS_TOOLBAR,
	//canvas
	_W_CLASS_CANVAS = _W_CLASS_COMPOSITE + 20,
	_W_CLASS_GLCANVAS,
	//shell
	_W_CLASS_SHELL = _W_CLASS_CANVAS + 5,
	_W_CLASS_LAST,
} w_class_id;

#endif /* SWTC_CORE_CONST_H_ */
