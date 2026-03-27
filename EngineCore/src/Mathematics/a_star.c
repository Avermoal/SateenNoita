#include "Mathematics/a_star.h"

#include <math.h>
#include <limits.h>

#include "EngineCore/tiles_map.h"
#include "EngineCore/tiles_info.h"

#define NO_PARENT -1
#define SIDES_NUMBER 4

static inline int heuristic(int y1, int x1, int y2, int x2)
{
  return abs(x1 - x2) + abs(y1- y2);
}

static bool iswalkable(struct gamemap* gmap, int y, int x)
{
  if(x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT){
    return false;
  }
  if(gmap->groundmap[y][x].isobstacle){
    return false;
  }
  if(gmap->mobs[y][x].id != ID_000000_VOID){
    return false;
  }
  return true;
}

bool astar(struct gamemap* gmap, int starty, int startx, int goaly, int goalx, int* nexty, int* nextx)
{
  if(startx == goalx && starty == goaly){
    *nextx = startx;
    *nexty = starty;
    return true;
  }
  /*Arrays for A* algorithm*/
  int gscores[MAP_HEIGHT][MAP_WIDTH];
  int fscores[MAP_HEIGHT][MAP_WIDTH];
  bool closed[MAP_HEIGHT][MAP_WIDTH];
  bool open[MAP_HEIGHT][MAP_WIDTH];
  int parentx[MAP_HEIGHT][MAP_WIDTH];
  int parenty[MAP_HEIGHT][MAP_WIDTH];
  /*Init*/
  for(int y = 0; y < MAP_HEIGHT; ++y){
    for(int x = 0; x < MAP_WIDTH; ++x){
      gscores[y][x] = INT_MAX;
      fscores[y][x] = INT_MAX;
      closed[y][x] = false;
      open[y][x] = false;
      parentx[y][x] = NO_PARENT;
      parenty[y][x] = NO_PARENT;
    }
  }
  gscores[starty][startx] = 0;
  fscores[starty][startx] = heuristic(starty, startx, goaly, goalx);
  open[starty][startx] = true;
  /*A* body*/
  while(1){
    /*Find node with minumum f*/
    int currentx = NO_PARENT, currenty = NO_PARENT;
    int bestf = INT_MAX;
    for(int y = 0; y < MAP_HEIGHT; ++y){
      for(int x = 0; x < MAP_WIDTH; ++x){
        if(open[y][x] && fscores[y][x] < bestf){
          bestf = fscores[y][x];
          currentx = x;
          currenty = y;
        }
      }
    }
    /*Path not find*/
    if(currentx == NO_PARENT){
      return false;
    }
    if(currentx == goalx && currenty == goaly){
      int stepx = goalx, stepy = goaly;
      int prevx = parentx[stepy][stepx];
      int prevy = parenty[stepy][stepx];
      while (prevx != startx || prevy != starty) {
        stepx = prevx;
        stepy = prevy;
        prevx = parentx[stepy][stepx];
        prevy = parenty[stepy][stepx];
      }
      *nextx = stepx;
      *nexty = stepy;
      return true;
    }
    open[currenty][currentx] = false;
    closed[currenty][currentx] = true;
    /*Naighbors*/
    int dx[SIDES_NUMBER] = {0, 1, 0, -1};
    int dy[SIDES_NUMBER] = {1, 0, -1, 0};
    for(int i = 0; i < SIDES_NUMBER; ++i){
      int nx = currentx + dx[i];
      int ny = currenty + dy[i];
      if(nx < 0 || nx >= MAP_WIDTH || ny < 0 || ny >= MAP_HEIGHT){
        continue;
      }
      if(closed[ny][nx]){
        continue;
      }
      if(!(nx == goalx && ny == goaly) && !iswalkable(gmap, ny, nx)){
      continue;
      }
      int gtentative = gscores[currenty][currentx] + 1;
      if(gtentative < gscores[ny][nx]){
        parentx[ny][nx] = currentx;
        parenty[ny][nx] = currenty;
        gscores[ny][nx] = gtentative;
        fscores[ny][nx] = gtentative + heuristic(ny, nx, goaly, goalx);
        if(!open[ny][nx]){
          open[ny][nx] = true;
        }
      }
    }
  }
}
