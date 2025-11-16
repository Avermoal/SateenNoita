#ifndef MATH_VECTOR_H
#define MATH_VECTOR_H

struct vec3{
  float x, y, z;
};

struct vec2{
  float x, y;
};

struct vec3 vec3_create(float x, float y, float z);

struct vec2 vec2_create(float x, float y);

#endif/*MATH_VECTOR_H*/
