#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>

enum {
  SCANCODE_UNKNOWN = 0,
  SCANCODE_A = 4,
  SCANCODE_B = 5,
  SCANCODE_C = 6,
  SCANCODE_D = 7,
  SCANCODE_E = 8,
  SCANCODE_F = 9,
  SCANCODE_G = 10,
  SCANCODE_H = 11,
  SCANCODE_I = 12,
  SCANCODE_J = 13,
  SCANCODE_K = 14,
  SCANCODE_L = 15,
  SCANCODE_M = 16,
  SCANCODE_N = 17,
  SCANCODE_O = 18,
  SCANCODE_P = 19,
  SCANCODE_Q = 20,
  SCANCODE_R = 21,
  SCANCODE_S = 22,
  SCANCODE_T = 23,
  SCANCODE_U = 24,
  SCANCODE_V = 25,
  SCANCODE_W = 26,
  SCANCODE_X = 27,
  SCANCODE_Y = 28,
  SCANCODE_Z = 29,
  SCANCODE_1 = 30,
  SCANCODE_2 = 31,
  SCANCODE_3 = 32,
  SCANCODE_4 = 33,
  SCANCODE_5 = 34,
  SCANCODE_6 = 35,
  SCANCODE_7 = 36,
  SCANCODE_8 = 37,
  SCANCODE_9 = 38,
  SCANCODE_0 = 39,
  SCANCODE_RETURN = 40,
  SCANCODE_ESCAPE = 41,
  SCANCODE_BACKSPACE = 42,
  SCANCODE_TAB = 43,
  SCANCODE_SPACE = 44,
  SCANCODE_MINUS = 45,
  SCANCODE_EQUALS = 46,
  SCANCODE_LEFTBRACKET = 47,
  SCANCODE_RIGHTBRACKET = 48,
  SCANCODE_BACKSLASH = 49, 
  SCANCODE_NONUSHASH = 50, 
  SCANCODE_SEMICOLON = 51,
  SCANCODE_APOSTROPHE = 52,
  SCANCODE_GRAVE = 53, // ~/`
  SCANCODE_COMMA = 54,
  SCANCODE_PERIOD = 55,
  SCANCODE_SLASH = 56,
  SCANCODE_CAPSLOCK = 57,
  SCANCODE_F1 = 58,
  SCANCODE_F2 = 59,
  SCANCODE_F3 = 60,
  SCANCODE_F4 = 61,
  SCANCODE_F5 = 62,
  SCANCODE_F6 = 63,
  SCANCODE_F7 = 64,
  SCANCODE_F8 = 65,
  SCANCODE_F9 = 66,
  SCANCODE_F10 = 67,
  SCANCODE_F11 = 68,
  SCANCODE_F12 = 69,
  SCANCODE_PRINTSCREEN = 70,
  SCANCODE_SCROLLLOCK = 71,
  SCANCODE_PAUSE = 72,
  SCANCODE_INSERT = 73,
  SCANCODE_HOME = 74,
  SCANCODE_PAGEUP = 75,
  SCANCODE_DELETE = 76,
  SCANCODE_END = 77,
  SCANCODE_PAGEDOWN = 78,
  SCANCODE_RIGHT = 79,
  SCANCODE_LEFT = 80,
  SCANCODE_DOWN = 81,
  SCANCODE_UP = 82,
  SCANCODE_NUMLOCKCLEAR = 83, 
  SCANCODE_KP_DIVIDE = 84,
  SCANCODE_KP_MULTIPLY = 85,
  SCANCODE_KP_MINUS = 86,
  SCANCODE_KP_PLUS = 87,
  SCANCODE_KP_ENTER = 88,
  SCANCODE_KP_1 = 89,
  SCANCODE_KP_2 = 90,
  SCANCODE_KP_3 = 91,
  SCANCODE_KP_4 = 92,
  SCANCODE_KP_5 = 93,
  SCANCODE_KP_6 = 94,
  SCANCODE_KP_7 = 95,
  SCANCODE_KP_8 = 96,
  SCANCODE_KP_9 = 97,
  SCANCODE_KP_0 = 98,
  SCANCODE_KP_PERIOD = 99,
  SCANCODE_NONUSBACKSLASH = 100,
  SCANCODE_APPLICATION = 101,
  SCANCODE_POWER = 102,
  SCANCODE_KP_EQUALS = 103,
  SCANCODE_F13 = 104,
  SCANCODE_F14 = 105,
  SCANCODE_F15 = 106,
  SCANCODE_F16 = 107,
  SCANCODE_F17 = 108,
  SCANCODE_F18 = 109,
  SCANCODE_F19 = 110,
  SCANCODE_F20 = 111,
  SCANCODE_F21 = 112,
  SCANCODE_F22 = 113,
  SCANCODE_F23 = 114,
  SCANCODE_F24 = 115,
  SCANCODE_EXECUTE = 116,
  SCANCODE_HELP = 117,
  SCANCODE_MENU = 118,
  SCANCODE_SELECT = 119,
  SCANCODE_STOP = 120,
  SCANCODE_AGAIN = 121, /**< redo */
  SCANCODE_UNDO = 122,
  SCANCODE_CUT = 123,
  SCANCODE_COPY = 124,
  SCANCODE_PASTE = 125,
  SCANCODE_FIND = 126,
  SCANCODE_MUTE = 127,
  SCANCODE_VOLUMEUP = 128,
  SCANCODE_VOLUMEDOWN = 129,
  SCANCODE_KP_COMMA = 133,
  SCANCODE_KP_EQUALSAS400 = 134,
  SCANCODE_INTERNATIONAL1 = 135, /**< used on Asian keyboards, see footnotes in USB doc */
  SCANCODE_INTERNATIONAL2 = 136,
  SCANCODE_INTERNATIONAL3 = 137, /**< Yen */
  SCANCODE_INTERNATIONAL4 = 138,
  SCANCODE_INTERNATIONAL5 = 139,
  SCANCODE_INTERNATIONAL6 = 140,
  SCANCODE_INTERNATIONAL7 = 141,
  SCANCODE_INTERNATIONAL8 = 142,
  SCANCODE_INTERNATIONAL9 = 143,
  SCANCODE_LANG1 = 144, /**< Hangul/English toggle */
  SCANCODE_LANG2 = 145, /**< Hanja conversion */
  SCANCODE_LANG3 = 146, /**< Katakana */
  SCANCODE_LANG4 = 147, /**< Hiragana */
  SCANCODE_LANG5 = 148, /**< Zenkaku/Hankaku */
  SCANCODE_LANG6 = 149, /**< reserved */
  SCANCODE_LANG7 = 150, /**< reserved */
  SCANCODE_LANG8 = 151, /**< reserved */
  SCANCODE_LANG9 = 152, /**< reserved */
  SCANCODE_ALTERASE = 153, /**< Erase-Eaze */
  SCANCODE_SYSREQ = 154,
  SCANCODE_CANCEL = 155,
  SCANCODE_CLEAR = 156,
  SCANCODE_PRIOR = 157,
  SCANCODE_RETURN2 = 158,
  SCANCODE_SEPARATOR = 159,
  SCANCODE_OUT = 160,
  SCANCODE_OPER = 161,
  SCANCODE_CLEARAGAIN = 162,
  SCANCODE_CRSEL = 163,
  SCANCODE_EXSEL = 164,
  SCANCODE_KP_00 = 176,
  SCANCODE_KP_000 = 177,
  SCANCODE_THOUSANDSSEPARATOR = 178,
  SCANCODE_DECIMALSEPARATOR = 179,
  SCANCODE_CURRENCYUNIT = 180,
  SCANCODE_CURRENCYSUBUNIT = 181,
  SCANCODE_KP_LEFTPAREN = 182,
  SCANCODE_KP_RIGHTPAREN = 183,
  SCANCODE_KP_LEFTBRACE = 184,
  SCANCODE_KP_RIGHTBRACE = 185,
  SCANCODE_KP_TAB = 186,
  SCANCODE_KP_BACKSPACE = 187,
  SCANCODE_KP_A = 188,
  SCANCODE_KP_B = 189,
  SCANCODE_KP_C = 190,
  SCANCODE_KP_D = 191,
  SCANCODE_KP_E = 192,
  SCANCODE_KP_F = 193,
  SCANCODE_KP_XOR = 194,
  SCANCODE_KP_POWER = 195,
  SCANCODE_KP_PERCENT = 196,
  SCANCODE_KP_LESS = 197,
  SCANCODE_KP_GREATER = 198,
  SCANCODE_KP_AMPERSAND = 199,
  SCANCODE_KP_DBLAMPERSAND = 200,
  SCANCODE_KP_VERTICALBAR = 201,
  SCANCODE_KP_DBLVERTICALBAR = 202,
  SCANCODE_KP_COLON = 203,
  SCANCODE_KP_HASH = 204,
  SCANCODE_KP_SPACE = 205,
  SCANCODE_KP_AT = 206,
  SCANCODE_KP_EXCLAM = 207,
  SCANCODE_KP_MEMSTORE = 208,
  SCANCODE_KP_MEMRECALL = 209,
  SCANCODE_KP_MEMCLEAR = 210,
  SCANCODE_KP_MEMADD = 211,
  SCANCODE_KP_MEMSUBTRACT = 212,
  SCANCODE_KP_MEMMULTIPLY = 213,
  SCANCODE_KP_MEMDIVIDE = 214,
  SCANCODE_KP_PLUSMINUS = 215,
  SCANCODE_KP_CLEAR = 216,
  SCANCODE_KP_CLEARENTRY = 217,
  SCANCODE_KP_BINARY = 218,
  SCANCODE_KP_OCTAL = 219,
  SCANCODE_KP_DECIMAL = 220,
  SCANCODE_KP_HEXADECIMAL = 221,
  SCANCODE_LCTRL = 224,
  SCANCODE_LSHIFT = 225,
  SCANCODE_LALT = 226, /**< alt, option */
  SCANCODE_LGUI = 227, /**< windows, command (apple), meta */
  SCANCODE_RCTRL = 228,
  SCANCODE_RSHIFT = 229,
  SCANCODE_RALT = 230, /**< alt gr, option */
  SCANCODE_RGUI = 231, /**< windows, command (apple), meta */
  SCANCODE_MODE = 257,
  SCANCODE_AUDIONEXT = 258,
  SCANCODE_AUDIOPREV = 259,
  SCANCODE_AUDIOSTOP = 260,
  SCANCODE_AUDIOPLAY = 261,
  SCANCODE_AUDIOMUTE = 262,
  SCANCODE_MEDIASELECT = 263,
  SCANCODE_WWW = 264,
  SCANCODE_MAIL = 265,
  SCANCODE_CALCULATOR = 266,
  SCANCODE_COMPUTER = 267,
  SCANCODE_AC_SEARCH = 268,
  SCANCODE_AC_HOME = 269,
  SCANCODE_AC_BACK = 270,
  SCANCODE_AC_FORWARD = 271,
  SCANCODE_AC_STOP = 272,
  SCANCODE_AC_REFRESH = 273,
  SCANCODE_AC_BOOKMARKS = 274,
  SCANCODE_BRIGHTNESSDOWN = 275,
  SCANCODE_BRIGHTNESSUP = 276,
  SCANCODE_DISPLAYSWITCH = 277, /**< display mirroring/dual display switch, video mode switch */
  SCANCODE_KBDILLUMTOGGLE = 278,
  SCANCODE_KBDILLUMDOWN = 279,
  SCANCODE_KBDILLUMUP = 280,
  SCANCODE_EJECT = 281,
  SCANCODE_SLEEP = 282,
  SCANCODE_APP1 = 283,
  SCANCODE_APP2 = 284,
  SCANCODE_AUDIOREWIND = 285,
  SCANCODE_AUDIOFASTFORWARD = 286,
  NUM_SCANCODES = 512 /**< not a key, just marks the number of scancodes for array bounds */
};


#define K_SCANCODE_MASK (1<<30)
#define SCANCODE_TO_KEYCODE(X)  (X | K_SCANCODE_MASK)

enum {
  K_UNKNOWN = 0,
  K_RETURN = '\r',
  K_ESCAPE = '\x1B',
  K_BACKSPACE = '\b',
  K_TAB = '\t',
  K_SPACE = ' ',
  K_EXCLAIM = '!',
  K_QUOTEDBL = '"',
  K_HASH = '#',
  K_PERCENT = '%',
  K_DOLLAR = '$',
  K_AMPERSAND = '&',
  K_QUOTE = '\'',
  K_LEFTPAREN = '(',
  K_RIGHTPAREN = ')',
  K_ASTERISK = '*',
  K_PLUS = '+',
  K_COMMA = ',',
  K_MINUS = '-',
  K_PERIOD = '.',
  K_SLASH = '/',
  K_0 = '0',
  K_1 = '1',
  K_2 = '2',
  K_3 = '3',
  K_4 = '4',
  K_5 = '5',
  K_6 = '6',
  K_7 = '7',
  K_8 = '8',
  K_9 = '9',
  K_COLON = ':',
  K_SEMICOLON = ';',
  K_LESS = '<',
  K_EQUALS = '=',
  K_GREATER = '>',
  K_QUESTION = '?',
  K_AT = '@',
  K_LEFTBRACKET = '[',
  K_BACKSLASH = '\\',
  K_RIGHTBRACKET = ']',
  K_CARET = '^',
  K_UNDERSCORE = '_',
  K_BACKQUOTE = '`',
  K_a = 'a',
  K_b = 'b',
  K_c = 'c',
  K_d = 'd',
  K_e = 'e',
  K_f = 'f',
  K_g = 'g',
  K_h = 'h',
  K_i = 'i',
  K_j = 'j',
  K_k = 'k',
  K_l = 'l',
  K_m = 'm',
  K_n = 'n',
  K_o = 'o',
  K_p = 'p',
  K_q = 'q',
  K_r = 'r',
  K_s = 's',
  K_t = 't',
  K_u = 'u',
  K_v = 'v',
  K_w = 'w',
  K_x = 'x',
  K_y = 'y',
  K_z = 'z',
  K_CAPSLOCK = SCANCODE_TO_KEYCODE(SCANCODE_CAPSLOCK),
  K_F1 = SCANCODE_TO_KEYCODE(SCANCODE_F1),
  K_F2 = SCANCODE_TO_KEYCODE(SCANCODE_F2),
  K_F3 = SCANCODE_TO_KEYCODE(SCANCODE_F3),
  K_F4 = SCANCODE_TO_KEYCODE(SCANCODE_F4),
  K_F5 = SCANCODE_TO_KEYCODE(SCANCODE_F5),
  K_F6 = SCANCODE_TO_KEYCODE(SCANCODE_F6),
  K_F7 = SCANCODE_TO_KEYCODE(SCANCODE_F7),
  K_F8 = SCANCODE_TO_KEYCODE(SCANCODE_F8),
  K_F9 = SCANCODE_TO_KEYCODE(SCANCODE_F9),
  K_F10 = SCANCODE_TO_KEYCODE(SCANCODE_F10),
  K_F11 = SCANCODE_TO_KEYCODE(SCANCODE_F11),
  K_F12 = SCANCODE_TO_KEYCODE(SCANCODE_F12),
  K_PRINTSCREEN = SCANCODE_TO_KEYCODE(SCANCODE_PRINTSCREEN),
  K_SCROLLLOCK = SCANCODE_TO_KEYCODE(SCANCODE_SCROLLLOCK),
  K_PAUSE = SCANCODE_TO_KEYCODE(SCANCODE_PAUSE),
  K_INSERT = SCANCODE_TO_KEYCODE(SCANCODE_INSERT),
  K_HOME = SCANCODE_TO_KEYCODE(SCANCODE_HOME),
  K_PAGEUP = SCANCODE_TO_KEYCODE(SCANCODE_PAGEUP),
  K_DELETE = '\x7F',
  K_END = SCANCODE_TO_KEYCODE(SCANCODE_END),
  K_PAGEDOWN = SCANCODE_TO_KEYCODE(SCANCODE_PAGEDOWN),
  K_RIGHT = SCANCODE_TO_KEYCODE(SCANCODE_RIGHT),
  K_LEFT = SCANCODE_TO_KEYCODE(SCANCODE_LEFT),
  K_DOWN = SCANCODE_TO_KEYCODE(SCANCODE_DOWN),
  K_UP = SCANCODE_TO_KEYCODE(SCANCODE_UP),
  K_NUMLOCKCLEAR = SCANCODE_TO_KEYCODE(SCANCODE_NUMLOCKCLEAR),
  K_KP_DIVIDE = SCANCODE_TO_KEYCODE(SCANCODE_KP_DIVIDE),
  K_KP_MULTIPLY = SCANCODE_TO_KEYCODE(SCANCODE_KP_MULTIPLY),
  K_KP_MINUS = SCANCODE_TO_KEYCODE(SCANCODE_KP_MINUS),
  K_KP_PLUS = SCANCODE_TO_KEYCODE(SCANCODE_KP_PLUS),
  K_KP_ENTER = SCANCODE_TO_KEYCODE(SCANCODE_KP_ENTER),
  K_KP_1 = SCANCODE_TO_KEYCODE(SCANCODE_KP_1),
  K_KP_2 = SCANCODE_TO_KEYCODE(SCANCODE_KP_2),
  K_KP_3 = SCANCODE_TO_KEYCODE(SCANCODE_KP_3),
  K_KP_4 = SCANCODE_TO_KEYCODE(SCANCODE_KP_4),
  K_KP_5 = SCANCODE_TO_KEYCODE(SCANCODE_KP_5),
  K_KP_6 = SCANCODE_TO_KEYCODE(SCANCODE_KP_6),
  K_KP_7 = SCANCODE_TO_KEYCODE(SCANCODE_KP_7),
  K_KP_8 = SCANCODE_TO_KEYCODE(SCANCODE_KP_8),
  K_KP_9 = SCANCODE_TO_KEYCODE(SCANCODE_KP_9),
  K_KP_0 = SCANCODE_TO_KEYCODE(SCANCODE_KP_0),
  K_KP_PERIOD = SCANCODE_TO_KEYCODE(SCANCODE_KP_PERIOD),
  K_APPLICATION = SCANCODE_TO_KEYCODE(SCANCODE_APPLICATION),
  K_POWER = SCANCODE_TO_KEYCODE(SCANCODE_POWER),
  K_KP_EQUALS = SCANCODE_TO_KEYCODE(SCANCODE_KP_EQUALS),
  K_F13 = SCANCODE_TO_KEYCODE(SCANCODE_F13),
  K_F14 = SCANCODE_TO_KEYCODE(SCANCODE_F14),
  K_F15 = SCANCODE_TO_KEYCODE(SCANCODE_F15),
  K_F16 = SCANCODE_TO_KEYCODE(SCANCODE_F16),
  K_F17 = SCANCODE_TO_KEYCODE(SCANCODE_F17),
  K_F18 = SCANCODE_TO_KEYCODE(SCANCODE_F18),
  K_F19 = SCANCODE_TO_KEYCODE(SCANCODE_F19),
  K_F20 = SCANCODE_TO_KEYCODE(SCANCODE_F20),
  K_F21 = SCANCODE_TO_KEYCODE(SCANCODE_F21),
  K_F22 = SCANCODE_TO_KEYCODE(SCANCODE_F22),
  K_F23 = SCANCODE_TO_KEYCODE(SCANCODE_F23),
  K_F24 = SCANCODE_TO_KEYCODE(SCANCODE_F24),
  K_EXECUTE = SCANCODE_TO_KEYCODE(SCANCODE_EXECUTE),
  K_HELP = SCANCODE_TO_KEYCODE(SCANCODE_HELP),
  K_MENU = SCANCODE_TO_KEYCODE(SCANCODE_MENU),
  K_SELECT = SCANCODE_TO_KEYCODE(SCANCODE_SELECT),
  K_STOP = SCANCODE_TO_KEYCODE(SCANCODE_STOP),
  K_AGAIN = SCANCODE_TO_KEYCODE(SCANCODE_AGAIN),
  K_UNDO = SCANCODE_TO_KEYCODE(SCANCODE_UNDO),
  K_CUT = SCANCODE_TO_KEYCODE(SCANCODE_CUT),
  K_COPY = SCANCODE_TO_KEYCODE(SCANCODE_COPY),
  K_PASTE = SCANCODE_TO_KEYCODE(SCANCODE_PASTE),
  K_FIND = SCANCODE_TO_KEYCODE(SCANCODE_FIND),
  K_MUTE = SCANCODE_TO_KEYCODE(SCANCODE_MUTE),
  K_VOLUMEUP = SCANCODE_TO_KEYCODE(SCANCODE_VOLUMEUP),
  K_VOLUMEDOWN = SCANCODE_TO_KEYCODE(SCANCODE_VOLUMEDOWN),
  K_KP_COMMA = SCANCODE_TO_KEYCODE(SCANCODE_KP_COMMA),
  K_KP_EQUALSAS400 = SCANCODE_TO_KEYCODE(SCANCODE_KP_EQUALSAS400),
  K_ALTERASE = SCANCODE_TO_KEYCODE(SCANCODE_ALTERASE),
  K_SYSREQ = SCANCODE_TO_KEYCODE(SCANCODE_SYSREQ),
  K_CANCEL = SCANCODE_TO_KEYCODE(SCANCODE_CANCEL),
  K_CLEAR = SCANCODE_TO_KEYCODE(SCANCODE_CLEAR),
  K_PRIOR = SCANCODE_TO_KEYCODE(SCANCODE_PRIOR),
  K_RETURN2 = SCANCODE_TO_KEYCODE(SCANCODE_RETURN2),
  K_SEPARATOR = SCANCODE_TO_KEYCODE(SCANCODE_SEPARATOR),
  K_OUT = SCANCODE_TO_KEYCODE(SCANCODE_OUT),
  K_OPER = SCANCODE_TO_KEYCODE(SCANCODE_OPER),
  K_CLEARAGAIN = SCANCODE_TO_KEYCODE(SCANCODE_CLEARAGAIN),
  K_CRSEL = SCANCODE_TO_KEYCODE(SCANCODE_CRSEL),
  K_EXSEL = SCANCODE_TO_KEYCODE(SCANCODE_EXSEL),
  K_KP_00 = SCANCODE_TO_KEYCODE(SCANCODE_KP_00),
  K_KP_000 = SCANCODE_TO_KEYCODE(SCANCODE_KP_000),
  K_THOUSANDSSEPARATOR = SCANCODE_TO_KEYCODE(SCANCODE_THOUSANDSSEPARATOR),
  K_DECIMALSEPARATOR = SCANCODE_TO_KEYCODE(SCANCODE_DECIMALSEPARATOR),
  K_CURRENCYUNIT = SCANCODE_TO_KEYCODE(SCANCODE_CURRENCYUNIT),
  K_CURRENCYSUBUNIT = SCANCODE_TO_KEYCODE(SCANCODE_CURRENCYSUBUNIT),
  K_KP_LEFTPAREN = SCANCODE_TO_KEYCODE(SCANCODE_KP_LEFTPAREN),
  K_KP_RIGHTPAREN = SCANCODE_TO_KEYCODE(SCANCODE_KP_RIGHTPAREN),
  K_KP_LEFTBRACE = SCANCODE_TO_KEYCODE(SCANCODE_KP_LEFTBRACE),
  K_KP_RIGHTBRACE = SCANCODE_TO_KEYCODE(SCANCODE_KP_RIGHTBRACE),
  K_KP_TAB = SCANCODE_TO_KEYCODE(SCANCODE_KP_TAB),
  K_KP_BACKSPACE = SCANCODE_TO_KEYCODE(SCANCODE_KP_BACKSPACE),
  K_KP_A = SCANCODE_TO_KEYCODE(SCANCODE_KP_A),
  K_KP_B = SCANCODE_TO_KEYCODE(SCANCODE_KP_B),
  K_KP_C = SCANCODE_TO_KEYCODE(SCANCODE_KP_C),
  K_KP_D = SCANCODE_TO_KEYCODE(SCANCODE_KP_D),
  K_KP_E = SCANCODE_TO_KEYCODE(SCANCODE_KP_E),
  K_KP_F = SCANCODE_TO_KEYCODE(SCANCODE_KP_F),
  K_KP_XOR = SCANCODE_TO_KEYCODE(SCANCODE_KP_XOR),
  K_KP_POWER = SCANCODE_TO_KEYCODE(SCANCODE_KP_POWER),
  K_KP_PERCENT = SCANCODE_TO_KEYCODE(SCANCODE_KP_PERCENT),
  K_KP_LESS = SCANCODE_TO_KEYCODE(SCANCODE_KP_LESS),
  K_KP_GREATER = SCANCODE_TO_KEYCODE(SCANCODE_KP_GREATER),
  K_KP_AMPERSAND = SCANCODE_TO_KEYCODE(SCANCODE_KP_AMPERSAND),
  K_KP_DBLAMPERSAND = SCANCODE_TO_KEYCODE(SCANCODE_KP_DBLAMPERSAND),
  K_KP_VERTICALBAR = SCANCODE_TO_KEYCODE(SCANCODE_KP_VERTICALBAR),
  K_KP_DBLVERTICALBAR = SCANCODE_TO_KEYCODE(SCANCODE_KP_DBLVERTICALBAR),
  K_KP_COLON = SCANCODE_TO_KEYCODE(SCANCODE_KP_COLON),
  K_KP_HASH = SCANCODE_TO_KEYCODE(SCANCODE_KP_HASH),
  K_KP_SPACE = SCANCODE_TO_KEYCODE(SCANCODE_KP_SPACE),
  K_KP_AT = SCANCODE_TO_KEYCODE(SCANCODE_KP_AT),
  K_KP_EXCLAM = SCANCODE_TO_KEYCODE(SCANCODE_KP_EXCLAM),
  K_KP_MEMSTORE = SCANCODE_TO_KEYCODE(SCANCODE_KP_MEMSTORE),
  K_KP_MEMRECALL = SCANCODE_TO_KEYCODE(SCANCODE_KP_MEMRECALL),
  K_KP_MEMCLEAR = SCANCODE_TO_KEYCODE(SCANCODE_KP_MEMCLEAR),
  K_KP_MEMADD = SCANCODE_TO_KEYCODE(SCANCODE_KP_MEMADD),
  K_KP_MEMSUBTRACT = SCANCODE_TO_KEYCODE(SCANCODE_KP_MEMSUBTRACT),
  K_KP_MEMMULTIPLY = SCANCODE_TO_KEYCODE(SCANCODE_KP_MEMMULTIPLY),
  K_KP_MEMDIVIDE = SCANCODE_TO_KEYCODE(SCANCODE_KP_MEMDIVIDE),
  K_KP_PLUSMINUS = SCANCODE_TO_KEYCODE(SCANCODE_KP_PLUSMINUS),
  K_KP_CLEAR = SCANCODE_TO_KEYCODE(SCANCODE_KP_CLEAR),
  K_KP_CLEARENTRY = SCANCODE_TO_KEYCODE(SCANCODE_KP_CLEARENTRY),
  K_KP_BINARY = SCANCODE_TO_KEYCODE(SCANCODE_KP_BINARY),
  K_KP_OCTAL = SCANCODE_TO_KEYCODE(SCANCODE_KP_OCTAL),
  K_KP_DECIMAL = SCANCODE_TO_KEYCODE(SCANCODE_KP_DECIMAL),
  K_KP_HEXADECIMAL = SCANCODE_TO_KEYCODE(SCANCODE_KP_HEXADECIMAL),
  K_LCTRL = SCANCODE_TO_KEYCODE(SCANCODE_LCTRL),
  K_LSHIFT = SCANCODE_TO_KEYCODE(SCANCODE_LSHIFT),
  K_LALT = SCANCODE_TO_KEYCODE(SCANCODE_LALT),
  K_LGUI = SCANCODE_TO_KEYCODE(SCANCODE_LGUI),
  K_RCTRL = SCANCODE_TO_KEYCODE(SCANCODE_RCTRL),
  K_RSHIFT = SCANCODE_TO_KEYCODE(SCANCODE_RSHIFT),
  K_RALT = SCANCODE_TO_KEYCODE(SCANCODE_RALT),
  K_RGUI = SCANCODE_TO_KEYCODE(SCANCODE_RGUI),
  K_MODE = SCANCODE_TO_KEYCODE(SCANCODE_MODE),
  K_AUDIONEXT = SCANCODE_TO_KEYCODE(SCANCODE_AUDIONEXT),
  K_AUDIOPREV = SCANCODE_TO_KEYCODE(SCANCODE_AUDIOPREV),
  K_AUDIOSTOP = SCANCODE_TO_KEYCODE(SCANCODE_AUDIOSTOP),
  K_AUDIOPLAY = SCANCODE_TO_KEYCODE(SCANCODE_AUDIOPLAY),
  K_AUDIOMUTE = SCANCODE_TO_KEYCODE(SCANCODE_AUDIOMUTE),
  K_MEDIASELECT = SCANCODE_TO_KEYCODE(SCANCODE_MEDIASELECT),
  K_WWW = SCANCODE_TO_KEYCODE(SCANCODE_WWW),
  K_MAIL = SCANCODE_TO_KEYCODE(SCANCODE_MAIL),
  K_CALCULATOR = SCANCODE_TO_KEYCODE(SCANCODE_CALCULATOR),
  K_COMPUTER = SCANCODE_TO_KEYCODE(SCANCODE_COMPUTER),
  K_AC_SEARCH = SCANCODE_TO_KEYCODE(SCANCODE_AC_SEARCH),
  K_AC_HOME = SCANCODE_TO_KEYCODE(SCANCODE_AC_HOME),
  K_AC_BACK = SCANCODE_TO_KEYCODE(SCANCODE_AC_BACK),
  K_AC_FORWARD = SCANCODE_TO_KEYCODE(SCANCODE_AC_FORWARD),
  K_AC_STOP = SCANCODE_TO_KEYCODE(SCANCODE_AC_STOP),
  K_AC_REFRESH = SCANCODE_TO_KEYCODE(SCANCODE_AC_REFRESH),
  K_AC_BOOKMARKS = SCANCODE_TO_KEYCODE(SCANCODE_AC_BOOKMARKS),
  K_BRIGHTNESSDOWN = SCANCODE_TO_KEYCODE(SCANCODE_BRIGHTNESSDOWN),
  K_BRIGHTNESSUP = SCANCODE_TO_KEYCODE(SCANCODE_BRIGHTNESSUP),
  K_DISPLAYSWITCH = SCANCODE_TO_KEYCODE(SCANCODE_DISPLAYSWITCH),
  K_KBDILLUMTOGGLE = SCANCODE_TO_KEYCODE(SCANCODE_KBDILLUMTOGGLE),
  K_KBDILLUMDOWN = SCANCODE_TO_KEYCODE(SCANCODE_KBDILLUMDOWN),
  K_KBDILLUMUP = SCANCODE_TO_KEYCODE(SCANCODE_KBDILLUMUP),
  K_EJECT = SCANCODE_TO_KEYCODE(SCANCODE_EJECT),
  K_SLEEP = SCANCODE_TO_KEYCODE(SCANCODE_SLEEP),
  K_APP1 = SCANCODE_TO_KEYCODE(SCANCODE_APP1),
  K_APP2 = SCANCODE_TO_KEYCODE(SCANCODE_APP2),
  K_AUDIOREWIND = SCANCODE_TO_KEYCODE(SCANCODE_AUDIOREWIND),
  K_AUDIOFASTFORWARD = SCANCODE_TO_KEYCODE(SCANCODE_AUDIOFASTFORWARD)
};


enum {
  MOD_NONE = 0x0000,
  MOD_LSHIFT = 0x0001,
  MOD_RSHIFT = 0x0002,
  MOD_LCTRL = 0x0040,
  MOD_RCTRL = 0x0080,
  MOD_LALT = 0x0100,
  MOD_RALT = 0x0200,
  MOD_LGUI = 0x0400,
  MOD_RGUI = 0x0800,
  MOD_NUM = 0x1000,
  MOD_CAPS = 0x2000,
  MOD_MODE = 0x4000,
  MOD_RESERVED = 0x8000,
  MOD_CTRL = MOD_LCTRL | MOD_RCTRL,
  MOD_SHIFT = MOD_LSHIFT | MOD_RSHIFT,
  MOD_ALT = MOD_LALT | MOD_RALT,
  MOD_GUI = MOD_LGUI | MOD_RGUI
};


// All game memory is encapsuled in this struct. It uses the basic
// technique of stack allocation.
typedef struct GameMemory
{
    uint8_t *ptr;
    uint8_t *cursor;
    size_t size;
} GameMemory;

// Allocate a block of memory
void *GameAllocateMemory(GameMemory *memory, size_t size)
{
  void *result = memory->cursor;
  memory->cursor += size;
  return result;
}

// Simple helper macro to make allocation of structs easier, you
// could also use a template for this
#define GameAllocateStruct(memory, type)                                       \
  (type *)GameAllocateMemory(memory, sizeof(type))

// I sure hope that these uint32_t cast to GLuint without any issues
typedef struct {
  uint32_t id;
} Shader;

typedef struct {
  uint32_t id;
} ShaderProgram;

#define PLATFORM_CREATE_SHADER_PROGRAM(n) ShaderProgram n(int shader_count, ...)
typedef PLATFORM_CREATE_SHADER_PROGRAM(PlatformCreateShaderProgramFn);

#define PLATFORM_LOAD_SHADER(n) Shader n(const char *filename)
typedef PLATFORM_LOAD_SHADER(PlatformLoadShaderFn);

#define PLATFORM_ATTACH_SHADER(n)                                              \
  void n(ShaderProgram program, Shader shader)
typedef PLATFORM_ATTACH_SHADER(PlatformAttachShaderFn);

#define PLATFORM_DETACH_SHADER(n)                                              \
  void n(ShaderProgram program, Shader shader)
typedef PLATFORM_DETACH_SHADER(PlatformDetachShaderFn);

#define PLATFORM_DELETE_SHADER(n) void n(Shader shader)
typedef PLATFORM_DELETE_SHADER(PlatformDeleteShaderFn);

// Demonstration boxes and/or particles
#define PLATFORM_DRAW_BOX(n)                                                   \
  void n(float x, float y, float width, float height, float r, float g,        \
         float b, float a, ShaderProgram program)
typedef PLATFORM_DRAW_BOX(PlatformDrawBoxFn);

// Image and Sprite loading
#define MAX_SURFACES 100
#define MAX_FILENAME_LENGTH 31

#define PLATFORM_ENSURE_IMAGE(n) void n(const char *filename, unsigned int texture_id)
typedef PLATFORM_ENSURE_IMAGE(PlatformEnsureImageFn);

#define PLATFORM_DRAW_TEXTURE(n)                                               \
  void n(unsigned int texture_index, float x, float y, float width,             \
         float height, int sprite_x, int sprite_y, int sprite_w, int sprite_h)
typedef PLATFORM_DRAW_TEXTURE(PlatformDrawTextureFn);

#define PLATFORM_QUIT(n) void n()
typedef PLATFORM_QUIT(PlatformQuitFn);

#define PLATFORM_CREATE_WINDOW(n)                                              \
  unsigned int n(const char *title, uint32_t x, uint32_t y, uint32_t width,    \
                 uint32_t height)
typedef PLATFORM_CREATE_WINDOW(PlatformCreateWindowFn);

#define PLATFORM_ENSURE_AUDIO(n)                                               \
  void n(const char *filename, unsigned int channel)
typedef PLATFORM_ENSURE_AUDIO(PlatformEnsureAudioFn);

#define PLATFORM_PLAY_AUDIO(n)                                                 \
  void n(unsigned int channel, uint16_t fade, uint16_t loops, uint8_t volume, uint32_t duration)
typedef PLATFORM_PLAY_AUDIO(PlatformPlayAudioFn);

#define PLATFORM_STOP_AUDIO(n) void n(unsigned int channel, int duration, bool fade)
typedef PLATFORM_STOP_AUDIO(PlatformStopAudioFn);

#define PLATFORM_ENSURE_MUSIC(n) void n(const char *filename, unsigned int track)
typedef PLATFORM_ENSURE_MUSIC(PlatformEnsureMusicFn);

#define PLATFORM_PLAY_MUSIC(n)                                                 \
  void n(unsigned int track, uint16_t fade, uint16_t loops,			\
         double position, uint8_t volume, uint8_t resume)
typedef PLATFORM_PLAY_MUSIC(PlatformPlayMusicFn);

#define PLATFORM_SET_POSITION_MUSIC(n) void n(double position)
typedef PLATFORM_SET_POSITION_MUSIC(PlatformSetPositionMusicFn);

#define PLATFORM_REWIND_MUSIC(n) void n()
typedef PLATFORM_REWIND_MUSIC(PlatformRewindMusicFn);

#define PLATFORM_PAUSE_MUSIC(n) void n()
typedef PLATFORM_PAUSE_MUSIC(PlatformPauseMusicFn);

#define PLATFORM_STOP_MUSIC(n) void n(uint16_t fade)
typedef PLATFORM_STOP_MUSIC(PlatformStopMusicFn);

#define PLATFORM_SCREENSHOT(n)                                                 \
  void n(unsigned int window, uint16_t x, uint16_t y, uint16_t width,          \
         uint16_t height)
typedef PLATFORM_SCREENSHOT(PlatformScreenshotFn);

enum { SOCKET_TCP = 0, SOCKET_UDP = 1 };

#define PLATFORM_LISTEN_AND_SERVE(n)                                           \
  unsigned int n(uint8_t socket_type, uint16_t port)
typedef PLATFORM_LISTEN_AND_SERVE(PlatformListenAndServeFn);

#define PLATFORM_CONNECT_TO_SERVER(n)                                          \
  unsigned int n(uint8_t socket_type, uint16_t port, const char *host)
typedef PLATFORM_CONNECT_TO_SERVER(PlatformConnectToServerFn);

#define PLATFORM_NET_SEND(n)                                                   \
  void n(unsigned int socket, void *message, long length)
typedef PLATFORM_NET_SEND(PlatformNetSendFn);

#define PLATFORM_NET_RECV(n)                                                   \
  void n(unsigned int socket, void *received, long length)
typedef PLATFORM_NET_RECV(PlatformNetRecvFn);

#define PLATFORM_CLOSE_CONNECTION(n) void n(unsigned int socket)
typedef PLATFORM_CLOSE_CONNECTION(PlatformCloseConnectionFn);

typedef struct
{
  // Draw
  PlatformCreateShaderProgramFn *PlatformCreateShaderProgram;
  PlatformLoadShaderFn *PlatformLoadShader;
  PlatformAttachShaderFn *PlatformAttachShader;
  PlatformDetachShaderFn *PlatformDetachShader;
  PlatformDeleteShaderFn *PlatformDeleteShader;
  PlatformDrawBoxFn *PlatformDrawBox;
  PlatformEnsureImageFn *PlatformEnsureImage;
  PlatformDrawTextureFn *PlatformDrawTexture;
  PlatformScreenshotFn *PlatformScreenshot;
  // App
  PlatformQuitFn *PlatformQuit;
  PlatformCreateWindowFn *PlatformCreateWindow;
  // Sound
  PlatformEnsureAudioFn *PlatformEnsureAudio;
  PlatformPlayAudioFn *PlatformPlayAudio;
  PlatformStopAudioFn *PlatformStopAudio;
  // Music
  PlatformEnsureMusicFn *PlatformEnsureMusic;
  PlatformPlayMusicFn *PlatformPlayMusic;
  PlatformSetPositionMusicFn *PlatformSetPositionMusic;
  PlatformRewindMusicFn *PlatformRewindMusic;
  PlatformPauseMusicFn *PlatformPauseMusic;
  PlatformStopMusicFn *PlatformStopMusic;
  //Net
  PlatformListenAndServeFn *PlatformListenAndServe;
  PlatformConnectToServerFn *PlatformConnectToServer;
  PlatformNetSendFn *PlatformNetSend;
  PlatformNetRecvFn *PlatformNetRecv;
  PlatformCloseConnectionFn *PlatformCloseConnection;
} PlatformAPI;

//
// These are all the game functions. These macros help maintain the
// signature across various places easier.
//

// This is an optional macro for exporting game funcs
#define func(a,b) extern a(b)

#define GAME_INIT(n) void n(GameMemory memory, PlatformAPI api, int screen_w, int screen_h)
typedef GAME_INIT(GameInitFn);
#define GAME_UPDATE(n) void n(float dt)
typedef GAME_UPDATE(GameUpdateFn);

#define GAME_RENDER(n) void n()
typedef GAME_RENDER(GameRenderFn);

#define GAME_QUIT(n) void n()
typedef GAME_QUIT(GameQuitFn);

#define GAME_LOW_MEMORY(n) void n()
typedef GAME_LOW_MEMORY(GameLowMemoryFn);

enum { CONNECT = 1, DISCONNECT = 0 };

enum { WINDOW_VISIBLE = 1, WINDOW_INVISIBLE = 0 };
enum { MOUSE_ENTERED = 1, MOUSE_LEFT = 0 };
enum { WINDOW_MAXIMIZED = 1, WINDOW_MINIMIZED = 0 };

#define GAME_WINDOW_SHOWN(n) void n(uint32_t window, uint8_t visible)
typedef GAME_WINDOW_SHOWN(GameWindowShownFn);
#define GAME_WINDOW_MOVED(n) void n(uint32_t window, int32_t x, int32_t y)
typedef GAME_WINDOW_MOVED(GameWindowMovedFn);
#define GAME_WINDOW_RESIZED(n) void n(uint window, int32_t width, int32_t height)
typedef GAME_WINDOW_RESIZED(GameWindowResizedFn);
#define GAME_WINDOW_MINMAXED(n) void n(uint32_t window, uint8_t max)
typedef GAME_WINDOW_MINMAXED(GameWindowMinMaxedFn);
#define GAME_WINDOW_MOUSED(n) void n(uint32_t window, uint8_t enter)        
typedef GAME_WINDOW_MOUSED(GameWindowMousedFn);
#define GAME_WINDOW_FOCUSED(n) void n(uint32_t window, uint8_t gained) 
typedef GAME_WINDOW_FOCUSED(GameWindowFocusedFn);
#define GAME_WINDOW_CLOSED(n) void n(uint32_t window)
typedef GAME_WINDOW_CLOSED(GameWindowClosedFn);

enum {
  BUTTON_PRESSED = 1,
  BUTTON_RELEASED = 0
};

#define GAME_KEYBOARD_INPUT(n)                                                 \
  void n(uint32_t window, uint8_t key_state, uint8_t repeat, uint8_t symbol)
typedef GAME_KEYBOARD_INPUT(GameKeyboardInputFn);

#define GAME_MOUSE_MOTION(n)                                                   \
  void n(uint32_t window, uint32_t mouse, int32_t x, int32_t y, int32_t xrel, int32_t yrel)
typedef GAME_MOUSE_MOTION(GameMouseMotionFn);

#define MOUSE_BUTTON(X)       (1 << ((X)-1))
#define BUTTON_LEFT     1
#define BUTTON_MIDDLE   2
#define BUTTON_RIGHT    3
#define BUTTON_X1       4
#define BUTTON_X2       5
#define BUTTON_LMASK    MOUSE_BUTTON(BUTTON_LEFT)
#define BUTTON_MMASK    MOUSE_BUTTON(BUTTON_MIDDLE)
#define BUTTON_RMASK    MOUSE_BUTTON(BUTTON_RIGHT)
#define BUTTON_X1MASK   MOUSE_BUTTON(BUTTON_X1)
#define BUTTON_X2MASK   MOUSE_BUTTON(BUTTON_X2)

#define GAME_MOUSE_BUTTON(n)                                                   \
  void n(uint32_t window, uint32_t mouse, uint8_t button, uint8_t state,       \
         uint8_t clicks, uint32_t x, uint32_t y)
typedef GAME_MOUSE_BUTTON(GameMouseButtonFn);

enum {
  MOUSE_WHEEL_DIRECTION_NORMAL = 0,
  MOUSE_WHEEL_DIRECTION_FLIPPED
};

#define GAME_MOUSE_WHEEL(n)                                                    \
  void n(uint32_t window, uint32_t mouse, int32_t x, int32_t y,                \
         uint8_t direction)
typedef GAME_MOUSE_WHEEL(GameMouseWheelFn);

enum {
  JOYSTICK_TYPE_UNKNOWN,
  JOYSTICK_TYPE_GAMECONTROLLER,
  JOYSTICK_TYPE_WHEEL,
  JOYSTICK_TYPE_ARCADE_STICK,
  JOYSTICK_TYPE_FLIGHT_STICK,
  JOYSTICK_TYPE_DANCE_PAD,
  JOYSTICK_TYPE_GUITAR,
  JOYSTICK_TYPE_DRUM_KIT,
  JOYSTICK_TYPE_ARCADE_PAD,
  JOYSTICK_TYPE_THROTTLE
};


#define GAME_JOY_DEVICE_EVENT(n) void n(uint32_t id, uint8_t event)
typedef GAME_JOY_DEVICE_EVENT(GameJoyDeviceEventFn);

#define GAME_JOY_BUTTON_EVENT(n) void n(uint32_t id, uint8_t button, uint8_t state)
typedef GAME_JOY_BUTTON_EVENT(GameJoyButtonEventFn);

enum {
  HAT_CENTERED = 0x00,
  HAT_UP = 0x01,
  HAT_RIGHT = 0x02,
  HAT_DOWN = 0x04,
  HAT_LEFT = 0x08,
  HAT_RIGHTUP = (HAT_RIGHT | HAT_UP),
  HAT_RIGHTDOWN = (HAT_RIGHT | HAT_DOWN),
  HAT_LEFTUP = (HAT_LEFT | HAT_UP),
  HAT_LEFTDOWN = (HAT_LEFT | HAT_DOWN),
};


#define GAME_JOY_HAT_EVENT(n) void n(uint32_t id, uint8_t hat, uint8_t value)
typedef GAME_JOY_HAT_EVENT(GameJoyHatEventFn);

#define GAME_JOY_BALL_EVENT(n) void n(uint32_t id, uint8_t ball, int16_t xrel, int16_t yrel)
typedef GAME_JOY_BALL_EVENT(GameJoyBallEventFn);

#define AXIS_X 0
#define AXIS_Y 1
#define AXIS_Z 2

#define GAME_JOY_AXIS_EVENT(n) void n(uint32_t id, uint8_t axis, uint16_t value)
typedef GAME_JOY_AXIS_EVENT(GameJoyAxisEventFn);

#define GAME_CONTROLLER_EVENT(n) void n(uint32_t id, uint8_t event)
typedef GAME_CONTROLLER_EVENT(GameControllerEventFn);

#define GAME_CONTROLLER_AXIS_EVENT(n)                                          \
  void n(uint32_t id, uint8_t axis, int16_t value)
typedef GAME_CONTROLLER_AXIS_EVENT(GameControllerAxisEventFn);

enum {
  CONTROLLER_TYPE_UNKNOWN = 0,
  CONTROLLER_TYPE_XBOX360,
  CONTROLLER_TYPE_XBOXONE,
  CONTROLLER_TYPE_PS3,
  CONTROLLER_TYPE_PS4,
  CONTROLLER_TYPE_NINTENDO_SWITCH_PRO,
  CONTROLLER_TYPE_VIRTUAL,
  CONTROLLER_TYPE_PS5
};

#define GAME_CONTROLLER_BUTTON_EVENT(n)                                        \
  void n(uint32_t id, uint8_t button, uint8_t state)
typedef GAME_CONTROLLER_BUTTON_EVENT(GameControllerButtonEventFn);

enum { TOUCHPAD_DOWN = 0, TOUCHPAD_UP, TOUCHPAD_MOTION };

#define GAME_CONTROLLER_TOUCHPAD_EVENT(n)                                      \
  void n(uint32_t id, uint8_t event, int32_t finger, float x, float y,         \
         float pressure)
typedef GAME_CONTROLLER_TOUCHPAD_EVENT(GameControllerTouchpadEventFn);

enum {
  SENSOR_UNKNOWN = 0,
  SENSOR_ACCEL,
  SENSOR_GYRO,
};

#define GAME_CONTROLLER_SENSOR_EVENT(n)                                        \
  void n(uint32_t id, int8_t sensor, const float *data, uint8_t length)
typedef GAME_CONTROLLER_SENSOR_EVENT(GameControllerSensorEventFn);

#define GAME_AUDIO_DEVICE_EVENT(n)                                             \
  void n(uint32_t id, uint8_t event, uint8_t is_input) 
typedef GAME_AUDIO_DEVICE_EVENT(GameAudioDeviceEventFn);

#define GAME_CHANNEL_HALTED(n) void n(uint8_t channel)
typedef GAME_CHANNEL_HALTED(GameChannelHaltedFn);

#define GAME_MUSIC_HALTED(n) void n()
typedef GAME_MUSIC_HALTED(GameMusicHaltedFn);

enum {
  TOUCH_DEVICE_INVALID = -1,
  TOUCH_DEVICE_DIRECT, /* touch screen with window-relative coordinates */
  TOUCH_DEVICE_INDIRECT_ABSOLUTE, /* trackpad with absolute device
                                     coordinates */
  TOUCH_DEVICE_INDIRECT_RELATIVE  /* trackpad with screen cursor-relative
                                         coordinates */
};

#define GAME_TOUCH_FINGER_EVENT(n)                                             \
  void n(uint32_t window, int64_t touch_id, int64_t finger_id, uint8_t event,  \
         float x, float y, float dx, float dy, float pressure)
typedef GAME_TOUCH_FINGER_EVENT(GameTouchFingerEventFn);

enum { DROP_COMPLETE = 0, DROP_BEGIN, DROP_FILE, DROP_TEXT };

#define GAME_DROP_EVENT(n)                                                     \
  void n(uint32_t window, uint8_t event, const char *file);
typedef GAME_DROP_EVENT(GameDropEventFn);

#define GAME_SENSOR_EVENT(n) void n(uint32_t id, int8_t type, const float *data, uint8_t length)
typedef GAME_SENSOR_EVENT(GameSensorEventFn);

#define GAME_USER_EVENT(n)                                                     \
  void n(uint32_t window, uint32_t type, int32_t code, const void *data, const void *data2)
typedef GAME_USER_EVENT(GameUserEventFn);

// Event map
enum {
  QUIT = 0x100, 
  APP_TERMINATING,
  APP_LOWMEMORY,
  APP_WILLENTERBACKGROUND,
  APP_DIDENTERBACKGROUND,
  APP_WILLENTERFOREGROUND,
  APP_DIDENTERFOREGROUND,
  LOCALECHANGED,        
  DISPLAYEVENT = 0x150, 
  WINDOWEVENT = 0x200,  
  KEYDOWN = 0x300,
  KEYUP,
  TEXTEDITING,
  TEXTINPUT,
  KEYMAPCHANGED,
  MOUSEMOTION = 0x400,
  MOUSEBUTTONDOWN,
  MOUSEBUTTONUP,
  MOUSEWHEEL,
  JOYAXISMOTION = 0x600,
  JOYBALLMOTION,
  JOYHATMOTION,
  JOYBUTTONDOWN,
  JOYBUTTONUP,
  JOYDEVICEADDED,
  JOYDEVICEREMOVED,
  CONTROLLERAXISMOTION = 0x650,
  CONTROLLERBUTTONDOWN,
  CONTROLLERBUTTONUP,
  CONTROLLERDEVICEADDED,
  CONTROLLERDEVICEREMOVED,
  CONTROLLERDEVICEREMAPPED,
  CONTROLLERTOUCHPADDOWN,
  CONTROLLERTOUCHPADMOTION,
  CONTROLLERTOUCHPADUP,
  CONTROLLERSENSORUPDATE,
  FINGERDOWN = 0x700,
  FINGERUP,
  FINGERMOTION,
  DOLLARGESTURE = 0x800,
  DOLLARRECORD,
  MULTIGESTURE,
  CLIPBOARDUPDATE = 0x900,
  DROPFILE = 0x1000,
  DROPTEXT,
  DROPBEGIN,
  DROPCOMPLETE,
  AUDIODEVICEADDED = 0x1100,
  AUDIODEVICEREMOVED,
  SENSORUPDATE = 0x1200,
  RENDER_TARGETS_RESET = 0x2000,
  RENDER_DEVICE_RESET,
  USEREVENT = 0x8000,
};

