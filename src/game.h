#ifndef GAME_H
#define GAME_H

#include "shared.h"

typedef struct {
  float x,y;
} Vec2;

typedef struct {
  float x, y, z;
} Vec3;

typedef struct {
  float r, g, b, a;
} Vec4;

typedef struct {
  Sprite* sprite;
  float dt;
  Vec4 color;
  uint8_t current_frame;
  ShaderProgram* shader_program;
} SpriteRenderer;

typedef struct {
  Vec2 position;
  Vec2 velocity;
  Vec2 accel;
} PhysicalComponent;

typedef struct {
  SpriteRenderer* sprite_renderer;
  PhysicalComponent* physical_component;
} Entity;

#endif
