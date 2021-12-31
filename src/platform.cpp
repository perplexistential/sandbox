#include <assert.h>
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

#include <GL/glut.h>
#include "shared.h"


#define BUILD_DIR "build"
#define GAME_LIB "build/libgame.so"
#define GAME_LIB_TEMP "build/libgame_temp.so"

struct GameCode
{
    GameInitFn *game_init;
    GameUpdateFn *game_update;
    GameRenderFn *game_render;

    void* handle;
    time_t last_file_time;
};

static struct
{
    SDL_Window *window;
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
                             "Houston, we have a problem!",
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
        glColor3f(1.0f, 1.0f, 1.0f);
        glVertex2f(x, y);
        glVertex2f(x+width, y);
        glVertex2f(x+width, y+height);
        glVertex2f(x, y+height);
    glEnd();
}

PlatformAPI GetPlatformAPI()
{
    PlatformAPI result = {};

    result.PlatformDrawBox = DrawBox;
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
            SDL_Delay(1000);
            state.game_code = LoadGameCode(GAME_LIB);
            state.game_code.game_init(state.game_memory, GetPlatformAPI());
	}

        SDL_Delay(1);
    }
}

int main(int argc, char *argv[])
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        Die("Failed to initialize SDL2: %s\n", SDL_GetError());
    }

    state.window
        = SDL_CreateWindow("Awesome Game",
                           SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                           640, 480,
                           SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    if(!state.window) {
        Die("Failed to create window: %s\n", SDL_GetError());
    }

    fflush(stdout);
    state.gl_context = SDL_GL_CreateContext(state.window);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    glOrtho(0.0f, 640.0f, 0.0f, 480.0f, 0.0f, 1.0f);

    state.game_memory = AllocateGameMemory();
    
    state.game_code = LoadGameCode(GAME_LIB);
    state.game_code.game_init(state.game_memory, GetPlatformAPI());
    
    GameLoop();

    return 0;
}
