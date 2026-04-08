#include "EngineCore/mobs_move_system.h"

#include "EngineCore/window.h"
#include "EngineCore/tiles_info.h"
#include "EngineCore/tiles_map.h"
#include "EngineCore/scene.h"
#include "EngineCore/stats_system.h"

#include "Mathematics/a_star.h"

void mobs_move_handler(struct window* pwindow)
{
  struct gamemap* gmap = &pwindow->scn->map.gmap;
  int pcy = gmap->pcy;
  int pcx = gmap->pcx;
  short id = 0;
  int nexty = 0, nextx = 0;
  for(int y = 0; y < MAP_HEIGHT; ++y){
    for(int x = 0; x < MAP_WIDTH; ++x){
      id = gmap->mobs[y][x].id;
      if(id != ID_000000_ERROR && id != ID_000000_VOID && id != ID_000012_PLAYER){
        if(astar(gmap, y, x, pcy, pcx, &nexty, &nextx)){
          if(nextx == pcx && nexty == pcy){
            /*PLACE FOR ATTACK*/
            addHP(&gmap->mobs[pcy][pcx], -1);
            continue;
          }
          nearest_swap_tile(gmap->mobs, y, x, nexty, nextx);
        }
      }
    }
  }
}
