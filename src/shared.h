#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_video.h>
#include <cstdint>
#include <stdio.h>
#include <cstddef>
#include <stdint.h>
#include <sys/types.h>
#include "kb_bs.h"

// All game memory is encapsuled in this struct. It uses the basic
// technique of stack allocation.
struct GameMemory
{
    uint8_t *ptr;
    uint8_t *cursor;
    size_t size;
};

// Allocate a block of memory
inline void *
GameAllocateMemory(GameMemory *memory, size_t size)
{
  void *result = memory->cursor;
  memory->cursor += size;
  return result;
}

// Simple helper macro to make allocation of structs easier, you
// could also use a template for this
#define GameAllocateStruct(memory, type)                                       \
  (type *)GameAllocateMemory(memory, sizeof(type))


// Demonstration boxes
#define PLATFORM_DRAW_BOX(n)                                                   \
  void n(float x, float y, float width, float height, float r, float g,        \
         float b, float a)
typedef PLATFORM_DRAW_BOX(PlatformDrawBoxFn);

// Demo boxes as a quad
#define PLATFORM_DRAW_QUAD(n)                                                   \
  void n(float bl_x, float bl_y, float br_x, float br_y, float ur_x,           \
         float ur_y, float ul_x, float ul_y, float r, float g, float b,        \
         float a)
typedef PLATFORM_DRAW_QUAD(PlatformDrawQuadFn);

// Image and Sprite loading
const unsigned int MAX_SURFACES = 3;
const unsigned int MAX_FILENAME_LENGTH = 31;

#define PLATFORM_ENSURE_IMAGE(n) unsigned int n(const char *filename)
typedef PLATFORM_ENSURE_IMAGE(PlatformEnsureImageFn);

#define PLATFORM_ENSURE_SPRITESHEET(n) unsigned int n(const char *filename)
typedef PLATFORM_ENSURE_SPRITESHEET(PlatformEnsureSpritesheetFn);

#define PLATFORM_DRAW_TEXTURE(n)                                               \
  void n(unsigned int textureIndex, float x, float y, float width,             \
         float height, int sprite_x, int sprite_y, int sprite_w, int sprite_h)
typedef PLATFORM_DRAW_TEXTURE(PlatformDrawTextureFn);

#define PLATFORM_QUIT(n) void n()
typedef PLATFORM_QUIT(PlatformQuitFn);

#define PLATFORM_CREATE_WINDOW(n)                                              \
  void n(const char *title, uint32_t x, uint32_t y, uint32_t width,            \
         uint32_t height)
typedef PLATFORM_CREATE_WINDOW(PlatformCreateWindowFn);

struct PlatformAPI
{
  PlatformDrawBoxFn *PlatformDrawBox;
  PlatformDrawQuadFn *PlatformDrawQuad;
  PlatformEnsureImageFn *PlatformEnsureImage;
  PlatformEnsureSpritesheetFn *PlatformEnsureSpritesheet;
  PlatformDrawTextureFn *PlatformDrawTexture;
  PlatformQuitFn *PlatformQuit;
  PlatformCreateWindowFn *PlatformCreateWindow;
};

//
// These are all the game functions. These macros help maintain the
// signature across various places easier.
//

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
#define GAME_WINDOW_RESIZED(n) void n(uint32_t window, int32_t width, int32_t height)
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

#define GAME_KEYBOARD_INPUT(n) void n(uint32_t window, uint8_t state, uint8_t repeat, uint8_t symbol)
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

#define GAME_AUDIO_DEVICE_EVENT(n)                                                  \
  void n(uint32_t id, uint8_t event, uint8_t is_input)
typedef GAME_AUDIO_DEVICE_EVENT(GameAudioDeviceEventFn);

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

