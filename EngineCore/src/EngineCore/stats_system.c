#include "EngineCore/stats_system.h"

#include <string.h>
#include <stdio.h>
#include <math.h>

#include "EngineCore/tiles_map.h"
#include "EngineCore/text_display_system.h"
#include "EngineCore/game_interface.h"

#define STR_LEN 32

void addHP(struct tile* mob, short delta)
{
  if(!mob || !mob->stts){
    return;
  }
  int newHP = mob->stts->HP + delta;
  if(newHP <= 0){
    mob->stts->HP = 0;
    mob->stts->isalive = false;
  }else{
    mob->stts->HP = newHP;
  }
}

void addMP(struct tile* mob, short delta)
{
  mob->stts->MP += delta;
}

struct stats update_player_stats_on_interface(struct container* cnt, struct tile* mob)
{
  char buffer[STR_LEN];
  snprintf(buffer, sizeof(buffer), "HP:%d", mob->stts->HP);
  update_text_content(cnt, buffer);
  return *mob->stts;
}
