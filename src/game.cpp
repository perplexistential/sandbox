#include <bits/types/time_t.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "shared.h"

#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))

struct Rect
{
  float x;
  float y;
  float width;
  float height;
};

struct Color
{
  float r;
  float g;
  float b;
  float a;
};

struct Vertex
{
  float x;
  float y;
};

struct Quad
{
  Vertex ul;
  Vertex ur;
  Vertex br;
  Vertex bl;
};

Quad screenQuad = {
  .ul = {0.0f, 1.0f},
  .ur = {1.0f, 1.0f},
  .br = {1.0f, 0.0f},
  .bl = {0.0f, 0.0f},
};


struct ColorBox
{
  float veloc_x;
  float veloc_y;
  float accel_x;
  float accel_y;
  float accel_r;
  float accel_g;
  float accel_b;
  float accel_a;
};

#define COLOR_SHIFT_RATE 0.2

#define COLLISION_DEMO_ENABLED true
#define COLLISION_DEMO_MAX_BOXES 40000
#define COLLISION_DEMO_INITIAL_BOX_COUNT (uint32_t)40000
#define COLLISION_DEMO_BOX_LENGTH 50.0f

bool checkCollision(Quad *a, Quad *b)
{
  // If any of the sides from A are outside of B
  //if (bottomA <= topB) {
  if (a->ul.y <= b->bl.y) {
    return false;
  }
  //if (topA >= bottomB) {
  if (a->bl.y >= b->ul.y) {
    return false;
  }
  //if (rightA <= leftB) {
  if (a->br.x <= b->bl.x) {
    return false;
  }
  //if (leftA >= rightB) {
  if (a->bl.x >= b->br.x) {
    return false;
  }
  // Collision!
  return true;
}

#define CHARACTER_DEMO_ENABLED true
#define CHARACTER_DEMO_SPRITE "as_.png"

struct Character
{
  unsigned int textureIndex;
  Quad bb;
  float accel_x;
  float accel_y;
  float veloc_x;
  float veloc_y;
  bool walking;
  bool falling;
  bool facing_right;
  bool crouched;
};

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
  ColorBox boxes[COLLISION_DEMO_MAX_BOXES];
  Quad box_rects[COLLISION_DEMO_MAX_BOXES];
  Color box_color[COLLISION_DEMO_MAX_BOXES];
  Quad wall;
  Color wallColor;
  Quad ground;
  Color groundColor;
  uint32_t boxCount;

  // Animating and controlling a Character Demo
  bool characterDemoInitialized;
  Character character;
  
  // User Input Demo
  bool keyboardDemoInitialized;
  Controller controller;
};


static GameState *state;

Rect newDemoBB()
{
  return {
    .x = 1.0f * (rand() % state->screen_w),
    .y = 1.0f * (rand() % state->screen_h),
    .width = COLLISION_DEMO_BOX_LENGTH,
    .height = COLLISION_DEMO_BOX_LENGTH,
  };
}

Quad newDemoQuad()
{
  float x0 = 60 + 1.0f * (rand() % (state->screen_w - 100));
  float y0 = 60 + 1.0f * (rand() % (state->screen_h - 100));
  float width = COLLISION_DEMO_BOX_LENGTH;
  float height = COLLISION_DEMO_BOX_LENGTH;
  return {
    .ul = {x0, y0 + height},
    .ur = {x0 + width, y0 + height},
    .br = {x0 + width, y0},
    .bl = {x0, y0},
  };
}

Color newColor()
{
  return {
    .r=0.1f * (rand() % 9), 
    .g=0.1f * (rand() % 9), 
    .b=0.1f * (rand() % 9), 
    .a=0.5f + 0.1f * (rand() % 5)
  };
}

ColorBox newDemoColorBox(unsigned int c)
{
  return {
    .veloc_x = MIN(500, 10.0f + 30.0f * c),
    .veloc_y = MIN(500, 10.0f + 20.0f * c),
    .accel_x = (rand() % (1+c)) % 2 == 0 ? -1.0f : 1.0f,
    .accel_y = (rand() % (1+c)) % 2 == 0 ? 1.0f : -1.0f,
    .accel_r=0.5f,
    .accel_g=0.5f,
    .accel_b=0.5f,
    .accel_a=0.5f,
  };
}

Quad newQuad(float x, float y, float w, float h)
{
  float x0 = x;
  float y0 = y;
  float width = w;
  float height = h;
  return {
    .ul = {x0, y0 + height},
    .ur = {x0 + width, y0 + height},
    .br = {x0 + width, y0},
    .bl = {x0, y0},
  };
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
    screenQuad = {
      .ul = {COLLISION_DEMO_BOX_LENGTH,
	1.0f*state->screen_h - COLLISION_DEMO_BOX_LENGTH},
      .ur = {state->screen_w*1.0f - COLLISION_DEMO_BOX_LENGTH,
	1.0f*state->screen_h - COLLISION_DEMO_BOX_LENGTH},
      .br = {state->screen_w*1.0f - COLLISION_DEMO_BOX_LENGTH, 0.0f},
      .bl = {COLLISION_DEMO_BOX_LENGTH, 0.0f},
    };
    state->collisionDemoInitialized = true;
    state->wall = newQuad(300.0f, 100.0f, 50.0f, 200.0f);
    state->wallColor = newColor();
    state->ground = {
      .ul = {0, 0.1f*state->screen_h},
      .ur = {state->screen_w*1.0f, 0.1f * state->screen_h},
      .br = {state->screen_w*1.0f, 0},
      .bl = {0, 0},
    };
    state->groundColor = newColor();
    for (unsigned int c=0; c < COLLISION_DEMO_INITIAL_BOX_COUNT; c++) {
      state->boxes[c] = newDemoColorBox(c);
      state->box_rects[c] = newDemoQuad();
      state->box_color[c] = newColor();
    }
    state->boxCount = COLLISION_DEMO_INITIAL_BOX_COUNT;
  }

  if (CHARACTER_DEMO_ENABLED && state->characterDemoInitialized) {
    state->characterDemoInitialized = true;
    state->character.textureIndex = state->api.PlatformEnsureImage(CHARACTER_DEMO_SPRITE);
    state->character.bb = newQuad(150.0f, 250.0f, 64.0f, 120.0);
  }
}

extern "C" GAME_KEYBOARD_INPUT(GameKeyboardInput)
{
  
}

float speed(float accel, float dt, float velocity)
{
  return accel * (dt * velocity);
}

void shiftColor(float* c, float* accel, float dt)
{
  *c += *accel * (dt * COLOR_SHIFT_RATE * (rand() % 9));
  if (*c >= 0.7f || *c <= 0.2f) 
    *accel *= -1.0f;
}

extern "C" GAME_UPDATE(GameUpdate)
{
  if (COLLISION_DEMO_ENABLED) {
    for (unsigned int c=0; c < state->boxCount; c++){
      
      // Determine the next x and y bounding boxes
      Vertex speed_vector = {
	.x=speed(state->boxes[c].accel_x, dt, state->boxes[c].veloc_x),
	.y=speed(state->boxes[c].accel_y, dt, state->boxes[c].veloc_y)
      };
      Quad bb_next_x = {
	.ul={state->box_rects[c].ul.x + speed_vector.x, state->box_rects[c].ul.y},
	.ur={state->box_rects[c].ur.x + speed_vector.x, state->box_rects[c].ur.y},
	.br={state->box_rects[c].br.x + speed_vector.x, state->box_rects[c].br.y},
	.bl={state->box_rects[c].bl.x + speed_vector.x, state->box_rects[c].bl.y},
      };
      Quad bb_next_y = {
	.ul={state->box_rects[c].ul.x, state->box_rects[c].bl.y + speed_vector.y},
	.ur={state->box_rects[c].ur.x, state->box_rects[c].ul.y + speed_vector.y},
	.br={state->box_rects[c].br.x, state->box_rects[c].ur.y + speed_vector.y},
	.bl={state->box_rects[c].bl.x, state->box_rects[c].br.y + speed_vector.y},
      };
      
      if (checkCollision(&bb_next_x, &state->wall)) {
	state->boxes[c].accel_x *= -1.0f;
      } else if (checkCollision(&bb_next_x, &state->ground)) {
	state->boxes[c].accel_x *= -1.0f;
      } else if (!checkCollision(&bb_next_x, &screenQuad)) {
	state->boxes[c].accel_x *= -1.0f;
      }
      
      if (checkCollision(&bb_next_y, &state->wall)) {
	state->boxes[c].accel_y *= -1.0f;
      } else if (checkCollision(&bb_next_y, &state->ground)) {
	state->boxes[c].accel_y *= -1.0f;
      } else if (!checkCollision(&bb_next_y, &screenQuad)) {
	state->boxes[c].accel_y *= -1.0f;
      }

      speed_vector = {
	.x=speed(state->boxes[c].accel_x, dt, state->boxes[c].veloc_x),
	.y=speed(state->boxes[c].accel_y, dt, state->boxes[c].veloc_y)
      };
      // TODO: this could be done better; vectorized addition
      state->box_rects[c].bl.x += speed_vector.x;
      state->box_rects[c].br.x += speed_vector.x;
      state->box_rects[c].ul.x += speed_vector.x;
      state->box_rects[c].ur.x += speed_vector.x;
      state->box_rects[c].bl.y += speed_vector.y;
      state->box_rects[c].br.y += speed_vector.y;
      state->box_rects[c].ul.y += speed_vector.y;
      state->box_rects[c].ur.y += speed_vector.y;

      shiftColor(&state->box_color[c].r, &state->boxes[c].accel_r, dt);
      shiftColor(&state->box_color[c].g, &state->boxes[c].accel_g, dt);
      shiftColor(&state->box_color[c].b, &state->boxes[c].accel_b, dt);
      shiftColor(&state->box_color[c].a, &state->boxes[c].accel_a, dt);
    }
  }

  if (CHARACTER_DEMO_ENABLED){
    /*
    Vertex speed_vector = {
      speed(state->character.accel_x, dt, state->character.veloc_x),
      speed(state->character.accel_y, dt, state->character.veloc_y)
    };
    */

    /*
    if (checkCollision(&state->character.bb, &state->wall)) {
      state->character.accel_x *= -1.0f;
    } else if (!checkCollision(&state->character.bb, &screenQuad)) {
      //} else if (state->character.bb.x < 0 || state->character.bb.x >
state->screen_w) { state->character.accel_x *= -1.0f;
    }

    if (checkCollision(&state->character.bb, &state->wall)) {
      state->character.accel_y *= -1.0f;
    } else if (!checkCollision(&state->character.bb, &state->wall)) {
      //} else if (state->character.bb.y < 0 || state->character.bb.y >
state->screen_h) { state->character.accel_y *= -1.0f;
    }
    */
    //state->character.bb.x += speed(state->character.accel_x, dt, state->character.veloc_x);
    //state->character.bb.y += speed(state->character.accel_y, dt, state->character.veloc_y);
  }
}


extern "C" GAME_RENDER(GameRender)
{
  if (COLLISION_DEMO_ENABLED) {
    state->api.PlatformDrawQuad(state->wall.bl.x,
				state->wall.bl.y,
				state->wall.br.x,
				state->wall.br.y,
				state->wall.ur.x,
				state->wall.ur.y,
				state->wall.ul.x,
				state->wall.ul.y,
				state->wallColor.r,
				state->wallColor.g,
				state->wallColor.b,
				state->wallColor.a);
    state->api.PlatformDrawQuad(state->ground.bl.x,
				state->ground.bl.y,
				state->ground.br.x,
				state->ground.br.y,
				state->ground.ur.x,
				state->ground.ur.y,
				state->ground.ul.x,
				state->ground.ul.y,
				state->groundColor.r,
				state->groundColor.g,
				state->groundColor.b,
				state->groundColor.a);
    
    for (unsigned int c=0; c < state->boxCount; c++) {
      state->api.PlatformDrawQuad(state->box_rects[c].bl.x,
                                  state->box_rects[c].bl.y,
                                  state->box_rects[c].br.x,
                                  state->box_rects[c].br.y,
                                  state->box_rects[c].ur.x,
                                  state->box_rects[c].ur.y,
                                  state->box_rects[c].ul.x,
                                  state->box_rects[c].ul.y,
                                  state->box_color[c].r,
                                  state->box_color[c].g,
                                  state->box_color[c].b,
                                  state->box_color[c].a);
    }
  }

  if (CHARACTER_DEMO_ENABLED) {

    /*
    state->api.PlatformDrawTexture(state->character.textureIndex,
				   state->character.bb.bl.x,
				   state->character.bb.bl.y,
				   state->character.bb.br.x,
				   state->character.bb.br.y,
				   state->character.bb.ur.x,
				   state->character.bb.ur.y,
				   state->character.bb.ul.x,
				   state->character.bb.ul.y,
                                   113, 160, 16, 32);
    */
  }
}
