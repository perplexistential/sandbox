#include <dlfcn.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>
#include <time.h>
#include <stdarg.h>

#include <GL/glew.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_joystick.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_net.h>

#include "shared.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define MAX_WINDOWS 2

// This should be 3000, probably...
#define MAX_SHADERS 100
#define MAX_SHADER_PROGRAMS 50

#define BUILD_DIR "build"
#define GAME_LIB "build/libgame.so"
#define GAME_LIB_TEMP "build/libgame_temp.so"

#define IMAGES_DIR "assets/images"
#define AUDIO_DIR "assets/audio"
#define MUSIC_DIR "assets/music"
#define SHADERS_DIR "assets/shaders"
#define SCREENSHOTS_DIR "screenshots"

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

typedef struct
{
  GLuint textureID;
} Texture;

#define MAX_AUDIOS 500
#define MAX_MUSIC 100

struct ID {
  GLuint ID;
};

typedef struct
{
  Mix_Chunk *chunk;
  int channel;  // This is "int" because SDL_mixer uses it
} Audio;

typedef struct
{
  Mix_Music *music;
  int track;
} Music;

#define MAX_SOCKETS 25

typedef struct {
  union {
    TCPsocket tcp;
    UDPsocket udp;
  } socket;
  IPaddress address;
  uint8_t socket_type;
  int channel;
} Connection;


static struct
{
  // Game
  GameCode game_code;
  GameMemory game_memory;
  // App
  int screen_x, screen_y, screen_w, screen_h;
  SDL_Window *windows[MAX_WINDOWS];
  int8_t window_count;
  SDL_GLContext gl_context[MAX_WINDOWS];
  // Video
  GLuint vao;
  GLuint vbo;
  GLuint vbo_color;
  Texture textures[MAX_SURFACES];
  // Audio
  Audio audio[MAX_AUDIOS];
  int channel_count;
  // Music
  Music music[MAX_MUSIC];
  int track_count;
  // Net
  Connection sockets[MAX_SOCKETS];
  int socket_count;

  int program_set;
} state;


void Quit()
{
    SDL_Quit();
    exit(0);
}

void Die(const char *fmt, ...)
{
    char buffer[1024];

    va_list va;

    va_start(va, fmt);
    vsprintf(buffer, fmt, va);
    va_end(va);

    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
			     "game just died",
			     buffer, state.windows[0]);
    
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

GameCode LoadGameCode(const char *path)
{
    GameCode result = {};
    result.last_file_time = GetFileWriteTime(path);
    char *error;

    result.handle = dlopen(path, RTLD_LAZY);
    dlerror();    /* Clear any existing error */
    
    if(result.handle) {
	result.game_init = (GameInitFn *)dlsym(result.handle, "GameInit");
	if ((error = dlerror()) != NULL)  {
	  fprintf(stderr, "%s\n", error);
	  exit(EXIT_FAILURE);
	}
	result.game_update = (GameUpdateFn *)dlsym(result.handle, "GameUpdate");
	if ((error = dlerror()) != NULL)  {
	    fprintf(stderr, "%s\n", error);
	    exit(EXIT_FAILURE);
	}
	result.game_render = (GameRenderFn *)dlsym(result.handle, "GameRender");
	if ((error = dlerror()) != NULL)  {
	    fprintf(stderr, "%s\n", error);
	    exit(EXIT_FAILURE);
	}
	
	result.game_window_shown = (GameWindowShownFn *)dlsym(result.handle, "GameWindowShown");
	if ((error = dlerror()) != NULL) {
	  result.game_window_shown = NULL;
	}
	
	result.game_window_moved = (GameWindowMovedFn *)dlsym(result.handle, "GameWindowMoved");
	if ((error = dlerror()) != NULL) {
	  result.game_window_shown = NULL;
	}
	
	result.game_window_resized = (GameWindowResizedFn *)dlsym(result.handle, "GameWindowResized");
	if ((error = dlerror()) != NULL) {
	  result.game_window_resized = NULL;
	}
	
	result.game_window_minmaxed = (GameWindowMinMaxedFn *)dlsym(result.handle, "GameWindowMinMaxed");
	if ((error = dlerror()) != NULL) {
	  result.game_window_minmaxed = NULL;
	}

	result.game_window_moused = (GameWindowMousedFn *)dlsym(result.handle, "GameWindowMoused");
	if ((error = dlerror()) != NULL) {
	  result.game_window_moused = NULL;
	}
	
	result.game_window_focused = (GameWindowFocusedFn *)dlsym(result.handle, "GameWindowFocused");
	if ((error = dlerror()) != NULL) {
	  result.game_window_focused = NULL;
	}
	
	result.game_window_closed = (GameWindowClosedFn *)dlsym(result.handle, "GameWindowClosed");
	if ((error = dlerror()) != NULL) {
	  result.game_window_closed = NULL;
	}
	
	result.game_quit = (GameQuitFn *)dlsym(result.handle, "GameQuit");
	if ((error = dlerror()) != NULL) {
	  result.game_quit = NULL;
	}
	
	result.game_keyboard_input = (GameKeyboardInputFn *)dlsym(result.handle, "GameKeyboardInput");
	if ((error = dlerror()) != NULL) {
	  result.game_keyboard_input = NULL;
	}
	
	result.game_mouse_button = (GameMouseButtonFn *)dlsym(result.handle, "GameMouseButton");
	if ((error = dlerror()) != NULL) {
	  result.game_mouse_button = NULL;
	}
	result.game_mouse_motion = (GameMouseMotionFn *)dlsym(result.handle, "GameMouseMotion");
	if ((error = dlerror()) != NULL) {
	  result.game_mouse_motion = NULL;
	}
	result.game_mouse_wheel = (GameMouseWheelFn *)dlsym(result.handle, "GameMouseWheel");
	if ((error = dlerror()) != NULL) {
	  result.game_mouse_wheel = NULL;
	}
	
	result.game_joy_device_event = (GameJoyDeviceEventFn *)dlsym(result.handle, "GameJoyDeviceEvent");
	if ((error = dlerror()) != NULL) {
	  result.game_joy_device_event = NULL;
	}
	result.game_joy_button_event = (GameJoyButtonEventFn *)dlsym(result.handle, "GameJoyButtonEvent");
	if ((error = dlerror()) != NULL) {
	  result.game_joy_button_event = NULL;
	}
	result.game_joy_axis_event = (GameJoyAxisEventFn *)dlsym(result.handle, "GameJoyAxisEvent");
	if ((error = dlerror()) != NULL) {
	  result.game_joy_axis_event = NULL;
	}
	result.game_joy_hat_event = (GameJoyHatEventFn *)dlsym(result.handle, "GameJoyHatEvent");
	if ((error = dlerror()) != NULL) {
	  result.game_joy_hat_event = NULL;
	}
	result.game_joy_ball_event = (GameJoyBallEventFn *)dlsym(result.handle, "GameJoyBallEvent");
	if ((error = dlerror()) != NULL) {
	  result.game_joy_ball_event = NULL;
	}

	result.game_controller_device_event = (GameControllerEventFn *)dlsym(result.handle, "GameControllerEvent");
	if ((error = dlerror()) != NULL) {
	  result.game_controller_device_event = NULL;
	}
	result.game_controller_button_event = (GameControllerButtonEventFn *)dlsym(result.handle, "GameControllerButtonEvent");
	if ((error = dlerror()) != NULL) {
	  result.game_controller_button_event = NULL;
	}
	result.game_controller_axis_event = (GameControllerAxisEventFn *)dlsym(result.handle, "GameControllerAxisEvent");
	if ((error = dlerror()) != NULL) {
	  result.game_controller_axis_event = NULL;
	}
	result.game_controller_touchpad_event = (GameControllerTouchpadEventFn *)dlsym(result.handle, "GameControllerTouchpadEvent");
	if ((error = dlerror()) != NULL) {
	  result.game_controller_touchpad_event = NULL;
	}
	result.game_controller_sensor_event = (GameControllerSensorEventFn *)dlsym(result.handle, "GameControllerSensorEvent");
	if ((error = dlerror()) != NULL) {
	  result.game_controller_sensor_event = NULL;
	}

	result.game_audio_device_event = (GameAudioDeviceEventFn *)dlsym(result.handle, "GameAudioDeviceEvent");
	if ((error = dlerror()) != NULL) {
	  result.game_audio_device_event = NULL;
	}
 
	result.game_channel_halted = (GameChannelHaltedFn *)dlsym(result.handle, "GameChannelHalted");
	if ((error = dlerror()) != NULL) {
	  result.game_channel_halted = NULL;
	}
	
	result.game_music_halted = (GameMusicHaltedFn *)dlsym(result.handle, "GameMusicHalted");
	if ((error = dlerror()) != NULL) {
	  result.game_music_halted = NULL;
	}

	result.game_touch_finger_event = (GameTouchFingerEventFn *)dlsym(result.handle, "GameTouchFingerEvent");
	if ((error = dlerror()) != NULL) {
	  result.game_touch_finger_event = NULL;
	}

	result.game_drop_event = (GameDropEventFn *)dlsym(result.handle, "GameDropEvent");
	if ((error = dlerror()) != NULL) {
	  result.game_drop_event = NULL;
	}

	result.game_sensor_event = (GameSensorEventFn *)dlsym(result.handle, "GameSensorEvent");
	if ((error = dlerror()) != NULL) {
	  result.game_sensor_event = NULL;
	}

	result.game_user_event = (GameUserEventFn *)dlsym(result.handle, "GameUserEvent");
	if ((error = dlerror()) != NULL) {
	  result.game_user_event = NULL;
	}
	
    }	
    return result;
}

void UnloadGameCode(GameCode *game_code)
{
  dlclose(game_code->handle);
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

#define FILE_OK 0
#define FILE_NOT_EXIST 1
#define FILE_TOO_LARGE 2
#define FILE_READ_ERROR 3

PLATFORM_SET_PROJECTION(SetProjection)
{
  
}

char * c_read_file(const char * f_name, int * err, size_t * f_size) {
    char * buffer;
    size_t length;
    FILE * f = fopen(f_name, "rb");
    size_t read_length;
    if (f) {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);
        // 1 GiB; best not to load a whole large file in one string
        /*if (length > 1073741824) {
            *err = FILE_TOO_LARGE;
            
            return NULL;
        }*/
        buffer = (char *)malloc(length + 1);
        if (length) {
            read_length = fread(buffer, 1, length, f);
            if (length != read_length) {
                 free(buffer);
                 *err = FILE_READ_ERROR;
                 return NULL;
            }
        }
        
        fclose(f);
        *err = FILE_OK;
        buffer[length] = '\0';
        *f_size = length;
    }
    else {
        *err = FILE_NOT_EXIST;
        return NULL;
    }
    return buffer;
}

const char *get_filename_ext(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}

PLATFORM_CREATE_SHADER_PROGRAM(CreateShaderProgram)
{
  ShaderProgram program;
  program.id = glCreateProgram();
  va_list vl;
  Shader shader;
  va_start(vl, shader_count);
  for (int i=0; i<shader_count; i++) {
    shader = va_arg(vl, Shader);
    glAttachShader(program.id, shader.id);
  }
  va_end(vl);
  glLinkProgram(program.id);
  glUseProgram(program.id);
  return program;
}

PLATFORM_LOAD_SHADER(LoadShader)
{
  int path_length;
  size_t path_size;
  char *path;
  char *shader_source;
  const char *_shader_source;
  int err;
  size_t f_size;
  const char *ext;
  Shader shader;
  GLint status;
  char shader_error[512];
  
  path_length = strlen(SHADERS_DIR)+strlen(filename) + 1; // +1 for the "/"
  path_size = path_length * sizeof(char)+ 1; // +1 for null terminator
  path = (char *) malloc(path_size);
  snprintf(path, path_size, "%s/%s", SHADERS_DIR, filename);
  shader_source = c_read_file(path, &err, &f_size);
  ext = get_filename_ext(filename);
  if (err) {
    Die("file not found: %s\n", path);
  }
  if (0 == strncmp(ext, "vert", 4)) {
    shader.id = glCreateShader(GL_VERTEX_SHADER);
  } else if (0 == strncmp(ext, "frag", 4)) {
    shader.id = glCreateShader(GL_FRAGMENT_SHADER);
  } else if (0 == strncmp(ext, "geom", 4)) {
    shader.id = glCreateShader(GL_GEOMETRY_SHADER);
  } else if (0 == strncmp(ext, "tesc", 4)) {
    shader.id = glCreateShader(GL_TESS_CONTROL_SHADER);
  } else if (0 == strncmp(ext, "tese", 4)) {
    shader.id = glCreateShader(GL_TESS_EVALUATION_SHADER);
  } else if (0 == strncmp(ext, "comp", 4)) {
    shader.id = glCreateShader(GL_COMPUTE_SHADER);
  } else {
    Die("unknown shader extension: %s; must be (frag, geom, tesc, tese, or comp)\n", ext);
  }
  _shader_source = shader_source;
  glShaderSource(shader.id, 1, &_shader_source, NULL);
  glCompileShader(shader.id);
  glGetShaderiv(shader.id, GL_COMPILE_STATUS, &status);
  if (GL_TRUE != status) {
    glGetShaderInfoLog(shader.id, 512, NULL, shader_error);
    Die("shader %s failed: \n%s\n", filename, shader_error);
  }
  free(shader_source);
  free(path);
  return shader;
}

PLATFORM_ATTACH_SHADER(AttachShader) {
  glAttachShader(program.id, shader.id);
  glLinkProgram(program.id);
}

PLATFORM_DETACH_SHADER(DetachShader) { glDetachShader(program.id, shader.id); }

PLATFORM_DELETE_SHADER(DeleteShader) { glDeleteShader(shader.id); }

PLATFORM_DRAW_BOX(DrawBox)
{
  /*
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
  glBegin(GL_QUADS);
    glColor4f(r, g, b, a);
    glVertex2f(x, y);
    glVertex2f(x+width, y);
    glVertex2f(x+width, y+height);
    glVertex2f(x, y+height);
  glEnd();
  glDisable(GL_BLEND);
  */

  float half_width = SCREEN_WIDTH * 0.5f;
  float half_height = SCREEN_HEIGHT * 0.5f;
  x = x - half_width;
  y = y - half_height;
  float x0 = x/half_width;
  float y0 = y/half_height;
  float x1 = (x+width)/half_width;
  float y1 = (y+height)/half_height;
  float vertices[] = {
    x0, y1, r, g, b, a,
    x1, y1, r, g, b, a,
    x1, y0, r, g, b, a,
    x0, y0, r, g, b, a,
  };

  //glBindVertexArray(state.vao);
  glBindBuffer(GL_ARRAY_BUFFER, state.vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
  glUseProgram(program.id);
  GLint posAttrib = glGetAttribLocation(program.id, "position");
  glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(posAttrib);

  GLint inColor = glGetAttribLocation(program.id, "color");
  glVertexAttribPointer(inColor, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(2*sizeof(float)));
  glEnableVertexAttribArray(inColor);
  
  glDrawArrays(GL_QUADS, 0, 4);
}

void opengl_load_texture(int textureIndex, const char *imagePath)
{
  SDL_Surface* newSurface = IMG_Load(imagePath);
  if (!newSurface) {
    Die("Failed to load the image: %s", IMG_GetError());
  }
  int mode = GL_RGB;
  if (newSurface->format->BytesPerPixel == 4) {
    mode = GL_RGBA;
  }
  glGenTextures(1, &state.textures[textureIndex].textureID);
  glBindTexture(GL_TEXTURE_2D, state.textures[textureIndex].textureID);
  glTexImage2D(GL_TEXTURE_2D,
	       0, mode,
	       newSurface->w, newSurface->h,
	       0, mode,
	       GL_UNSIGNED_BYTE, newSurface->pixels);
  
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

PLATFORM_ENSURE_IMAGE(EnsureImage)
{
  int imagePathLength = strlen(IMAGES_DIR)+strlen(filename) + 1; // +1 for the "/"
  size_t imagePathSize = imagePathLength * sizeof(char)+ 1; // +1 for null terminator
  char *imagePath = (char *) malloc(imagePathSize);
  snprintf(imagePath, imagePathSize, "%s/%s", IMAGES_DIR, filename);
  //opengl_load_texture(textureID, imagePath);
  free(imagePath);
}

PLATFORM_DRAW_TEXTURE(DrawTexture)
{
  /*
  float texW, texH;
  glGetTexLevelParameterfv( GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &texW);
  glGetTexLevelParameterfv( GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &texH);

  float texCoordX0 = 0;
  float texCoordX1 = sprite_w;
  float texCoordY0 = 0;
  float texCoordY1 = sprite_h;
  if (sprite_x != 0 && sprite_x != texW) {
    texCoordX0 = sprite_x / texW;
    texCoordX1 = (sprite_x + sprite_w) / texW;
  }
  if (sprite_y != 0 && sprite_y != texH) {
    texCoordY0 = sprite_y / texH;
    texCoordY1 = (sprite_y + sprite_h) / texH;
  }
  
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glBindTexture(GL_TEXTURE_2D, state.textures[textureIndex].textureID);
  glBegin(GL_QUADS);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glTexCoord2f(texCoordX0, texCoordY1); glVertex3f(x, y, 0);
    glTexCoord2f(texCoordX1, texCoordY1); glVertex3f(x + width, y, 0);
    glTexCoord2f(texCoordX1, texCoordY0); glVertex3f(x + width, y + height, 0);
    glTexCoord2f(texCoordX0, texCoordY0); glVertex3f(x, y + height, 0);
  glEnd();

  glDisable(GL_BLEND);
  glDisable(GL_TEXTURE_2D);
  */
}

PLATFORM_QUIT(QuitGame) {
  SDLNet_Quit();
  Mix_CloseAudio();
  for(int i=0; i<state.window_count; i++) { 
    SDL_GL_DeleteContext(state.gl_context[i]);
  }
  Quit();
}

PLATFORM_CREATE_WINDOW(CreateWindow) {
  int index = MAX_WINDOWS;
  if (state.window_count > MAX_WINDOWS) {
    return index;
  }
  
  SDL_Window *new_win = SDL_CreateWindow(title,
					 x, y, width, height,
					 SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);  
  if(!new_win) {
    Die("Failed to create window: %s\n", SDL_GetError());
  }
  state.windows[state.window_count] = new_win;
  // using OpenGL render context
  state.gl_context[state.window_count] = SDL_GL_CreateContext(state.windows[0]); 
  glewExperimental = GL_TRUE;
  glewInit();
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  //glMatrixMode(GL_PROJECTION);
  //glLoadIdentity();
  //glOrtho(0.0f, 1.0f*width, 0.0f, 1.0f*height, 0.0f, 1.0f);
  index = state.window_count;
  state.window_count++;
  if (0 == state.vao)
    glGenVertexArrays(1, &state.vao);
  if (0 == state.vbo)
    glGenBuffers(1, &state.vbo);
  return index;
}

void sdl_load_audio(int audioIndex, const char *audioPath)
{
  Mix_Chunk *chunk = Mix_LoadWAV(audioPath);
  if (!chunk) {
    Die("failed to load audio %s: %s\n", audioPath, Mix_GetError());
  }
  if (NULL != state.audio[audioIndex].chunk) {
    Mix_FreeChunk(state.audio[audioIndex].chunk);
  } else {
    state.audio[audioIndex].channel = state.channel_count++;
  }
  state.audio[audioIndex].chunk = chunk;
}

void sdl_load_music(int musicIndex, const char *musicPath)
{
  Mix_Music *music;
  music = Mix_LoadMUS(musicPath);
  if(!music) {
    Die("failed to load music %s: %s\n", musicPath, Mix_GetError());
  }
  if (NULL != state.music[musicIndex].music) {
    Mix_FreeMusic(state.music[musicIndex].music);
  } else {
    state.music[musicIndex].track = state.track_count++;
  }
  state.music[musicIndex].music = music;
}

PLATFORM_ENSURE_AUDIO(EnsureAudio)
{
  printf("file(%s), channel(%d)\n", filename, channel);
  int audioPathLength = strlen(AUDIO_DIR)+strlen(filename) + 1;
  char *audioPath = (char *) malloc(audioPathLength * sizeof(char));
  snprintf(audioPath, audioPathLength*sizeof(char), "%s/%s", AUDIO_DIR, filename);
  sdl_load_audio(channel, audioPath);
  free(audioPath);
}

PLATFORM_PLAY_AUDIO(PlayAudio)
{
  int result = 0;
  printf("channel(%d), fade(%d), loops(%d), volume(%d), duration(%d)\n",
	 channel, fade, loops, volume, duration);
  if (volume) {
    Mix_Volume(channel, volume);
    // TODO: Use Mix_VolumeChunk?
  }
  if (fade > 0) { 
    if (duration > 0) {
      result = Mix_FadeInChannelTimed(state.audio[channel].channel,
				      state.audio[channel].chunk,
				      loops, fade, duration);
    } else {
      result = Mix_FadeInChannel(state.audio[channel].channel,
				 state.audio[channel].chunk,
				 loops, fade);
    }
  } else {
    if (duration > 0) {
      result = Mix_PlayChannelTimed(state.audio[channel].channel,
				    state.audio[channel].chunk,
				    loops, duration);
    } else {
      result = Mix_PlayChannel(state.audio[channel].channel,
			       state.audio[channel].chunk,
			       loops);
    }
  }
  if (-1 == result) {
    printf("failed to play audio: %s", Mix_GetError());
  }
}

PLATFORM_STOP_AUDIO(StopAudio)
{
  printf("channel(%d), duration(%d), fade(%s)\n", channel, duration, fade ? "true" : "false");
  if (duration) {
    if (fade) {
      Mix_FadeOutChannel(channel, duration);
    } else {
      Mix_ExpireChannel(channel, duration);
    }
  } else {
    Mix_HaltChannel(channel);
  }
}

void channelDone(int channel) {
  if (state.game_code.game_channel_halted)
    state.game_code.game_channel_halted(channel);
}

PLATFORM_ENSURE_MUSIC(EnsureMusic)
{
  printf("file(%s), channel(%d)\n", filename, track);
  int audioPathLength = strlen(MUSIC_DIR)+strlen(filename) + 1;
  char *audioPath = (char *) malloc(audioPathLength * sizeof(char));
  snprintf(audioPath, audioPathLength*sizeof(char), "%s/%s", MUSIC_DIR, filename);
  sdl_load_music(track, audioPath);
  free(audioPath);
}

void musicDone() {
  if (state.game_code.game_music_halted)
    state.game_code.game_music_halted();
}

PLATFORM_PLAY_MUSIC(PlayMusic)
{
  printf("play %d fade(%d), loops(%d), position(%f), volume(%d)\n",
	 track, fade, loops, position, volume);
  Mix_VolumeMusic(volume);
  if (fade) {
    if (position > 0) {
      Mix_FadeInMusicPos(state.music[track].music, loops, fade, position);
    } else {
      Mix_FadeInMusic(state.music[track].music, loops, fade);
    }
  } else {
    if (position > 0) {
      Mix_FadeInMusicPos(state.music[track].music, loops, 0, position);
    } else {
      Mix_PlayMusic(state.music[track].music, loops);
    }
  }
}

PLATFORM_SET_POSITION_MUSIC(SetPositionMusic)
{
  Mix_SetMusicPosition(position);
}

PLATFORM_REWIND_MUSIC(RewindMusic)
{
  Mix_RewindMusic();
}

PLATFORM_PAUSE_MUSIC(PauseMusic)
{
  Mix_PausedMusic() ? Mix_ResumeMusic() : Mix_PauseMusic();
}

PLATFORM_STOP_MUSIC(StopMusic)
{
  printf("stop fade(%d)\n", fade);
  Mix_FadeOutMusic(fade);
}

PLATFORM_SCREENSHOT(Screenshot)
{
  /*
   * team worked; made my creams work : 
   * thanks to Dave for this screenshot magic
   * flux-compose: https://github.com/FluxHarmonic/flux-compose/blob/master/lib/graphics.c
   */
  // TODO: The screenshot should target a specific Window
  int i = 0;
  unsigned char *screen_bytes = NULL;
  unsigned char *image_bytes = NULL;
  size_t image_row_length = 4 * width;
  size_t image_data_size = sizeof(*image_bytes) * image_row_length * height;
  char output_file_path[40];
  snprintf(output_file_path, sizeof(output_file_path), "%s/screenshot_%d.png",
	   SCREENSHOTS_DIR, (int)time(NULL));
  // Allocate storage for the screen bytes
  screen_bytes = (unsigned char *)malloc(image_data_size);
  image_bytes = (unsigned char *)malloc(image_data_size);
  // Store the screen contents to a byte array
  glReadPixels(x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, screen_bytes);
  // Flip the rows of the byte array because OpenGL's coordinate system is flipped
  for (i = 0; i < height; i++) {
    memcpy(&image_bytes[image_row_length * i], &screen_bytes[image_row_length * (height - (i + 1))], sizeof(*image_bytes) * image_row_length);
  }
  SDL_Surface *surface = NULL;
  surface = SDL_CreateRGBSurfaceWithFormatFrom(image_bytes,
					       state.screen_w,
					       state.screen_h,
					       32, 4*state.screen_w,
					       SDL_PIXELFORMAT_RGBA32);
  if (NULL == surface) {
    printf("unable to create surface for screenshot: %s\n", SDL_GetError());
  } else {
    if (IMG_SavePNG(surface, output_file_path) != 0) {
      printf("unable to save PNG to %s\n", output_file_path);
    }
  }
  printf("screenshot taken of window(%d): %s\n", window, output_file_path);
  SDL_FreeSurface(surface);
  free(image_bytes);
  free(screen_bytes);
}



PLATFORM_LISTEN_AND_SERVE(ListenAndServe)
{
  if (MAX_SOCKETS <= state.socket_count) {
    printf("server count cannot exceed %d\n", MAX_SOCKETS);
  }
  unsigned int index = state.socket_count;
  
  memset(&state.sockets[index].address, 0, sizeof(IPaddress));
  for (int c=0; c<5; c++){
    switch(socket_type){
    case SOCKET_TCP:
      if(SDLNet_ResolveHost(&state.sockets[index].address,NULL,port)==-1) {
	printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
      }
      state.sockets[index].socket.tcp = SDLNet_TCP_Open(&state.sockets[index].address);
      if (state.sockets[index].socket.tcp) {
	state.socket_count++;
	return index;
      }
      break;
    case SOCKET_UDP:
      state.sockets[index].socket.udp = SDLNet_UDP_Open(port);
      if (state.sockets[index].socket.udp) {
	int channel = SDLNet_UDP_Bind(state.sockets[index].socket.udp,
				      -1, &state.sockets[index].address);
	if(channel==-1) {
	  printf("SDLNet_UDP_Bind: %s\n", SDLNet_GetError());
	  return index;
	}
	state.sockets[index].channel = channel;
	state.socket_count++;
	return index;
      }
      break;
    default:
      Die("unknown socket type: %d\n", socket_type);
    }
  }
  printf("failed to create %s server socket listening on port %d\n",
	 socket_type == SOCKET_TCP ? "TCP" : "UDP", port);
  return MAX_SOCKETS;
}

PLATFORM_CONNECT_TO_SERVER(ConnectToServer)
{
  unsigned int index = state.socket_count;
  memset(&state.sockets[index].address, 0, sizeof(IPaddress));
  switch(socket_type){
  case SOCKET_TCP:
    if (host != NULL) {
      if(SDLNet_ResolveHost(&state.sockets[index].address,host,port)==-1) {
	printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
      }
    }
    state.sockets[index].socket.tcp = SDLNet_TCP_Open(&state.sockets[index].address);
    if (state.sockets[index].socket.tcp) {
      state.socket_count++;
      return index;
    }
    break;
  case SOCKET_UDP:
    state.sockets[index].socket.udp = SDLNet_UDP_Open(port);
    if (state.sockets[index].socket.udp) {
      state.socket_count++;
      return index;
    }
    break;
  }
  return MAX_SOCKETS;
}

PLATFORM_NET_SEND(NetSend)
{
  UDPpacket *packet = NULL;
  switch(state.sockets[socket].socket_type) {
  case SOCKET_TCP:
    if(SDLNet_TCP_Send(state.sockets[socket].socket.tcp, message, length) <= length) {
      printf("tcp send: %s\n", SDLNet_GetError());
    }
    break;
  case SOCKET_UDP:
    packet = SDLNet_AllocPacket(length);
    if(!packet) {
      printf("failed to allocate packet: %s\n", SDLNet_GetError());
    } else {
      packet->data = (Uint8*)message;
      if (!SDLNet_UDP_Send(state.sockets[socket].socket.udp, packet->channel, packet)){
	printf("failed to send packet: no data sent\n");
      }
    }
    SDLNet_FreePacket(packet);
    break;
  }
}

PLATFORM_NET_RECV(NetRecv)
{
  UDPpacket *packet;

  switch(state.sockets[socket].socket_type){
  case SOCKET_TCP:
    if ( 0 >= SDLNet_TCP_Recv(state.sockets[socket].socket.tcp, received, length)) {
      printf("no data received\n");
    }
    break;
  case SOCKET_UDP:  
    packet = SDLNet_AllocPacket(length);
    if(!packet) {
      printf("failed to allocate packet: %s\n", SDLNet_GetError());
    } else {
      packet->data = (Uint8*)received;
      if (!SDLNet_UDP_Recv(state.sockets[socket].socket.udp, packet)){
	printf("failed to send packet: no data sent\n");
      }
    }
    SDLNet_FreePacket(packet);
    break;
  }
}

PLATFORM_CLOSE_CONNECTION(CloseConnection)
{
  switch(state.sockets[socket].socket_type) {
  case SOCKET_TCP:
    SDLNet_TCP_Close(state.sockets[socket].socket.tcp);
    break;
  case SOCKET_UDP:
    SDLNet_UDP_Close(state.sockets[socket].socket.udp);
    break;
  };
}

PlatformAPI GetPlatformAPI()
{
    PlatformAPI api = {};
    // Draw
    api.PlatformCreateShaderProgram = CreateShaderProgram;
    api.PlatformLoadShader = LoadShader;
    api.PlatformAttachShader = AttachShader;
    api.PlatformDetachShader = DetachShader;
    api.PlatformDeleteShader = DeleteShader;
    api.PlatformDrawBox = DrawBox;
    api.PlatformDrawTexture = DrawTexture;
    api.PlatformEnsureImage = EnsureImage;
    api.PlatformScreenshot = Screenshot;
    // App
    api.PlatformQuit = QuitGame;
    api.PlatformCreateWindow = CreateWindow;
    // Audio
    api.PlatformEnsureAudio = EnsureAudio;
    api.PlatformPlayAudio = PlayAudio;
    api.PlatformStopAudio = StopAudio;
    // Music
    api.PlatformEnsureMusic = EnsureMusic;
    api.PlatformPlayMusic = PlayMusic;
    api.PlatformSetPositionMusic = SetPositionMusic;
    api.PlatformRewindMusic = RewindMusic;
    api.PlatformPauseMusic = PauseMusic;
    api.PlatformStopMusic = StopMusic;
    // Net
    api.PlatformListenAndServe = ListenAndServe;
    api.PlatformConnectToServer = ConnectToServer;
    api.PlatformNetSend = NetSend;
    api.PlatformNetRecv = NetRecv;
    api.PlatformCloseConnection = CloseConnection;
    return api;
}

GameMemory AllocateGameMemory()
{
    GameMemory result = {};

    result.ptr = (uint8_t *)calloc(1, 1200000000);
    result.size = 1200000000;
    result.cursor = result.ptr;

    return result;
}

void GameLoop()
{
  for(;;) {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
      switch (event.type) {

      // App closing
      case SDL_QUIT:
      case SDL_APP_TERMINATING:
	if (state.game_code.game_quit)
	  state.game_code.game_quit();
	Quit();
	break;

      case SDL_APP_LOWMEMORY:
	if (state.game_code.game_low_memory)
	  state.game_code.game_low_memory();
	break;

      case SDL_DISPLAYEVENT:
	switch (event.display.event) {
	case SDL_DISPLAYEVENT_CONNECTED:
	  break;
	case SDL_DISPLAYEVENT_DISCONNECTED:
	  break;
	case SDL_DISPLAYEVENT_ORIENTATION:
	  break;
	}
	break;
	
      case SDL_WINDOWEVENT:
	switch (event.window.event) {
        case SDL_WINDOWEVENT_SHOWN:
	  if (state.game_code.game_window_shown)
	    state.game_code.game_window_shown(event.window.windowID, 1);
	  break;
        case SDL_WINDOWEVENT_HIDDEN:
	  if (state.game_code.game_window_shown)
	    state.game_code.game_window_shown(event.window.windowID, 0);
	  break;
        case SDL_WINDOWEVENT_MOVED:
	  if (state.game_code.game_window_moved) {
	    state.screen_x = event.window.data1;
	    state.screen_y = event.window.data2;
	    state.game_code.game_window_moved(event.window.windowID,
					      event.window.data1,
					      event.window.data2);
	  }
	  break;
        case SDL_WINDOWEVENT_RESIZED:
	  if (state.game_code.game_window_resized){
	    state.screen_w = event.window.data1;
	    state.screen_h = event.window.data2;
	    state.game_code.game_window_resized(event.window.windowID,
						event.window.data1,
						event.window.data2);
	    glViewport(state.screen_x, state.screen_y, state.screen_w, state.screen_h);
	    glMatrixMode(GL_PROJECTION);
	    glLoadIdentity();
	    glOrtho(0.0f, 1.0f*state.screen_w, 0.0f, 1.0f*state.screen_h, 0.0f, 1.0f);
	  }
	  break;
        case SDL_WINDOWEVENT_MINIMIZED:
	  if (state.game_code.game_window_minmaxed)
	    state.game_code.game_window_minmaxed(event.window.windowID, 1);
	  break;
        case SDL_WINDOWEVENT_MAXIMIZED:
	  if (state.game_code.game_window_minmaxed)
	    state.game_code.game_window_minmaxed(event.window.windowID, 0);
	  break;
        case SDL_WINDOWEVENT_ENTER:
	  if (state.game_code.game_window_moused)
	    state.game_code.game_window_moused(event.window.windowID, 1);
	  break;
        case SDL_WINDOWEVENT_LEAVE:
	  if (state.game_code.game_window_moused)
	    state.game_code.game_window_moused(event.window.windowID, 0);
	  break;
        case SDL_WINDOWEVENT_FOCUS_GAINED:
	  if (state.game_code.game_window_focused)
	    state.game_code.game_window_focused(event.window.windowID, 1);
	  break;
        case SDL_WINDOWEVENT_FOCUS_LOST:
	  if (state.game_code.game_window_focused)
	    state.game_code.game_window_focused(event.window.windowID, 0);
	  break;
        case SDL_WINDOWEVENT_CLOSE:
	  if (state.game_code.game_window_closed)
	    state.game_code.game_window_closed(event.window.windowID);
	  break;
        default:
	  //SDL_Log("Window %d got unknown event %d",
	  //        event.window.windowID, event.window.event);
	  break;
        }
	break;

	// Keyboard
      case SDL_KEYDOWN:
	if (state.game_code.game_keyboard_input)
	  state.game_code.game_keyboard_input(event.key.windowID,
					      BUTTON_PRESSED,
					      event.key.repeat,
					      event.key.keysym.scancode);
	break;
      case SDL_KEYUP:
	if (state.game_code.game_keyboard_input)
	  state.game_code.game_keyboard_input(event.key.windowID,
					      BUTTON_RELEASED,
					      event.key.repeat,
					      event.key.keysym.scancode);
	break;
	// Unsupported Keyboard-related
      case SDL_TEXTEDITING:
      case SDL_TEXTINPUT:
	break;
      case SDL_KEYMAPCHANGED:
	break;

	// Mouse
      case SDL_MOUSEMOTION:
	if (state.game_code.game_mouse_motion)
	  state.game_code.game_mouse_motion(event.motion.windowID,
					    event.motion.which,
					    event.motion.x,
					    event.motion.y,
					    event.motion.xrel,
					    event.motion.yrel);
	break;
      case SDL_MOUSEBUTTONDOWN:
	if (state.game_code.game_mouse_button)
	  state.game_code.game_mouse_button(event.button.windowID,
					    event.button.which,
					    event.button.button,
					    BUTTON_PRESSED,
					    event.button.clicks,
					    event.button.x,
					    event.button.y);
	break;
      case SDL_MOUSEBUTTONUP:
	if (state.game_code.game_mouse_button)
	  state.game_code.game_mouse_button(event.button.windowID,
					    event.button.which,
					    event.button.button,
					    BUTTON_RELEASED,
					    event.button.clicks,
					    event.button.x,
					    event.button.y);
	break;
      case SDL_MOUSEWHEEL:
	if (state.game_code.game_mouse_wheel)
	  state.game_code.game_mouse_wheel(event.wheel.windowID,
					   event.wheel.which,
					   event.wheel.x,
					   event.wheel.y,
					   event.wheel.direction);
	break;

	// Joystick
      case SDL_JOYAXISMOTION:
	if (state.game_code.game_joy_axis_event)
	  state.game_code.game_joy_axis_event(event.jaxis.which,
					      event.jaxis.axis,
					      event.jaxis.value);
	break;
      case SDL_JOYBALLMOTION:
	if (state.game_code.game_joy_ball_event)
	  state.game_code.game_joy_ball_event(event.jball.which,
					      event.jball.ball,
					      event.jball.xrel,
					      event.jball.yrel);
	break;
      case SDL_JOYHATMOTION:
	if (state.game_code.game_joy_hat_event)
	  state.game_code.game_joy_hat_event(event.jhat.which,
					     event.jhat.hat,
					     event.jhat.value);
	break;
      case SDL_JOYBUTTONDOWN:
	if (state.game_code.game_joy_button_event)
	  state.game_code.game_joy_button_event(event.jbutton.which,
						event.jbutton.button,
						BUTTON_PRESSED);
	break;
      case SDL_JOYBUTTONUP:
	if (state.game_code.game_joy_button_event)
	  state.game_code.game_joy_button_event(event.jbutton.which,
						event.jbutton.button,
						BUTTON_RELEASED);
	break;
      case SDL_JOYDEVICEADDED:
	if (state.game_code.game_joy_device_event)
	  state.game_code.game_joy_device_event(event.jdevice.which, CONNECT);
	break;
      case SDL_JOYDEVICEREMOVED:
	if (state.game_code.game_joy_device_event)
	  state.game_code.game_joy_device_event(event.jdevice.which, DISCONNECT);
	break;

	// Controller
      case SDL_CONTROLLERAXISMOTION:
	if (state.game_code.game_controller_axis_event)
	  state.game_code.game_controller_axis_event(event.caxis.which,
						     event.caxis.axis,
						     event.caxis.value);
	break;
      case SDL_CONTROLLERBUTTONDOWN:
	if (state.game_code.game_controller_button_event)
	  state.game_code.game_controller_button_event(event.cbutton.which,
						       event.cbutton.button,
						       BUTTON_PRESSED);
	break;
      case SDL_CONTROLLERBUTTONUP:
	if (state.game_code.game_controller_button_event)
	  state.game_code.game_controller_button_event(event.cbutton.which,
						       event.cbutton.button,
						       BUTTON_RELEASED);
	break;
      case SDL_CONTROLLERDEVICEADDED:
	if (state.game_code.game_controller_device_event)
	  state.game_code.game_controller_device_event(event.cdevice.which, CONNECT);
	break;
      case SDL_CONTROLLERDEVICEREMOVED:
	if (state.game_code.game_controller_device_event)
	  state.game_code.game_controller_device_event(event.cdevice.which, DISCONNECT);
	break;
      case SDL_CONTROLLERDEVICEREMAPPED:
	break;
      case SDL_CONTROLLERTOUCHPADDOWN:
	if (state.game_code.game_controller_touchpad_event)
	  state.game_code.game_controller_touchpad_event(event.ctouchpad.which,
							 TOUCHPAD_DOWN,
							 event.ctouchpad.finger,
							 event.ctouchpad.x,
							 event.ctouchpad.y,
							 event.ctouchpad.pressure);
	break;
      case SDL_CONTROLLERTOUCHPADMOTION:
	if (state.game_code.game_controller_touchpad_event)
	  state.game_code.game_controller_touchpad_event(event.ctouchpad.which,
							 TOUCHPAD_MOTION,
							 event.ctouchpad.finger,
							 event.ctouchpad.x,
							 event.ctouchpad.y,
							 event.ctouchpad.pressure);
	break;
      case SDL_CONTROLLERTOUCHPADUP:
	if (state.game_code.game_controller_touchpad_event)
	  state.game_code.game_controller_touchpad_event(event.ctouchpad.which,
							 TOUCHPAD_UP,
							 event.ctouchpad.finger,
							 event.ctouchpad.x,
							 event.ctouchpad.y,
							 event.ctouchpad.pressure);
	break;
	break;
      case SDL_CONTROLLERSENSORUPDATE:
	if (state.game_code.game_controller_sensor_event)
	  state.game_code.game_controller_sensor_event(event.csensor.which,
						       event.csensor.sensor,
						       event.csensor.data, 6);
	break;

	// Touch
      case SDL_FINGERDOWN:
	if (state.game_code.game_touch_finger_event)
	  state.game_code.game_touch_finger_event(event.tfinger.windowID,
						  event.tfinger.touchId,
						  event.tfinger.fingerId,
						  TOUCHPAD_DOWN,
						  event.tfinger.x,
						  event.tfinger.y,
						  event.tfinger.dx,
						  event.tfinger.dy,
						  event.tfinger.pressure);
	break;
      case SDL_FINGERUP:
	if (state.game_code.game_touch_finger_event)
	  state.game_code.game_touch_finger_event(event.tfinger.windowID,
						  event.tfinger.touchId,
						  event.tfinger.fingerId,
						  TOUCHPAD_UP,
						  event.tfinger.x,
						  event.tfinger.y,
						  event.tfinger.dx,
						  event.tfinger.dy,
						  event.tfinger.pressure);
	break;
      case SDL_FINGERMOTION:
	if (state.game_code.game_touch_finger_event)
	  state.game_code.game_touch_finger_event(event.tfinger.windowID,
						  event.tfinger.touchId,
						  event.tfinger.fingerId,
						  TOUCHPAD_MOTION,
						  event.tfinger.x,
						  event.tfinger.y,
						  event.tfinger.dx,
						  event.tfinger.dy,
						  event.tfinger.pressure);
	break;
	// Unsupported touch-related
      case SDL_DOLLARGESTURE:
      case SDL_DOLLARRECORD:
      case SDL_MULTIGESTURE:
	break;

	// Drops
      case SDL_DROPFILE:
	if (state.game_code.game_drop_event)
	  state.game_code.game_drop_event(event.drop.windowID,
					  DROP_FILE,
					  event.drop.file);
	SDL_free(event.drop.file);
	break;
      case SDL_DROPTEXT:
	if (state.game_code.game_drop_event)
	  state.game_code.game_drop_event(event.drop.windowID,
					  DROP_TEXT,
					  event.drop.file);
	SDL_free(event.drop.file);
	break;
      case SDL_DROPBEGIN:
	if (state.game_code.game_drop_event)
	  state.game_code.game_drop_event(event.drop.windowID,
					  DROP_BEGIN,
					  event.drop.file);
	SDL_free(event.drop.file);
	break;
      case SDL_DROPCOMPLETE:
	if (state.game_code.game_drop_event)
	  state.game_code.game_drop_event(event.drop.windowID,
					  DROP_COMPLETE,
					  event.drop.file);
	SDL_free(event.drop.file);
	break;

	// Audio Devices
      case SDL_AUDIODEVICEADDED:
	if (state.game_code.game_audio_device_event)
	  state.game_code.game_audio_device_event(event.adevice.which,
						  CONNECT,
						  event.adevice.iscapture);
	break;
      case SDL_AUDIODEVICEREMOVED:
	if (state.game_code.game_audio_device_event)
	  state.game_code.game_audio_device_event(event.adevice.which,
						  DISCONNECT,
						  event.adevice.iscapture);
	break;

	// Sensor 
      case SDL_SENSORUPDATE:
	if (state.game_code.game_sensor_event)
	  state.game_code.game_sensor_event(event.sensor.which,
					    event.sensor.type,
					    event.sensor.data, 6);
	break;
	
	// User event
      case SDL_USEREVENT:
	if (state.game_code.game_user_event)
	  state.game_code.game_user_event(event.user.windowID,
					  event.user.type,
					  event.user.code,
					  event.user.data1,
					  event.user.data2);
	break;
	
	// Unsupported for now
      case SDL_RENDER_TARGETS_RESET:
      case SDL_RENDER_DEVICE_RESET:
      case SDL_CLIPBOARDUPDATE:
      case SDL_LOCALECHANGED:
	break;
      }
    }

    // If there are servers, 
    
    state.game_code.game_update(1.0f/60.0f);

    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    state.game_code.game_render();
    for (int w = 0; w < state.window_count; w++) {
      SDL_GL_SwapWindow(state.windows[w]);
    }
    
    // RELOAD
    time_t new_dll_file_time = GetFileWriteTime(GAME_LIB);
    if(new_dll_file_time > state.game_code.last_file_time) {
      UnloadGameCode(&state.game_code);
      SDL_Delay(200);
      state.game_code = LoadGameCode(GAME_LIB);
      state.game_code.game_init(state.game_memory, GetPlatformAPI(), SCREEN_WIDTH, SCREEN_HEIGHT);
    }
    
    SDL_Delay(1);
  }
}


int main(int argc, char *argv[])
{
  memset(&state, 0, sizeof(state));
  for (int c = 0; c < argc; c++) {
    printf("%c\n", *argv[c]);
  }
  if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    Die("failed to initialize SDL2: %s\n", SDL_GetError());
  }
  // Image loading support
  if(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_TIF | IMG_INIT_WEBP) < 0) {
    Die("failed to initialize Image support: %s\n", IMG_GetError());
  }
  // TODO: MIX_INIT_MIDI?
  if(Mix_Init(MIX_INIT_FLAC | MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_MOD | MIX_INIT_OPUS) < 0) {
    Die("failed to initialize Mixer support: %s\n", Mix_GetError());
  }
  if(SDLNet_Init() == -1) {
    Die("failed to initialize SDLNet support", SDLNet_GetError());
  }
  SDL_version compile_version;
  const SDL_version *link_version=SDLNet_Linked_Version();
  SDL_NET_VERSION(&compile_version);
  printf("Compiled with SDL_net version: %d.%d.%d\n", 
	 compile_version.major,
	 compile_version.minor,
	 compile_version.patch);
  printf("Running with SDL_net version: %d.%d.%d\n", 
	 link_version->major,
	 link_version->minor,
	 link_version->patch);
  // open 44.1KHz, signed 16bit, system byte order,
  //      stereo audio, using 1024 byte chunks
  if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024)==-1) {
    Die("Mix_OpenAudio: %s\n", Mix_GetError());
  }
  if(0 > Mix_AllocateChannels(200)){
    Die("failed to allocate audio channels: %s\n", Mix_GetError());
  }
  Mix_ChannelFinished(channelDone);
  Mix_HookMusicFinished(musicDone);
  // print the number of music decoders available
  printf("There are %d music deocoders available\n", Mix_GetNumMusicDecoders());
  state.window_count = 0;
  state.screen_w = SCREEN_WIDTH;
  state.screen_h = SCREEN_HEIGHT;
  CreateWindow("Perplexistential Sandbox", 300, 1400, SCREEN_WIDTH, SCREEN_HEIGHT);
  // get version info
  const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
  const GLubyte* version = glGetString(GL_VERSION); // version as a string
  printf("Renderer: %s\n", renderer);
  printf("OpenGL version supported %s\n", version);
  // game state init
  state.game_memory = AllocateGameMemory();
  state.game_code = LoadGameCode(GAME_LIB);
  state.game_code.game_init(state.game_memory, GetPlatformAPI(), SCREEN_WIDTH, SCREEN_HEIGHT);
  GameLoop();
  return 0;
}
