#include "Mathematics/noises.h"

#include <math.h>
#include <stdlib.h>
#include <time.h>


/* PERLIN NOISE */

#define VEC_SIZE 2
#define VEC_COUNT 4
#define PT_SIZE 1024
#define PERIOD 0.3f
#define SHIFT 0.5f

static short permtable[PT_SIZE];

static inline float lerp(float a, float b, float t)
{
  return a + t*(b - a);
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
  v = permtable[v]&(VEC_COUNT - 1);
  
  switch(v){
    case 0:   vec[0] = 1;   vec[1] = 0;   break;
    case 1:   vec[0] = -1;  vec[1] = 0;   break;
    case 2:   vec[0] = 0;   vec[1] = 1;   break;
    case 3:   vec[0] = 0;   vec[1] = -1;  break;
    default:  vec[0] = 1;   vec[1] = 0;
  }
}

void init_perlin_noise(int seed)
{
  srand(seed);
  for(int i = 0; i < PT_SIZE; ++i){
    permtable[i] = i;
  }
  short temp = 0;
  for(int i = 0, j = 0; i < PT_SIZE; ++i){
    j = rand() % (i + 1);
    temp = permtable[i];
    permtable[i] = permtable[j];
    permtable[j] = temp;
  }
}

static float noise(float fx, float fy)
{
  /*Left-top square's corner coordinates*/
  int left = (int)(floor((double)fx));
  int top = (int)(floor((double)fy));
  /*Local point's coordinates in square*/
  float pointx = fx - (float)left;
  float pointy = fy - (float)top;
  /*Gradient's vectors (top, bottom, left, right gradients)*/
  float tlg[VEC_SIZE], trg[VEC_SIZE], blg[VEC_SIZE], brg[VEC_SIZE];
  get_random_gradient_vector(tlg, left,     top);
  get_random_gradient_vector(trg, left + 1, top);
  get_random_gradient_vector(blg, left,     top + 1);
  get_random_gradient_vector(brg, left + 1, top + 1);
  /*Distance from square corners to point*/
  float dtl[VEC_SIZE] = {pointx,      pointy};
  float dtr[VEC_SIZE] = {pointx - 1,  pointy};
  float dbl[VEC_SIZE] = {pointx,      pointy - 1};
  float dbr[VEC_SIZE] = {pointx - 1,  pointy - 1};
  /*Scalar multiply*/
  float tx1 = dot(dtl, tlg);
  float tx2 = dot(dtr, trg);
  float bx1 = dot(dbl, blg);
  float bx2 = dot(dbr, brg);
  /*Interpolation params*/
  pointx = quanticcurve(pointx);
  pointy = quanticcurve(pointy);
  /*Interpolation*/
  float tx = lerp(tx1, tx2, pointx);
  float bx = lerp(bx1, bx2, pointx);
  float tb = lerp(tx, bx, pointy);
  return tb;
}

void perlin_noise_map_gen(int maph, int mapw, float (*map)[mapw], int lbc)
{
  for(int h = 0; h < maph; ++h){
    for(int w = 0; w < mapw; ++w){
      /*To noise coordinates*/
      float nh = h * PERIOD;
      float nw = w * PERIOD;
      /*Map*/
      map[h][w] = noise(nh + SHIFT + lbc, nw + SHIFT);
    }
  }
}

/* CELLUAR NOISE*/


