#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "shared.h"

typedef struct
{
  float accel_x;
  float accel_y;
  //  float x;
  //float y;
  //float width;
  //float height;
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
} BoxMeta;

#define COLOR_SHIFT_RATE 0.2

#define COLLISION_DEMO_ENABLED true
#define COLLISION_SPLATTER false
#define COLLISION_DEMO_MAX_BOXES 10000
#define COLLISION_DEMO_INITIAL_BOX_COUNT 10000

bool checkCollision(Rect *a, Rect *b){
  // the sides of the rects
  int leftA, leftB;
  int rightA, rightB;
  int topA, topB;
  int bottomA, bottomB;

  // Calculate the sides of "A"
  leftA = a->x;
  rightA = a->x + a->w;
  topA = a->y;
  bottomA = a->y + a->h;

  // Calculate the sides of "B"
  leftB = b->x;
  rightB = b->x + b->w;
  topB = b->y;
  bottomB = b->y + b->h;

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

typedef struct
{
  uint16_t x, y, width, height;
} SpriteFrameDefinition;

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
const SpriteFrameDefinition CHAR_FACING_DOWN[MAX_FRAMES] = {
    FACING_DOWN_STEP_1,
    FACING_DOWN_STEP_2,
    FACING_DOWN_STEP_0,
};
const SpriteFrameDefinition CHAR_FACING_RIGHT[MAX_FRAMES] = {
    FACING_RIGHT_STEP_1,
    FACING_RIGHT_STEP_2,
    FACING_RIGHT_STEP_0,
};
const SpriteFrameDefinition CHAR_FACING_UP[MAX_FRAMES] = {
    FACING_UP_STEP_1,
    FACING_UP_STEP_2,
    FACING_UP_STEP_0,
};
const SpriteFrameDefinition CHAR_FACING_LEFT[MAX_FRAMES] = {
    FACING_LEFT_STEP_1,
    FACING_LEFT_STEP_2,
    FACING_LEFT_STEP_0,
};
#define NUMBER_OF_FACING_DIRS 4
enum { FACING_DOWN = 0, FACING_UP = 2 };
enum { FACING_RIGHT = 1, FACING_LEFT = 3 };
const SpriteFrameDefinition *CHAR_ANIMATION_FRAMES[NUMBER_OF_FACING_DIRS] = {
    CHAR_FACING_DOWN,
    CHAR_FACING_RIGHT,
    CHAR_FACING_UP,
    CHAR_FACING_LEFT,
};

typedef struct
{
  unsigned int textureIndex;
  BoxMeta bb;
  Rect rect;
  float current_frame;
  uint8_t facing;
  uint8_t cyclesPerSecond;
  bool walking;
  bool falling;
  bool crouched;
} Character;

const SpriteFrameDefinition* getSpriteFrame(Character *c) {
  return &(CHAR_ANIMATION_FRAMES[c->facing][(int)(floor(c->current_frame))]);
}

#define CONTROLLER_DEMO_ENABLED true

enum {
  CONTROLLER_UP = 0,
  CONTROLLER_DOWN,
  CONTROLLER_LEFT,
  CONTROLLER_RIGHT,
  CONTROLLER_JUMP,
  CONTROLLER_FIRE,
  CONTROLLER_MENU,
  CONTROLLER_PAUSE,
  CONTROLLER_TRIGGER_L,
  CONTROLLER_TRIGGER_R,
  CONTROLLER_SIZE,
};

const unsigned int button_map[CONTROLLER_SIZE] = {
  K_w,
  K_s,
  K_a,
  K_d,
  K_SPACE,
  K_ESCAPE,
  K_p,
  K_q,
  K_e,
  K_UNKNOWN,
};

typedef struct
{

  unsigned int state[CONTROLLER_SIZE];
  unsigned int previous[CONTROLLER_SIZE];

  float pointer_x, pointer_y;

  float action_queue_timer;
  int action_queue[10];
} Controller;

#define AUDIO_DEMO_ENABLED false

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

typedef struct
{
  unsigned int id;
  int x;
  int y;
  int w;
  int h;
} Window;

typedef struct
{
  GameMemory memory;
  PlatformAPI api;

  // Window meta
  Window window;

  bool onlyOnceInit;
  
  // Collision Demo
  bool collisionDemoInitialized;
  Rect boxRects[COLLISION_DEMO_MAX_BOXES];
  BoxMeta boxes[COLLISION_DEMO_MAX_BOXES];
  BoxMeta wall;
  Rect wall_rect;
  BoxMeta ground;
  Rect ground_rect;
  unsigned int boxCount;

  // Animating and controlling a Character Demo
  bool characterDemoInitialized;
  Character character;
  
  // User Input Demo
  bool keyboardDemoInitialized;
  Controller controller;
  
  bool audioDemoInitialized;

  bool showMenu;
  bool paused;
} GameState;

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

static GameState *state;

void newDemoBB(BoxMeta *bb, unsigned int c)
{
  bb->accel_x = rand() % 9 % 2 == 0 ? -1.0f : 1.0f;
  bb->accel_y = rand() % 9 % 2 == 0 ? 1.0f : -1.0f;
  bb->veloc_x = (rand() % (c+1)) * 0.120f;
  bb->veloc_y = (rand() % (c+1)) * 0.120f;
  bb->r=0.3f * (rand() % 9);
  bb->accel_r=1.0f;
  bb->g=0.0f * (rand() % 9);
  bb->accel_g=0.0f;
  bb->b=0.0f * (rand() % 9);
  bb->accel_b=0.0f;
  bb->a=0.25;
  bb->accel_a=1.0f;
}

void newBB(BoxMeta *bb, Rect *r, float x, float y, float w, float h, unsigned int c)
{
  newDemoBB(bb, c);

  float rx = 60.0f, ry = 60.0f;

  if (COLLISION_SPLATTER && x < 0 && y < 0) {
    if (c > 1000) {
      rx = state->window.w - 60.0f;
    }
    if (c > 2000) {
      ry = state->window.h - 60.0f;
    }
    if (c > 3000) {
      rx = 60.0f;
    }
    if (c > 4000) {
      rx = state->window.w * 0.5f;
      ry = 60.0f;
    }
    if (c > 5000) {
      rx = state->window.w;
      ry = state->window.h * 0.5f;
    }
    if (c > 6000) {
      ry = 60.0f;
    }
    if (c > 7000) {
      rx = state->window.w * 0.5f;
      ry = state->window.h - 60.0f;
    }
  }
  
  r->w = 5.0f;
  r->h = 5.0f;
  r->x = rx;
  r->y = ry;
  r->w = w;
  r->h = h;
  bb->a = 0.5f;
}

func(GAME_WINDOW_RESIZED, GameWindowResized)
{
  printf("window(%d) resized", window);
  state->window.w = width;
  state->window.h = height;
}

extern GAME_QUIT(GameQuit)
{
  printf("game: got a quit\n");
  // Take a screenshot on exit
  //state->api.PlatformScreenshot(0, 0, 0, state->screen_w, state->screen_h);
}


extern GAME_INIT(GameInit)
{ 
  state = GameAllocateStruct(&memory, GameState);
  state->api = api;
  if(state->memory.ptr == 0) {
    state->memory = memory;
  }

  if(!state->onlyOnceInit) {
    state->onlyOnceInit = true;
    memset(&state->controller, 0, sizeof(Controller));
    state->paused = false;
  }
  
  state->window.w = screen_w;
  state->window.h = screen_h;

  if (AUDIO_DEMO_ENABLED && !state->audioDemoInitialized) {
    state->api.PlatformEnsureMusic("Stormcrow56k_-_my_old_man.mp3", 0);
    state->api.PlatformEnsureMusic("Stormcrow56k_-_temaczal.mp3", 1);
    state->api.PlatformEnsureAudio("ByMennen.wav", 0);
    state->api.PlatformEnsureAudio("bird_caw1.wav", 1);
  }
  
  if (COLLISION_DEMO_ENABLED && !state->collisionDemoInitialized) {
    //srand(37);
    state->collisionDemoInitialized = true;
    newBB(&state->wall, &state->wall_rect, 300.0f, 100.0f, 50.0f, 200.0f, 0);
    newBB(&state->ground, &state->ground_rect, 0.0f, 0.0f, 1.0f*state->window.w, 30.0f, 0);
    for (unsigned int c=0; c < COLLISION_DEMO_INITIAL_BOX_COUNT; c++) {
      memset(&state->boxes[c], 0, sizeof(BoxMeta));
      newBB(&state->boxes[c], &state->boxRects[c], -1, -1, 5.0, 5.0, c);
    }
    state->boxCount = COLLISION_DEMO_INITIAL_BOX_COUNT;
  }
  
  if (CHARACTER_DEMO_ENABLED && !state->characterDemoInitialized) {
    state->characterDemoInitialized = true;
    memset(&state->character, 0, sizeof(Character));
    state->api.PlatformEnsureImage(CHARACTER_DEMO_SPRITE, 0);
    state->character.textureIndex = 0;
    state->character.rect.x = 150;
    state->character.rect.y = 250;
    state->character.rect.w = 64;
    state->character.rect.h = 128;
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
  if (*c >= 0.9f || *c <= 0.1f)
    *accel *= -1.0f;
}

extern GAME_UPDATE(GameUpdate)
{

  if (state->controller.state[CONTROLLER_PAUSE] &&
      state->controller.state[CONTROLLER_PAUSE] != state->controller.previous[CONTROLLER_PAUSE]) {
    state->paused = !state->paused;
  }

  if (AUDIO_DEMO_ENABLED) {
    // Pause music
    if (state->paused) {
      state->api.PlatformPauseMusic();
      state->api.PlatformPlayAudio(0, 0, 1, 100, 0);
      state->api.PlatformPlayMusic(1, 5, 0, 0.0f, 100, false);
    } else {
      state->api.PlatformStopMusic(0);
      state->api.PlatformPlayMusic(0, 0, 0, 0.0f, 100, true);
    }
  }

  if (COLLISION_DEMO_ENABLED && !state->paused) {
    for (unsigned int c=0; c < state->boxCount; c++){
      // Determine the next x and y bounding boxes
      Rect bb_next_x = {
	state->boxRects[c].x + speed(state->boxes[c].accel_x, dt, state->boxes[c].veloc_x),
	state->boxRects[c].y, state->boxRects[c].w, state->boxRects[c].h,
      };
      Rect bb_next_y = {
	state->boxRects[c].x,
	state->boxRects[c].y + speed(state->boxes[c].accel_y, dt, state->boxes[c].veloc_y),
	state->boxRects[c].w, state->boxRects[c].h
      };
      if (checkCollision(&bb_next_x, &state->wall_rect)) {
	state->boxes[c].accel_x *= -1.0f;
	state->boxes[c].veloc_x -= state->boxes[c].veloc_x * 0.05f;
      } else if (checkCollision(&bb_next_x, &state->ground_rect)) {
	state->boxes[c].accel_x *= -1.0f;
      } else if (bb_next_x.x < 0 || bb_next_x.x + bb_next_x.w > state->window.w) {
	state->boxes[c].accel_x *= -1.0f;
	state->boxes[c].veloc_x -= state->boxes[c].veloc_x * 0.05f;
      } else {
	state->boxes[c].veloc_x *= 0.99f;
      }
      if (checkCollision(&bb_next_y, &state->wall_rect)) {
	state->boxes[c].accel_y *= 0.0f;
	state->boxes[c].veloc_x *= 0.75f;
      } else if (checkCollision(&bb_next_y, &state->ground_rect)) {
	state->boxes[c].accel_y = -0.0f;
	state->boxes[c].veloc_y -= state->boxes[c].veloc_y * 0.95f;
	state->boxes[c].veloc_x *= 0.75;
      } else if (bb_next_y.y < 0 || bb_next_y.y + bb_next_y.h > state->window.h) {
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
      state->boxRects[c].x += speed(state->boxes[c].accel_x, dt, state->boxes[c].veloc_x);
      state->boxRects[c].y += speed(state->boxes[c].accel_y, dt, state->boxes[c].veloc_y);
      shiftColor(&state->boxes[c].r, &state->boxes[c].accel_r, dt);
      shiftColor(&state->boxes[c].g, &state->boxes[c].accel_g, dt);
      shiftColor(&state->boxes[c].b, &state->boxes[c].accel_b, dt);
      shiftColor(&state->boxes[c].a, &state->boxes[c].accel_a, dt);
    }
  }

  if (CHARACTER_DEMO_ENABLED && !state->paused) {
    // Determine the next x and y bounding boxes
    Rect bb_next_x = {
      state->character.rect.x + speed(state->character.bb.accel_x, dt, state->character.bb.veloc_x),
      state->character.rect.y,
      state->character.rect.w,
      state->character.rect.h,
    };
    Rect bb_next_y = {
      state->character.rect.x,
      state->character.rect.y + speed(state->character.bb.accel_y, dt, state->character.bb.veloc_y),
      state->character.rect.w,
      state->character.rect.h
    };
    if (checkCollision(&bb_next_x, &state->wall_rect)) {
      state->character.bb.accel_x *= -1.0f;
    } else if (checkCollision(&bb_next_x, &state->ground_rect)) {
      state->character.bb.accel_x *= -1.0f;
    } else if (bb_next_x.x < 0 || bb_next_x.x + bb_next_x.w > state->window.w) {
      state->character.bb.accel_x *= -1.0f;
    }
    if (checkCollision(&bb_next_y, &state->wall_rect)) {
      state->character.bb.accel_y *= -1.0f;
    } else if (checkCollision(&bb_next_y, &state->ground_rect)) {
      state->character.bb.accel_y *= -1.0f;
    } else if (bb_next_y.y < 0 || bb_next_y.y + bb_next_y.h > state->window.h) {
      state->character.bb.accel_y *= -1.0f;
    }
    float speed_x = speed(state->character.bb.accel_x, dt, state->character.bb.veloc_x);
    float speed_y = speed(state->character.bb.accel_y, dt, state->character.bb.veloc_y);
    float *speed = &speed_x;
    state->character.rect.x += speed_x;
    state->character.rect.y += speed_y;
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
    state->character.current_frame += (dt / (1.0 / abs((int)floor(*speed)))) * 10;
    if (state->character.current_frame >= 2){
      state->character.current_frame = 0;
    }
  }
  memcpy(&state->controller.previous, &state->controller.state, sizeof(state->controller.state));
  memset(&state->controller.state, 0, sizeof(state->controller.state));
}

extern GAME_RENDER(GameRender)
{
  state->api.PlatformDrawBox(&state->wall_rect,
			     //x, state->wall.y, state->wall.width, state->wall.height,
			     state->wall.r, state->wall.g, state->wall.b, state->wall.a, true);
  state->api.PlatformDrawBox(&state->ground_rect,
			     //state->ground.x, state->ground.y,
			     //state->ground.width, state->ground.height,
			     state->ground.r, state->ground.g, state->ground.b, state->ground.a, true);
  
  //state->api.PlatformDrawBoxes(state->boxes, state->boxCount, 
  for (unsigned int c=0; c < state->boxCount; c++) {
    state->api.PlatformDrawBox(&state->boxRects[c],
			       state->boxes[c].r,
			       state->boxes[c].g,
			       state->boxes[c].b,
			       state->boxes[c].a, true);
  }
  if (CHARACTER_DEMO_ENABLED) {
    const SpriteFrameDefinition *sf = getSpriteFrame(&state->character);
    state->api.PlatformDrawTexture(state->character.textureIndex,
				   state->character.rect.x,
				   state->character.rect.y,
				   state->character.rect.w,
				   state->character.rect.h,
				   sf->x, sf->y, sf->width, sf->height);
  }
}

extern GAME_KEYBOARD_INPUT(GameKeyboardInput)
{
  for (unsigned int i=0; i < CONTROLLER_SIZE; i++) {
    if (SCANCODE_TO_KEYCODE(symbol) == button_map[i]){
      state->controller.state[i] = key_state;
      break;
    }
  }
}

extern GAME_CHANNEL_HALTED(GameAudioChannelHalted)
{
  printf("%d audio channel halted\n", channel);
}


