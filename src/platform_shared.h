#ifndef PLATFORM_SHARED_H
#define PLATFORM_SHARED_H

#include "shared.h"

typedef struct
{
  GameInitFn *game_init;
  GameUpdateFn *game_update;
  GameRenderFn *game_render;

  GameQuitFn *game_quit;
  GameLowMemoryFn *game_low_memory;

  GameWindowShownFn *game_window_shown;
  GameWindowMovedFn *game_window_moved;
  GameWindowResizedFn *game_window_resized;
  GameWindowMinMaxedFn *game_window_minmaxed;
  GameWindowMousedFn *game_window_moused;
  GameWindowFocusedFn *game_window_focused;
  GameWindowClosedFn *game_window_closed;

  GameKeyboardInputFn *game_keyboard_input;
  GameMouseMotionFn *game_mouse_motion;
  GameMouseButtonFn *game_mouse_button;
  GameMouseWheelFn *game_mouse_wheel;
  GameJoyDeviceEventFn *game_joy_device_event;
  GameJoyButtonEventFn *game_joy_button_event;
  GameJoyHatEventFn *game_joy_hat_event;
  GameJoyAxisEventFn *game_joy_axis_event;
  GameJoyBallEventFn *game_joy_ball_event;
  GameControllerEventFn *game_controller_device_event;
  GameControllerButtonEventFn *game_controller_button_event;
  GameControllerAxisEventFn *game_controller_axis_event;
  GameControllerSensorEventFn *game_controller_sensor_event;
  GameControllerTouchpadEventFn *game_controller_touchpad_event;
  GameAudioDeviceEventFn *game_audio_device_event;
  GameChannelHaltedFn *game_channel_halted;
  GameMusicHaltedFn *game_music_halted;
  GameTouchFingerEventFn *game_touch_finger_event;
  GameDropEventFn *game_drop_event;
  GameSensorEventFn *game_sensor_event;
  GameUserEventFn *game_user_event;
  
  void* handle;
  time_t last_file_time;
} GameCode;

#define PLATFORM_INIT(n) void n(MemoryBlock memory, GameCode game_code, int screen_w, int screen_h)
typedef PLATFORM_INIT(PlatformInitFn);

#define PLATFORM_GAME_LOOP(n) void n()
typedef PLATFORM_GAME_LOOP(PlatformGameLoopFn);

#endif
