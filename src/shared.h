#include <stdio.h>
#include <cstddef>
#include <stdint.h>

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
#define PLATFORM_DRAW_BOX(n) void n(float x, float y, float width, float height, float r, float g, float b, float a)
typedef PLATFORM_DRAW_BOX(PlatformDrawBoxFn);

// Image and Sprite loading
const unsigned int MAX_SURFACES = 3;
const unsigned int MAX_FILENAME_LENGTH = 31;

#define PLATFORM_ENSURE_IMAGE(n) unsigned int n(const char *filename)
typedef PLATFORM_ENSURE_IMAGE(PlatformEnsureImageFn);

#define PLATFORM_ENSURE_SPRITESHEET(n) unsigned int n(const char *filename)
typedef PLATFORM_ENSURE_SPRITESHEET(PlatformEnsureSpritesheetFn);

#define PLATFORM_DRAW_TEXTURE(n) void n(unsigned int textureIndex, float x, float y, float width, float height, int sprite_x, int sprite_y, int sprite_w, int sprite_h)
typedef PLATFORM_DRAW_TEXTURE(PlatformDrawTextureFn);

struct PlatformAPI
{
  PlatformDrawBoxFn *PlatformDrawBox;
  PlatformEnsureImageFn *PlatformEnsureImage;
  PlatformEnsureSpritesheetFn *PlatformEnsureSpritesheet;
  PlatformDrawTextureFn *PlatformDrawTexture;
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
