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


// Only one platform api call as an example. This draws a white box.
#define PLATFORM_DRAW_BOX(n) void n(float x, float y, float width, float height)
typedef PLATFORM_DRAW_BOX(PlatformDrawBoxFn);

struct PlatformAPI
{
    PlatformDrawBoxFn *PlatformDrawBox;
};

//
// These are all the game functions. These macros help maintain the
// signature across various places easier.
//

#define GAME_INIT(n) void n(GameMemory memory, PlatformAPI api)
typedef GAME_INIT(GameInitFn);

#define GAME_UPDATE(n) void n(float dt)
typedef GAME_UPDATE(GameUpdateFn);

#define GAME_RENDER(n) void n()
typedef GAME_RENDER(GameRenderFn);
