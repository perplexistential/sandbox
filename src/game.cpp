#include <bits/types/time_t.h>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <cstring>
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

#define COLOR_SHIFT_RATE 0.2

#define COLLISION_DEMO_ENABLED true
#define COLLISION_SPLATTER false
#define COLLISION_DEMO_MAX_BOXES 50000
#define COLLISION_DEMO_INITIAL_BOX_COUNT 50000

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
#define CHARACTER_DEMO_SPRITE "as_.png"

struct SpriteFrame
{
  uint16_t x, y, width, height;
};

#define FACING_DOWN_STEP_0                                                     \
  { .x = 113, .y = 160, .width = 16, .height = 32 }

#define FACING_DOWN_STEP_1                                                     \
  { .x = 96, .y = 160, .width = 16, .height = 32 }

#define FACING_DOWN_STEP_2                                                     \
  { .x = 130, .y = 160, .width = 16, .height = 32 }

#define FACING_RIGHT_STEP_0                                                    \
  { .x = 113, .y = 193, .width = 16, .height = 32 }

#define FACING_RIGHT_STEP_1                                                    \
  { .x = 96, .y = 193, .width = 16, .height = 32 }

#define FACING_RIGHT_STEP_2                                                    \
  { .x = 130, .y = 193, .width = 16, .height = 32 }

#define FACING_UP_STEP_0                                                       \
  { .x = 113, .y = 226, .width = 16, .height = 32 }

#define FACING_UP_STEP_1                                                       \
  { .x = 96, .y = 226, .width = 16, .height = 32 }

#define FACING_UP_STEP_2                                                       \
  { .x = 130, .y = 226, .width = 16, .height = 32 }

#define FACING_LEFT_STEP_0                                                     \
  { .x = 113, .y = 259, .width = 16, .height = 32 }

#define FACING_LEFT_STEP_1                                                     \
  { .x = 96, .y = 259, .width = 16, .height = 32 }

#define FACING_LEFT_STEP_2                                                     \
  { .x = 130, .y = 259, .width = 16, .height = 32 }

#define MAX_FRAMES 3
const SpriteFrame CHAR_FACING_DOWN[MAX_FRAMES] = {
    FACING_DOWN_STEP_1,
    FACING_DOWN_STEP_2,
    FACING_DOWN_STEP_0,
};
const SpriteFrame CHAR_FACING_RIGHT[MAX_FRAMES] = {
    FACING_RIGHT_STEP_1,
    FACING_RIGHT_STEP_2,
    FACING_RIGHT_STEP_0,
};
const SpriteFrame CHAR_FACING_UP[MAX_FRAMES] = {
    FACING_UP_STEP_1,
    FACING_UP_STEP_2,
    FACING_UP_STEP_0,
};
const SpriteFrame CHAR_FACING_LEFT[MAX_FRAMES] = {
    FACING_LEFT_STEP_1,
    FACING_LEFT_STEP_2,
    FACING_LEFT_STEP_0,
};
#define NUMBER_OF_FACING_DIRS 4
enum { FACING_DOWN = 0, FACING_UP = 2};
enum { FACING_RIGHT = 1, FACING_LEFT = 3 };
const SpriteFrame *CHAR_ANIMATION_FRAMES[NUMBER_OF_FACING_DIRS] = {
    CHAR_FACING_DOWN,
    CHAR_FACING_RIGHT,
    CHAR_FACING_UP,
    CHAR_FACING_LEFT,
};

struct Character
{
  unsigned int textureIndex;
  BoundingBox bb;
  float current_frame;
  uint8_t facing;
  uint8_t cyclesPerSecond;
  bool walking;
  bool falling;
  bool crouched;
};

const SpriteFrame* getSpriteFrame(Character *c) {
  return &(CHAR_ANIMATION_FRAMES[c->facing][(int)(floor(c->current_frame))]);
}

#define CONTROLLER_DEMO_ENABLED true

enum {
  CONTROLLER_UP = 0,
  CONTROLLER_DOWN,
  CONTROLLER_LEFT,
  CONTROLLER_RIGHT,
  CONTROLLER_RUN,
  CONTROLLER_FIRE,
  CONTROLLER_MENU,
  CONTROLLER_CONTEXT_MENU,
  CONTROLLER_PAUSE,
  CONTROLLER_TRIGGER_L,
  CONTROLLER_TRIGGER_R,
};

struct Controller
{
  int up, down, left, right;
  int jump;
  int run;
  int fire;
  int menu;
  int context_menu;
  int pause;
  int trigger_l, trigger_r;

  int menu_open;

  float pointer_x, pointer_y;

  float action_queue_timer;
  int action_queue[10];
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
  
  // User Input Demo
  bool keyboardDemoInitialized;
  Controller controller;
};

#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

static GameState *state;

BoundingBox newDemoBB(unsigned int c)
{
  float x = 60.0f,y = 60.0f;

  if (COLLISION_SPLATTER) {
    if (c > 1000) {
      x = state->screen_w - 60.0f;
    }
    if (c > 2000) {
      y = state->screen_h - 60.0f;
    }
    if (c > 3000) {
      x = 60.0f;
    }
    if (c > 4000) {
      x = state->screen_w * 0.5f;
      y = 60.0f;
    }
    if (c > 5000) {
      x = state->screen_w;
      y = state->screen_h * 0.5f;
    }
    if (c > 6000) {
      y = 60.0f;
    }
    if (c > 7000) {
      x = state->screen_w * 0.5f;
      y = state->screen_h - 60.0f;
    }
  }
  return {
    .accel_x = rand() % 9 % 2 == 0 ? -1.0f : 1.0f,
    .accel_y = rand() % 9 % 2 == 0 ? 1.0f : -1.0f,
    .x = x,
    .y = y,
    .width = 5.0f,
    .height = 5.0f,
    .veloc_x = (rand() % (c+1)) * 0.020f,
    .veloc_y = (rand() % (c+1)) * 0.020f,
    .r=0.3f * (rand() % 9),
    .accel_r=1.0f,
    .g=0.0f * (rand() % 9),
    .accel_g=0.0f,
    .b=0.0f * (rand() % 9),
    .accel_b=0.0f,
    .a=0.25,
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
  bb.a = 0.5f;
  return bb;
}

func(GAME_WINDOW_RESIZED, GameWindowResized)
{
  printf("window(%d) resized", window);
  state->screen_w = width;
  state->screen_h = height;
}

extern "C" GAME_QUIT(GameQuit)
{
  printf("game: got a quit - gonna take a screenshot\n");
  state->api.PlatformScreenshot(0, 0, 0, state->screen_w, state->screen_h);
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

  state->collisionDemoInitialized = false;
  if (COLLISION_DEMO_ENABLED && !state->collisionDemoInitialized) {
    //srand(37);
    state->collisionDemoInitialized = true;
    state->wall = newBB(300.0f, 100.0f, 50.0f, 200.0f);
    state->ground = newBB(0.0f, 0.0f, 1.0f*state->screen_w, 30.0f);
    for (unsigned int c=0; c < COLLISION_DEMO_INITIAL_BOX_COUNT; c++) {
      memset(&state->boxes[c], 0, sizeof(BoundingBox));
      state->boxes[c] = newDemoBB(c);
    }
    state->boxCount = COLLISION_DEMO_INITIAL_BOX_COUNT;
  }
  
  if (CHARACTER_DEMO_ENABLED && !state->characterDemoInitialized) {
    state->characterDemoInitialized = true;
    memset(&state->character, 0, sizeof(Character));
    state->character.textureIndex = state->api.PlatformEnsureImage(CHARACTER_DEMO_SPRITE);
    state->character.bb.x = 150;
    state->character.bb.y = 250;
    state->character.bb.width = 64;
    state->character.bb.height = 128;
    state->character.bb.accel_x = 3.0f;
    state->character.bb.accel_y = 3.0f;
    state->character.bb.veloc_x = 15.0f;
    state->character.bb.veloc_y = 15.0f;
    state->character.current_frame = 0;
    state->character.cyclesPerSecond = 30;
  }
}

float speed(float accel, float dt, float velocity)
{
  return accel * (dt * velocity);
}

void shiftColor(float* c, float* accel, float dt)
{
  *c += *accel * (dt * COLOR_SHIFT_RATE * (rand() % 9));
  if (*c >= 0.7f || *c <= 0.3f)
    *accel *= -1.0f;
}

extern "C" GAME_UPDATE(GameUpdate)
{
  if (COLLISION_DEMO_ENABLED) {
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
	state->boxes[c].veloc_x -= state->boxes[c].veloc_x * 0.05f;
      } else if (checkCollision(&bb_next_x, &state->ground)) {
	state->boxes[c].accel_x *= -1.0f;
      } else if (bb_next_x.x < 0 || bb_next_x.x + bb_next_x.width > state->screen_w) {
	state->boxes[c].accel_x *= -1.0f;
	state->boxes[c].veloc_x -= state->boxes[c].veloc_x * 0.05f;
      } else {
	state->boxes[c].veloc_x *= 0.99f;
      }
      if (checkCollision(&bb_next_y, &state->wall)) {
	state->boxes[c].accel_y *= 0.0f;
	state->boxes[c].veloc_x *= 0.75f;
      } else if (checkCollision(&bb_next_y, &state->ground)) {
	state->boxes[c].accel_y = -0.0f;
	state->boxes[c].veloc_y -= state->boxes[c].veloc_y * 0.95f;
	state->boxes[c].veloc_x *= 0.75;
      } else if (bb_next_y.y < 0 || bb_next_y.y + bb_next_y.height > state->screen_h) {
	state->boxes[c].accel_y *= -1.0f;
      } else {
	if(state->boxes[c].accel_y > 0) {
	  state->boxes[c].veloc_y -= 9.8;
	  if (state->boxes[c].veloc_y < 0.1){
	    state->boxes[c].accel_y = -1.0f;
	    state->boxes[c].veloc_y = 0.1;
	  }
	} else {
	  state->boxes[c].accel_y = -1.0f;
	  state->boxes[c].veloc_y += 2.0f;
	}
      }
      state->boxes[c].x += speed(state->boxes[c].accel_x, dt, state->boxes[c].veloc_x);
      state->boxes[c].y += speed(state->boxes[c].accel_y, dt, state->boxes[c].veloc_y);
      shiftColor(&state->boxes[c].r, &state->boxes[c].accel_r, dt);
      shiftColor(&state->boxes[c].g, &state->boxes[c].accel_g, dt);
      shiftColor(&state->boxes[c].b, &state->boxes[c].accel_b, dt);
      //shiftColor(&state->boxes[c].a, &state->boxes[c].accel_a, dt);
    }
  }

  if (CHARACTER_DEMO_ENABLED) {
    // Determine the next x and y bounding boxes
    BoundingBox bb_next_x = {
      0,0,
      state->character.bb.x + speed(state->character.bb.accel_x, dt, state->character.bb.veloc_x),
      state->character.bb.y,
      state->character.bb.width,
      state->character.bb.height,
    };
    BoundingBox bb_next_y = {
      0,0,
      state->character.bb.x,
      state->character.bb.y + speed(state->character.bb.accel_y, dt, state->character.bb.veloc_y),
      state->character.bb.width,
      state->character.bb.height
    };
    if (checkCollision(&bb_next_x, &state->wall)) {
      state->character.bb.accel_x *= -1.0f;
    } else if (checkCollision(&bb_next_x, &state->ground)) {
      state->character.bb.accel_x *= -1.0f;
    } else if (bb_next_x.x < 0 || bb_next_x.x + bb_next_x.width > state->screen_w) {
      state->character.bb.accel_x *= -1.0f;
    }
    if (checkCollision(&bb_next_y, &state->wall)) {
      state->character.bb.accel_y *= -1.0f;
    } else if (checkCollision(&bb_next_y, &state->ground)) {
      state->character.bb.accel_y *= -1.0f;
    } else if (bb_next_y.y < 0 || bb_next_y.y + bb_next_y.height > state->screen_h) {
      state->character.bb.accel_y *= -1.0f;
    }
    float speed_x = speed(state->character.bb.accel_x, dt, state->character.bb.veloc_x);
    float speed_y = speed(state->character.bb.accel_y, dt, state->character.bb.veloc_y);
    float *speed = &speed_x;
    state->character.bb.x += speed_x;
    state->character.bb.y += speed_y;
    if (speed_x > speed_y) {
      if (speed_x <= 0) {
	state->character.facing = FACING_LEFT;
      } else {
	state->character.facing = FACING_RIGHT;
      }
    } else {
      if (speed_y < 0) {
	state->character.facing = FACING_DOWN;
      } else {
	state->character.facing = FACING_UP;
      }
      speed = &speed_y;
    }
    
    // "2" = the number of walking animation frames.
    // "1 +" because the frame at 0 is the stand ing animation frame
    state->character.current_frame += (dt / (1.0 / abs(*speed))) * 10;
    if (state->character.current_frame >= 2){
      state->character.current_frame = 0;
    }
    
  }
}


extern "C" GAME_RENDER(GameRender)
{
    state->api.PlatformDrawBox(state->wall.x, state->wall.y, state->wall.width, state->wall.height,
			       state->wall.r, state->wall.g, state->wall.b, state->wall.a);
    state->api.PlatformDrawBox(state->ground.x, state->ground.y,
			       state->ground.width, state->ground.height,
			       state->ground.r, state->ground.g, state->ground.b, state->ground.a);
    for (unsigned int c=0; c < state->boxCount; c++) {
      state->api.PlatformDrawBox(state->boxes[c].x,
				 state->boxes[c].y,
				 state->boxes[c].width,
				 state->boxes[c].height,
				 state->boxes[c].r,
				 state->boxes[c].g,
				 state->boxes[c].b,
				 state->boxes[c].a);
    }
    if (CHARACTER_DEMO_ENABLED) {
      const SpriteFrame *sf = getSpriteFrame(&state->character);
      state->api.PlatformDrawTexture(state->character.textureIndex,
				     state->character.bb.x,
				     state->character.bb.y,
				     state->character.bb.width,
				     state->character.bb.height,
				     sf->x, sf->y, sf->width, sf->height);
    }
}

extern "C" GAME_KEYBOARD_INPUT(GameKeyboardInput)
{
  
}

extern "C" GAME_AUDIO_CHANNEL_HALTED(GameAudioChannelHalted)
{
  printf("%d audio channel halted\n", channel);
}


