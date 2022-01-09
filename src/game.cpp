#include <bits/types/time_t.h>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
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
  float r;
  float accel_r;
  float g;
  float accel_g;
  float b;
  float accel_b;
  float a;
  float accel_a;
};

#define COLOR_SHIFT_RATE 0.1

#define COLLISION_DEMO_ENABLED true
#define COLLISION_DEMO_MAX_BOXES 11
#define COLLISION_DEMO_INITIAL_BOX_COUNT 11

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

  // Window meta
  int screen_w, screen_h;
  
  // Collision Demo
  bool collisionDemoInitialized;
  BoundingBox boxes[COLLISION_DEMO_MAX_BOXES];
  BoundingBox wall;
  BoundingBox ground;
  unsigned int boxCount;

  // Animating and controlling a Character Demo
  bool characterDemoInitialized;
  Character character;
};


static GameState *state;

BoundingBox newDemoBB(unsigned int c)
{
  return {
	.accel_x = c % 2 ? -1.0f : 1.0f,
	.accel_y = c % 2 ? 1.0f : -1.0f,
	.x = 3.0f * c,
	.y = 35 + 1.0f * c,
	.width = 50.0f,
	.height = 50.0f,
	.veloc_x = 10 + 30.0f * c,
	.veloc_y = 10 + 20.0f * c,
	.r=0.1f * (rand() % 9), 
	.accel_r=1.0f,
	.g=0.1f * (rand() % 9), 
	.accel_g=1.0f,
	.b=0.1f * (rand() % 9), 
	.accel_b=1.0f,
	.a=0.1f * (rand() % 5),
	.accel_a=1.0f,
  };
}

BoundingBox newBB(float x, float y, float w, float h)
{
  BoundingBox bb = newDemoBB(0);
  bb.x = x;
  bb.y = y;
  bb.width = w;
  bb.height = h;
  bb.a = 1.0f;
  return bb;
}

extern "C" GAME_INIT(GameInit)
{
  state = GameAllocateStruct(&memory, GameState);
  state->api = api;
  if(state->memory.ptr == 0) {
    state->memory = memory;
  }
  state->screen_w = screen_w;
  state->screen_h = screen_h;

  if (COLLISION_DEMO_ENABLED && !state->collisionDemoInitialized) {
    srand(37);
    state->collisionDemoInitialized = true;
    state->wall = newBB(300.0f, 100.0f, 50.0f, 200.0f);
    state->ground = newBB(0.0f, 0.0f, 1.0f*state->screen_w, 30.0f);
    for (unsigned int c=0; c < COLLISION_DEMO_INITIAL_BOX_COUNT; c++) {
      state->boxes[c] = newDemoBB(c);
    }
    state->boxCount = COLLISION_DEMO_INITIAL_BOX_COUNT;
  }

  state->characterDemoInitialized = false;
  if (CHARACTER_DEMO_ENABLED) {
    state->characterDemoInitialized = true;
    state->character.textureIndex = state->api.PlatformEnsureImage(CHARACTER_DEMO_SPRITE);
    state->character.bb.x = 150;
    state->character.bb.y = 250;
    state->character.bb.width = 64;
    state->character.bb.height = 128;
  }
}

float speed(float accel, float dt, float velocity)
{
  return accel * (dt * velocity);
}


void shiftColor(float* c, float* accel, float dt)
{
  *c += *accel * (dt * COLOR_SHIFT_RATE * (rand() % 9));
  if (*c >= 0.9f || *c <= 0.1f) 
    *accel *= -1.0f;
}

extern "C" GAME_UPDATE(GameUpdate)
{
  for (unsigned int c=0; c < state->boxCount; c++){
    
    // Determine the next x and y bounding boxes
    BoundingBox bb_next_x = {
      0,0,state->boxes[c].x + speed(state->boxes[c].accel_x, dt, state->boxes[c].veloc_x),
      state->boxes[c].y, state->boxes[c].width, state->boxes[c].height,
    };
    
    BoundingBox bb_next_y = {
      0,0,state->boxes[c].x,
      state->boxes[c].y + speed(state->boxes[c].accel_y, dt, state->boxes[c].veloc_y),
      state->boxes[c].width, state->boxes[c].height
    };
    
    if (checkCollision(&bb_next_x, &state->wall)) {
      state->boxes[c].accel_x *= -1.0f;
    } else if (checkCollision(&bb_next_x, &state->ground)) {
      state->boxes[c].accel_x *= -1.0f;
    } else if (bb_next_x.x < 0 || bb_next_x.x + bb_next_x.width > state->screen_w) {
      state->boxes[c].accel_x *= -1.0f;
    }
    
    if (checkCollision(&bb_next_y, &state->wall)) {
      state->boxes[c].accel_y *= -1.0f;
    } else if (checkCollision(&bb_next_y, &state->ground)) {
      state->boxes[c].accel_y *= -1.0f;
    } else if (bb_next_y.y < 0 || bb_next_y.y + bb_next_y.height > state->screen_h) {
      state->boxes[c].accel_y *= -1.0f;
    }
    
    state->boxes[c].x += speed(state->boxes[c].accel_x, dt, state->boxes[c].veloc_x);
    state->boxes[c].y += speed(state->boxes[c].accel_y, dt, state->boxes[c].veloc_y);

    shiftColor(&state->boxes[c].r, &state->boxes[c].accel_r, dt);
    shiftColor(&state->boxes[c].g, &state->boxes[c].accel_g, dt);
    shiftColor(&state->boxes[c].b, &state->boxes[c].accel_b, dt);
    shiftColor(&state->boxes[c].a, &state->boxes[c].accel_a, dt);
  }
  
  if (state->character.bb.x < 0 || state->character.bb.x > state->screen_w) {
    state->character.bb.accel_x *= -1.0f;
  } else if (checkCollision(&state->character.bb, &state->wall)) {
    state->character.bb.accel_x *= -1.0f;
  }
  if (state->character.bb.y < 0 || state->character.bb.y > state->screen_h) {
    state->character.bb.accel_y *= -1.0f;
  } else if (checkCollision(&state->character.bb, &state->wall)) {
    state->character.bb.accel_y *= -1.0f;
  }
  state->character.bb.x += speed(state->character.bb.accel_x, dt, state->character.bb.veloc_x);
  state->character.bb.y += speed(state->character.bb.accel_y, dt, state->character.bb.veloc_y);
}



extern "C" GAME_RENDER(GameRender)
{
  state->api.PlatformDrawBox(state->wall.x, state->wall.y, state->wall.width, state->wall.height,
			     state->wall.r, state->wall.g, state->wall.b, state->wall.a);
  state->api.PlatformDrawBox(state->ground.x, state->ground.y, state->ground.width, state->ground.height,
			     state->ground.r, state->ground.g, state->ground.b, state->ground.a);
  for (unsigned int c; c < state->boxCount; c++) {
    state->api.PlatformDrawBox(state->boxes[c].x,
			       state->boxes[c].y,
			       state->boxes[c].width,
			       state->boxes[c].height,
			       state->boxes[c].r,
			       state->boxes[c].g,
			       state->boxes[c].b,
			       state->boxes[c].a);
  }
  state->api.PlatformDrawTexture(state->character.textureIndex,
				 state->character.bb.x,
				 state->character.bb.y,
				 state->character.bb.width,
				 state->character.bb.height,
				 113, 160, 16, 32);
}
