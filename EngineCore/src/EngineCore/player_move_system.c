#include "EngineCore/player_move_system.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "EngineCore/window.h"
#include "EngineCore/scene.h"
#include "EngineCore/tiles_info.h"
#include "EngineCore/events.h"
#include "EngineCore/tiles_map.h"

#define MOVE_DELAY 0.22

bool player_move_handler(struct window* pwindow)
{
  bool player_is_move = false;
  /*MOVE*/
  if(pressed(GLFW_KEY_W) && !pressed(GLFW_KEY_S) && !pressed(GLFW_KEY_D) && !pressed(GLFW_KEY_A) && (pwindow->windata.curtime - pwindow->windata.last_move_time >= MOVE_DELAY)){
    move_player_on_place(&pwindow->scn->map.gmap, MT_FT);
    pwindow->windata.last_move_time = pwindow->windata.curtime;
    player_is_move = true;
  }
  if(pressed(GLFW_KEY_D) && !pressed(GLFW_KEY_A) && !pressed(GLFW_KEY_S) && !pressed(GLFW_KEY_W) && (pwindow->windata.curtime - pwindow->windata.last_move_time >= MOVE_DELAY)){
    move_player_on_place(&pwindow->scn->map.gmap, MT_RT);
    pwindow->windata.last_move_time = pwindow->windata.curtime;
    player_is_move = true;
  }
  if(pressed(GLFW_KEY_S) && !pressed(GLFW_KEY_W) && !pressed(GLFW_KEY_D) && !pressed(GLFW_KEY_A) && (pwindow->windata.curtime - pwindow->windata.last_move_time >= MOVE_DELAY)){
    move_player_on_place(&pwindow->scn->map.gmap, MT_DT);
    pwindow->windata.last_move_time = pwindow->windata.curtime;
    player_is_move = true;
  }
  if(pressed(GLFW_KEY_A) && !pressed(GLFW_KEY_D) && !pressed(GLFW_KEY_S) && !pressed(GLFW_KEY_W) && (pwindow->windata.curtime - pwindow->windata.last_move_time >= MOVE_DELAY)){
    move_player_on_place(&pwindow->scn->map.gmap, MT_LT);
    pwindow->windata.last_move_time = pwindow->windata.curtime;
    player_is_move = true;
  }
  return player_is_move;
}
