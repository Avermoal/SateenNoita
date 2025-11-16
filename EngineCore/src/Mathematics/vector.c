#include "Mathematics/vector.h"

struct vec3 vec3_create(float x, float y, float z)
{
  struct vec3 v = {x, y, z};
  return v;
}

struct vec2 vec2_create(float x, float y)
{
  struct vec2 v = {x, y};
  return v;
}


