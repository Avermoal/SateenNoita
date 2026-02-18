#include "EngineCore/player_move_system.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "EngineCore/window.h"
#include "EngineCore/scene.h"
#include "EngineCore/tiles_info.h"
#include "EngineCore/events.h"
#include "EngineCore/tiles_map.h"

#include<stdio.h>

void player_move_handler(struct window* pwindow)
{
  /*MOVE*/
  if(jpressed(GLFW_KEY_W) && jpressed(GLFW_KEY_D)){
    move_mob_on_place(&pwindow->scn->map.gmap, MT_FRT);
  }
  if(jpressed(GLFW_KEY_D) && jpressed(GLFW_KEY_S)){
    move_mob_on_place(&pwindow->scn->map.gmap, MT_DRT);
  }
  if(jpressed(GLFW_KEY_S) && jpressed(GLFW_KEY_A)){
    move_mob_on_place(&pwindow->scn->map.gmap, MT_DLT);
  }
  if(jpressed(GLFW_KEY_A) && jpressed(GLFW_KEY_W)){
    move_mob_on_place(&pwindow->scn->map.gmap, MT_FLT);
  }
  if(jpressed(GLFW_KEY_W)){
    printf("W\n");
    move_mob_on_place(&pwindow->scn->map.gmap, MT_FT);
  }
  if(jpressed(GLFW_KEY_D)){
    move_mob_on_place(&pwindow->scn->map.gmap, MT_RT);
  }
  if(jpressed(GLFW_KEY_S)){
    move_mob_on_place(&pwindow->scn->map.gmap, MT_DT);
  }
  if(jpressed(GLFW_KEY_A)){
    move_mob_on_place(&pwindow->scn->map.gmap, MT_LT);
  }
  
  /*FINISH Update tilemap*/
  updatetilemap(&pwindow->scn->map);
}
