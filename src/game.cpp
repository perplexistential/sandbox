#include <stdio.h>
#include "shared.h"

struct BoundingBox
{
  float accel_x;
  float accel_y;
  float x;
  float y;
  float width;
  float height;
  float veloc_x;
  float veloc_y;
};

#define COLLISION_DEMO_ENABLED true
#define COLLISION_DEMO_MAX_BOXES 10
#define COLLISION_DEMO_INITIAL_BOX_COUNT 10

bool checkCollision(BoundingBox *a, BoundingBox *b){
  // the sides of the rects
  int leftA, leftB;
  int rightA, rightB;
  int topA, topB;
  int bottomA, bottomB;

  // Calculate the sides of "A"
  leftA = a->x;
  rightA = a->x + a->width;
  topA = a->y;
  bottomA = a->y + a->height;

  // Calculate the sides of "B"
  leftB = b->x;
  rightB = b->x + b->width;
  topB = b->y;
  bottomB = b->y + b->height;

  // If any of the sides from A are outside of Bool
  if (bottomA <= topB) {
    return false;
  }
  if (topA >= bottomB) {
    return false;
  }
  if (rightA <= leftB) {
    return false;
  }
  if (leftA >= rightB) {
    return false;
  }
  // Collision!
  return true;
}

#define CHARACTER_DEMO_ENABLED true
#define CHARACTER_DEMO_SPRITE "as.png"

struct Character
{
  unsigned int textureIndex;
  BoundingBox bb;
  bool walking;
};

struct GameState
{
  GameMemory memory;
  PlatformAPI api;

  // Collision Demo
  bool collisionDemoInitialized;
  BoundingBox boxes[COLLISION_DEMO_MAX_BOXES];
  BoundingBox wall;
  unsigned int boxCount;

  // Animating and controlling a Character Demo
  bool characterDemoInitialized;
  Character character;
};


static GameState *state;

extern "C" GAME_INIT(GameInit)
{
    state = GameAllocateStruct(&memory, GameState);
    state->api = api;

    if(state->memory.ptr == 0) {
      state->memory = memory;
    }

    if (COLLISION_DEMO_ENABLED && !state->collisionDemoInitialized) {
      state->collisionDemoInitialized = true;
      state->wall = {.x = 300.0f, .y = 100.0f, .width=50.0f, .height=200.0f};
      for (unsigned int c=0; c < COLLISION_DEMO_INITIAL_BOX_COUNT; c++) {
	state->boxes[c] = {
	  .accel_x = c % 2 ? -1.0f : 1.0f,
	  .accel_y = c % 2 ? 1.0f : -1.0f,
	  .x = 3.0f * c,
	  .y = 1.0f * c,
	  .width = 50.0f,
	  .height = 50.0f,
	  .veloc_x = 10 + 30.0f * c,
	  .veloc_y = 10 + 20.0f * c,
	};
      }
      state->boxCount = COLLISION_DEMO_INITIAL_BOX_COUNT;
    }

    if (CHARACTER_DEMO_ENABLED) {
      state->characterDemoInitialized = true;
      state->character.textureIndex = state->api.PlatformEnsureImage(CHARACTER_DEMO_SPRITE);
      state->character.bb.x = 250;
      state->character.bb.y = 350;
      state->character.bb.width = 16;
      state->character.bb.height = 32;
    }
}

extern "C" GAME_UPDATE(GameUpdate)
{
  for (unsigned int c=0; c < state->boxCount; c++){
    if (state->boxes[c].x < 0 || state->boxes[c].x > 640) {
      state->boxes[c].accel_x *= -1.0f;
    } else if (state->boxes[c].y < 0 || state->boxes[c].y > 480) {
      state->boxes[c].accel_y *= -1.0f;
    } else {
      if (checkCollision(&state->boxes[c], &state->wall)) {
	state->boxes[c].accel_x *= -1.0f;
      } else if (checkCollision(&state->boxes[c], &state->wall)) {
	state->boxes[c].accel_y *= -1.0f;
      }
    }    
    state->boxes[c].x += state->boxes[c].accel_x * (dt * state->boxes[c].veloc_x);
    state->boxes[c].y += state->boxes[c].accel_y * (dt * state->boxes[c].veloc_y);
  }
  
  if (state->character.bb.x < 0 || state->character.bb.x > 300) {
    state->character.bb.accel_x *= -1.0f;
  } else if (checkCollision(&state->character.bb, &state->wall)) {
    state->character.bb.accel_x *= -1.0f;
  }
  if (state->character.bb.y < 0 || state->character.bb.y > 300) {
    state->character.bb.accel_y *= -1.0f;
  } else if (checkCollision(&state->character.bb, &state->wall)) {
    state->character.bb.accel_y *= -1.0f;
  }
  state->character.bb.x += state->character.bb.accel_x * (dt * state->character.bb.veloc_x);
  state->character.bb.y += state->character.bb.accel_y * (dt * state->character.bb.veloc_y);
}

extern "C" GAME_RENDER(GameRender)
{
  state->api.PlatformDrawBox(state->wall.x, state->wall.y, state->wall.width, state->wall.height);
  for (unsigned int c; c < state->boxCount; c++) {
    state->api.PlatformDrawBox(state->boxes[c].x,
			       state->boxes[c].y,
			       state->boxes[c].width,
			       state->boxes[c].height);
  }
  
  state->api.PlatformDrawTexture(state->character.textureIndex,
				 state->character.bb.x,
				 state->character.bb.y,
				 state->character.bb.width,
				 state->character.bb.height,
				 113, 160, 16, 32);
}
