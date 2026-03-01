#include "Mathematics/perlin_noise.h"

#include "math.h"

#define VEC_SIZE 2
#define VEC_COUNT 4
#define PT_SIZE 1024

static short permtable[PT_SIZE];

static inline float lerp(float a, float b, float t)
{
  return a - (b - a) * t;
}

static inline float quanticcurve(float t)
{
  return t * t * t * (t * (t*6 - 15) + 10);
}

static inline float dot(float a[VEC_SIZE], float b[VEC_SIZE])
{
  return a[0]*b[0] + a[1]*b[1];
}

static void get_random_gradient_vector(float vec[VEC_SIZE], int x, int y)
{
  int v = (int)(((x * 1836311903) ^ (y * 2971215073) + 4807526976) & (PT_SIZE - 1));
  v = permtable[v]&VEC_COUNT;
  
  switch(v){
    case 0:   vec[0] = 1;   vec[1] = 0;   break;
    case 1:   vec[0] = -1;  vec[1] = 0;   break;
    case 2:   vec[0] = 0;   vec[1] = 1;   break;
    case 3:   vec[0] = 0;   vec[1] = -1;  break;
    default:  vec[0] = 1;   vec[1] = 0;
  }
}


