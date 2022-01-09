#include <SDL2/SDL_stdinc.h>
#include <assert.h>
#include <cstddef>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>
// #include <SDL2/SDL_opengl_glext.h>
#include <SDL2/SDL_surface.h>

#include <GL/glut.h>
#include "shared.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define BUILD_DIR "build"
#define GAME_LIB "build/libgame.so"
#define GAME_LIB_TEMP "build/libgame_temp.so"

#define IMAGES_DIR "assets/images"

struct GameCode
{
    GameInitFn *game_init;
    GameUpdateFn *game_update;
    GameRenderFn *game_render;

    void* handle;
    time_t last_file_time;
};

struct Texture
{
  char filename[MAX_FILENAME_LENGTH];
  GLuint textureID;
};

static struct
{
  SDL_Window *window;
  Texture textures[MAX_SURFACES];
  SDL_GLContext gl_context;
  GameCode game_code;
  GameMemory game_memory;
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
                             buffer, state.window);
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
    }

    printf("reloaded game code successfully\n");
    return result;
}

void UnloadGameCode(GameCode *game_code)
{
    dlclose(game_code->handle);
    game_code->handle = 0;

    game_code->game_init = 0;
    game_code->game_update = 0;
    game_code->game_render = 0;
}

PLATFORM_DRAW_BOX(DrawBox)
{
    glBegin(GL_QUADS);
	glColor4f(r, g, b, a);
        glVertex2f(x, y);
        glVertex2f(x+width, y);
        glVertex2f(x+width, y+height);
        glVertex2f(x, y+height);
    glEnd();
}

PLATFORM_ENSURE_IMAGE(EnsureImage)
{
  bool found = false;
  int textureIndex = -1;
  int imagePathLength = strlen(IMAGES_DIR)+strlen(filename) + 1;
  char *imagePath = (char *) malloc(imagePathLength * sizeof(char));
  sprintf(imagePath, "%s/%s", IMAGES_DIR, filename);

  for(unsigned int i=0; i < MAX_SURFACES; i++){
    if (textureIndex == -1 && state.textures[i].textureID == 0){
      textureIndex = i;
    }
    if (0 == strncmp(state.textures[i].filename, filename, MAX_FILENAME_LENGTH)) {
      found = true;
      textureIndex = i;
    }
    if (found && -1 != textureIndex) {
      break;
    }
  }
  if (-1 == textureIndex) {
    Die("no space left to load the image: %s", filename);
  }
  if (!found) {
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glGenTextures(1, &state.textures[textureIndex].textureID);
    glBindTexture(GL_TEXTURE_2D, state.textures[textureIndex].textureID);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    SDL_Surface* newSurface = IMG_Load(imagePath);
    if (!newSurface) {
      Die("Failed to load the image: %s", IMG_GetError());
    }
    int mode = GL_RGB;
    if (newSurface->format->BytesPerPixel == 4) {
      mode = GL_RGBA;
    }
    glTexImage2D(GL_TEXTURE_2D,
		 0, GL_RGBA,
		 newSurface->w, newSurface->h,
		 0, GL_RGBA,
		 GL_UNSIGNED_BYTE, newSurface->pixels);
    
    strncpy(state.textures[textureIndex].filename, filename, MAX_FILENAME_LENGTH);
  }
  free(imagePath);
  return textureIndex;
}

PLATFORM_ENSURE_SPRITESHEET(EnsureSpritesheet)
{
  printf("loading spritesheets is not yet implemented");
  return -1;
}

PLATFORM_DRAW_TEXTURE(DrawTexture)
{
  glBindTexture(GL_TEXTURE_2D, state.textures[textureIndex].textureID);
  glBegin(GL_QUADS);
     glColor4f(1.0f, 0, 1.0f, 1.0f);
     glTexCoord2i(sprite_x, sprite_y); glVertex3f(x, y, 0);
     glTexCoord2i(sprite_x + sprite_w, sprite_y); glVertex3f(x + width, y, 0);
     glTexCoord2i(sprite_x + sprite_w, sprite_y + sprite_h); glVertex3f(x + width, y + height, 0);
     glTexCoord2i(sprite_x, sprite_y + sprite_h); glVertex3f(x, y + height, 0);
     //glTexCoord2i(0, 0); glVertex3f(x, y, 0);
     //glTexCoord2i(1, 0); glVertex3f(x + width, y, 0);
     //glTexCoord2i(1, 1); glVertex3f(x + width, y + height, 0);
     //glTexCoord2i(0, 1); glVertex3f(x, y + height, 0);
  glEnd();
}

PlatformAPI GetPlatformAPI()
{
    PlatformAPI result = {};

    result.PlatformDrawBox = DrawBox;
    result.PlatformDrawTexture = DrawTexture;
    result.PlatformEnsureImage = EnsureImage;
    return result;
}

GameMemory AllocateGameMemory()
{
    GameMemory result = {};

    result.ptr = (uint8_t *)calloc(1, 1024);
    result.size = 1024;
    result.cursor = result.ptr;

    return result;
}


void GameLoop()
{
  for(;;) {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
      if(event.type == SDL_QUIT) {
	Quit();
      }
    }
    
    state.game_code.game_update(1.0f/60.0f);
    
    glClear(GL_COLOR_BUFFER_BIT);
    state.game_code.game_render();
    SDL_GL_SwapWindow(state.window);
    
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
  if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    Die("Failed to initialize SDL2: %s\n", SDL_GetError());
  }
  
  if(IMG_Init(IMG_INIT_PNG) < 0) {
    Die("Failed to initialize PNG support: %s\n", IMG_GetError());
  }
  
  state.window
    = SDL_CreateWindow("Perplexistential Sandbox",
		       SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		       SCREEN_WIDTH, SCREEN_HEIGHT,
		       SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
  
  if(!state.window) {
    Die("Failed to create window: %s\n", SDL_GetError());
  }

  // using OpenGL render context
  state.gl_context = SDL_GL_CreateContext(state.window);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0f, 1.0f*SCREEN_WIDTH, 0.0f, 1.0f*SCREEN_HEIGHT, 0.0f, 1.0f);

  // game state init
  state.game_memory = AllocateGameMemory();
  state.game_code = LoadGameCode(GAME_LIB);
  state.game_code.game_init(state.game_memory, GetPlatformAPI(), SCREEN_WIDTH, SCREEN_HEIGHT);

  GameLoop();
  
  return 0;
}
