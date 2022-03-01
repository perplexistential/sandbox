#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>

#include "shared.h"
#include "platform_shared.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define MAX_WINDOWS 4

#define BUILD_DIR "build"
#define PLATFORM_LIB "build/libplatform.so"
#define PLATFORM_LIB_TEMP "build/libplatform_temp.so"
#define GAME_LIB "build/libgame.so"
#define GAME_LIB_TEMP "build/libgame_temp.so"

#define IMAGES_DIR "assets/images"
#define AUDIO_DIR "assets/audio"
#define MUSIC_DIR "assets/music"
#define SCREENSHOTS_DIR "screenshots"

#define MAX_AUDIOS 500
#define MAX_MUSIC 100
#define MAX_SOCKETS 25

static struct
{
  PlatformAPI api;
  MemoryBlock platform_memory;
  void* api_handle;
  time_t api_last_file_time;
  PlatformInitFn *PlatformInit;
  PlatformGameLoopFn *PlatformGameLoop;
  GameCode game_code;
  MemoryBlock game_memory;
  void* game_handle;
  time_t game_last_file_time;  
} state;

void Quit()
{
  exit(0);
}

void Die(const char *fmt, ...)
{
    char buffer[1024];

    va_list va;

    va_start(va, fmt);
    vsprintf(buffer, fmt, va);
    va_end(va);
    Quit();
}

time_t GetFileWriteTime(const char *file)
{
    struct stat buf;
    if(stat(file, &buf) == 0) {
        return buf.st_mtime;
    }
    return 0;
}

PlatformAPI LoadPlatformAPI(const char *path)
{
    PlatformAPI result = {};
    state.api_last_file_time = GetFileWriteTime(path);
    char *error;
    state.api_handle = dlopen(path, RTLD_LAZY);
    dlerror();    /* Clear any existing error */
    if(state.api_handle) {
      state.PlatformInit = (PlatformInitFn *)dlsym(state.api_handle, "PlatformInit");
      if ((error = dlerror()) != NULL)  {
        fprintf(stderr, "%s\n", error);
        exit(EXIT_FAILURE);
      }
      state.PlatformGameLoop = (PlatformGameLoopFn *)dlsym(state.api_handle, "PlatformGameLoop");
      if ((error = dlerror()) != NULL)  {
        fprintf(stderr, "%s\n", error);
        exit(EXIT_FAILURE);
      }
      result.PlatformDrawBox = (PlatformDrawBoxFn *)dlsym(state.api_handle, "PlatformDrawBox");
      if ((error = dlerror()) != NULL)  {
        fprintf(stderr, "%s\n", error);
        exit(EXIT_FAILURE);
      }
      result.PlatformDrawTexture = (PlatformDrawTextureFn *)dlsym(state.api_handle, "PlatformDrawTexture");
      if ((error = dlerror()) != NULL)  {
        fprintf(stderr, "%s\n", error);
        exit(EXIT_FAILURE);
      }
      result.PlatformEnsureImage = (PlatformEnsureImageFn *)dlsym(state.api_handle, "PlatformEnsureImage");
      if ((error = dlerror()) != NULL)  {
        fprintf(stderr, "%s\n", error);
        exit(EXIT_FAILURE);
      }
      result.PlatformScreenshot = (PlatformScreenshotFn *)dlsym(state.api_handle, "PlatformScreenshot");
      if ((error = dlerror()) != NULL)  {
        fprintf(stderr, "%s\n", error);
        exit(EXIT_FAILURE);
      }
      result.PlatformQuit = (PlatformQuitFn *)dlsym(state.api_handle, "PlatformQuit");
      if ((error = dlerror()) != NULL)  {
        fprintf(stderr, "%s\n", error);
        exit(EXIT_FAILURE);
      }
      result.PlatformCreateWindow = (PlatformCreateWindowFn *)dlsym(state.api_handle, "PlatformCreateWindow");
      if ((error = dlerror()) != NULL)  {
        fprintf(stderr, "%s\n", error);
        exit(EXIT_FAILURE);
      }
      result.PlatformEnsureAudio = (PlatformEnsureAudioFn *)dlsym(state.api_handle, "PlatformEnsureAudio");
      if ((error = dlerror()) != NULL)  {
        fprintf(stderr, "%s\n", error);
        exit(EXIT_FAILURE);
      }
      result.PlatformPlayAudio = (PlatformPlayAudioFn *)dlsym(state.api_handle, "PlatformPlayAudio");
      if ((error = dlerror()) != NULL)  {
        fprintf(stderr, "%s\n", error);
        exit(EXIT_FAILURE);
      }
      result.PlatformStopAudio = (PlatformStopAudioFn *)dlsym(state.api_handle, "PlatformStopAudio");
      if ((error = dlerror()) != NULL)  {
        fprintf(stderr, "%s\n", error);
        exit(EXIT_FAILURE);
      }
      result.PlatformEnsureMusic = (PlatformEnsureMusicFn *)dlsym(state.api_handle, "PlatformEnsureMusic");
      if ((error = dlerror()) != NULL)  {
        fprintf(stderr, "%s\n", error);
        exit(EXIT_FAILURE);
      }
      result.PlatformPlayMusic = (PlatformPlayMusicFn *)dlsym(state.api_handle, "PlatformPlayMusic");
      if ((error = dlerror()) != NULL)  {
        fprintf(stderr, "%s\n", error);
        exit(EXIT_FAILURE);
      }
      result.PlatformSetPositionMusic = (PlatformSetPositionMusicFn *)dlsym(state.api_handle, "PlatformSetPositionMusic");
      if ((error = dlerror()) != NULL)  {
        fprintf(stderr, "%s\n", error);
        exit(EXIT_FAILURE);
      }
      result.PlatformRewindMusic = (PlatformRewindMusicFn *)dlsym(state.api_handle, "PlatformRewindMusic");
      if ((error = dlerror()) != NULL)  {
        fprintf(stderr, "%s\n", error);
        exit(EXIT_FAILURE);
      }
      result.PlatformPauseMusic = (PlatformPauseMusicFn *)dlsym(state.api_handle, "PlatformPauseMusic");
      if ((error = dlerror()) != NULL)  {
        fprintf(stderr, "%s\n", error);
        exit(EXIT_FAILURE);
      }
      result.PlatformStopMusic = (PlatformStopMusicFn *)dlsym(state.api_handle, "PlatformStopMusic");
      if ((error = dlerror()) != NULL)  {
        fprintf(stderr, "%s\n", error);
        exit(EXIT_FAILURE);
      }
      result.PlatformListenAndServe = (PlatformListenAndServeFn *)dlsym(state.api_handle, "PlatformListenAndServe");
      if ((error = dlerror()) != NULL)  {
        fprintf(stderr, "%s\n", error);
        exit(EXIT_FAILURE);
      }
      result.PlatformConnectToServer = (PlatformConnectToServerFn *)dlsym(state.api_handle, "PlatformConnectToServer");
      if ((error = dlerror()) != NULL)  {
        fprintf(stderr, "%s\n", error);
        exit(EXIT_FAILURE);
      }
      result.PlatformNetSend = (PlatformNetSendFn *)dlsym(state.api_handle, "PlatformNetSend");
      if ((error = dlerror()) != NULL)  {
        fprintf(stderr, "%s\n", error);
        exit(EXIT_FAILURE);
      }
      result.PlatformNetRecv = (PlatformNetRecvFn *)dlsym(state.api_handle, "PlatformNetRecv");
      if ((error = dlerror()) != NULL)  {
        fprintf(stderr, "%s\n", error);
        exit(EXIT_FAILURE);
      }
      result.PlatformCloseConnection = (PlatformCloseConnectionFn *)dlsym(state.api_handle, "PlatformCloseConnection");
      if ((error = dlerror()) != NULL)  {
        fprintf(stderr, "%s\n", error);
        exit(EXIT_FAILURE);
      }
    }
    return result;
}

GameCode LoadGameCode(const char *path)
{
    GameCode result = {};
    state.game_last_file_time = GetFileWriteTime(path);
    char *error;

    state.game_handle = dlopen(path, RTLD_LAZY);
    dlerror();    /* Clear any existing error */
    
    if(state.game_handle) {
	result.game_init = (GameInitFn *)dlsym(state.game_handle, "GameInit");
	if ((error = dlerror()) != NULL)  {
	  fprintf(stderr, "%s\n", error);
	  exit(EXIT_FAILURE);
	}
	result.game_update = (GameUpdateFn *)dlsym(state.game_handle, "GameUpdate");
	if ((error = dlerror()) != NULL)  {
	    fprintf(stderr, "%s\n", error);
	    exit(EXIT_FAILURE);
	}
	result.game_render = (GameRenderFn *)dlsym(state.game_handle, "GameRender");
	if ((error = dlerror()) != NULL)  {
	    fprintf(stderr, "%s\n", error);
	    exit(EXIT_FAILURE);
	}
	result.game_window_shown = (GameWindowShownFn *)dlsym(state.game_handle, "GameWindowShown");
	if ((error = dlerror()) != NULL) {
	  result.game_window_shown = NULL;
	}
	result.game_window_moved = (GameWindowMovedFn *)dlsym(state.game_handle, "GameWindowMoved");
	if ((error = dlerror()) != NULL) {
	  result.game_window_shown = NULL;
	}
	result.game_window_resized = (GameWindowResizedFn *)dlsym(state.game_handle, "GameWindowResized");
	if ((error = dlerror()) != NULL) {
	  result.game_window_resized = NULL;
	}
	result.game_window_minmaxed = (GameWindowMinMaxedFn *)dlsym(state.game_handle, "GameWindowMinMaxed");
	if ((error = dlerror()) != NULL) {
	  result.game_window_minmaxed = NULL;
	}
	result.game_window_moused = (GameWindowMousedFn *)dlsym(state.game_handle, "GameWindowMoused");
	if ((error = dlerror()) != NULL) {
	  result.game_window_moused = NULL;
	}
	result.game_window_focused = (GameWindowFocusedFn *)dlsym(state.game_handle, "GameWindowFocused");
	if ((error = dlerror()) != NULL) {
	  result.game_window_focused = NULL;
	}
	result.game_window_closed = (GameWindowClosedFn *)dlsym(state.game_handle, "GameWindowClosed");
	if ((error = dlerror()) != NULL) {
	  result.game_window_closed = NULL;
	}
	result.game_quit = (GameQuitFn *)dlsym(state.game_handle, "GameQuit");
	if ((error = dlerror()) != NULL) {
	  result.game_quit = NULL;
	}
	result.game_keyboard_input = (GameKeyboardInputFn *)dlsym(state.game_handle, "GameKeyboardInput");
	if ((error = dlerror()) != NULL) {
	  result.game_keyboard_input = NULL;
	}
	result.game_mouse_button = (GameMouseButtonFn *)dlsym(state.game_handle, "GameMouseButton");
	if ((error = dlerror()) != NULL) {
	  result.game_mouse_button = NULL;
	}
	result.game_mouse_motion = (GameMouseMotionFn *)dlsym(state.game_handle, "GameMouseMotion");
	if ((error = dlerror()) != NULL) {
	  result.game_mouse_motion = NULL;
	}
	result.game_mouse_wheel = (GameMouseWheelFn *)dlsym(state.game_handle, "GameMouseWheel");
	if ((error = dlerror()) != NULL) {
	  result.game_mouse_wheel = NULL;
	}
	result.game_joy_device_event = (GameJoyDeviceEventFn *)dlsym(state.game_handle, "GameJoyDeviceEvent");
	if ((error = dlerror()) != NULL) {
	  result.game_joy_device_event = NULL;
	}
	result.game_joy_button_event = (GameJoyButtonEventFn *)dlsym(state.game_handle, "GameJoyButtonEvent");
	if ((error = dlerror()) != NULL) {
	  result.game_joy_button_event = NULL;
	}
	result.game_joy_axis_event = (GameJoyAxisEventFn *)dlsym(state.game_handle, "GameJoyAxisEvent");
	if ((error = dlerror()) != NULL) {
	  result.game_joy_axis_event = NULL;
	}
	result.game_joy_hat_event = (GameJoyHatEventFn *)dlsym(state.game_handle, "GameJoyHatEvent");
	if ((error = dlerror()) != NULL) {
	  result.game_joy_hat_event = NULL;
	}
	result.game_joy_ball_event = (GameJoyBallEventFn *)dlsym(state.game_handle, "GameJoyBallEvent");
	if ((error = dlerror()) != NULL) {
	  result.game_joy_ball_event = NULL;
	}
	result.game_controller_device_event = (GameControllerEventFn *)dlsym(state.game_handle, "GameControllerEvent");
	if ((error = dlerror()) != NULL) {
	  result.game_controller_device_event = NULL;
	}
	result.game_controller_button_event = (GameControllerButtonEventFn *)dlsym(state.game_handle, "GameControllerButtonEvent");
	if ((error = dlerror()) != NULL) {
	  result.game_controller_button_event = NULL;
	}
	result.game_controller_axis_event = (GameControllerAxisEventFn *)dlsym(state.game_handle, "GameControllerAxisEvent");
	if ((error = dlerror()) != NULL) {
	  result.game_controller_axis_event = NULL;
	}
	result.game_controller_touchpad_event = (GameControllerTouchpadEventFn *)dlsym(state.game_handle, "GameControllerTouchpadEvent");
	if ((error = dlerror()) != NULL) {
	  result.game_controller_touchpad_event = NULL;
	}
	result.game_controller_sensor_event = (GameControllerSensorEventFn *)dlsym(state.game_handle, "GameControllerSensorEvent");
	if ((error = dlerror()) != NULL) {
	  result.game_controller_sensor_event = NULL;
	}
	result.game_audio_device_event = (GameAudioDeviceEventFn *)dlsym(state.game_handle, "GameAudioDeviceEvent");
	if ((error = dlerror()) != NULL) {
	  result.game_audio_device_event = NULL;
	}
	result.game_channel_halted = (GameChannelHaltedFn *)dlsym(state.game_handle, "GameChannelHalted");
	if ((error = dlerror()) != NULL) {
	  result.game_channel_halted = NULL;
	}
	result.game_music_halted = (GameMusicHaltedFn *)dlsym(state.game_handle, "GameMusicHalted");
	if ((error = dlerror()) != NULL) {
	  result.game_music_halted = NULL;
	}
	result.game_touch_finger_event = (GameTouchFingerEventFn *)dlsym(state.game_handle, "GameTouchFingerEvent");
	if ((error = dlerror()) != NULL) {
	  result.game_touch_finger_event = NULL;
	}
	result.game_drop_event = (GameDropEventFn *)dlsym(state.game_handle, "GameDropEvent");
	if ((error = dlerror()) != NULL) {
	  result.game_drop_event = NULL;
	}
	result.game_sensor_event = (GameSensorEventFn *)dlsym(state.game_handle, "GameSensorEvent");
	if ((error = dlerror()) != NULL) {
	  result.game_sensor_event = NULL;
	}
	result.game_user_event = (GameUserEventFn *)dlsym(state.game_handle, "GameUserEvent");
	if ((error = dlerror()) != NULL) {
	  result.game_user_event = NULL;
	}
    }	
    return result;
}

void UnloadPlatformCode(PlatformAPI *api)
{
  dlclose(state.api_handle);
  api->PlatformDrawBox = 0;
  api->PlatformDrawTexture = 0;
  api->PlatformEnsureImage = 0;
  api->PlatformScreenshot = 0;
  api->PlatformQuit = 0;
  api->PlatformCreateWindow = 0;
  api->PlatformEnsureAudio = 0;
  api->PlatformPlayAudio = 0;
  api->PlatformStopAudio = 0;
  api->PlatformEnsureMusic = 0;
  api->PlatformPlayMusic = 0;
  api->PlatformSetPositionMusic = 0;
  api->PlatformRewindMusic = 0;
  api->PlatformPauseMusic = 0;
  api->PlatformStopMusic = 0;
  api->PlatformListenAndServe = 0;
  api->PlatformConnectToServer = 0;
  api->PlatformNetSend = 0;
  api->PlatformNetRecv = 0;
  api->PlatformCloseConnection = 0;
}

void UnloadGameCode(GameCode *game_code)
{
  dlclose(state.game_handle);
  game_code->handle = 0;
  game_code->game_init = 0;
  game_code->game_update = 0;
  game_code->game_render = 0;
  game_code->game_quit = 0;
  game_code->game_low_memory = 0;  
  game_code->game_window_shown = 0;
  game_code->game_window_moved = 0;       
  game_code->game_window_resized = 0;  
  game_code->game_window_minmaxed = 0;
  game_code->game_window_moused = 0;
  game_code->game_window_focused = 0;
  game_code->game_window_closed = 0;	   
  game_code->game_keyboard_input = 0;
  game_code->game_mouse_motion = 0;
  game_code->game_mouse_button = 0;
  game_code->game_mouse_wheel = 0;
  game_code->game_joy_device_event = 0;
  game_code->game_joy_button_event = 0;
  game_code->game_joy_hat_event = 0;
  game_code->game_joy_axis_event = 0;
  game_code->game_joy_ball_event = 0;
  game_code->game_controller_device_event = 0;
  game_code->game_controller_button_event = 0;
  game_code->game_controller_axis_event = 0;
  game_code->game_controller_sensor_event = 0;
  game_code->game_controller_touchpad_event = 0;
  game_code->game_audio_device_event = 0;
  game_code->game_channel_halted = 0;
  game_code->game_music_halted = 0;
  game_code->game_touch_finger_event = 0;
  game_code->game_drop_event = 0;
  game_code->game_sensor_event = 0;
  game_code->game_user_event = 0;
}

PlatformAPI GetPlatformAPI() { return state.api; }

GameCode GetGameAPI() { return state.game_code; }

MemoryBlock AllocateGameMemory()
{
    MemoryBlock result = {};

    result.ptr = (uint8_t *)calloc(1, 120000000);
    result.size = 120000000;
    result.cursor = result.ptr;

    return result;
}

MemoryBlock AllocatePlatformMemory()
{
    MemoryBlock result = {};

    result.ptr = (uint8_t *)calloc(1, 10000000);
    result.size = 10000000;
    result.cursor = result.ptr;

    return result;
}

void GameLoop()
{
  for(;;) {

    state.PlatformGameLoop();

    time_t new_dll_file_time = GetFileWriteTime(PLATFORM_LIB);
    if(new_dll_file_time > state.api_last_file_time) {
      UnloadPlatformCode(&state.api);
      usleep(200);
      state.api = LoadPlatformAPI(GAME_LIB);
      state.PlatformInit(state.game_memory, GetGameAPI(), SCREEN_WIDTH, SCREEN_HEIGHT);
    }
    // RELOAD
    new_dll_file_time = GetFileWriteTime(GAME_LIB);
    if(new_dll_file_time > state.game_last_file_time) {
      UnloadGameCode(&state.game_code);
      usleep(200);
      state.game_code = LoadGameCode(GAME_LIB);
      state.game_code.game_init(state.game_memory, GetPlatformAPI(), SCREEN_WIDTH, SCREEN_HEIGHT);
    }
    
    usleep(1);
  }
}


int main(int argc, char *argv[])
{
  memset(&state, 0, sizeof(state));
  for (int c = 0; c < argc; c++) {
    printf("%c\n", *argv[c]);
  }
  // game state init
  state.game_memory = AllocateGameMemory();
  state.platform_memory = AllocatePlatformMemory();
  state.game_code = LoadGameCode(GAME_LIB);
  state.api = LoadPlatformAPI(PLATFORM_LIB);
  // TODO: the SCREEN_WIDTH/HEIGHT should be loaded from some other config, later.
  state.PlatformInit(state.platform_memory, GetGameAPI(), SCREEN_WIDTH, SCREEN_HEIGHT);
  state.game_code.game_init(state.game_memory, GetPlatformAPI(), SCREEN_WIDTH, SCREEN_HEIGHT);
  GameLoop();
  return 0;
}
