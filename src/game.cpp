
#include <stdio.h>
#include "shared.h"


struct GameState
{
    GameMemory memory;
    PlatformAPI api;
    float accel_x;
    float accel_y;
    float x;
    float y;
};

static GameState *state;

extern "C" GAME_INIT(GameInit)
{
    state = GameAllocateStruct(&memory, GameState);
    state->api = api;

    if(state->memory.ptr == 0) {
      state->memory = memory;
    }
}

extern "C" GAME_UPDATE(GameUpdate)
{
    if (state->accel_x < 0) {
      if (state->x < 0) {
	state->accel_x = 1.0;
      }
    } else if (state->accel_x > 0) { 
      if (state->x > 300) {
	state->accel_x = -1.0;
      }
    }else {
      state->accel_x = 1;
    }
    if (state->accel_y < 0) {
      if (state->y < 0) {
	state->accel_y = 1.0;
      }
    } else if (state->accel_y > 0) { 
      if (state->y > 500) {
	state->accel_y = -1.0;
      }
    }else {
      state->accel_y = 1;
    }
    state->x += state->accel_x * (dt * 50.0f);
    state->y += state->accel_y * (dt * 50.0f);
}

extern "C" GAME_RENDER(GameRender)
{
    state->api.PlatformDrawBox(state->x, state->y, 50.0f, 50.0f);
}
